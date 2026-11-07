#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <conio.h>
#include <cstdio>
#include <limits>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <sodium.h>

using namespace std;


void userDashboard(string username);
bool verifyPassword(const string& password, const string& storedHash);

string getPassword()
{
    string password;
    char ch;

    while (true)
    {
        ch = _getch();

        if (ch == 13)
        {
            break;
        }
        else if (ch == 8)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            password += ch;
            cout << '*';
        }
    }

    cout << endl;

    return password;
}

void login()
{
    string username, password;
    string savedUsername, savedPassword;

    cout << "====================================================\n";
    cout << "                    USER LOGIN                     \n";
    cout << "====================================================\n";

    for (int attempt = 1; attempt <= 3; attempt++)
    {
        bool loginSuccessful = false;

        cout << "\nLogin Attempt " << attempt << " of 3\n";

        cout << "Enter username: ";
        getline(cin, username);

        cout << "Enter password: ";
        password = getPassword();

        ifstream userFile("users.txt");

        if (!userFile.is_open())
        {
            cout << "No registered users found. Please register first."
                 << endl;
            return;
        }

        while (userFile >> savedUsername >> savedPassword)
        {
           if (savedUsername == username &&
    verifyPassword(password, savedPassword))
{
    loginSuccessful = true;
    break;
}
        }

        userFile.close();

        if (loginSuccessful)
{
    cout << "\nLogin successful. Welcome, " << username << "!" << endl;

    cout << "\nDEBUG: Entering dashboard...\n";

    userDashboard(username);

    break;
}
        else
        {
            cout << "\nIncorrect username or password." << endl;

            if (attempt < 3)
            {
                cout << "Please try again." << endl;
            }
            else
            {
                cout << "Maximum login attempts reached. "
                     << "Returning to the main menu." << endl;
            }
        }
    }
}

// Function to hash a password
string hashPassword(const string& password)
{
    if (password.empty())
    {
        return "";
    }

    char hashedPassword[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(
        hashedPassword,
        password.c_str(),
        static_cast<unsigned long long>(password.length()),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE
    ) != 0)
    {
        return "";
    }

    return string(hashedPassword);
}

bool verifyPassword(const string& password, const string& storedHash)
{
    if (password.empty() || storedHash.empty())
    {
        return false;
    }

    return crypto_pwhash_str_verify(
        storedHash.c_str(),
        password.c_str(),
        static_cast<unsigned long long>(password.length())
    ) == 0;
}


void registerUser()
{
    string username, password, savedUsername, savedPassword;
    string confirmPassword, hashedPassword;
    bool usernameExists = false;

    cout << "====================================================\n";
    cout << "              USER REGISTRATION SYSTEM              \n";
    cout << "====================================================\n\n";

    while (true)
    {
        cout << "Enter new username: ";
        getline(cin, username);

        if (username.empty() || username.find_first_of(" \t\r\n") != string::npos)
        {
            cout << "Username cannot be empty or contain whitespace. Please try again.\n\n";
            continue;
        }

        usernameExists = false;

        ifstream checkFile("users.txt");

        while (checkFile >> savedUsername >> savedPassword)
        {
            if (savedUsername == username)
            {
                usernameExists = true;
                break;
            }
        }

        checkFile.close();

        if (usernameExists)
        {
            cout << "Username already exists. Please choose another username.\n\n";
        }
        else
        {
            break;
        }
    }

    cout << "\nEnter new password: ";
    password = getPassword();

    while (password.empty())
    {
        cout << "Password cannot be empty. Please enter a password: ";
        password = getPassword();
    }

    cout << "Confirm password: ";
    confirmPassword = getPassword();

    while (password != confirmPassword)
    {
        cout << "Passwords do not match. Please try again.\n";

        cout << "Confirm password: ";
        confirmPassword = getPassword();
    }

    cout << "Passwords match!" << endl;
    hashedPassword = hashPassword(password);

    if (hashedPassword.empty())
    {
        cout << "Password hashing failed. Registration unsuccessful.\n";
        return;
    }

    ofstream userFile("users.txt", ios::app);

    if (!userFile.is_open())
    {
        cout << "Error opening file. Registration failed.\n";
        return;
    }

    userFile << username << " " << hashedPassword << endl;

    if (!userFile)
    {
        cout << "Error saving user details.\n";
        userFile.close();
        return;
    }

    userFile.close();

    cout << "\nRegistration successful for user: "
         << username << endl;
}

