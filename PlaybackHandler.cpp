#include "PlaybackHandler.h"

std::string PlaybackHandler::GetLength(std::string url)
{
	std::string ar = "cmd.exe /c  youtube-dl \"";
	ar = ar + url; 
	ar = ar + "\" --get-duration";


	byte buzzer[100];
	auto pipe = _popen(ar.c_str(), "rb");
	if (!pipe) {
		std::cout << "Failed to open Pipe" << std::endl;
		return "";
	}
	size_t bytes_read;
	std::string Length;
	while ((bytes_read = fread(buzzer, 1, 100, pipe)) > 0)
	{
		Length = std::string((char*)buzzer, bytes_read);
	}
	_pclose(pipe);
	return Length;
}

std::string PlaybackHandler::GetName(std::string url)
{
	std::string ar = "cmd.exe /c  youtube-dl \"";
	ar = ar + url; //-err_detect ignore_err 
	//ar = ar + "\" | ffmpeg -i pipe:0 -ac 2 -f s16le -ar 48000 pipe:1 -loglevel panic -sample_fmt s16"; //-qscale:a 3 -f ogv output.ogv
	ar = ar + "\" --get-title"; //-qscale:a 3 -f ogv output.ogv
	//-c:a libopus


	byte buzzer[100];
	auto pipe = _popen(ar.c_str(), "rb");
	if (!pipe) {
		std::cout << "Failed to open Pipe" << std::endl;
		return "";
	}
	size_t bytes_read;
	std::string Length;
	while ((bytes_read = fread(buzzer, 1, 100, pipe)) > 0)
	{
		Length = std::string((char*)buzzer, bytes_read);

	}
	_pclose(pipe);
	return Length;
}

void PlaybackHandler::HandlePlaylist(std::string url)
{
	std::string ar = "cmd.exe /c  youtube-dl \"";
	ar = ar + url; //-err_detect ignore_err 
	//ar = ar + "\" | ffmpeg -i pipe:0 -ac 2 -f s16le -ar 48000 pipe:1 -loglevel panic -sample_fmt s16"; //-qscale:a 3 -f ogv output.ogv
	ar = ar + "\" --get-id"; //-qscale:a 3 -f ogv output.ogv
	//-c:a libopus


	byte buzzer[500];
	auto pipe = _popen(ar.c_str(), "rb");
	if (!pipe) {
		std::cout << "Failed to open Pipe" << std::endl;
		return;
	}
	size_t bytes_read;
	std::string Length;
	while ((bytes_read = fread(buzzer, 1, 100, pipe)) > 0)
	{
		Length += std::string((char*)buzzer, bytes_read);

	}
	_pclose(pipe);
	std::string baseUrl = "https://www.youtube.com/watch?v=";
	auto result = Helpers::Split(Length, '\n');
	for (auto& i : result)
	{
		std::cout << i << std::endl;
		Add(baseUrl + i);
	}
}

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
	if (url.find("list=") != std::string::npos)
	{
		HandlePlaylist(url);
		return;
	}

	auto length = GetLength(url);
	auto name = GetName(url);
	Queue.push_back(YoutubeMusicObject(url, name.erase(name.length() - 1), length.erase(length.length() - 1)));
	TryPlay();
	/*for (auto i : Queue)
	{
		std::cout << i << std::endl;
	}*/
}

void PlaybackHandler::Remove(int index)
{
	index = index - 1;
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
		ar = ar + Queue[0].Url; //-err_detect ignore_err 
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
		Skipping = false;
		size_t bytes_read;
		StartTime = std::chrono::high_resolution_clock::now();
		while ((bytes_read = fread(buzzer, 1, 11520, pipe)) > 0)
		{
			if (Stopping)
				break;
			if (Skipping)
				break;
			if (Pausing)
			{
				std::this_thread::sleep_for(std::chrono::duration<double>(1));
				continue;
			}
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

void PlaybackHandler::Pause()
{
	Pausing = true;
}

void PlaybackHandler::Resume()
{
	Pausing = false;
}

void PlaybackHandler::Skip()
{
	Skipping = true;
}

std::string PlaybackHandler::QueueString()
{
	try
	{
		std::string result = "```";
		auto stop = std::chrono::high_resolution_clock::now();
		auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(stop - StartTime).count();
		int hours = elapsedSeconds / 3600;
		int minutes = elapsedSeconds / 60;
		int seconds = elapsedSeconds % 60;

		bool first = true;
		int numba = 1;
		for (auto& i : Queue)
		{
			if (first)
				result += "Playing =>   ";
			
				result += (first ? "" : std::string(13, ' ')) + "[" + (std::string)(Queue.size() > 9 && numba < 10 ? "0" : "") + std::to_string(numba++) + "] ";
			int spaces = 50 - i.Name.size();
			result += i.Name + (spaces > 0 ? std::string(spaces, ' ') : "") + " ";

			if (first)
				result += "[" + (hours == 0 ? "" : std::to_string(hours) + ":") + (minutes == 0 ? "00" : minutes > 10 ? std::to_string(minutes) : "0" + std::to_string(minutes)) + ":" + (seconds == 0 ? "00" : seconds > 10 ? std::to_string(seconds) : "0" + std::to_string(seconds)) + "/" + i.Duration + "]";
			else
				result += "[" + i.Duration + "]";
			result += "\n";

			first = false;
		}
		result += "```";
		//std::string(20 - Stats.size(), ' ');
		if (result == "``````")
			return "Queue is empty";
		std::cout << result.size() << std::endl;
		return result;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
		return "";
	}
}
