#pragma once

#include <iostream>
#include <sstream>
using namespace std;

class User;
class Log;
class Account;
class Session;

class Entry {
    private : 
    int userId1;
    int userId2;
    

    public : 

    Entry(){}

    Entry(int a , int b){}
    
    ~Entry(){}

    string get_Request(int a ,  int b , int amount){}

        

    string give_Request(int a , int b , int amount ){}

};

class Template1 : public Entry {
    private:
    int UserId1;
    int UserId2;

    public:

    string get_Request(int a ,  int b , int amount){}

    string give_Request(int a , int b , int amount ){}


};

class Template2 : public Entry {

    private:
    int userId;

    public :

    string get_Request(int a , int amount , string arr[] , int num ){}

    string give_Request(int a , int amount , string arr[] , int num ){}

};

class Template3 : public Entry {
    
    private :
    int userId;
    

    private:
    string get_Request(int a , int amount[] , string arr[] ,int num){}

    string give_Request(int a , int amount[] , string arr[] ,int num ){}

};

        