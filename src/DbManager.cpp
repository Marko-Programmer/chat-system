#include "../include/DbManager.h"

DbManager::DbManager(const std::string& server, const std::string& username, const std::string& password, const std::string& database) {
	driver = get_driver_instance();
	con = std::unique_ptr<sql::Connection>(driver->connect(server, username, password));
	con->setSchema(database);
}



// Users

std::unordered_map<std::string, User> DbManager::loadUsers() {
	std::unordered_map<std::string, User> dbUsers;
	
	try
	{

		std::unique_ptr<sql::Statement> stmt(con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT username, hashed_password FROM chatsystem.users"));

		while (res->next()) {
			std::string username = res->getString("username");
			std::string password = res->getString("hashed_password");
			dbUsers.insert({username, User(username, password)});
		}

	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error in loadUsers(): " << e.what() << std::endl;
	}

	return dbUsers;
}

void DbManager::registerUser(const User& newUser) {
	try
	{
		 
		std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO users (username, hashed_password) VALUES (?, ?);"));
  
		pstmt->setString(1, newUser.getUsername());
		pstmt->setString(2, newUser.getPassword());
		pstmt->executeUpdate();

	}

	catch (sql::SQLException& e) {
		std::cerr << "SQL error in registerUser(): " << e.what() << std::endl;
	}
}

int DbManager::getUserIdByUsername(const std::string& username) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> getIdStmt(con->prepareStatement("SELECT id FROM users WHERE username = ?"));
		getIdStmt->setString(1, username);
		std::unique_ptr<sql::ResultSet> res(getIdStmt->executeQuery());
		int userId = 0;
		if (res->next()) {
			userId = res->getInt("id");
		}
		else {
			std::cerr << "No user found with username: " << username << std::endl;
		}
		return userId;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error in getUserByUsername(): " << e.what() << std::endl;
		return 1;
	}
}

std::string DbManager::getUsernameByUserId(int id) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> getUsernameStmt(con->prepareStatement("SELECT username FROM users WHERE id = ?"));
		getUsernameStmt->setInt(1, id);
		std::unique_ptr<sql::ResultSet> res(getUsernameStmt->executeQuery());
		std::string username = "empty";
		if (res->next()) {
			username = res->getString("username");
		}
		else {
			std::cerr << "No user found with id: " << id << std::endl;
		}
		return username;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error in getUsernameByUserId(): " << e.what() << std::endl;
		return "error";
	}
}

void DbManager::addFriend(const std::string& senderUsername, const std::string& receiverUsername) {
	try {
		int senderId = getUserIdByUsername(senderUsername);
		int receiverId = getUserIdByUsername(receiverUsername);

		std::unique_ptr<sql::PreparedStatement> checkStmt(con->prepareStatement(
			"SELECT status, action_user_id FROM friendships "
			"WHERE (user_one_id = ? AND user_two_id = ?) "
			"   OR (user_one_id = ? AND user_two_id = ?);"
		));
		checkStmt->setInt(1, senderId);
		checkStmt->setInt(2, receiverId);
		checkStmt->setInt(3, receiverId);
		checkStmt->setInt(4, senderId);

		std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());

		if (res->next()) {
			std::string status = res->getString("status");
			int actionUserId = res->getInt("action_user_id");

			if (status == "pending") {

				if (actionUserId == senderId) {
					std::cout << "You already sent a friend request to " << receiverUsername << "." << std::endl;

					std::cout << "Do you want to cancel this invite?" << std::endl;
					std::cout << "[1] Cancel this invite" << std::endl;
					std::cout << "[2] Leave it" << std::endl;
					char choice;
					std::cin >> choice;
					std::cin.ignore();

					if (choice == '1') {
						std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
							"DELETE FROM friendships "
							"WHERE ((user_one_id = ? AND user_two_id = ?) OR (user_one_id = ? AND user_two_id = ?)) "
							"AND status = 'pending' AND action_user_id = ?"
						));
						stmt->setInt(1, senderId);
						stmt->setInt(2, receiverId);
						stmt->setInt(3, receiverId);
						stmt->setInt(4, senderId);
						stmt->setInt(5, senderId);
						stmt->executeUpdate();
						clearScreen();
						cancelFriendRequestLog(senderUsername, receiverUsername);
						std::cout << "Friend request to " << receiverUsername << " has been canceled.\n" << std::endl;
					}
					else if (choice == '2') {
						clearScreen();
						std::cout << "Friend request left unchanged.\n" << std::endl;
					}
					else {
						clearScreen();
						std::cout << "Invalid input. Nothing changed.\n" << std::endl;
					}

				}
				else {
					std::unique_ptr<sql::PreparedStatement> acceptStmt(con->prepareStatement(
						"UPDATE friendships SET status = 'accepted', action_user_id = ? "
						"WHERE (user_one_id = ? AND user_two_id = ?) "
						"   OR (user_one_id = ? AND user_two_id = ?);"
					));
					acceptStmt->setInt(1, senderId);
					acceptStmt->setInt(2, senderId);
					acceptStmt->setInt(3, receiverId);
					acceptStmt->setInt(4, receiverId);
					acceptStmt->setInt(5, senderId);
					acceptStmt->executeUpdate();

					acceptFriendRequestLog(senderUsername, receiverUsername);
					std::cout << "Friend request from " << receiverUsername << " accepted!\n" << std::endl;
				}
			}
			else if (status == "accepted") {
				std::cout << "You are already friends with " << receiverUsername << ".\n" << std::endl;
			}
		}
		else {
			std::unique_ptr<sql::PreparedStatement> insertStmt(con->prepareStatement(
				"INSERT INTO friendships(user_one_id, user_two_id, status, action_user_id) "
				"VALUES (?, ?, 'pending', ?);"
			));
			insertStmt->setInt(1, senderId);
			insertStmt->setInt(2, receiverId);
			insertStmt->setInt(3, senderId);
			insertStmt->executeUpdate();
			sendFriendRequestLog(senderUsername, receiverUsername);
			std::cout << "Friend request sent to " << receiverUsername << ".\n" << std::endl;
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error in addFriend(): " << e.what() << std::endl;
	}
}

