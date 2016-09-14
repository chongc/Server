#include "stdafx.h"
#include "MainConnector.h"
#include "User.h"
#include <iostream>
#include <vector>
#include "Command.h"
#include "ClientConnector.h"
std::vector <ClientConnector*> Connectors;
MainConnector::MainConnector(SOCKET &pSocket) :Connector(pSocket)
{
}
MainConnector::~MainConnector()
{
	
}

int MainConnector::MainStart()
{
	int iResult = 0;
	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != NO_ERROR)
	{
		std::cout << "WSAStartup启动失败！" << std::endl;
		return 1;
	}

	SOCKET ServiceScoket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == ServiceScoket)
	{
		std::cout << "socket建立失败！" << std::endl;
		std::cout << "错误码是：" << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in addr = { AF_INET, htons(PORT) };
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	if (bind(ServiceScoket, (sockaddr*)&addr, sizeof(addr)) == 0)
	{
		std::cout << "开放端口" << PORT << std::endl;
	}
	else
	{
		std::cout << "bind绑定端口失败！" << std::endl;
		std::cout << "错误码是：" << WSAGetLastError() << std::endl;
		iResult = closesocket(ServiceScoket);
		if (iResult == SOCKET_ERROR)
			std::cout << " closesocket function failed with error " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;

	}

	if (listen(ServiceScoket, SOMAXCONN) == SOCKET_ERROR){
		std::cout << "listen function failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ServiceScoket);
		WSACleanup();
		return 1;
	}
	Command::Initialize();
	Packet::Initialize();
	while (true){
		sockaddr_in client;
		int sockaddrlenth = sizeof(client);
		SOCKET AcceptSocket = accept(ServiceScoket, (sockaddr*)&client, &sockaddrlenth);
		if (AcceptSocket == INVALID_SOCKET)
		{
			std::cout << "accept函数失败！" << std::endl;
			std::cout << "错误码是：" << WSAGetLastError() << std::endl;
			closesocket(ServiceScoket);
			WSACleanup();
			return 1;
		}
		std::cout << "有客户端正在尝试连接：" << inet_ntoa(client.sin_addr) << "--" << htons(client.sin_port) << std::endl;
		
		ClientConnector* cc = new ClientConnector(AcceptSocket);
		Connectors.push_back(cc);
		cc->ClientStart();
	}

	closesocket(ServiceScoket);
	WSACleanup();
	return 0;
}