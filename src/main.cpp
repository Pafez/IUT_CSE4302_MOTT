#include "./CLI.h"
#include "./Pages.h"

int main() {
    CLI::push(startPage);
    CLI::run();
    return 0;
}