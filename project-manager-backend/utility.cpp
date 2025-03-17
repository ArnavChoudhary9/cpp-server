#include "utility.h"
#include "fileManager.h"

nlohmann::json Utility::ReadJSON(const std::string& filePath) {
	std::string rawJSON = FileManager::ReadFile(filePath);
	return nlohmann::json::parse(rawJSON);
}
