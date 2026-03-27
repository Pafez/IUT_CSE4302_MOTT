#include "BackLog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

BackLog::BackLog(int _accountID)
{
    accountID = _accountID;

    try
    {
        deserialize();
    }
    catch (const std::exception &e)
    {
        std::cerr << "BackLog load failed: " << e.what() << '\n';
        nextRequestID = 1;
        currentRequests.clear();
    }
}

void BackLog::serialize()
{
    std::ofstream filen("../data/backlogs" + std::to_string(accountID) + ".txt");

    if (!filen)
        throw std::runtime_error("Could not open backlog file for writing.");

    filen << nextRequestID << '\n';

    if (!filen)
        throw std::runtime_error("Failed while writing backlog nextRequestID.");

    for (int i = 0; i < currentRequests.size(); i++)
    {
        filen << currentRequests[i].requestID << '|'
              << currentRequests[i].requestFrom << '|'
              << currentRequests[i].amount << '|'
              << currentRequests[i].ref << '\n';

        if (!filen)
            throw std::runtime_error("Failed while writing backlog request data.");
    }
}

void BackLog::deserialize()
{
    std::ifstream filen("../data/backlogs" + std::to_string(accountID) + ".txt");
    std::string line;

    currentRequests.clear();

    if (!filen)
    {
        nextRequestID = 1;
        return;
    }

    if (std::getline(filen, line))
    {
        try
        {
            nextRequestID = std::stoi(line);
        }
        catch (const std::exception &)
        {
            throw std::runtime_error("Invalid backlog header: nextRequestID is corrupted.");
        }
    }
    else
    {
        nextRequestID = 1;
        return;
    }

    int lineNumber = 1;

    while (std::getline(filen, line))
    {
        lineNumber++;

        try
        {
            std::stringstream ss(line);
            std::string reqid, req, amountStr, ref;

            if (!std::getline(ss, reqid, '|') ||
                !std::getline(ss, req, '|') ||
                !std::getline(ss, amountStr, '|') ||
                !std::getline(ss, ref))
            {
                throw std::runtime_error("Wrong field format");
            }

            int rid = std::stoi(reqid);
            int requestFrom = std::stoi(req);
            double amount = std::stod(amountStr);

            Request temp(rid, requestFrom, amount, ref);
            currentRequests.push_back(temp);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Skipping bad backlog line " << lineNumber
                      << ": " << e.what() << '\n';
        }
    }
}

void BackLog::addRequest(int _reqFrom, double _amount, std::string _ref)
{
    try
    {
        currentRequests.push_front(Request(nextRequestID, _reqFrom, _amount, _ref));
        nextRequestID++;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to add backlog request: " << e.what() << '\n';
    }
}

void BackLog::removeRequest(int _reqID)
{
    for (auto it = currentRequests.begin(); it != currentRequests.end(); ++it)
    {
        if (it->requestID == _reqID)
        {
            currentRequests.erase(it);
            return;
        }
    }

    std::cerr << "BackLog request ID not found: " << _reqID << '\n';
}

const std::deque<Request> &BackLog::getRequests() const
{
    return currentRequests;
}

// void BackLog::showRequests() const
// {
//     if (currentRequests.empty())
//     {
//         std::cout << "No pending requests.\n";
//         return;
//     }

//     for (int i = 0; i < currentRequests.size(); i++)
//     {
//         std::cout << "Request ID: " << currentRequests[i].requestID
//                   << ", From: " << currentRequests[i].requestFrom
//                   << ", Amount: " << currentRequests[i].amount
//                   << ", Ref: " << currentRequests[i].ref << '\n';
//     }
// }

BackLog::~BackLog()
{
    try
    {
        serialize();
    }
    catch (const std::exception &e)
    {
        std::cerr << "BackLog save failed: " << e.what() << '\n';
    }
}