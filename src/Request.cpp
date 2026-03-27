#include "Request.h"

Request::Request(int _reqFrom, double _amount):
    requestFrom(_reqFrom),
    amount(_amount)
{}

Request::~Request() {}
