#include "Transaction.h"

Transaction::Transaction(int _accID, double _debtIncAmount):
    accID(_accID), debtIncAmount(_debtIncAmount)
{
}

Transaction::~Transaction()
{
}

int Transaction::getAccID() const {
    return accID;
}

double Transaction::getDebtIncAmount() const {
    return debtIncAmount;
}
