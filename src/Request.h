#pragma once

class Request
{
public:
    const int requestFrom;
    const double amount;

    Request(int _reqFrom, double _amount);
    ~Request();
};
