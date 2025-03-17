#pragma once

#include <string>
#include "json.hpp"

class Utility {
public:
	static nlohmann::json ReadJSON(const std::string&);
};
