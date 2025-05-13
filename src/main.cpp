#include <iostream> 
#include <string> 
#include <unordered_map> 


#include "../include/utils.h"
#include "../include/EntryMenu.h"
#include "../include/DbManager.h"
#include "../include/User.h"
#include "../include/ChatMenu.h" 


int main(){

    auto config = loadConfig("config.txt");  
    DbManager database(config["SERVER"], config["DB_USERNAME"], config["DB_PASSWORD"], config["DB_NAME"]); 
    std::unordered_map<std::string, User> users = database.loadUsers();
    User* activeUser = nullptr;
     

    while(true){
        EntryMenu entryMenu(users, &database, activeUser);
        entryMenu.displayMenu();

        while (activeUser != nullptr) {
            ChatMenu chatMenu(activeUser, &database);
            chatMenu.displayChatMenu();
        }
    }


    return 0;
}