#include "Log.h"
#include <fstream>
#include <sstream>
#include <iostream>

Log::Log(int _accountID)
{
    accountID = _accountID;
    deserialize();
}

void Log::serialize()
{
    std::ofstream filen("data/logs" + std::to_string(accountID) + ".txt");

    if (!filen)
        return;

    filen << nextRequestID << '\n';

    for (int i = 0; i < requestHistory.size(); i++)
    {
        filen << requestHistory[i].requestID << '|'
              << requestHistory[i].requestFrom << '|'
              << requestHistory[i].amount << '|'
              << requestHistory[i].ref << '\n';
    }
}

void Log::deserialize()
{
    std::ifstream filen("data/logs" + std::to_string(accountID) + ".txt");
    std::string line;

    requestHistory.clear();

    if (!filen)
    {
        nextRequestID = 1;
        return;
    }

    if (std::getline(filen, line))
    {
        nextRequestID = std::stoi(line);
    }
    else
    {
        nextRequestID = 1;
        return;
    }

    while (std::getline(filen, line))
    {
        std::stringstream ss(line);
        std::string reqid, req, amountStr, ref;

        std::getline(ss, reqid, '|');
        std::getline(ss, req, '|');
        std::getline(ss, amountStr, '|');
        std::getline(ss, ref);

        int rid = std::stoi(reqid);
        int requestFrom = std::stoi(req);
        double amount = std::stod(amountStr);

        Request temp(rid, requestFrom, amount, ref);
        requestHistory.push_back(temp);
    }
}

void Log::addLogEntry(int _reqFrom, double _amount, std::string _ref)
{
    requestHistory.push_front(Request(nextRequestID, _reqFrom, _amount, _ref));
    nextRequestID++;
}

const std::deque<Request> &Log::getLogs() const
{
    return requestHistory;
}

void Log::showLogs() const
{
    if (requestHistory.empty())
    {
        std::cout << "No logs found.\n";
        return;
    }

    for (int i = 0; i < requestHistory.size(); i++)
    {
        std::cout << "Request ID: " << requestHistory[i].requestID
                  << ", From: " << requestHistory[i].requestFrom
                  << ", Amount: " << requestHistory[i].amount
                  << ", Ref: " << requestHistory[i].ref << '\n';
    }
}

Log::~Log()
{
    serialize();
}