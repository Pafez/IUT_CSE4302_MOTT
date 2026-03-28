#pragma once

class Transaction
{
private:
    int accID;
    double debtIncAmount;
public:
    Transaction(int _accID, double _debtIncAmount);
    ~Transaction();

    int getAccID() const;
    double getDebtIncAmount() const;
};

 

