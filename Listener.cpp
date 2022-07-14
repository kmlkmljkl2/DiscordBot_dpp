//#include "Listener.h"
//#include "Common.h"
//#include "Player.h"
//#include "Client.h"
//#include <iostream>
//using namespace ExitGames;
//
//class Listener : ExitGames::LoadBalancing::Listener
//{
//private:
//	Listener(void) : Client(*this, "", "01042015_1.28")
//	{
//
//	}
//
//	const bool KeepRunning = true;
//	ExitGames::LoadBalancing::Client Client;
//
//	void Run()
//	{
//		while (KeepRunning)
//		{
//			Client.service();
//			SLEEP(100);
//		}
//	}
//
//	
//
//
//public:
//	virtual void connect()
//	{
//
//	}
//
//	
//
//	// Inherited via Listener
//	virtual void debugReturn(int debugLevel, const Common::JString& string) override
//	{
//	}
//
//	virtual void connectionErrorReturn(int errorCode) override
//	{
//	}
//
//	virtual void clientErrorReturn(int errorCode) override
//	{
//	}
//
//	virtual void warningReturn(int warningCode) override
//	{
//	}
//
//	virtual void serverErrorReturn(int errorCode) override
//	{
//	}
//
//	virtual void joinRoomEventAction(int playerNr, const Common::JVector<int>& playernrs, const LoadBalancing::Player& player) override
//	{
//	}
//
//	virtual void leaveRoomEventAction(int playerNr, bool isInactive) override
//	{
//	}
//
//	virtual void customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent) override
//	{
//	}
//
//	virtual void connectReturn(int errorCode, const Common::JString& errorString, const Common::JString& region, const Common::JString& cluster) override
//	{
//	}
//
//	virtual void disconnectReturn(void) override
//	{
//	}
//
//	virtual void leaveRoomReturn(int errorCode, const Common::JString& errorString) override
//	{
//	}
//
//};
//
////Listener::Listener() : Client(*this, "", "01042015_1.28")
////{
////	Run();
////}
////void Listener::connect(const ExitGames::LoadBalancing::ConnectOptions options)
////{
////}
////void Listener::connectionErrorReturn(int errorCode)
////{
////}
////void Listener::clientErrorReturn(int errorCode)
////{
////}
////void Listener::warningReturn(int warningCode)
////{
////}
////void Listener::serverErrorReturn(int errorCode)
////{
////}
////void Listener::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
////{
////}
////void Listener::leaveRoomEventAction(int playerNr, bool isInactive)
////{
////}
////void Listener::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
////{
////}
////void Listener::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
////{
////}
////void Listener::disconnectReturn(void)
////{
////}
////void Listener::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
////{
////}
////void Listener::connect(const ExitGames::LoadBalancing::ConnectOptions options)
////{
////
////}