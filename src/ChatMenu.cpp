#include "../include/ChatMenu.h"



ChatMenu::ChatMenu(User*& activeUser, DbManager* database): activeUser(activeUser), database(database){}
 

// Chat menus 

void ChatMenu::displayChatMenu() {

    std::cout << "=== Chat Menu ===" << std::endl;
    std::cout << "[1] Add new Friend" << std::endl;
    std::cout << "[2] Delete Friend\n" << std::endl;

    std::cout << "-- Your Friends --" << std::endl;
    std::vector<std::string> usernames = database->printFriends(*activeUser, 3);

    std::cout << "\n[0] Exit Profile" << std::endl;
    std::cout << "Choose the option: ";

    int choice;
    std::cin >> choice;

    int maxChoice = usernames.size() + 2;
    while (choice < 0 || choice > maxChoice) {
        std::cout << "That's uncorrect choice, try again: ";
        std::cin >> choice;
        std::cin.ignore();
    }

    std::string chatPartner;

    if (choice == 0) {
        clearScreen();
        std::cout << activeUser->getUsername() << ", you have successfully logged out.\n" << std::endl;
        database->logoutLog(*activeUser);
        activeUser = nullptr;
    }
    else if (choice == 1) {
        addFriend();
    }
    else if (choice == 2) {
        deleteFriend();
    }
    else if (choice >= 3 || choice <= usernames.size() + 3) {
        chatPartner = usernames[choice - 3];
        manageChat(chatPartner);
    }
}

void ChatMenu::manageChat(const std::string& chatPartner) {

    char choice;

    do {
        clearScreen();
        std::cout << "Chat with " << chatPartner << std::endl;
        std::cout << "\t1. Send message" << std::endl;
        std::cout << "\t2. Search messages by keyword" << std::endl;
        std::cout << "\t3. Delete message" << std::endl;
        std::cout << "\t0. Exit chat" << std::endl;
        std::cout << "Choose the option: ";

        std::cin >> choice;
        std::cin.ignore();



        while (choice < '0' || choice > '3') {
            std::cout << "That's uncorrect choice, try again: ";
            std::cin >> choice;
            std::cin.ignore();
        }

        switch (choice)
        {
        case '1':
            sendMessage(*activeUser, chatPartner);
            break;

        case '2':
            searchByKeyword(*activeUser, chatPartner);
            break;

        case '3':
            deleteMessage(*activeUser, chatPartner);
            break;

        case '0':
            clearScreen();
            std::cout << "Exiting chat with " << chatPartner << "...\n" << std::endl;
            break;
        }

    } while (choice != '0');

}



// Chat activity 

void ChatMenu::sendMessage(User& activeUser, const std::string& chatPartner) {

    std::string sendMessage;
    std::vector<Message> messages = database->loadMessages(activeUser, chatPartner);

    do {
    clearScreen();
    std::cout << "Chat with " << chatPartner << "\n" << std::endl;

    
    for (Message message : messages) {
        std::cout << "[" << message.getTimestamp() << "] " << message.getSenderUsername() << ": " << message.getText() << "\n" << std::endl;
    }
     

    std::cout << "Write a message [/exit]: ";
    getline(std::cin, sendMessage);


    if (!sendMessage.empty() && sendMessage != "/exit") {
        std::string timestamp = getCurrentTimestamp();
        Message msg(activeUser.getUsername(), chatPartner, sendMessage, timestamp);
        messages.push_back(msg);
        database->sendMessage(activeUser.getUsername(), chatPartner, sendMessage, timestamp);
    }

    } while (sendMessage != "/exit");
}

