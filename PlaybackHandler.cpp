#include "PlaybackHandler.h"

PlaybackHandler::PlaybackHandler(dpp::discord_client* client, dpp::snowflake guild) : Client(client), TargetGuild(guild)
{
	if (!guild)
		throw "Guild cannot be null";
	if (!client)
		throw "Client cannot be null";
}

PlaybackHandler::~PlaybackHandler()
{
	Stop();
}

void PlaybackHandler::Add(std::string url)
{
	Queue.push_back(url);
	TryPlay();
	/*for (auto i : Queue)
	{
		std::cout << i << std::endl;
	}*/
}

void PlaybackHandler::Remove(int index)
{
	if (index > Queue.size())
		return;

	Queue.erase(Queue.begin() + index);
}

void PlaybackHandler::TryPlay()
{
	if (Stopping || Playing)
		return;

	if (Queue.size() <= 0)
		return;

	Playing = true;
	

	dpp::voiceconn* v = Client->get_voice(TargetGuild);
	if (!v)
	{
		Playing = false;
		return;
	}
	v->voiceclient->set_send_audio_type(v->voiceclient->satype_live_audio);


	while (Queue.size() > 0)
	{
		std::string ar = "cmd.exe /c  youtube-dl -f bestaudio -q --ignore-errors -o - \"";
		ar = ar + Queue[0]; //-err_detect ignore_err 
		//ar = ar + "\" | ffmpeg -i pipe:0 -ac 2 -f s16le -ar 48000 pipe:1 -loglevel panic -sample_fmt s16"; //-qscale:a 3 -f ogv output.ogv
		ar = ar + "\" | ffmpeg -thread_queue_size 11520 -i pipe:0 -ac 2 -f s16le -ar 48000 pipe:1 -loglevel panic"; //-qscale:a 3 -f ogv output.ogv
		//-c:a libopus


		byte buzzer[11520];
		auto pipe = _popen(ar.c_str(), "rb");
		if (!pipe) {
			std::cout << "Failed to open Pipe" << std::endl;
			return;
		}
		bool errored = true;

		size_t bytes_read;
		while ((bytes_read = fread(buzzer, 1, 11520, pipe)) > 0)
		{
			if (Stopping)
				break;
			v = Client->get_voice(TargetGuild);
			if (bytes_read < 11520)
			{
				std::cout << "11520 was bigger than bytes read: " << bytes_read << std::endl;
				continue;
			}
			if (!v)
				break;

			try
			{
				if (v->voiceclient && v->voiceclient->is_ready())
				{
					v->voiceclient->send_audio_raw((uint16_t*)buzzer, bytes_read);
				}
				else
				{
					std::cout << "Was not ready" << std::endl;
				}
			}
			catch (std::exception ex)
			{
				std::cout << ex.what() << std::endl;
			}


			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(46.0049));
		}
		_pclose(pipe);
		if (Queue.size() > 0)
		{
			Queue.erase(Queue.begin());
			std::this_thread::sleep_for(std::chrono::duration<double>(0.5));

		}
	}
	Playing = false;
	return;
	//SECURITY_ATTRIBUTES sa;
	//sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	//sa.bInheritHandle = TRUE;
	//sa.lpSecurityDescriptor = NULL;

	//// Create the child output pipe.
	//HANDLE hChildStdoutRd, hChildStdoutWr;
	//CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &sa, 0);

	//// Set the handle as inheritable.
	//SetHandleInformation(hChildStdoutRd, HANDLE_FLAG_INHERIT, 0);

	//// Create the child input pipe.
	//HANDLE hChildStdinRd, hChildStdinWr;
	//CreatePipe(&hChildStdinRd, &hChildStdinWr, &sa, 0);

	//// Set the handle as inheritable.
	//SetHandleInformation(hChildStdinWr, HANDLE_FLAG_INHERIT, 0);

	//// Set up the start up info struct.
	//STARTUPINFO si;
	//ZeroMemory(&si, sizeof(STARTUPINFO));
	//si.cb = sizeof(STARTUPINFO);
	//si.hStdError = hChildStdoutWr;
	//si.hStdOutput = hChildStdoutWr;
	//si.hStdInput = hChildStdinRd;
	//si.dwFlags |= STARTF_USESTDHANDLES;

	//// Create the child process.
	//PROCESS_INFORMATION pi;
	//CreateProcess(NULL, const_cast<LPSTR>(ar.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	//// Close the write end of the pipes.
	//CloseHandle(hChildStdoutWr);
	//CloseHandle(hChildStdinRd);
	//SLEEP(2000);
	//// Read the output from the child process.
	//char buffer[8192];
	//DWORD bytesRead;

	//std::string out;

	//while (ReadFile(hChildStdoutRd, buffer, 8192, &bytesRead, NULL))
	//{
	//	out.reserve(out.size() + bytesRead);
	//	out.append(buffer, bytesRead);

	//	if (out.size() < 11520)
	//		continue;

	//	dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
	//	if (v && v->voiceclient && v->voiceclient->is_ready())
	//	{
	//		std::string packet(out.substr(0, 11520));
	//		out.erase(out.begin(), out.begin() + 11520);


	//		v->voiceclient->send_audio_raw((uint16_t*)packet.data(), packet.size());



	//	}
	//}
	//while (out.size() > 200)
	//{
	//	dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
	//	if (v && v->voiceclient && v->voiceclient->is_ready())
	//	{
	//		if (out.size() > 11520)
	//		{
	//			std::string packet(out.substr(0, 11520));
	//			out.erase(out.begin(), out.begin() + 11520);

	//			v->voiceclient->send_audio_raw((uint16_t*)packet.data(), packet.size());

	//		}
	//		else
	//		{
	//			try
	//			{
	//				v->voiceclient->send_audio_raw((uint16_t*)out.data(), out.size());
	//			}
	//			catch (std::exception& e)
	//			{
	//				std::cout << e.what() << std::endl;
	//				out.clear();
	//			}
	//		}

	//	}

	//}
	//if (out.size() > 0)
	//{
	//	std::cout << out << std::endl;
	//}

	//CloseHandle(hChildStdoutRd);
	//CloseHandle(hChildStdinWr);
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);

}

void PlaybackHandler::Stop()
{
	Stopping = true;
	Queue.clear();
}
