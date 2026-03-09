#pragma once
#include"../lib/picosha2.h"
#include<random>
#include<chrono>

class Password{
    std::string hash;
    std::string salt;

    std::string salt_generator(size_t length = 16);

public:
    Password();
    //When Account is Created
    Password(std::string inp);

    //Account Reading from file
    void setHash(std::string h);
    void setSalt(std::string s);
    
    //Getters
    std::string getHash() const;
    std::string getSalt() const;

};
