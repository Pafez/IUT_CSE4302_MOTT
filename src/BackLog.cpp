#include "BackLog.h"
#include <fstream>
#include <sstream>

BackLog::BackLog(int _accountID)
{
    accountID = _accountID;
    deserialize();
}

void BackLog::serialize()
{
    std::ofstream filen("../data/backlogs" + std::to_string(accountID) + ".txt");

    filen << nextRequestID << '\n';

    for (int i = 0; i < currentRequests.size(); i++)
    {
        filen << currentRequests[i].requestID << '|' << currentRequests[i].requestFrom << '|' << currentRequests[i].amount << '|' << currentRequests[i].ref;
        filen << '\n';
    }
    filen.close();
}
// requestFrom|23.34|ref
// requestFrom|23.34|ref

void BackLog::deserialize()
{
    std::ifstream filen("../data/backlogs" + std::to_string(accountID) + ".txt");
    std::string line;

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
        currentRequests.push_back(temp);
    }
}

void BackLog::addRequest(int _reqFrom, double _amount, std::string _ref)
{
    currentRequests.push_front(Request(nextRequestID, _reqFrom, _amount, _ref));
    nextRequestID++;
}

void BackLog::removeRequest(int _reqID)
{
    for (int i = 0; i < currentRequests.size(); i++)
    {
        if (currentRequests[i].requestID == _reqID)
        {
            currentRequests.erase(currentRequests.begin() + i);
            return;
        }
    }
}

BackLog::~BackLog()
{
    serialize();
}
