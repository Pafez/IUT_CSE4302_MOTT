#pragma once

class Account;

class Transaction
{
private:
    int accID;
    double debtIncAmount;
public:
    Transaction();
    ~Transaction();

    int getAccID() const;
    double getDebtIncAmount() const;
};



