#include "./Session.h"

Account Session::current;
bool Session::active = false;

void Session::login(const Account& a){
    current = a;
    active = true;
}

void Session::logout(){
    current = Account();
    active = false;
}

bool Session::isActive(){return active;}

Account& Session::getAccount(){return current;}

/*Session Usage:

// on login
Account a;
if (loginAcc(username, password, a))
    Session::login(a);

// anywhere else in the codebase
if (Session::isActive()) {
    Account& acc = Session::getAccount();
    cout << acc.get_name();
}

// on logout
Session::logout();

*/