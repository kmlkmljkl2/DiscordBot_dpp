// c++Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>

#include <dpp/dpp.h>
#include "Logger.cpp"
#include "CommandHandler.cpp"
#include <fstream>
#include <JString.h>
#include "DiscordBotStuff.cpp"
#define _CRT_SECURE_NO_WARNINGS



void onMessage_Event(const dpp::message_create_t& event) {
    if (event.msg.author.is_bot()) return;
    if (!event.msg.content._Starts_with("-"))
    {
      
		//for (int i = 0; CommandHandler::StoreVector.size() > i; i++)
		//{
		//	auto Bot = CommandHandler::StoreVector[i];
  //          if (Bot->ChannelId == event.msg.channel_id)
  //          {
  //              ExitGames::Common::Hashtable data = ExitGames::Common::Hashtable();
  //              data.put((byte)0, 2); // view ID
  //              data.put((byte)2, Bot->Client.getServerTime()); // Server Time
  //              data.put((byte)3, "Chat"); // RPC Name


  //              const char* test[] = {event.msg.content.c_str(), event.msg.author.username.c_str()};

  //              data.put((byte)4, test);


  //              //data.put((byte));


  //              Bot->Client.opRaiseEvent(true, data, 200, ExitGames::LoadBalancing::RaiseEventOptions());
  //          }

		//}

        return;
    }
  
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
    if (cmd == "test")
    {
        CommandHandler::Test(event);
    }
    else if (cmd == "meow")
    {
        CommandHandler::Meow(event);
    }
    else if (cmd == "create")
    {
        CommandHandler::CreateRoom(event);
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
    else if (cmd == "debug")
    {
        CommandHandler::Debug(event);
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
    DiscordBotStuff::Init();


    Logger::LogDebug("Starting");
    DiscordBot.on_log(dpp::utility::cout_logger());

    DiscordBot.on_message_create.attach(onMessage_Event);

    DiscordBot.start(false);
    
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
