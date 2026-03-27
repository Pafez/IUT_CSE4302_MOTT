#include"./Account.h"
#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

Account::Account(): id(0){}
Account::Account(std::string u, std::string p): id(getNextID()), pass(p), username(u){}

int Account::getID() const{return id;}
std::string Account::get_name() const{return username;}
std::string Account::getHash() const{return pass.getHash();}
std::string Account::getSalt() const{return pass.getSalt();}

void Account::setData(int i, string u, string h, string s){
        id = i;
        username = u;
        pass.setHash(h);
        pass.setSalt(s);
}

bool Account::verify_pass(std::string p){
        std::string temp;
        p += pass.getSalt();
        picosha2::hash256_hex_string(p, temp);
        return (pass.getHash() == temp);
}

int getNextID() {
    ifstream file("../data/counter.txt");
    int count = 1;
    if (file) file >> count;
    file.close();
    saveCounter(count + 1);
    return count;
}

void saveCounter(int count) {
    ofstream file("../data/counter.txt");
    file << count;
}

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

bool save_acc(const Account& a){
    Account temp;
    if(loadAcc(a.get_name(), temp)){
        cout << "Account \"" << a.get_name() << "\" already exists!\n";
        return false;
    }

    ofstream file("../data/accounts.txt", ios::app);
    if (!file.is_open()) {
        cout << "Error opening file!\n";
        return false;
    }

    file << a.getID() << '|' << a.get_name() << '|' << a.getHash() << '|' << a.getSalt() << endl;
    cout << "Account: \"" << a.get_name() << "\" saved!" << endl;

    file.close();
    return true;
}

bool check_line(const string& line, Account& a){
    stringstream ss(line);
    string id, user, hash, salt;

    getline(ss, id, '|');
    getline(ss, user, '|');
    getline(ss, hash, '|');
    getline(ss, salt);

    if (!salt.empty() && salt.back() == '\r') salt.pop_back();

    if (user.empty() || hash.empty() || salt.empty()) return false;

    a.setData(stoi(id), user, hash, salt);
    return true;
}

bool loadAcc(const string& searched, Account& a){
    ifstream file("../data/accounts.txt");

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

std::unordered_map<int, Account> loadAllAccounts() {
    std::unordered_map<int, Account> accounts;
    std::ifstream file("../data/accounts.txt");
    std::string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        Account temp;
        if (!check_line(line, temp)) continue;

        accounts[temp.getID()] = temp;
    }

    return accounts;
}