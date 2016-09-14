#pragma once


#include "Connector.h"
#include "User.h"
class ClientConnector :public Connector
{
public:
	ClientConnector(){ user = NULL; };

	ClientConnector(SOCKET &pSocket);
	ClientConnector(User* pUser, SOCKET &pSocket);
	~ClientConnector();
	void ClientStart();
	void SetUser(User* pUser){ user = pUser; };
	User* GetUser(){ return user; }
private:
	User* user;
	
};


