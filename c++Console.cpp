// c++Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <iostream>
#include <dpp/dpp.h>
#include "Logger.cpp"
#include "CommandHandler.cpp"
#define _CRT_SECURE_NO_WARNINGS

const std::string Token = "OTUzMTA3MTY4MDkyMTI3MjMz.Gkbq6x.qEhHumlKsGs4RxCoCLLFubnURxG2x9iux40PvA";

void onMessage_Event(const dpp::message_create_t& event) {
    if (event.msg.author.is_bot()) return;
    if (!event.msg.content._Starts_with("-")) return;
  
    std::string cmd = event.msg.content.substr(1, event.msg.content.find_first_of(' ') - 1);
    
    if (cmd == "test")
    {
        CommandHandler::Test(event);
    }
    else if (cmd == "meow")
    {
        CommandHandler::Meow(event);
    }
    else if (cmd == "ahegao")
    {
        CommandHandler::Ahegao(event);
    }
    else if (cmd == "test")
    {

    }
    else
    {
        return;
    }





    Logger::LogDebug("Executed command: " + cmd);

};


int main()
{
  /*  CommandHandler::Meow();
    return 0;*/
    Logger::LogDebug("Starting");

    dpp::cluster bot(Token, dpp::intents::i_default_intents | dpp::intents::i_message_content | dpp::intents::i_direct_messages | dpp::intents::i_guild_emojis | dpp::intents::i_guild_message_reactions);
   
    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
        });


    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
        }
        });

    bot.on_message_create.attach(onMessage_Event);

    bot.start(false);
    
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