void DbManager::deleteFriend(const std::string& senderUsername, const std::string& receiverUsername) {
	try
	{
		int senderId = getUserIdByUsername(senderUsername);
		int receiverId = getUserIdByUsername(receiverUsername);

		std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
			"DELETE FROM chatsystem.friendships "
			"WHERE(user_one_id = ? AND user_two_id = ? ) "
			"OR(user_one_id = ? AND user_two_id = ? ) "
		));
		stmt->setInt(1, senderId);
		stmt->setInt(2, receiverId);
		stmt->setInt(3, receiverId);
		stmt->setInt(4, senderId);
		stmt->execute();

		deleteFromFriendsLog(senderUsername, receiverUsername);
		std::cout << receiverUsername << " deleted from friends succesfully!\n" << std::endl;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error in deleteFriend(): " << e.what() << std::endl;
	}
}



// Log

void DbManager::logUserAction(const User& user, std::string action, std::string status, std::string description) {
	try
	{
 
		int userId = getUserIdByUsername(user.getUsername());

		std::unique_ptr<sql::PreparedStatement> insertLogStmt(con->prepareStatement("INSERT INTO user_logs(username_id, action, status, description) VALUES(?, ?, ?, ?)"));
		insertLogStmt->setInt(1, userId);
		insertLogStmt->setString(2, action);
		insertLogStmt->setString(3, status);
		insertLogStmt->setString(4, description);
		insertLogStmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error in registerLog(): " << e.what() << std::endl;
	}
}

void DbManager::registerLog(const User& newUser) {
	logUserAction(newUser, "register", "success", ""); 
} 

void DbManager::loginLog(const User& user) {
	logUserAction(user, "login", "success", "");
}

void DbManager::loginPassFailLog(const User& user) {
	logUserAction(user, "login", "failure", "Wrong password entered.");
} 

void DbManager::logoutLog(const User& user) {
	logUserAction(user, "logout", "success", "");
}


void DbManager::sendFriendRequestLog(const std::string& senderUsername, const std::string& receiverUsername) {
	User sender(senderUsername);
		logUserAction(sender, "friendship", "sent", senderUsername + " sent a friend request to  " + receiverUsername + ".");
}

void DbManager::cancelFriendRequestLog(const std::string& senderUsername, const std::string& receiverUsername) {
	User sender(senderUsername);
	logUserAction(sender, "friendship", "cancelled", senderUsername + " cancelled a friend request to " + receiverUsername + ".");
}

void DbManager::acceptFriendRequestLog(const std::string& senderUsername, const std::string& receiverUsername) {
	User sender(senderUsername);
	logUserAction(sender, "friendship", "accepted", senderUsername + " accepted a friend request from " + receiverUsername + ".");
}

