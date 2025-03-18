#include "fileManager.h"

#include <iostream>

std::string FileManager::ReadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Read the entire file into a string buffer
    return buffer.str();
}

bool FileManager::CreateFile(const std::string& file_path, bool mkdir) {
    std::cout << "File creation requested: " << file_path << std::endl;
    return false;
}
