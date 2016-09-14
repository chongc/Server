#pragma once
#include "Command.h"
#include "Pack_C2S_File.h"
class CMD_ServerReceiveFile :
	public Command
{
public:
	CMD_ServerReceiveFile();
	~CMD_ServerReceiveFile();
	static Command* Create();
	//void Accept(Connector* con)override;
	void Handle(Packet&, ClientConnector& con)override;
};