void DbManager::deleteFromFriendsLog(const std::string& senderUsername, const std::string& receiverUsername) {
	User sender(senderUsername);
	logUserAction(sender, "friendship", "deleted", senderUsername + " removed " + receiverUsername + " from friends.");
}

// Print Users

std::vector<std::string> DbManager::printFriends(const User& user, int startNum) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> getIdStmt(con->prepareStatement("SELECT u.username FROM users u "
			"JOIN friendships f ON(u.id = f.user_one_id AND f.user_two_id = (SELECT id FROM users WHERE username = ?)) "
			"OR(u.id = f.user_two_id AND f.user_one_id = (SELECT id FROM users WHERE username = ?))"
			"WHERE f.status = 'accepted'; "));
		getIdStmt->setString(1, user.getUsername());
		getIdStmt->setString(2, user.getUsername());

		std::vector<std::string> usernames;
		std::unique_ptr<sql::ResultSet> res(getIdStmt->executeQuery());

		int i = startNum;
		while (res->next()) {
			std::string username = res->getString("username");
			usernames.push_back(username);
			std::cout << "[" << i++ << "] " << username << std::endl;
		} 

		if (usernames.empty()) {
			std::cout << "You don't have any friends yet." << std::endl;
		}

		return usernames;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQL error in printFriends(): " << e.what() << std::endl;
		return {};
	}
}

std::vector<std::pair<std::string, std::string>> DbManager::printUnknowns(const User& user) {
	try
	{
		std::unique_ptr<sql::PreparedStatement> getIdStmt(con->prepareStatement(
			"SELECT u.username,"
			"   CASE"
			"       WHEN f.status = 'pending' AND f.action_user_id = m.id THEN 'sent'"
			"       WHEN f.status = 'pending' AND f.action_user_id != m.id THEN 'received'"
			"       ELSE 'none'"
			"   END AS status"
			"   FROM chatsystem.users u"
			"   JOIN chatsystem.users m ON m.username = ?"
			"   LEFT JOIN chatsystem.friendships f"
			"   ON("
			"       (f.user_one_id = u.id AND f.user_two_id = m.id)"
			"       OR"
			"       (f.user_two_id = u.id AND f.user_one_id = m.id)"
			"   )"
			"   WHERE u.username != ?"
			"   AND("
			"       f.status IS NULL OR f.status != 'accepted'"
			"   );"
		));

		getIdStmt->setString(1, user.getUsername());
		getIdStmt->setString(2, user.getUsername());

		std::vector<std::pair<std::string, std::string>> users;  
		std::unique_ptr<sql::ResultSet> res(getIdStmt->executeQuery());

		int i = 1;
		while (res->next()) {
			std::string username = res->getString("username");
			std::string status = res->getString("status");
			users.push_back({username, status});

			if (status == "none") {
				std::cout << "[" << i++ << "] " << username << " - No request sent yet." << std::endl;
			}
			else if (status == "sent") {
				std::cout << "[" << i++ << "] " << username << " - You have already sent a friend request." << std::endl;
			}
			else if (status == "received") {
				std::cout << "[" << i++ << "] " << username << " - You have received a friend request, do you want to accept it?" << std::endl;
			}
		}

		if (users.empty()) {
			std::cout << "There are no more users to add as friends." << std::endl;
		}

		return users;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQL error in printUnknowns(): " << e.what() << std::endl;
		return {};
	}
}



// Messages

std::vector<Message> DbManager::loadMessages(const User& user, const std::string& receiverUsername) {
	std::vector<Message> messages;

	try
	{
		int senderId = getUserIdByUsername(user.getUsername());
		int receiverId = getUserIdByUsername(receiverUsername);

		std::unique_ptr<sql::PreparedStatement> stmt(
			con->prepareStatement(
				"SELECT sender_id, receiver_id, text, timestamp FROM messages "
				"WHERE (sender_id = ? AND receiver_id = ?) OR (sender_id = ? AND receiver_id = ?)"
				" ORDER BY timestamp"
			)
		);

		stmt->setInt(1, senderId);
		stmt->setInt(2, receiverId);
		stmt->setInt(3, receiverId);
		stmt->setInt(4, senderId);

		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());


		while (res->next()) {
			int sid = res->getInt("sender_id");
			int rid = res->getInt("receiver_id");
			std::string senderUsername = getUsernameByUserId(sid);
			std::string recvUsername = getUsernameByUserId(rid);
			std::string text = res->getString("text");
			std::string timestamp = res->getString("timestamp");
			messages.emplace_back(senderUsername, recvUsername, encryptDecrypt(text), timestamp);
		}

	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQL error in loadMessages(): " << e.what() << std::endl;
	}


	return messages;
}

