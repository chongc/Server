#include "stdafx.h"
#include "CMD_HandleTextMessage.h"
#include "main.h"
#include "Pack_S2C_UserstatusReturn.h"
#include "Pack_C2S_TextMessage.h"
#include "ClientConnector.h"
CMD_HandleTextMessage::CMD_HandleTextMessage()
{
}


CMD_HandleTextMessage::~CMD_HandleTextMessage()
{
}
Command* CMD_HandleTextMessage::Create()
{
	
	return new CMD_HandleTextMessage();
}




void CMD_HandleTextMessage::Handle(Packet& p, ClientConnector& con)
{
	Pack_C2S_TextMessage * p_c2s_Textmsg = (Pack_C2S_TextMessage*)&p;
	char UserStatusResult = IsOnline(p_c2s_Textmsg->ToId);
	if (UserStatusResult == USERSTATUS_ONLINE){
		User* u = GetUser(p_c2s_Textmsg->ToId);
		SOCKET ToSocket = GetUserSocket(p_c2s_Textmsg->ToId);
		ClientConnector s2c(u, ToSocket);
		s2c.SendPacket(*p_c2s_Textmsg);
	}
}