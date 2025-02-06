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
        password = newPass;
        updateFile();
        cout << "Password changed successfully!" << endl;
    }

    void viewTransactionHistory() {
        // Implement transaction history viewing functionality here
    }

    void updateAccountInformation(const string& newAccountHolder) {
        accountHolder = newAccountHolder;
        updateFile();
        cout << "Account information updated successfully!" << endl;
    }

    void closeAccount() {
        // Implement account closing functionality here
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
    const string correctAdminPass = "1234"; // Admin password set to 1234
    return adminPass == correctAdminPass;
}

int main() {
    Account account;
    int choice;

    while (true) {
        cout << "Bank Management System" << endl;
        cout << "1. Login" << endl;
        cout << "2. Admin Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
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
                        cout << "3. View Account Details" << endl;
                        cout << "4. Change Password" << endl;
                        cout << "5. Logout" << endl;
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
                            account.viewAccountDetails();
                        } else if (transChoice == 4) {
                            string newPass;
                            cout << "Enter new password: ";
                            cin >> newPass;
                            account.changePassword(newPass);
                        } else if (transChoice == 5) {
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
            case 2: {
                string adminPass;
                cout << "Enter Admin Password: ";
                cin >> adminPass;
                if (adminLogin(adminPass)) {
                    cout << "Admin login successful!" << endl;
                    int adminChoice;
                    while (true) {
                        cout << "1. Show Accounts" << endl;
                        cout << "2. Create Account" << endl;
                        cout << "3. Delete Account" << endl;
                        cout << "4. View Transaction History" << endl;
                        cout << "5. Update Account Information" << endl;
                        cout << "6. Close Account" << endl;
                        cout << "7. Logout" << endl;
                        cout << "Enter your choice: ";
                        cin >> adminChoice;
                        if (adminChoice == 1) {
                            account.showAccounts();
                        } else if (adminChoice == 2) {
                            account.createAccount();
                        } else if (adminChoice == 3) {
                            string accNum, pass;
                            cout << "Enter Account Number to delete: ";
                            cin >> accNum;
                            cout << "Enter Password: ";
                            cin >> pass;
                            if (account.deleteAccount(accNum, pass)) {
                                cout << "Account deleted successfully." << endl;
                            } else {
                                cout << "Invalid account number or password. Account not deleted." << endl;
                            }
                        } else if (adminChoice == 4) {
                            account.viewTransactionHistory();
                        } else if (adminChoice == 5) {
                            string newAccountHolder;
                            cout << "Enter new account holder name: ";
                            cin >> newAccountHolder;
                            account.updateAccountInformation(newAccountHolder);
                        } else if (adminChoice == 6) {
                            account.closeAccount();
                        } else if (adminChoice == 7) {
                            break;
                        } else {
                            cout << "Invalid choice. Please try again." << endl;
                        }
                    }
                } else {
                    cout << "Invalid admin password." << endl;
                }
                break;
            }
            case 3:
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
