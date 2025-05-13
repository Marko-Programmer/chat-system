#include "../include/utils.h"  


bool isValidInput(const std::string& input) {
    return !input.empty() && input.find(' ') == std::string::npos;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::size_t simpleHash(const std::string& password) {
    return std::hash<std::string>{}(password);
}

std::map<std::string, std::string> loadConfig(const std::string& filename) {
    std::map<std::string, std::string> config;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            config[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }
    return config;
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now = *std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int getValidInput() {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number: ";
        }
        else {
            return choice;
        }
    }
}

std::string toLowerCase(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return std::tolower(c);
        });
    return text;
}
