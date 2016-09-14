#pragma once
#include "Command.h"
#include "Pack_C2S_GetContacts.h"
class CMD_GetContacts :
	public Command
{
public:
	CMD_GetContacts();
	~CMD_GetContacts();
	static Command* Create();
	//void Accept(Connector* con)override;
	void Handle(Packet&, ClientConnector& con)override;

};

