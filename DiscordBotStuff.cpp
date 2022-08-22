#pragma once
#include <dpp/dpp.h>
#include "DiscordBotStuff.h"

void DiscordBotStuff::Init()
{
	delete DiscordBot;
	std::string Tokenstring;
	std::ifstream myfile;
	myfile.open("C:/Users/kevin/source/repos/c++Console/Discord.txt");
	if (myfile.is_open())
	{
		std::getline(myfile, Tokenstring); // gets first line
	}
	DiscordBot = new dpp::cluster(Tokenstring, dpp::intents::i_default_intents | dpp::intents::i_message_content | dpp::intents::i_direct_messages | dpp::intents::i_guild_emojis | dpp::intents::i_guild_message_reactions);
}

void DiscordBotStuff::SendMsg(dpp::message msg)
{
	DiscordBot->message_create_sync(msg);
}