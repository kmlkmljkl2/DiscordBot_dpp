#include "PlaybackHandler.h"
#include "Logger.h"

std::string PlaybackHandler::GetLength(std::string url)
{
	std::string ar = "cmd.exe /c  yt-dlp.exe \"";
	ar = ar + url;
	ar = ar + "\" --get-duration --skip-download";

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
	fclose(pipe);
	return Length;
}

std::string PlaybackHandler::GetName(std::string url)
{
	std::string ar = "cmd.exe /c yt-dlp.exe \"" + url + "\" --get-title --skip-download";

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
	fclose(pipe);
	return Length;
}

void PlaybackHandler::GetData(std::string url)
{
	std::string ar = "cmd.exe /c  yt-dlp.exe \"" + url + "\" --get-id --get-title --skip-download --get-duration";
	byte buzzer[100];
	auto pipe = _popen(ar.c_str(), "rb");
	if (!pipe) {
		std::cout << "Failed to open Pipe" << std::endl;
		return;
	}
	size_t bytes_read;
	std::string Length;
	std::string baseUrl = "https://www.youtube.com/watch?v=";

	while ((bytes_read = fread(buzzer, 1, 100, pipe)) > 0)
	{
		Length += std::string((char*)buzzer, bytes_read);
		/*	if (Queue.size() == 0)
			{
				auto result = Helpers::Split(Length, '\n');
				if (result.size() < 4)
					continue;

				Queue.push_back(YoutubeMusicObject(baseUrl + result[1], result[0], result[2]));
				int i = 0;
				for (char c : Length)
				{
					if (c == '\n')
					{
						i++;
					}
					Length.erase(0);
					if (i == 3)
						break;
				}

				TryPlay();
			}*/
			//Logger::LogDebug(Length);
	}
	_pclose(pipe);
	fclose(pipe);
	//return;
	auto result = Helpers::Split(Length, '\n');

	/*std::cout << Length << std::endl;*/
	//std::cout << result.size() << std::endl;

	for (int i = 0; result.size() > i; i += 3)
	{
		// Position 0 holds the video name
		// Position 1 holds the Video ID
		// Position 2 holds the video's length

		Queue.push_back(YoutubeMusicObject(baseUrl + result[i + 1], result[i], result[i + 2]));
	}
	std::this_thread::sleep_for(std::chrono::duration<double>(1));
	TryPlay();
}

//void PlaybackHandler::StartAudio()
//{
//	return;
//	double delay = static_cast<double>(1000) / static_cast<double>(1 / 0.06) - 3;
//	while (EndlessLoop)
//	{
//		if (AudioBuffer.size() == 0 || AudioBuffer.size() < 11520)
//		{
//			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
//			continue;
//		}
//		if (Pausing)
//		{
//			std::this_thread::sleep_for(std::chrono::duration<double>(1));
//			continue;
//		}
//
//		std::shared_lock<std::shared_mutex> lock(myMutex);
//
//		//std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
//
//
//		std::copy(AudioBuffer.begin(), AudioBuffer.begin() + 11520, VoiceBuffer);
//
//		auto v = Client->get_voice(TargetGuild);
//		//	std::cout << elements_copied << std::endl;
//		if (!v)
//		{
//			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(59));
//			continue;
//		}
//
//		if (v->voiceclient && v->voiceclient->is_ready())
//		{
//			v->voiceclient->send_audio_raw((uint16_t*)VoiceBuffer, 11520);
//			Logger::LogInfo(std::to_string(v->voiceclient->get_secs_remaining()));
//			AudioBuffer.erase(AudioBuffer.begin(), AudioBuffer.begin() + 11520);
//
//		}
//		else
//		{
//			std::cout << "Was not ready" << std::endl;
//		}
//		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay)); //46.88)
//
//
//
//		//std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
//		//std::chrono::duration<double> elapsed_seconds = end - start;
//
//
//	//	if (AudioBuffer.size() == 0 || AudioBuffer.size() < reservedBytes)
//	//	{
//	//		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(59));
//	//		continue;
//	//	}
//
//	//	std::shared_lock<std::shared_mutex> lock(myMutex);
//	//
//	//	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
//
//
//	//	auto end_iter = std::copy(AudioBuffer.begin(), AudioBuffer.begin() + reservedBytes, VoiceBuffer);
//	//	auto elements_copied = std::distance(VoiceBuffer, end_iter);
//
//	//	auto v = Client->get_voice(TargetGuild);
//	////	std::cout << elements_copied << std::endl;
//	//	if (!v)
//	//	{
//	//		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(59));
//	//		continue;
//	//	}
//
//	//	if (v->voiceclient && v->voiceclient->is_ready())
//	//	{
//	//		v->voiceclient->send_audio_raw((uint16_t*)VoiceBuffer, elements_copied);
//
//	//		//if(AudioBuffer.size() > 195840)
//	//			AudioBuffer.erase(AudioBuffer.begin(), AudioBuffer.begin() + elements_copied);
//	//		/*else
//	//		{
//	//			AudioBuffer.clear();
//	//		}*/
//
//	//	}
//	//	else
//	//	{
//	//		std::cout << "Was not ready" << std::endl;
//	//	}
//	//	std::this_thread::sleep_for(std::chrono::duration<double, std::milli>((double)46.935)); //46.88)
//	//	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
//	//	std::chrono::duration<double> elapsed_seconds = end - start;
//	//	//std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
//	}
//}

