#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> // for atof and system
#include <ctime>   // for timestamp

using namespace std;

class Account {
public:
    string accountNumber;
    string accountHolder;
    double balance;
    string password;

    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Account Holder Name: ";
        cin >> accountHolder;
        cout << "Enter Initial Balance: ";
        cin >> balance;
        cout << "Set Password: ";
        cin >> password;
        saveToFile();
    }

    bool login(const string& accNum, const string& pass) {
        ifstream inFile("accounts.txt");
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string storedAccNum, storedAccHolder, storedBalanceStr, storedPassword;
                getline(ss, storedAccNum, ',');
                getline(ss, storedAccHolder, ',');
                getline(ss, storedBalanceStr, ',');
                getline(ss, storedPassword, ',');
                if (storedAccNum == accNum && storedPassword == pass) {
                    accountNumber = storedAccNum;
                    accountHolder = storedAccHolder;
                    balance = atof(storedBalanceStr.c_str());
                    password = storedPassword;
                    return true;
                }
            }
            inFile.close();
        }
        return false;
    }

    void showAccounts() {
        ifstream inFile("accounts.txt");
        if (inFile.is_open()) {
            string line;
            cout << left << setw(15) << "Account Number" << setw(25) << "Account Holder" << setw(10) << "Balance" << endl;
            cout << "-------------------------------------------------------------" << endl;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string accountNumber, accountHolder, balanceStr, password;
                getline(ss, accountNumber, ',');
                getline(ss, accountHolder, ',');
                getline(ss, balanceStr, ',');
                getline(ss, password, ',');
                double balance = atof(balanceStr.c_str());
                cout << left << setw(15) << accountNumber << setw(25) << accountHolder << setw(10) << fixed << setprecision(2) << balance << endl;
            }
            inFile.close();
        } else {
            cout << "Unable to open file for reading." << endl;
        }
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount." << endl;
            return;
        }
        balance += amount;
        logTransaction("Deposit", amount);
        updateFile();
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount." << endl;
            return;
        }
        if (amount <= balance) {
            balance -= amount;
            logTransaction("Withdraw", amount);
            updateFile();
        } else {
            cout << "Insufficient balance." << endl;
        }
    }

    bool deleteAccount(const string& accNum, const string& pass) {
        ifstream inFile("accounts.txt");
        ofstream tempFile("temp.txt");
        bool accountDeleted = false;
        if (inFile.is_open() && tempFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string storedAccNum, storedAccHolder, storedBalanceStr, storedPassword;
                getline(ss, storedAccNum, ',');
                getline(ss, storedAccHolder, ',');
                getline(ss, storedBalanceStr, ',');
                getline(ss, storedPassword, ',');
                if (!(storedAccNum == accNum && storedPassword == pass)) {
                    tempFile << line << endl;
                } else {
                    accountDeleted = true;
                }
            }
            inFile.close();
            tempFile.close();
            remove("accounts.txt");
            rename("temp.txt", "accounts.txt");
        } else {
            cout << "Unable to open file for updating." << endl;
        }
        return accountDeleted;
    }

    void viewAccountDetails() {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << endl;
    }

    void changePassword(const string& newPass) {
        if (newPass.empty()) {
            cout << "Password cannot be empty." << endl;
            return;
        }
        password = newPass;
        updateFile();
        cout << "Password changed successfully!" << endl;
    }

    void viewTransactionHistory() {
        ifstream inFile("transactions.txt");
        if (inFile.is_open()) {
            string line;
            cout << left << setw(10) << "Type" << setw(10) << "Amount" << setw(10) << "Balance" << setw(20) << "Timestamp" << endl;
            cout << "------------------------------------------------------------" << endl;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string type, amountStr, balanceStr, timestamp;
                getline(ss, type, ',');
                getline(ss, amountStr, ',');
                getline(ss, balanceStr, ',');
                getline(ss, timestamp, ',');
                cout << left << setw(10) << type << setw(10) << amountStr << setw(10) << balanceStr << setw(20) << timestamp << endl;
            }
            inFile.close();
        } else {
            cout << "No transaction history found." << endl;
        }
    }

    void updateAccountInformation(const string& newAccountHolder) {
        if (newAccountHolder.empty()) {
            cout << "Account holder name cannot be empty." << endl;
            return;
        }
        accountHolder = newAccountHolder;
        updateFile();
        cout << "Account information updated successfully!" << endl;
    }

private:
    void saveToFile() {
        ofstream outFile("accounts.txt", ios::app);
        if (outFile.is_open()) {
            outFile << accountNumber << "," << accountHolder << "," << balance << "," << password << endl;
            outFile.close();
            cout << "Account created and saved successfully!" << endl;
        } else {
            cout << "Unable to open file for writing." << endl;
        }
    }

    void updateFile() {
        ifstream inFile("accounts.txt");
        ofstream tempFile("temp.txt");
        if (inFile.is_open() && tempFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string storedAccNum, storedAccHolder, storedBalanceStr, storedPassword;
                getline(ss, storedAccNum, ',');
                getline(ss, storedAccHolder, ',');
                getline(ss, storedBalanceStr, ',');
                getline(ss, storedPassword, ',');

                if (storedAccNum == accountNumber) {
                    tempFile << accountNumber << "," << accountHolder << "," << balance << "," << password << endl;
                } else {
                    tempFile << line << endl;
                }
            }
            inFile.close();
            tempFile.close();
            remove("accounts.txt");
            rename("temp.txt", "accounts.txt");
        } else {
            cout << "Unable to open file for updating." << endl;
        }
    }

    void logTransaction(const string& type, double amount) {
        ofstream outFile("transactions.txt", ios::app);
        if (outFile.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now);
            outFile << type << "," << amount << "," << balance << "," << dt;
            outFile.close();
        } else {
            cout << "Unable to open file for logging transaction." << endl;
        }
    }
};

bool adminLogin(const string& adminPass) {
    const string correctAdminPass = "1234"; // Admin password set to 1234
    return adminPass == correctAdminPass;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bank Management System");
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")) {
        cout << "Error loading font" << endl;
        return -1;
    }

    Account account;
    int choice;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Create menu text
        sf::Text title("Bank Management System", font, 30);
        title.setFillColor(sf::Color::Black);
        title.setPosition(200, 50);

        sf::Text option1("1. Login", font, 20);
        option1.setFillColor(sf::Color::Black);
        option1.setPosition(200, 150);

        sf::Text option2("2. Admin Login", font, 20);
        option2.setFillColor(sf::Color::Black);
        option2.setPosition(200, 200);

        sf::Text option3("3. Exit", font, 20);
        option3.setFillColor(sf::Color::Black);
        option3.setPosition(200, 250);

        // Display menu
        window.draw(title);
        window.draw(option1);
        window.draw(option2);
        window.draw(option3);

        window.display();
    }

    return 0;
}