void DbManager::sendMessage(const std::string& senderUsername, const std::string& receiverUsername, const std::string& text, const std::string& timestamp) {
	try
	{

		std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO messages (sender_id, receiver_id, text, timestamp)" 
																			"VALUES (?, ?, ?, ?);"));
		int sender_id = getUserIdByUsername(senderUsername);
		int receiver_id = getUserIdByUsername(receiverUsername);

		pstmt->setInt(1, sender_id);
		pstmt->setInt(2, receiver_id);
		pstmt->setString(3, encryptDecrypt(text));
		pstmt->setString(4, timestamp);
		pstmt->executeUpdate();

	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQL error in sendMessage(): " << e.what() << std::endl;
	}	 
}
 
std::vector<Message> DbManager::searchMessages(const User& user, const std::string& receiverUsername, const std::string& keyWord) {
	std::vector<Message> messages;
	try {
		int senderId = getUserIdByUsername(user.getUsername());
		int receiverId = getUserIdByUsername(receiverUsername);

		std::unique_ptr<sql::PreparedStatement> stmt(
			con->prepareStatement(
				"SELECT sender_id, receiver_id, text, timestamp FROM messages "
				"WHERE ((sender_id = ? AND receiver_id = ?) OR (sender_id = ? AND receiver_id = ?)) "
				"ORDER BY timestamp"
			)
		);

		stmt->setInt(1, senderId);
		stmt->setInt(2, receiverId);
		stmt->setInt(3, receiverId);
		stmt->setInt(4, senderId);

		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

		while (res->next()) {
			int sid = res->getInt("sender_id");
			int rid = res->getInt("receiver_id");
			std::string senderUsername = getUsernameByUserId(sid);
			std::string recvUsername = getUsernameByUserId(rid);
			std::string text = res->getString("text");
			std::string timestamp = res->getString("timestamp");
			 
			std::string decryptedText = encryptDecrypt(text);
			std::string lowerCaseMessage = toLowerCase(decryptedText); 
			std::string lowerCaseKeyword = toLowerCase(keyWord);

			if (std::regex_search(lowerCaseMessage, std::regex("\\b" + lowerCaseKeyword + "\\b"))) {
				messages.emplace_back(senderUsername, recvUsername, decryptedText, timestamp);
			}
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQL error in searchMessages(): " << e.what() << std::endl;
	}

	return messages;
}

void DbManager::deleteMessage(const std::string& senderUsername, const std::string& receiverUsername, int deleteIndex) {

	try
	{ 
		int messageId = 0;

		std::unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"SELECT id FROM ("
				"   SELECT id, ROW_NUMBER() OVER (ORDER BY timestamp) AS rn "
				"   FROM messages "
				"   WHERE (sender_id = ? AND receiver_id = ?) OR (sender_id = ? AND receiver_id = ?)"
				") AS numbered "
				"WHERE rn = ?"
			)
		);

		int senderId = getUserIdByUsername(senderUsername);
		int receiverId = getUserIdByUsername(receiverUsername);
		pstmt->setInt(1, senderId);
		pstmt->setInt(2, receiverId);
		pstmt->setInt(3, receiverId);
		pstmt->setInt(4, senderId);
		pstmt->setInt(5, deleteIndex);

		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		if (res->next()) {
			messageId = res->getInt("id");
		} 

		if (messageId != 0) {
			std::unique_ptr<sql::PreparedStatement> delStmt(
				con->prepareStatement("DELETE FROM messages WHERE id = ?")
			);
			delStmt->setInt(1, messageId);
			delStmt->executeUpdate();
		}
		else {
			std::cerr << "Message with given index not found." << std::endl;
		}
	} 
	catch (sql::SQLException& e)
	{
		std::cerr << "SQL error in deleteMessage(): " << e.what() << std::endl;
	}

}


std::string DbManager::encryptDecrypt(const std::string& text) {
	std::string key = "f5!e#*^B%qrg3@g";

	std::string result = text;

	for (int i = 0; i < result.length(); i++) {
		result[i] ^= key[i % key.length()];
	}

	return result;
}
