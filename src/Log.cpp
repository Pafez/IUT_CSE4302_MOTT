#include "Log.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

Log::Log(int _accountID)
{
    accountID = _accountID;

    try
    {
        deserialize();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Log load failed: " << e.what() << '\n';
        nextRequestID = 1;
        requestHistory.clear();
    }
}

void Log::serialize()
{
    std::ofstream filen("../data/logs" + std::to_string(accountID) + ".txt");

    if (!filen)
        throw std::runtime_error("Could not open log file for writing.");

    filen << nextRequestID << '\n';

    if (!filen)
        throw std::runtime_error("Failed while writing log nextRequestID.");

    for (int i = 0; i < requestHistory.size(); i++)
    {
        filen << requestHistory[i].requestID << '|'
              << requestHistory[i].requestFrom << '|'
              << requestHistory[i].amount << '|'
              << requestHistory[i].ref << '\n';

        if (!filen)
            throw std::runtime_error("Failed while writing log request data.");
    }
}

void Log::deserialize()
{
    std::ifstream filen("../data/logs" + std::to_string(accountID) + ".txt");
    std::string line;

    requestHistory.clear();

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
            throw std::runtime_error("Invalid log header: nextRequestID is corrupted.");
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
            requestHistory.push_back(temp);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Skipping bad log line " << lineNumber
                      << ": " << e.what() << '\n';
        }
    }
}

void Log::addRequest(int _reqFrom, double _amount, std::string _ref)
{
    try
    {
        requestHistory.push_front(Request(nextRequestID, _reqFrom, _amount, _ref));
        nextRequestID++;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to add log request: " << e.what() << '\n';
    }
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
    try
    {
        serialize();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Log save failed: " << e.what() << '\n';
    }
}