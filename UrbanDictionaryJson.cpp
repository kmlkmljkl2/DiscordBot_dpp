#include "UrbanDictionaryJson.h"

UrbanDictionaryJson::UrbanDictionaryJson(void)
{
}

UrbanDictionaryJson::UrbanDictionaryJson(nlohmann::json json)
{
	for (auto i : json)
	{
		Definitions.push_back(i["definition"]);
	}
}

std::string UrbanDictionaryJson::GetNext()
{
	if (Definitions.size() > Counter)
	{
		++Counter;
		return Definitions[Counter] + "\t" + std::to_string(Counter + 1) + "/" + std::to_string(Definitions.size());
	}
	return "";
}

std::string UrbanDictionaryJson::GetPrevious()
{
	if (Counter - 1 != -1 && !Definitions.empty())
	{
		--Counter;
		return Definitions[Counter] + "\t" + std::to_string(Counter + 1) + "/" + std::to_string(Definitions.size());
	}
	return "";
}