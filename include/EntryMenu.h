#pragma once
#include <iostream> 
#include <string> 
#include <unordered_map> 
#include "../include/utils.h"
#include "../include/User.h"
#include "../include/DbManager.h"


class EntryMenu {
private:
	std::unordered_map<std::string, User> &users;
	DbManager* database;
	User*& activeUser;

public:
	EntryMenu(std::unordered_map<std::string, User>& users, DbManager* database, User*& activeUser);

	void displayMenu();

	void registration();
	void login();
	void exit();

};

