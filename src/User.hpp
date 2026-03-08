#pragma once

#include <iostream>

class Table;

class User
{
private:
    const int userID;
    std::string name;
    std::string passHash;
    Table * tableRef;
public:
    User(int UID, std::string Name, std::string Pass);
    ~User();
};

