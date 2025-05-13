#pragma once
#include <iostream> 
#include <memory> 
#include <string> 
#include <unordered_map> 
#include <vector> 
#include <regex> 

#include "../include/User.h"
#include "../include/Message.h"
#include "../include/utils.h"

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>


class DbManager
{
private:
	sql::Driver* driver;
	std::unique_ptr<sql::Connection> con; 


public:
	DbManager(const std::string& server, const std::string& username, const std::string& password, const std::string& database);

	// Users
	std::unordered_map<std::string, User> loadUsers();
	void registerUser(const User& newUser);
	int getUserIdByUsername(const std::string& username);
	std::string getUsernameByUserId(int id);
	void addFriend(const std::string& senderUsername, const std::string& receiverUsername);
	void deleteFriend(const std::string& senderUsername, const std::string& receiverUsername);

	// Log
	void logUserAction(const User& user, std::string action, std::string status, std::string description);
	void registerLog(const User& newUser);
	void loginLog(const User& user);
	void loginPassFailLog(const User& user);
	void logoutLog(const User& user);

	void sendFriendRequestLog(const std::string& senderUsername, const std::string& receiverUsername);
	void cancelFriendRequestLog(const std::string& senderUsername, const  std::string& receiverUsername);
	void acceptFriendRequestLog(const std::string& receiverUsername, const  std::string& senderUsername);
	void deleteFromFriendsLog(const std::string& senderUsername, const std::string& receiverUsername);

	// Print Users
	std::vector<std::string> printFriends(const User& user, int startNum);
	std::vector<std::pair<std::string, std::string>> printUnknowns(const User& user);

	// Messages
	std::vector<Message> loadMessages(const User& user, const std::string& receiverUsername);
	void sendMessage(const std::string& senderUsername, const std::string& receiverUsername, const std::string& text, const std::string& timestamp);
	std::vector<Message> searchMessages(const User& user, const std::string& receiverUsername, const std::string& keyWord);
	void deleteMessage(const std::string& senderUsername, const std::string& receiverUsername, int deleteIndex);

	std::string encryptDecrypt(const std::string& text);


};
