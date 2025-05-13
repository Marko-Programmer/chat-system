#pragma once
#include <iostream>
class User
{
private:

	std::string username;
	std::string password;
	bool isActive;

public:

	User();
	User(std::string username);
	User(std::string username, std::string password);

	void setUsername(const std::string& newUsername);
	std::string getUsername() const;
	void setPassword(const std::string& newPassword);
	std::string getPassword() const;
	void setActive(bool status);

	bool checkPassword(const std::string& Password);

};

