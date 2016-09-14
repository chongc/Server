#include "stdafx.h"
#include "ClientConnector.h"
#include <process.h>
#include <vector>
#include <iostream>
#include "Command.h"
std::vector <ClientConnector*> Connectors;
unsigned __stdcall ReceiveMessageThread(LPVOID IpParameter);

ClientConnector::ClientConnector(User* pUser, SOCKET &pSocket)
	:user(pUser), Connector(pSocket)
{
	
}
ClientConnector::ClientConnector(SOCKET &pSocket)
	:Connector(pSocket)
{
	user = NULL;
}

ClientConnector::~ClientConnector()
{

}

void ClientConnector::ClientStart()
{
	HANDLE receiveThread = (HANDLE)_beginthreadex(NULL, 0, &ReceiveMessageThread, this, 0, NULL);
}

unsigned __stdcall  ReceiveMessageThread(LPVOID IpParameter)
{
	//User u = GetUser()
	ClientConnector* con = (ClientConnector*)(LPVOID)IpParameter;
	SOCKET ClientSocket = con->GetSocket();

	extern char loginResult;
	do{

		Packet* p = con->ReceivePacket();
		if (p == NULL){
		
			delete p;
			break;
		}
			
		Command* c = Command::GetCommand(p->Type);
		c->Handle(*p, *con);
		delete p;
		delete c;

	} while (loginResult != LOGIN_OK);
	Connectors.push_back(con);
	while (true)
	{
		Packet* p = con->ReceivePacket();
		if (p == NULL)
		{
			if (con->GetUser() == NULL){
				std::cout << "客户端退出" << std::endl;
			}
			else{
				std::cout << con->GetUser()->GetName() << "退出" << std::endl;
			}
			delete p;
			break;
		}
			
		Command* cmd = Command::GetCommand(p->Type);
		cmd->Handle(*p, *con);
		delete p;
		delete cmd;
	}
	
	std::vector<ClientConnector*>::iterator iter = Connectors.begin();
	for (; iter != Connectors.end(); iter++)
	{
		if (*iter == con)
		{
			Connectors.erase(iter);
			break;
		}
	}
	
	
	closesocket(ClientSocket);
	return 0;
}

