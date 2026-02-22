#include"../lib/picosha2.h"
#include<iostream>
#include<random>
#include <chrono>
using namespace std;

class Password{
    string hash;
    string salt;

    string salt_generator(size_t length = 16){
        static const string chars =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        static mt19937 gen(chrono::high_resolution_clock::now().time_since_epoch().count());
        static uniform_int_distribution<> dis(0, chars.size() - 1);

        string salt;
        salt.reserve(length);

        for (size_t i = 0; i < length; i++)
            salt += chars[dis(gen)];

        return salt;
    }

public:
    Password(){}
    //When Account is Created
    Password(string inp){
        salt = salt_generator();
        inp+= salt;
        picosha2::hash256_hex_string(inp, hash);
    }

    //Account Reading from file
    void setHash(string h){hash = h;}
    void setSalt(string s){salt = s;}
    
    //Getters
    string getHash() const{return hash;}
    string getSalt() const{return salt;}

};
