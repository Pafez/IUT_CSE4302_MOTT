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

    string Template1 :: get_Request(int a ,  int b , int amount){

        ostringstream oss;

        oss << " User " << a << " gets " << amount << " Taka from User " << b << endl;
        string line  = oss.str();

        return line;
    }

    string Template1 :: give_Request(int a , int b , int amount ){

        ostringstream oss;

        oss << " User " << a << " gives " << amount << " Taka to User " << b << endl;
        string line = oss.str();

        return line;
    }

    string Template2 :: get_Request( int a , int amount , string arr[] , int num){

        ostringstream oss ;
        int size = num ;

        while(num--){
            oss << " User " << a << " gets " << amount/size << " Taka from User " << arr[num-1]  << "\n";
        }

        string line = oss.str();

        return line;
    }

    string Template2 :: give_Request(int a , int amount , string arr[] , int num ){

        ostringstream oss ;
        int size = num ;

        while(num--){
            oss << " User " << a << " gives " << amount/size << " Taka to User " << arr[num-1]  << "\n";
        }

        string line = oss.str();

        return line;
    }

    string Template3 :: get_Request(int a , int amount[] , string arr[], int num){

        ostringstream oss;
        int size = num;

        
        while(num--){
            oss << " User " << a << " gets" << amount[num-1] << " Taka from User " << arr[num-1]  << "\n";
        }

        string line = oss.str();

        return line;
    }

    string Template3 :: give_Request(int a , int amount[] , string arr[] ,int num ){

        ostringstream oss;
        int size = num;

        
        while(num--){
            oss << " User " << a << " gives" << amount[num-1] << " Taka to User " << arr[num-1]  << "\n";
        }

        string line = oss.str();

        return line;
    }


