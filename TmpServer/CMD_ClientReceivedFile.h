#pragma once
#include "Command.h"
#include "Pack_C2S_Filefinish.h"
class CMD_ClientReceivedFile : public Command
{
public:
	CMD_ClientReceivedFile();
	~CMD_ClientReceivedFile();
	static Command* Create();
	//void Accept(Connector* con)override;
	void Handle(Packet&, ClientConnector& con)override;
};