PlaybackHandler::PlaybackHandler(dpp::discord_client* client, dpp::snowflake guild) : Client(client), TargetGuild(guild)
{
	if (!guild)
		throw "Guild cannot be null";
	if (!client)
		throw "Client cannot be null";
	//std::thread(&PlaybackHandler::StartAudio, this).detach();
}

PlaybackHandler::~PlaybackHandler()
{
	//EndlessLoop = false;
	Stop();
	//delete FfmpegBuffer;
	//delete VoiceBuffer;
}

void PlaybackHandler::Add(std::string url, bool playnow)
{
	if (url.find("list=") != std::string::npos)
	{
		GetData(url);
		return;
	}

	auto length = GetLength(url);
	auto name = GetName(url);
	if (playnow)
	{
		Queue.insert(Queue.begin() + 1, YoutubeMusicObject(url, name.erase(name.length() - 1), length.erase(length.length() - 1)));
	}
	else
	{
		Queue.push_back(YoutubeMusicObject(url, name.erase(name.length() - 1), length.erase(length.length() - 1)));
	}
	TryPlay();
}

void PlaybackHandler::PlayNow(std::string url)
{
	Add(url, true);
}

void PlaybackHandler::Remove(int index)
{
	index = index - 1;
	if (index > Queue.size())
		return;

	Queue.erase(Queue.begin() + index);
}

