#include "../include/User.h"

User::User() : username(""), password(""), isActive(false) {}
User::User(std::string username) : username(username) {}
User::User(std::string username, std::string password): username(username), password(password), isActive(false) {}


void User::setUsername(const std::string& newUsername){
	username = newUsername;
}

std::string User::getUsername() const {
	return username;
}

void User::setPassword(const std::string& newPassword){
	password = newPassword;
}

std::string User::getPassword() const {
	return password;
}

bool User::checkPassword(const std::string& Password) {
	return password == Password;
}

void User::setActive(bool status){
	isActive = status;
}
 