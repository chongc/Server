#pragma once
#include "Command.h"
#include "Pack_C2S_QueryUserstatus.h"
class CMD_HandleQueryUserstatus :
	public Command
{
public:
	CMD_HandleQueryUserstatus();
	~CMD_HandleQueryUserstatus();
	static Command* Create();
	//void Accept(Connector* con)override;
	void Handle(Packet&, ClientConnector& con)override;

};

