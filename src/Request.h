#pragma once

#include <iostream>

class Request
{
public:
    int requestID;
    int requestFrom;
    double amount;
    std::string ref;

    Request(int _reqID, int _reqFrom, double _amount, std::string _ref);
    ~Request();
};
