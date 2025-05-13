#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream> 
#include <map> 
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>  



bool isValidInput(const std::string& input);

void clearScreen();

std::size_t simpleHash(const std::string& password);

std::map<std::string, std::string> loadConfig(const std::string& filename);

std::string getCurrentTimestamp();

int getValidInput();

std::string toLowerCase(std::string text);  
 
