#include "./Pages.h"
#include "./CLI.h"
#include "./Session.h"
#include "./Account.h"
#include "./Entry.h"
#include "./BackLog.h"
#include "./Log.h"
#include "./Transaction.h"
#include "./Table.h"
#include <cctype>
#include <exception>
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
    Table table(0, 0);

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
        if (save_acc(a)) {
            std::cout << "Account created! Please log in.\n";
            try {
                table.addAccount(a.getID());
            }
            catch (const std::exception &) {
                std::cout << "Username already exists\n";
                return;
            }
        }
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
    << "4. Show Table Balance\n"
    << "5. Logout\n"
    << "Choice: ";

    std::string choiceS;
    std::cin >> choiceS;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int choice;
    try {
        choice = std::stoi(choiceS);
    }
    catch (std::exception &) {
        std::cout << "Invalid choice. Please use a Number.\n";
        return;
    }

    if (choice == 1) CLI::push(entryPage);
    else if (choice == 2) CLI::push(backlogPage);
    else if (choice == 3) CLI::push(userlogPage);
    else if (choice == 4) CLI::push(balancePage);
    else if (choice == 5){
        Session::logout();
        CLI::pop();
    }
    else std::cout << "Invalid choice.\n";
}

void entryPage(){
    std::cout << "\n---Select Entry Type---\n"
    << "1. One to One\n"
    << "2. One to Many (Divide an amount equally)\n"
    << "3. One to Many (Separate amounts)\n"
    << "4. Back\n"
    << "Choice: ";
    
    std::string choiceS;
    std::cin >> choiceS;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int choice;
    try {
        choice = std::stoi(choiceS);
    }
    catch (std::exception &) {
        std::cout << "Invalid choice. Please use a Number.\n";
        return;
    }

    if(choice == 1) CLI::push(template1Page);
    else if (choice == 2) CLI::push(template2Page);
    else if (choice == 3) CLI::push(template3Page);
    else if (choice == 4) CLI::pop();
    else std::cout << "Invalid choice.\n";
}

void template1Page(){
    std::cout << "\n---One to One Request---\n";
    
    Account temp;
    bool validRecipient = false;
    while(!validRecipient){
        std::cout << "Enter Recipient Username: ";
        std::string username;
        std::cin >> username;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if(!loadAcc(username, temp)){
            std::cout << "Invalid Username. Try again.\n";
        } else {
            validRecipient = true;
        }
    }

    double amount ;
    std::cout << "\nEnter Amount: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nEnter Reference: ";
    std::string reference;
    std::getline(std::cin, reference);

    Template1 a(temp.getID(), amount, reference);
    std::cout << "Entry created successfully!\n";

    BackLog thisBackLog(temp.getID());
    thisBackLog.addRequest(Session::getAccount().getID(), amount, reference);
    
    CLI::pop();  
}

void template2Page(){
    std::cout << "\n---One to Many (Divide Equally)---\n";
    
    std::vector<int> recipientIDs;
    std::string input;

    while(true){
        std::cout << "Enter Recipient Username (or 'done' to finish): ";
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nEnter Reference: ";
    std::string reference;
    std::getline(std::cin, reference);

    Template2 a(recipientIDs, totalAmount, reference);
    std::cout << "Entry created successfully!\n";

    std::vector<Template1> linearEntries = Entry::linearizeEntry(a, ManyReciept_OneAmount);
    for (int i = 0; i < linearEntries.size(); i++) {
        BackLog thisBackLog(linearEntries[i].getRecipients()[0]);
        thisBackLog.addRequest(Session::getAccount().getID(), linearEntries[i].getValues()[0], reference);
    }
         
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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            amounts.push_back(amount);
            std::cout << "Added " << input << " with amount " << amount << ".\n";
        }
    }

    std::cout << "\nEnter Reference: ";
    std::string reference;
    std::getline(std::cin, reference);

    Template3 a(recipientIDs, amounts, reference);
    std::cout << "Entry created successfully!\n";

    std::vector<Template1> linearEntries = Entry::linearizeEntry(a, ManyReciept_ManyAmount);
    for (int i = 0; i < linearEntries.size(); i++) {
        BackLog thisBackLog(linearEntries[i].getRecipients()[0]);
        thisBackLog.addRequest(Session::getAccount().getID(), linearEntries[i].getValues()[0], reference);
    }

    CLI::pop();  
}