void changePassword()
{
    string username, currentPassword;
    string newPassword, confirmPassword;
    string savedUsername, savedPassword;
    string tempUsername, tempPassword;

    cout << "====================================================\n";
    cout << "                  CHANGE PASSWORD                  \n";
    cout << "====================================================\n\n";

    cout << "\nEnter username: ";
    getline(cin, username);

    cout << "\nEnter current password: ";
    currentPassword = getPassword();

    ifstream userFile("users.txt");
    if (!userFile.is_open())
    {
        cout << "Error opening user file.\n";
        return;
    }

    bool userFound = false;
    while (userFile >> savedUsername >> savedPassword)
    {
        if (savedUsername == username && verifyPassword(currentPassword, savedPassword))
        {
            userFound = true;
            break;
        }
    }
    userFile.close();

    if (!userFound)
    {
        cout << "\nIncorrect username or password.\n";
        return;
    }

    cout << "\nEnter new password: ";
    newPassword = getPassword();

    while (newPassword.empty())
    {
        cout << "Password cannot be empty. Please try again: ";
        newPassword = getPassword();
    }

    cout << "Confirm new password: ";
    confirmPassword = getPassword();

    while (newPassword != confirmPassword)
    {
        cout << "Passwords do not match. Please try again.\n";

        cout << "Confirm new password: ";
        confirmPassword = getPassword();
    }

    cout << "\nNew passwords match!\n";

    const string hashedNewPassword = hashPassword(newPassword);
    if (hashedNewPassword.empty())
    {
        cout << "Password hashing failed. Password was not changed.\n";
        return;
    }

    ifstream originalFile("users.txt");
    if (!originalFile.is_open())
    {
        cout << "Error reopening user file.\n";
        return;
    }

    ofstream tempFile("temp.txt");
    if (!tempFile.is_open())
    {
        cout << "Error creating temporary file.\n";
        return;
    }

    bool passwordUpdated = false;
    while (originalFile >> tempUsername >> tempPassword)
    {
        if (tempUsername == username)
        {
            tempPassword = hashedNewPassword;
            passwordUpdated = true;
        }

        tempFile << tempUsername << " " << tempPassword << '\n';
        if (!tempFile)
        {
            break;
        }
    }

    const bool writeSucceeded = tempFile.good();
    originalFile.close();
    tempFile.close();

    if (!writeSucceeded || !tempFile || !passwordUpdated)
    {
        remove("temp.txt");
        cout << "Error writing updated user details. The original file was not changed.\n";
        return;
    }

    if (!MoveFileExA("temp.txt", "users.txt", MOVEFILE_REPLACE_EXISTING))
    {
        remove("temp.txt");
        cout << "Error replacing user file. The original file was not changed.\n";
        return;
    }

    cout << "\nPassword changed successfully!\n";
}

void viewAccountInformation(string username)
{
    cout << "\n====================================================\n";
    cout << "                ACCOUNT INFORMATION                \n";
    cout << "====================================================\n";

    cout << "\nUsername: " << username << endl;
    cout << "Account Status: Active" << endl;

    cout << "\n====================================================\n";
}

void userDashboard(string username)
{
    int choice = 0;

    do
    {
        cout << "\n====================================================\n";
        cout << "                  USER DASHBOARD                   \n";
        cout << "====================================================\n";

        cout << "\nWelcome, " << username << "!\n\n";

        cout << "1. View Account Information\n";
        cout << "2. Logout\n";

        cout << "\nEnter your choice: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid choice. Please enter 1 or 2.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            viewAccountInformation(username);
            cout << "\nPress Enter to return to the dashboard...";
            cin.get();
        }
        else if (choice == 2)
        {
            cout << "\nLogging out...\n";
        }
        else
        {
            cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 2);
}



int main()
{
    if (sodium_init() < 0)
{
    cout << "Security initialization failed!" << endl;
    return 1;
}

    int choice;
    do{
    // Display welcome message
    cout << "====================================================\n";
    cout << "              LOGIN AND REGISTRATION SYSTEM          \n";
    cout << "====================================================\n\n";

    cout << "Please select an option:\n";
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Change Password\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";

if (!(cin >> choice))
{
    cout << "\nInvalid input! Please enter a number.\n";

    cin.clear();
    cin.ignore(1000, '\n');

    continue;
}

cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (choice)
{
    case 1:
        login();
        break;

    case 2:
        registerUser();
        break;

    case 3:
        changePassword();
        break;

    case 4:
        cout << "Exiting program..." << endl;
        break;

    default:
        cout << "Invalid choice. Please try again." << endl;
    }
}while (choice != 4);
   return 0;
}
