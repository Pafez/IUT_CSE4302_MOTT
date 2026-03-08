//#include "User.hpp"
#include "Entry.hpp"



    Entry :: Entry(){
        userId1 = 0000 ;
        userId2 = 0000 ;
    }
    Entry :: Entry(int a , int b){
        userId1 = a ;
        userId2 = b ;

    }
    
    Entry :: ~Entry(){}

    string Entry :: get_Request(int a ,  int b , int amount){

        ostringstream oss;

        oss << " User " << a << " gets " << amount << " Taka from User " << b << endl;
        string line  = oss.str();

        return line; 
    }

    string Entry :: give_Request(int a , int b , int amount ){

        ostringstream oss;

        oss << " User " << a << " gives " << amount << " Taka to User " << b << endl;
        string line = oss.str();

        return line;
    }
