#pragma once
#include <vector>
#include <iostream>
#include "nlohmann/json.hpp"

class UrbanDictionaryJson
{
public:
	std::vector<std::string> Definitions;
	UrbanDictionaryJson(void);
	UrbanDictionaryJson(nlohmann::json json);
};