#include "Command.h"

Command::Command(std::function<void(const dpp::message_create_t& event, std::string args)> method, std::string description, std::string alternateName) : Method(method), Description(description), AlternateName(alternateName)
{
}

Command::Command(void)
{
}