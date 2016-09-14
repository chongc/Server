#pragma once
#include "Command.h"
#include "Pack_C2S_Login.h"
class CMD_LoginVerify :public Command
{
public:
	CMD_LoginVerify();
	~CMD_LoginVerify();
	static Command* Create();
	//void Accept(Connector* con)override;
	void Handle(Packet&, ClientConnector& con)override;


};

