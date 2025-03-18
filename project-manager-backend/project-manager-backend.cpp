#include "httplib.h"
#include "projectManager.h"
#include "utility.h"
#include "fileManager.h"

#include <iostream>

#define CONFIG_PATH "ProjectConfigurations.json"

#pragma region HelperFunctions

void logger(const httplib::Request& req, const httplib::Response& res) {
    std::cout << "Request: " << req.method << " " << req.path << " From: " << req.remote_addr << std::endl;
    std::cout << "Response: " << res.status << std::endl;
}

#pragma endregion

int main() {
    std::cout << "Initializing the server ..." << std::endl;
    httplib::Server svr;
    ProjectManager manager;

#pragma region Endpoints

    std::cout << "Configuring endpoints" << std::endl;

    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("{ \"status\": \"running.\" }", "application/json");
    });

    svr.Get("/project-configs", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string projectConfigs = FileManager::ReadFile(CONFIG_PATH);
            res.set_content(projectConfigs, "application/json");
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content("{ \"error\": " + std::string(e.what()) + " }", "application/json");
        }
    });

    svr.Get("/create", [&manager](const httplib::Request& req, httplib::Response& res) {
        std::string requested = req.get_param_value("language");
        std::string name = req.get_param_value("name");

        // var: requested will be empty if not set in params,
        // effectively this checks if the language parameter is set or not.
        if (requested.empty()) {
            res.status = 400;
            res.set_content("{ \"error\": \"no language input\" }", "application/json");
            return;
        }

        if (name.empty()) {
            res.status = 400;
            res.set_content("{ \"error\": \"no name\" }", "application/json");
            return;
        }

        static nlohmann::json configs = Utility::ReadJSON(CONFIG_PATH);

        nlohmann::json projectConfig = {
            {"valid", false}
        };

        for (const auto& langEntry : configs["languages"]) {
            if (langEntry.contains(requested)) {
                projectConfig = langEntry[requested];
                break;
            }
        }

        if (!projectConfig["valid"]) {
            res.status = 400;
            res.set_content("{ \"error\": \"invalid language input\" }", "application/json");
            return;
        }

        if (!manager.Create(name, projectConfig)) {
            res.status = 500;
            res.set_content("{ \"error\": \"an error occured during project creation\" }", "application/json");
            return;
        }

        res.status = 200;
        res.set_content("{ \"success\": \"project created\" }", "application/json");
    });

    // Custom 404 error handler
    svr.set_error_handler([](const httplib::Request& req, httplib::Response& res) {
        if (res.status == 404) {
            res.set_content(
                "<html><body><h1>404 Not Found</h1><p>The requested URL " + req.path +
                " was not found on this server.</p></body></html>",
                "text/html"
            );
        }
    });

    // Shutdown
    svr.Get("/shutdown", [&svr](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
        res.set_content("{ \"success\": \"server successfully shutdown\" }", "application/json");

        std::cout << "Shutting the server down ..." << std::endl;
        svr.stop();
    });

#pragma endregion

    svr.set_logger(logger);
    std::cout << "Starting the server ..." << std::endl;
    svr.listen("localhost", 8080);
    return 0;
}
