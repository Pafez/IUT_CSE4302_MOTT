#pragma once

#include <iostream>
#include <unordered_map>

class User;
class Log;

class Table
{
private:
    const int tableID;
    std::string name;
    std::unordered_map<int, User *> userRefs;
    std::unordered_map<int, int> userDebts;
    Log * logRef;
public:
    Table(int TID);
    ~Table();
};
