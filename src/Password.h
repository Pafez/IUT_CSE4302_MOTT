#include"lib/picosha2.h"
#include<iostream>
#include<random>
using namespace std;

class Password{
    string hash;
    string salt;

    string salt_generator(size_t length = 16){
        static const string chars =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, chars.size() - 1);

        string salt;
        for (size_t i = 0; i < length; i++)
            salt += chars[dis(gen)];

        return salt;
    }

public:
    Password(string inp){
        salt = salt_generator();
        picosha2::hash256_hex_string(inp, hash);
    }

    string getPass() const{return hash + salt;}
    string getSalt() const{return salt;}

};
