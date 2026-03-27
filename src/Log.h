#pragma once

#include "Request.h"
#include <deque>
#include <string>

class Log
{
private:
    int accountID;
    int nextRequestID;
    std::deque<Request> requestHistory;

public:
    Log(int _accountID);
    ~Log();

    void serialize();
    void deserialize();

    void addLogEntry(int _reqFrom, double _amount, std::string _ref);

    const std::deque<Request> &getLogs() const;
    void showLogs() const;
};