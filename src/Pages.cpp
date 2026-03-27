#include "./Pages.h"
#include "./CLI.h"
#include "./Session.h"
#include "./Account.h"
#include "./Entry.h"
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
    << "4. Logout\n"
    << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1) CLI::push(entryPage);
    else if (choice == 2) ;
    else if (choice == 3) ;  
    else if (choice == 4){
        Session::logout();
        CLI::pop();
    }
    else std::cout << "Invalid choice.\n";
}

void entryPage(){
    std::cout << "\n---Select Entry Type---\n"
    << "1. One to One\n"
    << "2. One to Many (Divide an amount equally)\n"
    << "3. One to Many (Seperate amounts)\n"
    << "4. Back\n"
    << "Choice: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(choice == 1) CLI::push(template1Page);
    else if (choice == 2) CLI::push(template2Page);
    else if (choice == 3) CLI::push(template3Page);
    else if (choice == 4) CLI::pop();
    else std::cout << "Invalid choice.\n";
}

void template1Page(){
    std::cout << "\n---One to One Request---\n";
    
    Retry:
    std::cout << "Enter Recipient: ";
    std::string username;
    std::cin >> username;
    Account temp;
    if(!loadAcc(username, temp)){
        std::cout << "Invalid Username. Try again.\n";
        goto Retry;
    }

    std::cout << "\nEnter Amount: ";
    double amount;
    std::cin >> amount;

    std::cout << "\nEnter Reference: ";
    std::string reference;
    std::cin >> reference;

    Template1 a(temp.getID(), amount, reference);
}

void template1Page(){
    std::cout << "\n---One to One Request---\n";
    
    Account temp;
    bool validRecipient = false;
    while(!validRecipient){
        std::cout << "Enter Recipient Username: ";
        std::string username;
        std::cin >> username;
        
        if(!loadAcc(username, temp)){
            std::cout << "Invalid Username. Try again.\n";
        } else {
            validRecipient = true;
        }
    }

    double amount ;
    std::cout << "\nEnter Amount: ";
    std::cin >> amount;

    std::cout << "\nEnter Reference: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string reference;
    std::getline(std::cin, reference);

    Template1 a(temp.getID(), amount, reference);
    std::cout << "Entry created successfully!\n";
    
    CLI::pop();  
}

void template2Page(){
    std::cout << "\n---One to Many (Divide Equally)---\n";
    
    std::vector<int> recipientIDs;
    std::string input;

    while(true){
        std::cout << "Enter Recipient Username (or 'done' to finish): ";
        std::cin >> input;
        
        if(input == "done"){
            if(recipientIDs.empty()){
                std::cout << "Please add at least one recipient.\n";
                continue;
            }
            break;
        }

        Account temp;
        if(!loadAcc(input, temp)){
            std::cout << "Invalid Username. Try again.\n";
        } else {
            recipientIDs.push_back(temp.getID());
            std::cout << "Added " << input << ".\n";
        }
    }

    std::cout << "\nEnter Total Amount: ";
    int totalAmount;
    std::cin >> totalAmount;

    std::cout << "\nEnter Reference: ";
    std::string reference;
    std::cin >> reference;

    Template2 a(recipientIDs, totalAmount, reference);
    CLI::pop();  
}

void template3Page(){
    std::cout << "\n---One to Many (Different Amounts)---\n";
    
    std::vector<int> recipientIDs;
    std::vector<double> amounts;
    std::string input;

    while(true){
        std::cout << "Enter Recipient Username (or 'done' to finish): ";
        std::cin >> input;
        
        if(input == "done"){
            if(recipientIDs.empty()){
                std::cout << "Please add at least one recipient.\n";
                continue;
            }
            break;
        }

        Account temp;
        if(!loadAcc(input, temp)){
            std::cout << "Invalid Username. Try again.\n";
        } else {
            recipientIDs.push_back(temp.getID());
            
            std::cout << "Enter Amount for " << input << ": ";
            double amount;
            std::cin >> amount;
            amounts.push_back(amount);
            std::cout << "Added " << input << " with amount " << amount << ".\n";
        }
    }

    std::cout << "\nEnter Reference: ";
    std::string reference;
    std::cin >> reference;

    Template3 a(recipientIDs, amounts, reference);
    CLI::pop();  
}