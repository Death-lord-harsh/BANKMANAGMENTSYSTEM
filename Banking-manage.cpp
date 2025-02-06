#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> // for atof

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
        balance += amount;
        updateFile();
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            updateFile();
        } else {
            cout << "Insufficient balance." << endl;
        }
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
};

bool adminLogin(const string& adminPass) {
    const string correctAdminPass = "admin123"; // Example admin password
    return adminPass == correctAdminPass;
}

int main() {
    Account account;
    int choice;

    while (true) {
        cout << "Bank Management System" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Show Accounts" << endl;
        cout << "3. Login" << endl;
        cout << "4. Admin Login" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                account.createAccount();
                break;
            case 2:
                account.showAccounts();
                break;
            case 3: {
                string accNum, pass;
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Password: ";
                cin >> pass;
                if (account.login(accNum, pass)) {
                    cout << "Login successful!" << endl;
                    int transChoice;
                    while (true) {
                        cout << "1. Deposit" << endl;
                        cout << "2. Withdraw" << endl;
                        cout << "3. Logout" << endl;
                        cout << "Enter your choice: ";
                        cin >> transChoice;
                        if (transChoice == 1) {
                            double amount;
                            cout << "Enter amount to deposit: ";
                            cin >> amount;
                            account.deposit(amount);
                            cout << "Deposit successful. New balance: " << account.balance << endl;
                        } else if (transChoice == 2) {
                            double amount;
                            cout << "Enter amount to withdraw: ";
                            cin >> amount;
                            account.withdraw(amount);
                            cout << "Withdrawal successful. New balance: " << account.balance << endl;
                        } else if (transChoice == 3) {
                            break;
                        } else {
                            cout << "Invalid choice. Please try again." << endl;
                        }
                    }
                } else {
                    cout << "Invalid account number or password." << endl;
                }
                break;
            }
            case 4: {
                string adminPass;
                cout << "Enter Admin Password: ";
                cin >> adminPass;
                if (adminLogin(adminPass)) {
                    cout << "Admin login successful!" << endl;
                    // Add admin-specific functionality here
                } else {
                    cout << "Invalid admin password." << endl;
                }
                break;
            }
            case 5:
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
