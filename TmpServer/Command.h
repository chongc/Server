#pragma once
#include "ClientConnector.h"
#include <map>
class Command;
typedef Command* (*COMMAND_CREATOR)();

class Command
{
public:
	Command();
	~Command();
	static std::map<int, COMMAND_CREATOR> commandMap;
	static Command* GetCommand(char type);
	static void Initialize();
	//virtual void Accept(Connector* con)=0;
	virtual void Handle(Packet&, ClientConnector& con) = 0;

public:
	static char Type;
};

