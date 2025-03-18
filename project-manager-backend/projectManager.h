#pragma once

#include "json.hpp"

#include <string>

// @TODO: Add getLastError();
// @TODO: Verify m_CurrentWorkingDir;

// Singleton
class ProjectManager {
public:
	ProjectManager() : m_CurrentWorkingDir(".") {}
	bool Create(const std::string&, const nlohmann::json&);

private:
	std::string m_CurrentWorkingDir;
};
