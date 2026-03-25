#pragma once

class Transaction
{
private:
    const int accID;
    const double debtIncAmount;
public:
    Transaction(int _accID, double _debtIncAmount);
    ~Transaction();

    int getAccID() const;
    double getDebtIncAmount() const;
};

 

