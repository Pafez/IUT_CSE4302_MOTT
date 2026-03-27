#pragma once

#include <iostream>

class Request
{
public:
    const int requestID;
    const int requestFrom;
    const double amount;
    const std::string ref;

    Request(int _reqID, int _reqFrom, double _amount, std::string _ref);
    ~Request();
};
