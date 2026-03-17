#pragma once

#include <iostream>
#include <unordered_map>
#include <exception>

class Transaction;
class Log;

class Table
{
private:
    const int tableID;
    std::string name;
    std::string fileName;
    std::unordered_map<int, double> accDebts;
    const int logID;

    void serializeAccDebts() const;
    void deserializeAccDebts();

    class AccountNotFound: public std::exception {
    public:
        const char* what() const noexcept override {
            return "Account not found in table.";
        }
    };

    class AccountAlreadyExists: public std::exception {
    public:
        const char* what() const noexcept override {
            return "Account already exists in table.";
        }
    };
public:
    Table(int TID, int LogID);
    ~Table();

    int getTableID() const {return tableID;}
    std::string getTableName() const {return name;}

    double getAccDebt(int accID) const;
    bool hasAccount(int accID) const;

    void addAccount(int accID, double initialDebt = 0.0);
    void processTransaction(const Transaction & transaction);
};
