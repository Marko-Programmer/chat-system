#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/utils.h"

TEST_CASE("User password verification works", "[input]") {
    REQUIRE(isValidInput("CorrectInput"));
    REQUIRE_FALSE(isValidInput("Uncorrect input"));
    REQUIRE_FALSE(isValidInput(""));
}


TEST_CASE("simpleHash produces consistent results", "[hash]") {
    std::string password = "mypassword123";
    REQUIRE(simpleHash(password) == simpleHash(password));
    REQUIRE(simpleHash("admin") != simpleHash("user"));
}


TEST_CASE("toLowerCase converts strings to lowercase", "[tolower]") {
    REQUIRE(toLowerCase("HELLO") == "hello");
    REQUIRE(toLowerCase("Hello123") == "hello123");
    REQUIRE(toLowerCase("") == "");
}


TEST_CASE("loadConfig loads key=value pairs from file", "[config]") {
    std::ofstream configFile("test_config.txt");
    configFile << "username=admin\n";
    configFile << "password=1234\n";
    configFile.close();

    auto config = loadConfig("test_config.txt");

    REQUIRE(config["username"] == "admin");
    REQUIRE(config["password"] == "1234");

    std::remove("test_config.txt"); 
}

// cd "Chat System Project"

// g++ tests/test_utils.cpp src/utils.cpp  -o tests -std=c++17

// ./tests   