#include "../include/EntryMenu.h"
#include "../include/utils.h"

EntryMenu::EntryMenu(std::unordered_map<std::string, User>& users, DbManager* database, User*& activeUser) : users(users), database(database), activeUser(activeUser) {}


void EntryMenu::registration() {
    clearScreen();

    std::cout << "Enter your Username: ";
    std::string username; 
    getline(std::cin, username);

    while (!isValidInput(username)) {
        std::cout << "It is not valid Username, try again: ";
        getline(std::cin, username);
    }

    while (users.find(username) != users.end()) {
        std::cout << "This Username is already taken. Please try another one: ";
        getline(std::cin, username);
    }

    std::cout << "Enter password: ";
    std::string password;
    getline(std::cin, password);

    while (!isValidInput(password)) {
        std::cout << "It is not valid password, try again: ";
        getline(std::cin, password);
    }

    std::string hashed_pass = std::to_string(simpleHash(password));

    User newUser(username, hashed_pass);
    users.insert({ username, newUser });

    database->registerUser(newUser);
    database->registerLog(newUser);

    users[username].setActive(true);
    activeUser = &users[username];


    clearScreen();


    std::cout << "Registration successful " << username << "!" << std::endl;
}


void EntryMenu::login() {
    clearScreen(); 

    std::cout << "Enter your Username: ";
    std::string username;
    getline(std::cin, username);

    while (!isValidInput(username)) {
        std::cout << "It is not valid Username, try again: ";
        getline(std::cin, username);
    }

    while (users.find(username) == users.end()) {
        std::cout << "This Username does not exist. Please enter your Username: ";
        getline(std::cin, username);
    }

    activeUser = &users[username];


    std::cout << "Enter password: ";
    std::string password; 
    getline(std::cin, password);


    while (!isValidInput(password)) {
        std::cout << "It is not valid password, try again: ";
        getline(std::cin, password);
    }
 


    password = std::to_string(simpleHash(password)); 

    while (!activeUser->checkPassword(password)) {
        std::cout << "It is not correct password, try again: ";
        getline(std::cin, password);

        password = std::to_string(simpleHash(password));
        database->loginPassFailLog(*activeUser);
    }


    clearScreen();
    database->loginLog(*activeUser);
    users[username].setActive(true);
    std::cout << "Hello " << username << "!\n" << std::endl;  


}

void EntryMenu::exit() {
    clearScreen();
    std::cout << "Goodbye! See you soon!\n";
    std::exit(0);
}

void EntryMenu::displayMenu() {
    std::cout << "=== Entry Menu ===" << std::endl;
    std::cout << "\t[1] To register" << std::endl;
    std::cout << "\t[2] To login" << std::endl;
    std::cout << "\t[0] To exit" << std::endl;
    std::cout << "Choose the option: ";
    char choice;

    std::cin >> choice;
    std::cin.ignore();



    while (choice < '0' || choice > '2') {
        std::cout << "That's uncorrect choice, try again: ";
        std::cin >> choice;
        std::cin.ignore();
    }

    switch (choice)
    {
    case '1':
        registration();
        break;

    case '2':
        login();
        break;

    case '0':
        exit();
        break;
    }
}