void ChatMenu::searchByKeyword(User& activeUser, const std::string& chatPartner) {
    clearScreen();
    std::string keyWord;

    std::cout << "Chat with " << chatPartner << std::endl;
    std::cout << "\nWrite a key word to find a message [/exit]: ";
    getline(std::cin, keyWord);

    while(keyWord != "/exit") {

        std::vector<Message> messages = database->searchMessages(activeUser, chatPartner, keyWord);

        clearScreen();
        std::cout << "Chat with " << chatPartner << std::endl;
        std::cout << "Messages with '" << keyWord << "' word:\n" << std::endl;

        if (messages.empty()) {
            std::cout << "There is no messages with '" << keyWord << "' word." << std::endl;
        }
        else {
            for (Message message : messages) {
                std::cout << "[" << message.getTimestamp() << "] " << message.getSenderUsername() << ": " << message.getText() << "\n" << std::endl;
            }
        }
         
         
        std::cout << "Write a key word to find a message [/exit]: ";
        getline(std::cin, keyWord);
    } 
}

void ChatMenu::deleteMessage(User& activeUser, const std::string& chatPartner) {

    clearScreen();
    int deleteIndex;

    std::vector<Message> messages = database->loadMessages(activeUser, chatPartner);


    std::cout << "Chat with " << chatPartner << "\n" << std::endl;

    int i = 1;
    for (Message message : messages) {
        std::cout << "\t" << i++ << ". [" << message.getTimestamp() << "] " << message.getSenderUsername() << ": " << message.getText() << "\n" << std::endl;
    }

    if (messages.size() == 0) {
        std::cout << "The chat is empty!\n" << std::endl;
    }

    std::cout << "\Write the index of the message that you want to delete [0 to exit]: ";
    std::cin >> deleteIndex;

    while (deleteIndex != 0){

        if (deleteIndex > 0 && deleteIndex <= messages.size()) {

            database->deleteMessage(activeUser.getUsername(), chatPartner, deleteIndex);
            messages.erase(messages.begin() + deleteIndex - 1); 

            clearScreen();
            std::cout << "Updated chat with " << chatPartner << "\n";
            int j = 1;
            for (const Message& message : messages) {
                std::cout << "\t" << j++ << ". [" << message.getTimestamp() << "] " << message.getSenderUsername() << ": " << message.getText() << "\n" << std::endl;
            }
        }
        else {
            std::cout << "Invalid index. Please try again.\n" << std::endl;
        }

        std::cout << "Write the index of the message that you want to delete [0 to exit]: ";
        std::cin >> deleteIndex;
    }
}



// Friends activity

void ChatMenu::addFriend() {

    int unknownChoice;
    clearScreen();

    do {
        std::cout << "=== Add Friend ===\n" << std::endl;
        std::vector<std::pair<std::string, std::string>> unknowns = database->printUnknowns(*activeUser);
        std::cout << "\n[0] Exit Profile" << std::endl;
        std::cout << "Enter the number of the friend to send request: ";

        unknownChoice = getValidInput();

        if (unknownChoice == 0) {
            clearScreen();
            break;
        }

        clearScreen();
        if (unknownChoice >= 1 && unknownChoice <= unknowns.size()) {
            std::string receiverUsername = unknowns[unknownChoice - 1].first;
            database->addFriend(activeUser->getUsername(), receiverUsername);
        }
        else {
            std::cout << "Invalid choice. Please try again.\n" << std::endl;
        }

    } while (true);

}

void ChatMenu::deleteFriend() {

    int unknownChoice;
    clearScreen();

    do {
        std::cout << "=== Delete Friend ===\n" << std::endl;
        std::vector<std::string> friendsUsernames = database->printFriends(*activeUser, 1);
        std::cout << "\n[0] Exit Profile" << std::endl;
        std::cout << "Enter the number of the friend to send request: ";

        unknownChoice = getValidInput();

        if (unknownChoice == 0) {
            clearScreen();
            break;
        }

        clearScreen();
        if (unknownChoice >= 1 && unknownChoice <= friendsUsernames.size()) {
            std::string receiverUsername = friendsUsernames[unknownChoice - 1];
            database->deleteFriend(activeUser->getUsername(), receiverUsername);
        }
        else {
            std::cout << "Invalid choice. Please try again.\n" << std::endl;
        }

    } while (true);

}
