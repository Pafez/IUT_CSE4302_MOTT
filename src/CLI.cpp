#include "./CLI.h"

std::stack<Page> CLI::pageStack;

void CLI::push(Page p){pageStack.push(p);}

void CLI::pop() {
    if (!pageStack.empty()) pageStack.pop();
}

void CLI::replace(Page p) {
    if (!pageStack.empty()) pageStack.pop();
    pageStack.push(p);
}

void CLI::run() {
    while (!pageStack.empty()) {
        pageStack.top()();  // run current page
    }
}