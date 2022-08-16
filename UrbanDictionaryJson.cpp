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