void PlaybackHandler::Remove(int startIndex, int end)
{
	startIndex = startIndex - 1;
	end = end - 1;
	if (startIndex < 0 || end < 0)
		return;

	if (startIndex > Queue.size() || end > Queue.size() || end < startIndex)
		return;
	Queue.erase(Queue.begin() + startIndex, Queue.begin() + end);
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

	//const int target_rate = 1536;  //target rate in KB/s
	//const int buffer_size = 11520;  //size of buffer in bytes
	//std::chrono::milliseconds delay_time;
	//int test = (buffer_size * 1000) / (target_rate * 1024) * 3.6 * 2;
	//delay_time = std::chrono::milliseconds(test);
	//std::cout << delay_time.count() << std::endl;

	//60 - 3
	//double delay = static_cast<double>(1000) / static_cast<double>(1 / 0.06) - 13.55;

	while (Queue.size() > 0)
	{
		std::string ar = "cmd.exe /c  yt-dlp.exe -f bestaudio " + std::string(Debugging ? "" : "-q") + std::string(" --ignore-errors -o - \"");
		ar = ar + Queue[0].Url; //-err_detect ignore_err
		//ar = ar + "\" | ffmpeg -i pipe:0 -ac 2 -f s16le -ar 48000 pipe:1 -loglevel panic -sample_fmt s16"; //-qscale:a 3 -f ogv output.ogv
		ar = ar + "\" | ffmpeg -re -i pipe:0 -ac 2 -f s16le -ar 48000 pipe:1 " + std::string(Debugging ? "-loglevel panic" : "-loglevel quiet"); //-loglevel panic  //-qscale:a 3 -f ogv output.ogv
		//-c:a libopus

		auto pipe = _popen(ar.c_str(), "rb");
		if (!pipe) {
			std::cout << "Failed to open Pipe" << std::endl;
			return;
		}
		//bool errored = true;
		Skipping = false;
		int bytes_read;
		StartTime = std::chrono::high_resolution_clock::now();

		byte test[11520];
		std::cout << "Sizeof unit16_t " << sizeof(uint16_t) << " and sizeof the array " << sizeof(test) << std::endl;
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2000));
		while ((bytes_read = fread(test, 1, 11520, pipe)) > 0)
		{
			if (Stopping)
				break;
			if (Skipping)
				break;
			while (Pausing)
			{
				std::this_thread::sleep_for(std::chrono::duration<double>(1));
			}
			if (bytes_read < 11520)
				continue;

			{
				//v = Client->get_voice(TargetGuild);
				/*if (bytes_read < 11520)
				{
					std::cout << "11520 was bigger than bytes read: " << bytes_read << std::endl;
					continue;
				}*/
				//if (!v)
				//	break;
				//std::unique_lock<std::shared_mutex> lock(myMutex);
			}

			v->voiceclient->send_audio_raw((uint16_t*)test, bytes_read);

			{
				//std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(delay));

				//currentsong.insert(currentsong.end(), FfmpegBuffer, FfmpegBuffer + bytes_read);

				//Logger::LogInfo(std::to_string(AudioBuffer.size()));

				//if (AudioBuffer.size() > 11520)

				//if (v->voiceclient && v->voiceclient->is_ready())
				//{
				//	v->voiceclient->send_audio_raw(/*(uint16_t*)*/buzzer, bytes_read);
				//}
				//else
				//{
				//	std::cout << "Was not ready" << std::endl;
				//}

				//auto stop = std::chrono::high_resolution_clock::now();
				//totalbytes += bytes_read;
				//double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(stop - songtime).count();
				//average = (double)(totalbytes / (double)(seconds / 1000));
				//std::cout << average << std::endl;
				//int result = 0;
				//if (bytesperSecond > average && (average > previousAverage || (average - previousAverage) < 100))
				//{
				//	if ((PlaybackDelay - 0.1) > 45.8)
				//	{
				//

				//		result = bytesperSecond - average;
				//		/*if (result > 100000)
				//		{
				//			PlaybackDelay -= 0.1;
				//		}*/
				//		if (result > 10000)
				//		{
				//			PlaybackDelay -= 0.01;
				//		}
				//		else if (result < 10000)
				//		{
				//			PlaybackDelay -= 0.001;
				//		}
				//		else if (result < 1000)
				//		{
				//			PlaybackDelay -= 0.0001;
				//		}
				//		else
				//			PlaybackDelay -= 0.00001;

				//	}
				//}
				//else if (bytesperSecond < average && (previousAverage > average || previousAverage - average > 100))
				//{
				//	if ((PlaybackDelay + 0.1) < 46.1)
				//	{
				//		result = average - bytesperSecond;
				//		/*if (result > 100000)
				//		{
				//			PlaybackDelay += 0.1;
				//		}*/
				//		if (result > 10000)
				//		{
				//			PlaybackDelay += 0.01;
				//		}
				//		else if (result < 10000)
				//		{
				//			PlaybackDelay += 0.001;
				//		}
				//		else if (result < 1000)
				//		{
				//			PlaybackDelay += 0.0001;
				//		}
				//		else
				//			PlaybackDelay += 0.00001;
				//	}
				//}
				//if (seconds / 1000 > 30)
				//{
				//	songtime = std::chrono::high_resolution_clock::now();
				//	totalbytes = 0;
				//}
				//previousAverage = average;
				//std::cout << PlaybackDelay << std::endl;
				//std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(PlaybackDelay));
			}
		}
		_pclose(pipe);
		fclose(pipe);
		if (Queue.size() > 0)
		{
			Queue.erase(Queue.begin());
			std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
		}
	}
	Playing = false;
	return;
	{
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

void PlaybackHandler::Shuffle()
{
	std::random_device rd;  // seed for the random number engine
	std::mt19937 gen(rd());  // Mersenne Twister 19937 generator
	std::shuffle(Queue.begin() + 1, Queue.end(), gen);
}

std::string PlaybackHandler::QueueString()
{
	try
	{
		std::string Endresult = "```\n";
		auto stop = std::chrono::high_resolution_clock::now();
		auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(stop - StartTime).count();
		int hours = elapsedSeconds / 3600;
		int minutes = elapsedSeconds / 60;
		int seconds = elapsedSeconds % 60;

		bool first = true;
		int numba = 1;
		for (auto& i : Queue)
		{
			std::string result = "";
			if (first)
				result += "Playing =>   ";

			result += (first ? "" : std::string(13, ' ')) + "[" + (std::string)(Queue.size() > 9 && numba < 11 ? "0" : "") + std::to_string(numba++) + "] ";
			if (i.Name.size() > 50)
			{
				i.Name.erase(46);
				i.Name += "...";
			}

			size_t spaces = 50 - i.Name.size();
			result += i.Name + (spaces > 0 ? std::string(spaces, ' ') : "") + " ";

			if (first)
				result += "[" + (hours == 0 ? "" : std::to_string(hours) + ":") + (minutes == 0 ? "00" : minutes > 10 ? std::to_string(minutes) : "0" + std::to_string(minutes)) + ":" + (seconds == 0 ? "00" : seconds > 10 ? std::to_string(seconds) : "0" + std::to_string(seconds)) + "/" + i.Duration + "]";
			else
				result += "[" + i.Duration + "]";
			result += "\n";

			if (Endresult.size() + result.size() > 1960)
			{
				Endresult += "\ntheres more lele\nTotal Queue: " + std::to_string(Queue.size());
				break;
			}
			Endresult += result;

			first = false;
		}
		Endresult += "```";
		//std::string(20 - Stats.size(), ' ');
		if (Endresult == "```\n```")
			return "Queue is empty";
		std::cout << Endresult.size() << std::endl;
		return Endresult;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
		return "";
	}
}