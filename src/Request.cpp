#include "Request.h"

Request::Request(int _reqID, int _reqFrom, double _amount, std::string _ref) : requestID(_reqID),
                                                                               requestFrom(_reqFrom),
                                                                               amount(_amount),
                                                                               ref(_ref)
{
}

Request::~Request() {}
