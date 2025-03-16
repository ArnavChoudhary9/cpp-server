#include <iostream>
#include <fstream>
#include <sstream>
#include "httplib.h"

#pragma region HelperFunctions

std::string readJSONFile(const std::string & filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Read the entire file into a string buffer
    return buffer.str();
}

void logger(const httplib::Request& req, const httplib::Response& res) {
    std::cout << "Request: " << req.method << " " << req.path << " From: " << req.remote_addr << std::endl;
    std::cout << "Response: " << res.status << std::endl;
}

#pragma endregion

int main() {
    std::cout << "Initializing the server ..." << std::endl;
    httplib::Server svr;

#pragma region Endpoints

    std::cout << "Configuring endpoints" << std::endl;

    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("The Backend is up and running.", "text/plain");
    });

    svr.Get("/project-configs", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string projectConfigs = readJSONFile("ProjectConfigurations.json");
            res.set_content(projectConfigs, "application/json");
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content("Error fetching Project Configurations: " + std::string(e.what()), "text/plain");
        }
    });

    // Custom 404 error handler
    svr.set_error_handler([](const httplib::Request& req, httplib::Response& res) {
        res.status = 404; // Set HTTP status code to 404
        res.set_content(
            "<html><body><h1>404 Not Found</h1><p>The requested URL " + req.path +
            " was not found on this server.</p></body></html>",
            "text/html"
        );
    });

#pragma endregion

    svr.set_logger(logger);
    std::cout << "Starting the server ..." << std::endl;
    svr.listen("localhost", 8080);
    return 0;
}
