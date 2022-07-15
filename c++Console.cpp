// c++Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <iostream>
#include <dpp/dpp.h>
#include "Logger.cpp"
#include "CommandHandler.cpp"
#include <fstream>
#include <JString.h>
#define _CRT_SECURE_NO_WARNINGS

static std::string Token = "";

void onMessage_Event(const dpp::message_create_t& event) {
    if (event.msg.author.is_bot()) return;
    if (!event.msg.content._Starts_with("-")) return;
  
    std::string cmd = event.msg.content.substr(1, event.msg.content.find_first_of(' ') - 1);
    
    if (cmd == "s" || cmd == "start")
    {
        CommandHandler::Start(event);
    }
    else if (cmd == "meow")
    {
        CommandHandler::Meow(event);
    }
    else if (cmd == "ahegao")
    {
        CommandHandler::Ahegao(event);
    }
    else if (cmd == "dc")
    {
        CommandHandler::Disconnect(event);
    }
    else if (cmd == "l")
    {
        CommandHandler::List(event);
    }
    else if (cmd == "j" || cmd == "join")
    {
        CommandHandler::Join(event);
    }
    else if (cmd == "currentroom")
    {
        CommandHandler::CurrentRoom(event);
    }
    else
    {
        return;
    }
    Logger::LogDebug("Executed command: " + cmd);

};


int main()
{
 /*  ExitGames::Common::JString MasterServer = "192.168.2.221:5055";
   ExitGames::Common::JString GasterServer = "127.0.0.1:5058";
   

   auto MasterServerip = MasterServer.UTF8Representation().toString().substring(0, MasterServer.length() - 4).cstr();

   GasterServer = MasterServerip + GasterServer.substring(GasterServer.length() - 4, GasterServer.length());

   std::cout << MasterServer.UTF8Representation().cstr() <<  " " << GasterServer.UTF8Representation().cstr() << std::endl;
*/




  //  return 0;
    std::string test;
    std::ifstream myfile;
    myfile.open("C:/Users/kevin/source/repos/c++Console/Discord.txt");
    if (myfile.is_open())
    {
        std::getline(myfile, test);
        Token = test;

    }
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
