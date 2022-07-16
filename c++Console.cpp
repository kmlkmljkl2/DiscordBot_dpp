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
    std::string EventArgs = event.msg.content.substr(event.msg.content.find_first_of(cmd) + cmd.length());
    /*if (event.msg.content.find(' '))
    {
       EventArgs = event.msg.content.substr(event.msg.content.find_first_of(' ') - 1, event.msg.content.length());
    }*/
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
        CommandHandler::Join(event, EventArgs);
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
    //std::string name = "[000000]>>))  [bb0000]B[c30000]l[cc0000]o[d40000]o[dd0000]d [e50000]F[ee0000]o[f60000]r [ff0000]T[ff0000]h[f70000]e [f00000]B[e80000]l[e00000]o[d90000]o[d10000]d [ca0000]G[c20000]o[bb0000]d[bb0000]";
    //try {
    //    //"\[a-zA-Z0-9\]{6}"
    //std::string test2 = std::regex_replace(name, std::regex("\\[[a-zA-Z0-9\]{6}\\]"), "");
    //std::cout << test2 << std::endl;

    //}
    //catch (std::exception const& e) {
    //    std::cerr << e.what() << std::endl;
    //}

    //return 0;
    std::string Tokenstring;
    std::ifstream myfile;
    myfile.open("C:/Users/kevin/source/repos/c++Console/Discord.txt");
    if (myfile.is_open())
    {
        std::getline(myfile, Tokenstring);
        Token = Tokenstring;

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
