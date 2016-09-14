#include "stdafx.h"
#include "CMD_ClientReceivedFile.h"
#include "Pack_C2S_Filefinish.h"
#include "ClientConnector.h"
#include "main.h"
CMD_ClientReceivedFile::CMD_ClientReceivedFile()
{
}


CMD_ClientReceivedFile::~CMD_ClientReceivedFile()
{
}

Command* CMD_ClientReceivedFile::Create()
{
	
	return new CMD_ClientReceivedFile();
}


void CMD_ClientReceivedFile::Accept(Connector* con)
{
//	Con = con;
//
//	p_c2s_filefns = (Pack_C2S_Filefinish*)Con->ReceivePacket();
//	
//	Handle();
//
//
}

void CMD_ClientReceivedFile::Handle(Packet& p, ClientConnector& con)
{
	Pack_C2S_Filefinish* p_c2s_filefns = (Pack_C2S_Filefinish*)&p;
	
	Pack_C2S_Filefinish ff(con.GetUser()->GetId(), p_c2s_filefns->fileFinish);
	User* u = GetUser(p_c2s_filefns->ToId);
	SOCKET ToSocket = GetUserSocket(p_c2s_filefns->ToId);
	ClientConnector s2c(u, ToSocket);
	s2c.SendPacket(ff);
}
