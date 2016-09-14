#pragma once
#include "Command.h"
#include "Pack_C2S_QueryReceivefileResult.h"
class CMD_HandleReceivefileReturn :
	public Command
{
public:
	CMD_HandleReceivefileReturn();
	~CMD_HandleReceivefileReturn();
	static Command* Create();
	//void Accept(Connector* con)override;
	void Handle(Packet&, ClientConnector& con)override;

};

