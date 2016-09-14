#include "stdafx.h"
#include "ClientConnector.h"
#include <process.h>
#include <vector>
#include <iostream>
#include "Command.h"
extern std::vector <ClientConnector*> Connectors;
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
	HANDLE receiveThread = (HANDLE)_beginthreadex(NULL, 0, &ReceiveMessageThread, (LPVOID)Connectors.size(), 0, NULL);
}

unsigned __stdcall  ReceiveMessageThread(LPVOID IpParameter)
{
	//User u = GetUser()
	int UserIndex = (int)(LPVOID)IpParameter - 1;
	ClientConnector* con = Connectors[UserIndex];
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
	
	/*std::vector<ClientConnector*>::iterator it = Connectors.begin() + UserIndex;
	Connectors.erase(it);*/
	closesocket(ClientSocket);
	return 0;
}

