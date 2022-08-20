#pragma once
#include <vector>
#include <iostream>
#include "nlohmann/json.hpp"

class UrbanDictionaryJson
{
private:
	int Counter = 0;

public:
	std::vector<std::string> Definitions;
	UrbanDictionaryJson(void);
	UrbanDictionaryJson(nlohmann::json json);

	std::string GetNext();
	std::string GetPrevious();
};