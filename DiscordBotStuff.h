#pragma once
#include <iostream>
#include <dpp/dpp.h>

static class DiscordBotStuff
{
private:

public:
	static inline dpp::cluster* DiscordBot;//("", dpp::intents::i_default_intents | dpp::intents::i_message_content | dpp::intents::i_direct_messages | dpp::intents::i_guild_emojis | dpp::intents::i_guild_message_reactions);

	static void Init();
	static void SendMsg(dpp::message msg);
};