#include"./Password.h"

std::string Password::salt_generator(size_t length){
    static const std::string chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    static std::uniform_int_distribution<> dis(0, chars.size() - 1);

    std::string salt;
    salt.reserve(length);

    for (size_t i = 0; i < length; i++)
        salt += chars[dis(gen)];

    return salt;
}

Password::Password(){}
    //When Account is Created
Password::Password(std::string inp){
    salt = salt_generator();
    inp+= salt;
    picosha2::hash256_hex_string(inp, hash);
}

void Password::setHash(std::string h){hash = h;}
void Password::setSalt(std::string s){salt = s;}

std::string Password::getHash() const{return hash;}
std::string Password::getSalt() const{return salt;}