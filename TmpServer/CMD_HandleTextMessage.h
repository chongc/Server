#pragma once
#include "Command.h"
#include "Pack_C2S_TextMessage.h"
class CMD_HandleTextMessage : public Command
{
public:
	CMD_HandleTextMessage();
	~CMD_HandleTextMessage();
	static Command* Create();
	//void Accept(Connector* con)override;
	void Handle(Packet&, ClientConnector& con)override;

};

