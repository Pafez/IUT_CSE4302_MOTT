#include "./Pages.h"
#include "./CLI.h"
#include "./Session.h"
#include "./Account.h"
#include "./Entry.h"
#include "./Network.h"
#include <iostream>
#include <limits>

// ─────────────────────────────────────────────────────────────────────────────
// Helpers: network-aware wrappers around Account file functions
// ─────────────────────────────────────────────────────────────────────────────

// Tries to register an account locally or remotely
static bool do_register(const std::string& username, const std::string& password) {
    if (net_is_client()) {
        return net_save_acc(username, password);
    }
    Account a(username, password);
    return save_acc(a);
}

// Tries to login locally or remotely; fills 'a' on success
static bool do_login(const std::string& username, const std::string& password, Account& a) {
    if (net_is_client()) {
        int id; std::string hash, salt;
        if (!net_login_acc(username, password, id, hash, salt)) return false;
        a.setData(id, username, hash, salt);
        return true;
    }
    return loginAcc(username, password, a);
}

// Tries to load an account by username locally or remotely; fills 'a' on success
static bool do_load_acc(const std::string& username, Account& a) {
    if (net_is_client()) {
        int id; std::string hash, salt;
        if (!net_load_acc(username, id, hash, salt)) return false;
        a.setData(id, username, hash, salt);
        return true;
    }
    return loadAcc(username, a);
}

// Saves a serialized entry locally or remotely
static bool do_save_entry(const std::string& serialized) {
    if (net_is_client()) {
        return net_send_entry(serialized);
    }
    // Local: append directly to entries file
    std::ofstream file("../data/entries.txt", std::ios::app);
    if (!file) return false;
    file << serialized << "\n";
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Pages
// ─────────────────────────────────────────────────────────────────────────────

void startPage() {
    std::cout << "\n=== Account System ===\n"
              << "1. Register\n"
              << "2. Login\n"
              << "3. Exit\n"
              << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if      (choice == 1) CLI::push(registerPage);
    else if (choice == 2) CLI::push(loginPage);
    else if (choice == 3) CLI::pop();
    else std::cout << "Invalid choice.\n";
}

void registerPage() {
    std::string username, password, confirm;

    std::cout << "\n--- Register ---\n"
              << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;
    std::cout << "Confirm: ";
    std::cin >> confirm;

    if (password != confirm) {
        std::cout << "Passwords do not match!\n";
    } else {
        if (do_register(username, password))
            std::cout << "Account created! Please log in.\n";
        else
            std::cout << "Username already taken.\n";
    }

    CLI::pop();
}

void loginPage() {
    std::string username, password;

    std::cout << "\n--- Login ---\n"
              << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    Account a;
    if (do_login(username, password, a)) {
        Session::login(a);
        CLI::replace(menuPage);
    } else {
        std::cout << "Invalid username or password.\n";
        CLI::pop();
    }
}

void menuPage() {
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

    if      (choice == 1) CLI::push(entryPage);
    else if (choice == 2) { /* TODO */ }
    else if (choice == 3) { /* TODO */ }
    else if (choice == 4) {
        Session::logout();
        CLI::pop();
    }
    else std::cout << "Invalid choice.\n";
}

void entryPage() {
    std::cout << "\n--- Select Entry Type ---\n"
              << "1. One to One\n"
              << "2. One to Many (Divide equally)\n"
              << "3. One to Many (Separate amounts)\n"
              << "4. Back\n"
              << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if      (choice == 1) CLI::push(template1Page);
    else if (choice == 2) CLI::push(template2Page);
    else if (choice == 3) CLI::push(template3Page);
    else if (choice == 4) CLI::pop();
    else std::cout << "Invalid choice.\n";
}

void template1Page() {
    std::cout << "\n--- One to One Request ---\n";

    Account temp;
    while (true) {
        std::cout << "Enter Recipient Username: ";
        std::string username;
        std::cin >> username;
        if (do_load_acc(username, temp)) break;
        std::cout << "Invalid Username. Try again.\n";
    }

    double amount;
    std::cout << "Enter Amount: ";
    std::cin >> amount;

    std::cout << "Enter Reference: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string reference;
    std::getline(std::cin, reference);

    Template1 a(temp.getID(), amount, reference);
    if (do_save_entry(a.serialize()))
        std::cout << "Entry saved successfully!\n";
    else
        std::cout << "Failed to save entry.\n";

    CLI::pop();
}

void template2Page() {
    std::cout << "\n--- One to Many (Divide Equally) ---\n";

    std::vector<int> recipientIDs;
    std::string input;

    while (true) {
        std::cout << "Enter Recipient Username (or 'done' to finish): ";
        std::cin >> input;

        if (input == "done") {
            if (recipientIDs.empty()) { std::cout << "Add at least one recipient.\n"; continue; }
            break;
        }

        Account temp;
        if (!do_load_acc(input, temp)) {
            std::cout << "Invalid Username. Try again.\n";
        } else {
            recipientIDs.push_back(temp.getID());
            std::cout << "Added " << input << ".\n";
        }
    }

    std::cout << "Enter Total Amount: ";
    int totalAmount;
    std::cin >> totalAmount;

    std::cout << "Enter Reference: ";
    std::string reference;
    std::cin >> reference;

    Template2 a(recipientIDs, totalAmount, reference);
    if (do_save_entry(a.serialize()))
        std::cout << "Entry saved successfully!\n";
    else
        std::cout << "Failed to save entry.\n";

    CLI::pop();
}

void template3Page() {
    std::cout << "\n--- One to Many (Different Amounts) ---\n";

    std::vector<int> recipientIDs;
    std::vector<double> amounts;
    std::string input;

    while (true) {
        std::cout << "Enter Recipient Username (or 'done' to finish): ";
        std::cin >> input;

        if (input == "done") {
            if (recipientIDs.empty()) { std::cout << "Add at least one recipient.\n"; continue; }
            break;
        }

        Account temp;
        if (!do_load_acc(input, temp)) {
            std::cout << "Invalid Username. Try again.\n";
        } else {
            recipientIDs.push_back(temp.getID());
            double amount;
            std::cout << "Enter Amount for " << input << ": ";
            std::cin >> amount;
            amounts.push_back(amount);
            std::cout << "Added " << input << " with amount " << amount << ".\n";
        }
    }

    std::cout << "Enter Reference: ";
    std::string reference;
    std::cin >> reference;

    Template3 a(recipientIDs, amounts, reference);
    if (do_save_entry(a.serialize()))
        std::cout << "Entry saved successfully!\n";
    else
        std::cout << "Failed to save entry.\n";

    CLI::pop();
}
