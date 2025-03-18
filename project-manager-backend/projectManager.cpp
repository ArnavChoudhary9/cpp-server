#include "projectManager.h"

#include "fileManager.h"

bool ProjectManager::Create(const std::string& name, const nlohmann::json& config) {
	if (!config.contains("files")) {
		return false;
	}

	// @TODO: When validate user defined configs
	// 
	// Assumes that var: files is a list, and contains valid filenames,
	// and since the configs are created by developer, they are assumed
	// to be correct
	for (const auto& file : config["files"]) {
		auto filePath = m_CurrentWorkingDir + "\\" + name + "\\" + std::string(file);
		if (!FileManager::CreateFile(filePath, true)) {
			return false;
		}
	}

	return true;
}
