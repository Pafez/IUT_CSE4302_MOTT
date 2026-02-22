#pragma once
#include"./Password.h"
using namespace std;

class Account{
    string username;
    Password pass;
public:
    Account(){}
    //Account Creation
    Account(string u, string p):pass(p), username(u){}

    //Reading Account from file
    void setData(string u, string h, string s){
        username = u;
        pass.setHash(h);
        pass.setSalt(s);
    }

    //Getters
    string get_name() const{return username;}
    string getHash(){return pass.getHash();}
    string getSalt(){return pass.getSalt();}

    //Password verification
    bool verify_pass(string p){
        string temp;
        p += pass.getSalt();
        picosha2::hash256_hex_string(p, temp);
        return (pass.getHash() == temp);
    }

};


void save_acc(Account a);
bool check_line(const string& line, Account& a);
bool loadAcc(const string& searched, Account& a);
bool loginAcc(const string& username, const string& password, Account &a);
