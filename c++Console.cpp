// c++Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>

#include <dpp/dpp.h>
#include "Logger.cpp"
#include "CommandHandler.h"
#include <fstream>
#include <JString.h>
#include "Command.h"
#include "Helpers.h"
#include "DiscordBotStuff.h"
#define _CRT_SECURE_NO_WARNINGS


std::map<std::string, Command> CommandList;

std::string HelpMsg;

void onMessage_Event(const dpp::message_create_t& event)
{
	if (event.msg.author.is_bot()) return;
	if (!event.msg.content._Starts_with("-"))
	{
		for (int i = 0; CommandHandler::StoreVector.size() > i; i++)
		{
			auto Bot = CommandHandler::StoreVector[i];
			if (Bot->ChannelId == event.msg.channel_id && Bot->Client.getState() == 15)
			{
				ExitGames::Common::Hashtable data = ExitGames::Common::Hashtable();
				data.put((byte)0, 2); // view ID
				data.put((byte)2, Bot->Client.getServerTime()); // Server Time

				//Having both byte 3 and byte 5 set makes MC auto-ban you
				data.put<byte, char[5]>(3, "Chat"); // RPC Name

				auto msg = ExitGames::Common::JString(event.msg.content.c_str());
				auto name = ExitGames::Common::JString(event.msg.author.username.c_str());

				Common::Object t[2];
				t[0] = Common::Helpers::ValueToObject<Common::Object>::get(msg);
				t[1] = Common::Helpers::ValueToObject<Common::Object>::get(name);

				data.put<byte>(4, t, 2);
				Bot->Client.opRaiseEvent(true, data, 200, ExitGames::LoadBalancing::RaiseEventOptions());
			}
		}

		return;
	}

	std::string cmd = event.msg.content.substr(1, event.msg.content.find_first_of(' ') - 1);
	std::string EventArgs = event.msg.content.substr(event.msg.content.find_first_of(cmd) + cmd.length());
	if (EventArgs != "")
		EventArgs = EventArgs.substr(1);
	bool NeedsHelp = cmd == "help";

	if (NeedsHelp)
	{
		if (EventArgs == "")
		{
			event.send(HelpMsg);
			return;
		}
	}
	cmd = Helpers::ToLower(cmd);

	//std::cout << "\"" << EventArgs << "\"" << std::endl;

	for (auto i : CommandList)
	{
		if (cmd == i.first || cmd == i.second.AlternateName)
		{
			if (NeedsHelp)
			{
				event.send(i.second.Description);
				break;
			}
			if (i.second.RequireArgs && EventArgs == "")
			{
				event.reply("Command requires Arguments!");
				break;
			}
			Logger::LogDebug("Executed Command " + i.first);
			i.second.Method(event, EventArgs);
			break;
		}
	}
	//Command Not Found
};

void InitCommands()
{
	CommandList["test"] = Command(CommandHandler::Test, "Testing stuff");
	CommandList["start"] = Command(CommandHandler::Start, "Starts a BotInstance", "s");
	CommandList["playerlist"] = Command(CommandHandler::PlayerList, "Gives you the Player list", "p");
	CommandList["debug"] = Command(CommandHandler::Debug, "Debugging purposes");
	CommandList["join"] = Command(CommandHandler::Join, "Joins a Room -join [Name]", "j");
	CommandList["list"] = Command(CommandHandler::List, "Gives you the Room list", "l");
	CommandList["disconnect"] = Command(CommandHandler::Disconnect, "Disconnects the bot", "dc");
	CommandList["ahegao"] = Command(CommandHandler::Ahegao, "best kind of gao");
	CommandList["meow"] = Command(CommandHandler::Meow, "Meow");
	CommandList["ud"] = Command(CommandHandler::UrbanDictionary, "Gets the Definition for the given term");
	CommandList["next"] = Command(CommandHandler::Next, "Gives you the next definition given by -ud");
	CommandList["back"] = Command(CommandHandler::Back, "Gives you the previous definition given by -ud");


	CommandList["join"].RequireArgs = true;
	CommandList["ud"].RequireArgs = true;

	HelpMsg += "```";
	for (auto i : CommandList)
	{
		HelpMsg += "\n" + i.first;
		HelpMsg += std::string(20 - i.first.size(), ' ');
		HelpMsg += i.second.Description;
	}
	HelpMsg += "```";
}

int main()
{

	DiscordBotStuff::Init();
	CommandHandler::Init();
	InitCommands();
	Logger::LogDebug("Starting");


	DiscordBotStuff::DiscordBot->on_log(dpp::utility::cout_logger());
	DiscordBotStuff::DiscordBot->on_message_create.attach(onMessage_Event);
	while (DiscordBotStuff::DiscordBot->token == "")
	{
		SLEEP(1000);
	}

	DiscordBotStuff::DiscordBot->start(false);

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