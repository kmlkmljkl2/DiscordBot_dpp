#pragma once

#include <dpp/dpp.h>


inline static dpp::cluster DiscordBot("", dpp::intents::i_default_intents | dpp::intents::i_message_content | dpp::intents::i_direct_messages | dpp::intents::i_guild_emojis | dpp::intents::i_guild_message_reactions);



static class DiscordBotStuff
{
private:
	inline static std::string Token = "";
	



public:

    static void Init()
    {
        std::string Tokenstring;
        std::ifstream myfile;
        myfile.open("C:/Users/kevin/source/repos/c++Console/Discord.txt");
        if (myfile.is_open())
        {
            std::getline(myfile, Tokenstring); // gets first line
            Token = Tokenstring;
        }
        DiscordBot.token = Token;
    }
};