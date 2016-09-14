#pragma once
#include "Connector.h"

class MainConnector :public Connector
{
public:
	MainConnector(){};
	MainConnector(SOCKET &pSocket);
	static int MainStart();
	~MainConnector();
	
};

