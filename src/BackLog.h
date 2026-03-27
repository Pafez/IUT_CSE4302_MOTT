#pragma once

#include "Request.h"
#include <deque>
#include <string>

class BackLog
{
private:
    int accountID;
    int nextRequestID;
    std::deque<Request> currentRequests;

public:
    BackLog(int _accountID);
    ~BackLog();

    void serialize();
    void deserialize();

    void addRequest(int _reqFrom, double _amount, std::string _ref);
    void removeRequest(int _reqID);

    const std::deque<Request> &getRequests() const;
    // void showRequests() const;
};