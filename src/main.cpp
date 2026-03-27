#include "./CLI.h"
#include "./Pages.h"
#include "./Network.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    // ── Run as SERVER ────────────────────────────────────────────────────────
    // Usage:  program.exe server
    if (argc >= 2 && std::string(argv[1]) == "server") {
        std::cout << "=== MOTT Server Mode ===\n";
        net_run_server();   // blocks forever
        return 0;
    }

    // ── Run as CLIENT ────────────────────────────────────────────────────────
    // Usage:  program.exe client <server-ip>
    if (argc >= 3 && std::string(argv[1]) == "client") {
        std::string host = argv[2];
        std::cout << "=== MOTT Client Mode ===\n";
        std::cout << "Connecting to " << host << "...\n";

        if (!net_connect_to_server(host)) {
            std::cerr << "Failed to connect. Make sure the server is running.\n";
            return 1;
        }

        CLI::push(startPage);
        CLI::run();
        net_disconnect();
        return 0;
    }

    // ── Run LOCALLY (original behaviour, no networking) ──────────────────────
    std::cout << "=== MOTT Local Mode ===\n";
    std::cout << "Tip: run with 'server' or 'client <ip>' for network mode.\n\n";
    CLI::push(startPage);
    CLI::run();
    return 0;
}
