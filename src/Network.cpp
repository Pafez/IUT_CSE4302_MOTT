#include "Network.h"
#include "Account.h"
#include "Entry.h"
#include "Session.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Globals
// ─────────────────────────────────────────────────────────────────────────────
static SOCKET   g_clientSock = INVALID_SOCKET;
static bool     g_isClient   = false;
static std::mutex g_fileMutex;   // server-side: protect file access across threads

// ─────────────────────────────────────────────────────────────────────────────
// Low-level send / recv  (newline-delimited messages)
// ─────────────────────────────────────────────────────────────────────────────
void net_send(SOCKET sock, const std::string& msg) {
    std::string line = msg + "\n";
    int total = 0;
    int len   = (int)line.size();
    while (total < len) {
        int sent = send(sock, line.c_str() + total, len - total, 0);
        if (sent == SOCKET_ERROR) return;
        total += sent;
    }
}

std::string net_recv(SOCKET sock) {
    std::string result;
    char ch = 0;
    while (true) {
        int r = recv(sock, &ch, 1, 0);
        if (r <= 0) break;
        if (ch == '\n') break;
        result += ch;
    }
    return result;
}

std::string net_send_recv(SOCKET sock, const std::string& msg) {
    net_send(sock, msg);
    return net_recv(sock);
}

