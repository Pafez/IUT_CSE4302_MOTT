#pragma once
#include <functional>
#include <stack>
#include <string>

// a Page is a function that returns the next page
using Page = std::function<void()>;

class CLI {
    static std::stack<Page> pageStack;

public:
    static void push(Page p);   // go to a new page
    static void pop();          // go back
    static void replace(Page p); // go to page without ability to go back
    static void run();           // start the CLI
};