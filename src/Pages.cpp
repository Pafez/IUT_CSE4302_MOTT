#include "./Pages.h"
#include "./CLI.h"
#include "./Session.h"
#include "./Account.h"
#include <iostream>
#include <limits>

void startPage(){
    std::cout << "\n=== Account System ===\n"
    << "1. Register\n"
    << "2. Login\n"
    << "3. Exit\n"
    << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1) CLI::push(registerPage);
    else if (choice == 2) CLI::push(loginPage);
    else if (choice == 3) CLI::pop();  // exits
    else std::cout << "Invalid choice.\n";
}

void registerPage(){
    std::string username, password, confirm;

    std::cout << "\n--- Register ---\n"
    << "Username: ";  
    std::cin >> username;
    std::cout << "Password: ";  
    std::cin >> password;
    std::cout << "Confirm: ";   
    std::cin >> confirm;

    if(password != confirm) std::cout << "Passwords do not match!\n";
    else{
        Account a(username, password);
        if (save_acc(a)) std::cout << "Account created! Please log in.\n";
    }

    CLI::pop();  // back to startPage
}

void loginPage(){
    std::string username, password;

    std::cout << "\n--- Login ---\n"
    << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    Account a;
    if(loginAcc(username, password, a)){
        Session::login(a);
        CLI::replace(menuPage);  // replace so back = logout
    } 
    else{
        std::cout << "Invalid username or password.\n";
        CLI::pop();  // back to startPage
    }
}

void menuPage(){
    std::cout << "\n--- Menu ---\n"
    << "Welcome, " << Session::getAccount().get_name() << "\n"
    << "1. Make Entry\n"
    << "2. Pending Transactions\n"
    << "3. Print Transaction Log\n"
    << "4. Logout\n";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1) ;
    else if (choice == 2) ;
    else if (choice == 3) ;  // exits
    else if (choice == 4){
        Session::logout();
        CLI::pop();
    }
    else std::cout << "Invalid choice.\n";
}

