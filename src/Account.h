#pragma once
#include"./Password.h"

class Account{
    int id;
    std::string username;
    Password pass;
public:
    Account();
    //Account Creation
    Account(std::string u, std::string p);

    //Reading Account from file
    void setData(int i, std::string u, std::string h, std::string s);

    //Getters
    int getID() const;
    std::string get_name()const;
    std::string getHash() const;
    std::string getSalt() const;

    //Password verification
    bool verify_pass(std::string p);

};

int getNextID();
void saveCounter(int count);
bool save_acc(const Account& a);
bool check_line(const std::string& line, Account& a);
bool loadAcc(const std::string& searched, Account& a);
bool loginAcc(const std::string& username, const std::string& password, Account &a);
