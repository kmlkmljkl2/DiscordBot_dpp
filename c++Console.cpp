// c++Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>

#include <dpp/dpp.h>
#include "Logger.h"
#include "CommandHandler.h"
#include <fstream>
#include <JString.h>
#include "Command.h"
#include "Helpers.h"
#include "DiscordBotStuff.h"
#include "Logger.h"
#define _CRT_SECURE_NO_WARNINGS

std::map<std::string, Command> CommandList;

std::string HelpMsg;

void onMessage_Event(const dpp::message_create_t& event)
{
	//std::cout << event.msg.content << std::endl;
	if (event.msg.author.is_bot()) return;
	if (!event.msg.content._Starts_with("+"))
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
				Bot->Client.opRaiseEvent(false, data, 200, ExitGames::LoadBalancing::RaiseEventOptions());
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

	for (auto& i : CommandList)
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
			std::thread([event, EventArgs, i]()
				{
					try
					{
						i.second.Method(event, EventArgs);
					}
					catch (std::exception ex)
					{
						Logger::LogError(ex.what());
					}
				}).detach();

				break;
		}
	}
	//Command Not Found
};

void onButtonClick(const dpp::button_click_t& event)
{
}

void onVoiceStateUpdate(const dpp::voice_state_update_t& state)
{
	Logger::LogDebug(std::string("User ") + dpp::find_user(state.state.user_id)->username + std::string(" changed his state to "));

	//dpp::find_channel(state.from->get_voice()->voiceclient()->channel_id)->get_voice_members().size();
}

void onSlashCommand(const dpp::slashcommand_t& event)
{
	if (event.command.get_command_name() == "getusercreationtime")
	{
		auto& userid = std::get<dpp::snowflake>(event.get_parameter("user"));
		auto guild = dpp::find_guild(event.command.guild_id);
		auto user = dpp::find_guild_member(event.command.guild_id, userid);

		auto Time = user.get_user()->get_creation_time();
		std::time_t end_time = std::time_t(Time);
		char Timeasd[40];
		ctime_s(Timeasd, 40, &end_time);

		event.reply(user.get_user()->username + ": " + std::string(Timeasd));
	}
}

void onDiscordBotReady(const dpp::ready_t& event)
{
	if (dpp::run_once<struct register_bot_commands>())
	{
		dpp::slashcommand newcommand("Getusercreationtime", "Gets the Creation time of Animals that hide it from their profile", DiscordBotStuff::DiscordBot->me.id);
		newcommand.add_option(
			dpp::command_option(dpp::co_user, "user", "The type of animal", true)/*.
			add_choice(dpp::command_option_choice("Dog", std::string("animal_dog"))).
			add_choice(dpp::command_option_choice("Cat", std::string("animal_cat"))).
			add_choice(dpp::command_option_choice("Penguin", std::string("animal_penguin")*/
			//)
			//)
		);

		/* Register the command */
		DiscordBotStuff::DiscordBot->global_command_create(newcommand);
	}
}

void InitCommands()
{
	CommandList["start"] = Command(CommandHandler::Start, "Starts a BotInstance", "s");
	CommandList["playerlist"] = Command(CommandHandler::PlayerList, "Gives you the Player list", "p");
	CommandList["debug"] = Command(CommandHandler::Debug, "Debugging purposes");
	CommandList["join"] = Command(CommandHandler::Join, "Joins a Room -join [Name]", "j");
	CommandList["list"] = Command(CommandHandler::List, "Gives you the Room list", "l");
	CommandList["disconnect"] = Command(CommandHandler::Disconnect, "Disconnects the bot", "dc");
	CommandList["ahegao"] = Command(CommandHandler::Ahegao, "best kind of gao");
	CommandList["meow"] = Command(CommandHandler::Meow, "Meow", "miau");
	CommandList["ud"] = Command(CommandHandler::UrbanDictionary, "Gets the Definition for the given term");
	CommandList["next"] = Command(CommandHandler::Next, "Gives you the next definition given by -ud");
	CommandList["back"] = Command(CommandHandler::Back, "Gives you the previous definition given by -ud");
	CommandList["join"].RequireArgs = true;
	CommandList["ud"].RequireArgs = true;
	CommandList["vc"] = Command(CommandHandler::JoinVC, "Joins the VoiceChat");
	CommandList["leave"] = Command(CommandHandler::LeaveVC, "Leaves the VoiceChat");
	CommandList["play"] = Command(CommandHandler::Play, "Plays a YouTube link");
	CommandList["play"].RequireArgs = true;
	CommandList["skip"] = Command(CommandHandler::Skip, "Skips the currently playing Song");
	CommandList["resume"] = Command(CommandHandler::Resume, "Resumes to playing music after it was Paused by Pause");
	CommandList["pause"] = Command(CommandHandler::Pause, "Pauses the currently playing Music");
	CommandList["queue"] = Command(CommandHandler::Queue, "Returns the current Music Queue");
	CommandList["remove"] = Command(CommandHandler::Remove, "Removes one Index given by -queue");
	CommandList["shuffle"] = Command(CommandHandler::Shuffle, "Randomizes the Playback Queue");
	CommandList["playnow"] = Command(CommandHandler::PlayNow, "Adds the given song as the next in Queue");
	CommandList["playnow"].RequireArgs = true;

	//CommandList.insert(std::pair<std::string, Command>("", Command()));

	HelpMsg += "```";
	for (auto i : CommandList)
	{
		HelpMsg += "\n" + i.first;
		HelpMsg += std::string(20 - i.first.size(), ' ');
		HelpMsg += i.second.Description;
	}
	HelpMsg += "```";
}
void InitTestCommands()
{
	CommandList["test"] = Command(CommandHandler::Test, "Testing stuff");
	CommandList["createroom"] = Command(CommandHandler::CreateRoom, "Create da room", "create");
}

int main()
{
	//CommandHandler::Play();

	//return 0;
	DiscordBotStuff::Init();
	CommandHandler::Init();
	InitCommands();
	//InitSlashcmds
	InitTestCommands();

	Logger::LogDebug("Starting");

	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (DiscordBotStuff::DiscordBot->token == "")
	{
		SLEEP(1000);
	}
	DiscordBotStuff::DiscordBot->on_voice_state_update(onVoiceStateUpdate);
	DiscordBotStuff::DiscordBot->on_button_click(onButtonClick);
	DiscordBotStuff::DiscordBot->on_ready(onDiscordBotReady);
	DiscordBotStuff::DiscordBot->on_slashcommand(onSlashCommand);
	DiscordBotStuff::DiscordBot->on_log(dpp::utility::cout_logger());
	DiscordBotStuff::DiscordBot->on_message_create.attach(onMessage_Event);
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