// ─────────────────────────────────────────────────────────────────────────────
// Client  –  connect / disconnect
// ─────────────────────────────────────────────────────────────────────────────
bool net_connect_to_server(const std::string& host) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "[NET] WSAStartup failed.\n";
        return false;
    }

    g_clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (g_clientSock == INVALID_SOCKET) {
        std::cerr << "[NET] socket() failed.\n";
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(NET_PORT);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    if (connect(g_clientSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "[NET] Could not connect to " << host << ":" << NET_PORT << "\n";
        closesocket(g_clientSock);
        g_clientSock = INVALID_SOCKET;
        return false;
    }

    g_isClient = true;
    std::cout << "[NET] Connected to server at " << host << ":" << NET_PORT << "\n";
    return true;
}

void net_disconnect() {
    if (g_clientSock != INVALID_SOCKET) {
        closesocket(g_clientSock);
        g_clientSock = INVALID_SOCKET;
    }
    g_isClient = false;
    WSACleanup();
}

bool net_is_client() { return g_isClient; }


bool net_save_acc(const std::string& username, const std::string& password) {
    // Ask server to create the account (server handles hashing)
    std::string msg = "REGISTER|" + username + "|" + password;
    std::string resp = net_send_recv(g_clientSock, msg);
    return (resp.rfind("OK", 0) == 0);
}

bool net_load_acc(const std::string& username,
                  int& outID, std::string& outHash, std::string& outSalt) {
    std::string msg  = "LOAD_ACC|" + username;
    std::string resp = net_send_recv(g_clientSock, msg);
    if (resp.rfind("OK|", 0) != 0) return false;

    // OK|id|username|hash|salt
    std::stringstream ss(resp.substr(3));
    std::string id, user, hash, salt;
    getline(ss, id,   '|');
    getline(ss, user, '|');
    getline(ss, hash, '|');
    getline(ss, salt);
    outID   = stoi(id);
    outHash = hash;
    outSalt = salt;
    return true;
}

bool net_login_acc(const std::string& username, const std::string& password,
                   int& outID, std::string& outHash, std::string& outSalt) {
    std::string msg  = "LOGIN|" + username + "|" + password;
    std::string resp = net_send_recv(g_clientSock, msg);
    if (resp.rfind("OK|", 0) != 0) return false;

    std::stringstream ss(resp.substr(3));
    std::string id, user, hash, salt;
    getline(ss, id,   '|');
    getline(ss, user, '|');
    getline(ss, hash, '|');
    getline(ss, salt);
    outID   = stoi(id);
    outHash = hash;
    outSalt = salt;
    return true;
}

int net_get_next_id() {
    std::string resp = net_send_recv(g_clientSock, "GET_NEXT_ID");
    if (resp.rfind("OK|", 0) == 0) return stoi(resp.substr(3));
    return -1;
}

void net_save_counter(int n) {
    net_send_recv(g_clientSock, "SAVE_COUNTER|" + std::to_string(n));
}

bool net_send_entry(const std::string& serialized) {
    std::string msg  = "ENTRY|" + serialized;
    std::string resp = net_send_recv(g_clientSock, msg);
    return (resp.rfind("OK", 0) == 0);
}


static void handle_client(SOCKET clientSock) {
    std::cout << "[SERVER] Client connected.\n";

    while (true) {
        std::string line = net_recv(clientSock);
        if (line.empty()) break;   // client disconnected

        // Parse command and fields
        std::stringstream ss(line);
        std::string cmd;
        getline(ss, cmd, '|');

        // ── REGISTER ──────────────────────────────────────────────────
        if (cmd == "REGISTER") {
            std::string username, password;
            getline(ss, username, '|');
            getline(ss, password);

            std::lock_guard<std::mutex> lock(g_fileMutex);
            Account a(username, password);
            if (save_acc(a))
                net_send(clientSock, "OK");
            else
                net_send(clientSock, "ERR|Account already exists");
        }

        // ── LOGIN ─────────────────────────────────────────────────────
        else if (cmd == "LOGIN") {
            std::string username, password;
            getline(ss, username, '|');
            getline(ss, password);

            std::lock_guard<std::mutex> lock(g_fileMutex);
            Account a;
            if (loginAcc(username, password, a)) {
                // OK|id|username|hash|salt
                std::string resp = "OK|" + std::to_string(a.getID()) + "|"
                                 + a.get_name() + "|"
                                 + a.getHash()  + "|"
                                 + a.getSalt();
                net_send(clientSock, resp);
            } else {
                net_send(clientSock, "ERR|Invalid credentials");
            }
        }

        // ── LOAD_ACC ──────────────────────────────────────────────────
        else if (cmd == "LOAD_ACC") {
            std::string username;
            getline(ss, username);

            std::lock_guard<std::mutex> lock(g_fileMutex);
            Account a;
            if (loadAcc(username, a)) {
                std::string resp = "OK|" + std::to_string(a.getID()) + "|"
                                 + a.get_name() + "|"
                                 + a.getHash()  + "|"
                                 + a.getSalt();
                net_send(clientSock, resp);
            } else {
                net_send(clientSock, "ERR|Not found");
            }
        }

        // ── GET_NEXT_ID ───────────────────────────────────────────────
        else if (cmd == "GET_NEXT_ID") {
            std::lock_guard<std::mutex> lock(g_fileMutex);
            int id = getNextID();
            net_send(clientSock, "OK|" + std::to_string(id));
        }

        // ── SAVE_COUNTER ──────────────────────────────────────────────
        else if (cmd == "SAVE_COUNTER") {
            std::string n;
            getline(ss, n);
            std::lock_guard<std::mutex> lock(g_fileMutex);
            saveCounter(stoi(n));
            net_send(clientSock, "OK");
        }

        // ── ENTRY ─────────────────────────────────────────────────────
        else if (cmd == "ENTRY") {
            std::string serialized;
            getline(ss, serialized);

            // Append to entries log file
            std::lock_guard<std::mutex> lock(g_fileMutex);
            std::ofstream file("../data/entries.txt", std::ios::app);
            if (file) {
                file << serialized << "\n";
                net_send(clientSock, "OK");
            } else {
                net_send(clientSock, "ERR|Could not write entry");
            }
        }

        // ── Unknown ───────────────────────────────────────────────────
        else {
            net_send(clientSock, "ERR|Unknown command");
        }
    }

    closesocket(clientSock);
    std::cout << "[SERVER] Client disconnected.\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// Server  –  main listen loop
// ─────────────────────────────────────────────────────────────────────────────
void net_run_server() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "[SERVER] WSAStartup failed.\n";
        return;
    }

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "[SERVER] socket() failed.\n";
        return;
    }

    // Allow port reuse so restarts don't block
    int opt = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(NET_PORT);

    if (bind(listenSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "[SERVER] bind() failed.\n";
        return;
    }

    listen(listenSock, SOMAXCONN);
    std::cout << "[SERVER] Listening on port " << NET_PORT << "...\n";

    while (true) {
        SOCKET clientSock = accept(listenSock, nullptr, nullptr);
        if (clientSock == INVALID_SOCKET) continue;

        // Spawn a thread for each client
        std::thread(handle_client, clientSock).detach();
    }

    closesocket(listenSock);
    WSACleanup();
}
