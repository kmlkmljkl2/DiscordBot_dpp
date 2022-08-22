#include <iostream>
#include <functional>
#include <dpp/dispatcher.h>
#pragma once
class Command
{
private:

public:
	Command(std::function<void(const dpp::message_create_t& event, std::string args)> method, std::string description, std::string alternateName = "");
	Command(void);
	std::string Description;
	std::string AlternateName;
	std::function<void(const dpp::message_create_t& event, std::string args)> Method;
	bool RequireArgs = false;
};