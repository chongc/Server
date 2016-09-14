#include "stdafx.h"
#include "CMD_HandleQueryUserstatus.h"
#include "main.h"
#include "Pack_S2C_UserstatusReturn.h"
CMD_HandleQueryUserstatus::CMD_HandleQueryUserstatus()
{
}


CMD_HandleQueryUserstatus::~CMD_HandleQueryUserstatus()
{
}

Command* CMD_HandleQueryUserstatus::Create()
{
	
	return new CMD_HandleQueryUserstatus();
}




void CMD_HandleQueryUserstatus::Handle(Packet& p, ClientConnector& con)
{
	Pack_C2S_QueryUserstatus* p_c2s_qUserstatus = (Pack_C2S_QueryUserstatus*)&p;
	char UserStatusResult = IsOnline(p_c2s_qUserstatus->ToId);
	Pack_S2C_UserstatusReturn ur(UserStatusResult);
	con.SendPacket(ur);
}