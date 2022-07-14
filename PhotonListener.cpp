#include "PhotonListener.h"

void Run(PhotonListener* list)
{
	while (list->KeepRunning)
	{
		list->Client.service();
		SLEEP(100);
	}
}

PhotonListener::PhotonListener(std::string str) : Client(*this, "", "01042015_1.28")
{
	std::thread t(Run, this);
	//t.join();
	t.detach();
}

PhotonListener::~PhotonListener()
{
	KeepRunning = false;
}

void PhotonListener::ConnectTest(void)
{
	ExitGames::LoadBalancing::ConnectOptions options(ExitGames::LoadBalancing::AuthenticationValues(), "cunt", "135.125.239.180", ExitGames::LoadBalancing::ServerType::MASTER_SERVER);

	Client.connect(options);
}

void PhotonListener::debugReturn(int debugLevel, const Common::JString& string)
{
	std::cout << debugLevel << " " << string << std::endl;
}

void PhotonListener::connectionErrorReturn(int errorCode)
{
	std::cout << errorCode << std::endl;
}

void PhotonListener::clientErrorReturn(int errorCode)
{
	std::cout << errorCode << std::endl;
}

void PhotonListener::warningReturn(int warningCode)
{
	std::cout << warningCode << std::endl;
}

void PhotonListener::serverErrorReturn(int errorCode)
{
	std::cout << errorCode << std::endl;
}

void PhotonListener::joinRoomEventAction(int playerNr, const Common::JVector<int>& playernrs, const LoadBalancing::Player& player)
{
	std::cout << "Player " << playerNr << " joined" << std::endl;
}

void PhotonListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::cout << "Player " << playerNr << " left" << std::endl;
}

void PhotonListener::customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent)
{
}

void PhotonListener::connectReturn(int errorCode, const Common::JString& errorString, const Common::JString& region, const Common::JString& cluster)
{
	std::cout << "connectReturn " << errorCode << " " << errorString << " " << region << " " << cluster << std::endl;
}

void PhotonListener::disconnectReturn(void)
{
	std::cout << "disconnected" << std::endl;
}

void PhotonListener::leaveRoomReturn(int errorCode, const Common::JString& errorString)
{
	std::cout << "left Room " << errorCode << " " << errorString << std::endl;
}