#include "fileManager.h"

#include <iostream>
#include <fstream>
#include <filesystem>

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
    try {
        std::filesystem::path path(file_path);

        // Make parent directories
        if (mkdir) {
            std::filesystem::create_directories(path.parent_path());
        }

        // Create file
        std::ofstream file(path);
        if (!file) {
            return false;
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error occured during file creation: " << e.what() << std::endl;
        return false;
    }
}