void backlogPage(){
    std::unordered_map<int, Account> accData = loadAllAccounts();
    std::cout << "--------Pending requests--------\n";

    Account currentAccount = Session::getAccount();
    BackLog log(currentAccount.getID());
    Log history(currentAccount.getID());
    Table currentTable(0, 0);
    int size = log.currentRequests.size();

    if(size){
        for(int i=0; i<size; i++){
            std::cout << i+1 << ". ";
            int fromID = log.currentRequests[i].requestFrom;
            double amount = log.currentRequests[i].amount;
            std::string reference = log.currentRequests[i].ref;
            Account fromAcc = accData[fromID];

            std::cout << fromAcc.get_name() + ": " + std::to_string(amount) + ". Reference: " + reference + "\n"; //1. Name: Amount. Reference
        }
        bool done = false;

        while (!done and log.currentRequests.size())
        {
            std::cout << "Choose which to respond to or \"done\" if done: ";
            std::string choice;
            int int_choice;
            
            while(true){
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if(choice == "done" or choice == "Done"){
                    done = true;
                    break;
                }
                else{
                    try{
                        int_choice = std::stoi(choice);
                    }
                    catch(std::exception& e){
                        std::cout << "\nInvalid."
                        << "\nTry again: ";
                        continue;
                    }
                    int currentSize = static_cast<int>(log.currentRequests.size());
                    if(int_choice<1 or int_choice>currentSize){
                        std::cout << "\nInvalid choice."
                        << "\nTry again: ";
                    }
                    else break;
                }
            }

            if(done) break;

            std::cout << "\nAccept? (Y/N) or C for Cancel: ";
            char ans;
            while(true){
                std::cin >> ans;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ans = std::toupper(ans);
                if(ans != 'Y' and ans != 'N' and ans != 'C'){
                    std::cout << "\nPlease make valid choice."
                    << "\nTry Again: ";
                }
                else break;
            }
            if(ans == 'C') continue;
            else if(ans == 'Y'){
                std::cout << "Request Accepted!";

                const Request selectedRequest = log.currentRequests[int_choice-1];

                Transaction payerTransaction(selectedRequest.requestFrom, -selectedRequest.amount);
                Transaction payeeTransaction(currentAccount.getID(), selectedRequest.amount);
                currentTable.processTransaction(payerTransaction);
                currentTable.processTransaction(payeeTransaction);
                history.addLogEntry(selectedRequest.requestFrom, selectedRequest.amount, selectedRequest.ref);
                Log(selectedRequest.requestFrom).addLogEntry(currentAccount.getID(), -selectedRequest.amount, selectedRequest.ref);
                log.removeRequest(selectedRequest.requestID);
            }
            else{                       //For N
                std::cout << "Request Denied!";
                log.removeRequest(log.currentRequests[int_choice-1].requestID);
            }
        }
    }
    else{
        std::cout << "No Pending Requests\n";
    }
    CLI::pop();
}

void userlogPage(){
    std::unordered_map<int, Account> accData = loadAllAccounts();
    std::cout << "---------------LOG---------------\n";

    Account currentAccount = Session::getAccount();
    Log log(currentAccount.getID());
    Table currentTable(0, 0);
    int size = log.getLogs().size();

    if(size){
        for(int i=0; i<size; i++){
            std::cout << i+1 << ". ";
            int fromID = log.getLogs()[i].requestFrom;
            double amount = log.getLogs()[i].amount;
            std::string reference = log.getLogs()[i].ref;
            Account fromAcc = accData[fromID];

            std::cout << fromAcc.get_name() + ": " + std::to_string(amount) + ". Reference: " + reference + "\n"; //1. Name: Amount. Reference
        }
        std::cout << "\nEnter anything to go Back.\n";
        std::string why;
        std::getline(std::cin, why);
    }
    else{
        std::cout << "Nothing to see!\n";
    }
    CLI::pop();
}

void balancePage(){
    std::unordered_map<int, Account> accData = loadAllAccounts();
    Table currentTable(0, 0);
    Account currentAccount = Session::getAccount();
    int currentID = currentAccount.getID();

    double currentUserDebt = 0.0;
    if (currentTable.hasAccount(currentID)) {
        currentUserDebt = currentTable.getAccDebt(currentID);
    }

    std::cout << "Your Balance: " << -currentUserDebt << "\n\n";
    std::cout << "Others' Balance:\n";

    for (const auto& kv : accData) {
        int accID = kv.first;
        if (accID == currentID) {
            continue;
        }
        if (!currentTable.hasAccount(accID)) {
            continue;
        }

        const Account& account = kv.second;
        double userDebt = currentTable.getAccDebt(accID);
        std::cout << account.get_name() << ": " << -userDebt << "\n";
    }

    std::cout << "\nEnter anything to go Back.\n";
    std::string input;
    std::getline(std::cin, input);
    CLI::pop();
}