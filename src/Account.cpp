#include"./Account.h"
#include<fstream>
#include<sstream>
using namespace std;

/*Testing
int main(){

    // string test = "asdrl-aooh2";
    // string name = "Central";
    // Account a(name, test);
    // save_acc(a);

    // Account c;
    // string test2 = "Password345";
    // if(loginAcc(name, test, c)){
    //     cout << "Login Successful: " << c.getSalt() << endl;
    // }
    // else cout << "Login Failed: " << c.getSalt() << endl;

}
*/

void save_acc(Account a){
    ofstream file("accounts.txt", ios::app);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    file << a.get_name() << '|' << a.getHash() << '|' << a.getSalt() << endl;

    cout << "Account: \"" << a.get_name() << "\" saved!" << endl;

    file.close();
}

bool check_line(const string& line, Account& a){
    stringstream ss(line);
    string user, hash, salt;

    getline(ss, user, '|');
    getline(ss, hash, '|');
    getline(ss, salt);

    if (!salt.empty() && salt.back() == '\r')   
    salt.pop_back();

    if (user.empty() || hash.empty() || salt.empty())
        return false;

    a.setData(user, hash, salt);
    return true;
}

bool loadAcc(const string& searched, Account& a){
    ifstream file("accounts.txt");

    string line;

    while(getline(file, line)){
        if(line.empty()) continue;

        Account temp;

        if(!check_line(line, temp)) continue; //incase of incomplete line
        if(temp.get_name() == searched){
            a = temp;
            return true;
        }
    }

    return false;
}

bool loginAcc(const string& username, const string& password, Account &a){
    Account temp;
    if(loadAcc(username, temp)){
        cout << "Found account!" << endl;
    }
    else{
        cout << "Account not found!" << endl;
        return false;
    }

    if(temp.verify_pass(password)){
        a = temp;
        return true;
    }
    else return false;

}