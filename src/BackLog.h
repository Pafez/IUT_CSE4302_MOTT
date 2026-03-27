#pragma once

#include "Entry.h"
#include "Request.h"

#include <fstream>
#include <vector>
#include <deque>

class BackLog
{
private:
    int accountID;
    int nextRequestID;

public:
    std::deque<Request> currentRequests;
    BackLog(int _accountID);
    ~BackLog();

    //  "../data/backlogs" + accountID + ".txt"
    void serialize();
    void deserialize();

    void addRequest(int _reqFrom, double _amount, std::string _ref);
    void removeRequest(int _reqID);
};
