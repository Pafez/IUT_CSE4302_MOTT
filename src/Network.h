#pragma once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <functional>

#pragma comment(lib, "ws2_32.lib")

// ─── Port ────────────────────────────────────────────────────────────────────
constexpr int NET_PORT = 54321;

// ─── Message protocol helpers ────────────────────────────────────────────────
// All messages are newline-terminated plain text.
// Fields are separated by '|'.
//
// Client → Server:
//   REGISTER|username|password
//   LOGIN|username|password
//   LOAD_ACC|username
//   SAVE_ACC|id|username|hash|salt
//   ENTRY|<serialized entry string>
//   GET_NEXT_ID
//   SAVE_COUNTER|<n>
//
// Server → Client:
//   OK
//   OK|<data fields…>
//   ERR|<reason>

std::string  net_send_recv(SOCKET sock, const std::string& msg);   // client-side helper
void         net_send    (SOCKET sock, const std::string& msg);    // send a line
std::string  net_recv    (SOCKET sock);                            // receive a line

// ─── Server ──────────────────────────────────────────────────────────────────
// Call once from main() on the host machine.
// Blocks forever, spawning one thread per connected client.
void net_run_server();

// ─── Client ──────────────────────────────────────────────────────────────────
// Call once from main() on the client machine before anything else.
// Returns true on successful connection.
bool net_connect_to_server(const std::string& host);
void net_disconnect();
bool net_is_client();          // true when running as a connected client

// ─── Client-side remote versions of Account file functions ───────────────────
// These replace the real implementations when running as a client.
// Pages.cpp / Account.cpp call the same function names — the linker
// picks the right version based on net_is_client().
//
// You should call these wrappers instead of the originals when net_is_client().
bool        net_save_acc  (const std::string& username, const std::string& password);
bool        net_load_acc  (const std::string& username,
                           int& outID, std::string& outHash, std::string& outSalt);
bool        net_login_acc (const std::string& username, const std::string& password,
                           int& outID, std::string& outHash, std::string& outSalt);
int         net_get_next_id();
void        net_save_counter(int n);
bool        net_send_entry(const std::string& serialized);
