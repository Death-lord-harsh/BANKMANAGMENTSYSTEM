#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib> // for atof and system
#include <ctime>   // for timestamp

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

void setColor(const string& colorCode) {
    cout << "\033[" << colorCode << "m";
}

void resetColor() {
    cout << "\033[0m";
}

void printBanner() {
    setColor("1;34"); // Bold Blue
    cout << R"(
                      ___   _   _  _ _  __   __  __   _   _  _   _   ___ ___ __  __ ___ _  _ _____ 
                     | _ ) /_\ | \| | |/ /  |  \/  | /_\ | \| | /_\ / __| __|  \/  | __| \| |_   _|
                     | _ \/ _ \| .` | ' <   | |\/| |/ _ \| .` |/ _ \ (_ | _|| |\/| | _|| .` | | |  
                     |___/_/ \_\_|\_|_|\_\  |_|  |_/_/ \_\_|\_/_/ \_\___|___|_|  |_|___|_|\_| |_|                                                                                                   
    )" << endl;
    resetColor();
}

class Account {
public:
    string accountNumber;
    string accountHolder;
    string address;
    int age;
    string gender;
    string dob;
    double balance;
    string password;

    void createAccount() {
    setColor("1;32"); // Bold Green
    cout << "Enter Account Number: ";
    resetColor();
    cin >> accountNumber;

    setColor("1;32"); // Bold Green
    cout << "Enter Account Holder Name: ";
    resetColor();
    cin >> accountHolder;

    setColor("1;32"); // Bold Green
    cout << "Enter Initial Balance: ";
    resetColor();
    cin >> balance;

    setColor("1;32"); // Bold Green
    cout << "Set Password: ";
    resetColor();
    cin >> password;

    setColor("1;32"); // Bold Green
    cout << "Enter Address: ";
    resetColor();
    cin.ignore(); // to ignore the newline character left by previous input
    getline(cin, address);

    setColor("1;32"); // Bold Green
    cout << "Enter Age: ";
    resetColor();
    cin >> age;

    setColor("1;32"); // Bold Green
    cout << "Enter Gender: ";
    resetColor();
    cin >> gender;

    setColor("1;32"); // Bold Green
    cout << "Enter Date of Birth (dd-mm-yyyy): ";
    resetColor();
    cin >> dob;

    saveToFile();
    saveToUserInfo();
}

void saveToUserInfo() {
    ofstream outFile("userinfo.txt", ios::app);
    if (outFile.is_open()) {
        outFile << accountNumber << "," << accountHolder << "," << address << "," << age << "," << gender << "," << dob << endl;
        outFile.close();
        setColor("1;32"); // Bold Green
        cout << "Account information saved to userinfo.txt successfully!" << endl;
        resetColor();
    } else {
        setColor("1;31"); // Bold Red
        cout << "Unable to open userinfo.txt for writing." << endl;
        resetColor();
    }
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
                // Load additional details from userinfo.txt
                ifstream userInfoFile("userinfo.txt");
                if (userInfoFile.is_open()) {
                    string userInfoLine;
                    while (getline(userInfoFile, userInfoLine)) {
                        stringstream ssUserInfo(userInfoLine);
                        string userAccNum, userAddress, userAge, userGender, userDob;
                        getline(ssUserInfo, userAccNum, ',');
                        getline(ssUserInfo, accountHolder, ',');
                        getline(ssUserInfo, userAddress, ',');
                        getline(ssUserInfo, userAge, ',');
                        getline(ssUserInfo, userGender, ',');
                        getline(ssUserInfo, userDob, ',');
                        if (userAccNum == accNum) {
                            address = userAddress;
                            age = stoi(userAge);
                            gender = userGender;
                            dob = userDob;
                            break;
                        }
                    }
                    userInfoFile.close();
                }
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
            setColor("1;34"); // Bold Blue
            cout << left << setw(15) << "Account Number" << setw(25) << "Account Holder" << setw(10) << "Balance" << endl;
            cout << "-------------------------------------------------------------" << endl;
            resetColor();
            while (getline(inFile, line)) {
                stringstream ss(line);
                string accountNumber, accountHolder, balanceStr, password;
                getline(ss, accountNumber, ',');
                getline(ss, accountHolder, ',');
                getline(ss, balanceStr, ',');
                getline(ss, password, ',');
                double balance = atof(balanceStr.c_str());
                setColor("0;36"); // Cyan
                cout << left << setw(15) << accountNumber << setw(25) << accountHolder << setw(10) << fixed << setprecision(2) << balance << endl;
                resetColor();
            }
            inFile.close();
        } else {
            setColor("1;31"); // Bold Red
            cout << "Unable to open file for reading." << endl;
            resetColor();
        }
    }

    void deposit(double amount) {
        if (amount <= 0) {
            setColor("1;31"); // Bold Red
            cout << "Invalid deposit amount." << endl;
            resetColor();
            return;
        }
        balance += amount;
        logTransaction("Deposit", amount);
        updateFile();
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            setColor("1;31"); // Bold Red
            cout << "Invalid withdrawal amount." << endl;
            resetColor();
            return;
        }
        if (amount <= balance) {
            balance -= amount;
            logTransaction("Withdraw", amount);
            updateFile();
        } else {
            setColor("1;31"); // Bold Red
            cout << "Insufficient balance." << endl;
            resetColor();
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
            setColor("1;31"); // Bold Red
            cout << "Unable to open file for updating." << endl;
            resetColor();
        }
        return accountDeleted;
    }

   void viewAccountDetails() {
    setColor("1;34"); // Bold Blue
    cout << "Account Number: " << accountNumber << endl;
    cout << "Account Holder: " << accountHolder << endl;
    cout << "Address: " << address << endl;
    cout << "Age: " << age << endl;
    cout << "Gender: " << gender << endl;
    cout << "Date of Birth: " << dob << endl;
    cout << "Balance: " << fixed << setprecision(2) << balance << endl;
    resetColor();
}

    void changePassword(const string& newPass) {
        if (newPass.empty()) {
            setColor("1;31"); // Bold Red
            cout << "Password cannot be empty." << endl;
            resetColor();
            return;
        }
        password = newPass;
        updateFile();
        setColor("1;32"); // Bold Green
        cout << "Password changed successfully!" << endl;
        resetColor();
    }

    void viewTransactionHistory() {
        ifstream inFile("transactions.txt");
        if (inFile.is_open()) {
            string line;
            setColor("1;34"); // Bold Blue
            cout << left << setw(10) << "Type" << setw(10) << "Amount" << setw(10) << "Balance" << setw(20) << "Timestamp" << endl;
            cout << "------------------------------------------------------------" << endl;
            resetColor();
            while (getline(inFile, line)) {
                stringstream ss(line);
                string type, amountStr, balanceStr, timestamp;
                getline(ss, type, ',');
                getline(ss, amountStr, ',');
                getline(ss, balanceStr, ',');
                getline(ss, timestamp, ',');
                setColor("0;36"); // Cyan
                cout << left << setw(10) << type << setw(10) << amountStr << setw(10) << balanceStr << setw(20) << timestamp << endl;
                resetColor();
            }
            inFile.close();
        } else {
            setColor("1;31"); // Bold Red
            cout << "No transaction history found." << endl;
            resetColor();
        }
    }

    void updateAccountInformation(const string& newAccountHolder) {
        setColor("1;33"); // Bold Yellow
        cout << "Enter new account holder name: ";
        resetColor();
        cin >> accountHolder;
        setColor("1;33"); // Bold Yellow
        cout << "Enter address: ";
        resetColor();
        cin.ignore(); // to ignore the newline character left by previous input
        getline(cin, address);
        setColor("1;33"); // Bold Yellow
        cout << "Enter age: ";
        resetColor();
        cin >> age;
        setColor("1;33"); // Bold Yellow
        cout << "Enter gender: ";
        resetColor();
        cin >> gender;
        setColor("1;33"); // Bold Yellow
        cout << "Enter date of birth (dd-mm-yyyy): ";
        resetColor();
        cin >> dob;

        // Save to userinfo.txt
        ofstream outFile("userinfo.txt", ios::app);
        if (outFile.is_open()) {
            outFile << accountNumber << "," << accountHolder << "," << address << "," << age << "," << gender << "," << dob << endl;
            outFile.close();
            setColor("1;32"); // Bold Green
            cout << "Account information updated and saved to userinfo.txt successfully!" << endl;
            resetColor();
        } else {
            setColor("1;31"); // Bold Red
            cout << "Unable to open userinfo.txt for writing." << endl;
            resetColor();
        }

        updateFile();
        setColor("1;32"); // Bold Green
        cout << "Account information updated successfully!" << endl;
        resetColor();
    }

private:
    void saveToFile() {
        ofstream outFile("accounts.txt", ios::app);
        if (outFile.is_open()) {
            outFile << accountNumber << "," << accountHolder << "," << balance << "," << password << endl;
            outFile.close();
            setColor("1;32"); // Bold Green
            cout << "Account created and saved successfully!" << endl;
            resetColor();
        } else {
            setColor("1;31"); // Bold Red
            cout << "Unable to open file for writing." << endl;
            resetColor();
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
            setColor("1;31"); // Bold Red
            cout << "Unable to open file for updating." << endl;
            resetColor();
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
            setColor("1;31"); // Bold Red
            cout << "Unable to open file for logging transaction." << endl;
            resetColor();
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
        clearScreen();
        printBanner();
        setColor("1;34"); // Bold Blue
        cout << "Welcome to the Bank Management System" << endl;
        setColor("1;32"); // Bold Green
        cout << "1. Login" << endl;
        cout << "2. Admin Login" << endl;
        cout << "3. Exit" << endl;
        setColor("1;33"); // Bold Yellow
        cout << "Enter your choice: ";
        resetColor();
        cin >> choice;
        clearScreen();

        switch (choice) {
            case 1: {
                string accNum, pass;
                setColor("1;33"); // Bold Yellow
                cout << "Enter Account Number: ";
                resetColor();
                cin >> accNum;
                setColor("1;33"); // Bold Yellow
                cout << "Enter Password: ";
                resetColor();
                cin >> pass;
                if (account.login(accNum, pass)) {
                    setColor("1;32"); // Bold Green
                    cout << "Login successful!" << endl;
                    resetColor();
                    int transChoice;
                    while (true) {
                        setColor("1;32"); // Bold Green
                        cout << "1. Deposit" << endl;
                        cout << "2. Withdraw" << endl;
                        cout << "3. View Account Details" << endl;
                        cout << "4. View Transaction History" << endl;
                        cout << "5. Change Password" << endl;
                        cout << "6. Logout" << endl;
                        setColor("1;33"); // Bold Yellow
                        cout << "Enter your choice: ";
                        resetColor();
                        cin >> transChoice;
                        clearScreen();
                        if (transChoice == 1) {
                            double amount;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter amount to deposit: ";
                            resetColor();
                            cin >> amount;
                            account.deposit(amount);
                            setColor("1;32"); // Bold Green
                            cout << "Deposit successful. New balance: " << account.balance << endl;
                            resetColor();
                        } else if (transChoice == 2) {
                            double amount;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter amount to withdraw: ";
                            resetColor();
                            cin >> amount;
                            account.withdraw(amount);
                            setColor("1;32"); // Bold Green
                            cout << "Withdrawal successful. New balance: " << account.balance << endl;
                            resetColor();
                        } else if (transChoice == 3) {
                            account.viewAccountDetails();
                            int backChoice;
                            setColor("1;32"); // Bold Green
                            cout << "1. Back to Account Menu" << endl;
                            cout << "2. Main Menu" << endl;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter your choice: ";
                            resetColor();
                            cin >> backChoice;
                            clearScreen();
                            if (backChoice == 2) {
                                break;
                            }
                        } else if (transChoice == 4) {
                            account.viewTransactionHistory();
                            int backChoice;
                            setColor("1;32"); // Bold Green
                            cout << "1. Back to Account Menu" << endl;
                            cout << "2. Main Menu" << endl;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter your choice: ";
                            resetColor();
                            cin >> backChoice;
                            clearScreen();
                            if (backChoice == 2) {
                                break;
                            }
                        } else if (transChoice == 5) {
                            string newPass;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter new password: ";
                            resetColor();
                            cin >> newPass;
                            account.changePassword(newPass);
                        } else if (transChoice == 6) {
                            break;
                        } else {
                            setColor("1;31"); // Bold Red
                            cout << "Invalid choice. Please try again." << endl;
                            resetColor();
                        }
                    }
                } else {
                    setColor("1;31"); // Bold Red
                    cout << "Invalid account number or password." << endl;
                    resetColor();
                }
                break;
            }
            case 2: {
                string adminPass;
                setColor("1;33"); // Bold Yellow
                cout << "Enter Admin Password: ";
                resetColor();
                cin >> adminPass;
                if (adminLogin(adminPass)) {
                    setColor("1;32"); // Bold Green
                    cout << "Admin login successful!" << endl;
                    resetColor();
                    int adminChoice;
                    while (true) {
                        setColor("1;32"); // Bold Green
                        cout << "1. Show Accounts" << endl;
                        cout << "2. Create Account" << endl;
                        cout << "3. Delete Account" << endl;
                        cout << "4. Update Account Information" << endl;
                        cout << "5. Logout" << endl;
                        setColor("1;33"); // Bold Yellow
                        cout << "Enter your choice: ";
                        resetColor();
                        cin >> adminChoice;
                        clearScreen();
                        if (adminChoice == 1) {
                            account.showAccounts();
                            int backChoice;
                            setColor("1;32"); // Bold Green
                            cout << "1. Back to Admin Menu" << endl;
                            cout << "2. Main Menu" << endl;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter your choice: ";
                            resetColor();
                            cin >> backChoice;
                            clearScreen();
                            if (backChoice == 2) {
                                break;
                            }
                        } else if (adminChoice == 2) {
                            account.createAccount();
                        } else if (adminChoice == 3) {
                            string accNum, pass;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter Account Number to delete: ";
                            resetColor();
                            cin >> accNum;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter Password: ";
                            resetColor();
                            cin >> pass;
                            if (account.deleteAccount(accNum, pass)) {
                                setColor("1;32"); // Bold Green
                                cout << "Account deleted successfully." << endl;
                                resetColor();
                            } else {
                                setColor("1;31"); // Bold Red
                                cout << "Invalid account number or password. Account not deleted." << endl;
                                resetColor();
                            }
                        } else if (adminChoice == 4) {
                            string newAccountHolder;
                            setColor("1;33"); // Bold Yellow
                            cout << "Enter new account holder name: ";
                            resetColor();
                            cin >> newAccountHolder;
                            account.updateAccountInformation(newAccountHolder);
                        } else if (adminChoice == 5) {
                            break;
                        } else {
                            setColor("1;31"); // Bold Red
                            cout << "Invalid choice. Please try again." << endl;
                            resetColor();
                        }
                    }
                } else {
                    setColor("1;31"); // Bold Red
                    cout << "Invalid admin password." << endl;
                    resetColor();
                }
                break;
            }
            case 3:
                return 0;
            default:
                setColor("1;31"); // Bold Red
                cout << "Invalid choice. Please try again." << endl;
                resetColor();
        }
    }
    return 0;
}
