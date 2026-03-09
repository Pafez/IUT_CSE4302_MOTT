#pragma once
#include "./Account.h"

class Session {
    static Account current;
    static bool active;

public:
    static void login(const Account& a);
    static void logout();
    static bool isActive();
    static Account& getAccount();
};