#include "stdafx.h"
#include "CMD_ServerReceiveFile.h"
#include "Pack_S2C_QueryReceivefileorNot.h"
#include "ClientConnector.h"
#include "main.h"
#include "CMD_HandleReceivefileReturn.h"
#include "CMD_ClientReceivedFile.h"
#include <iostream>
MyString FileName;
UINT32 FileLen;
CMD_ServerReceiveFile::CMD_ServerReceiveFile()
{
}


CMD_ServerReceiveFile::~CMD_ServerReceiveFile()
{
}
Command* CMD_ServerReceiveFile::Create()
{
	
	return new CMD_ServerReceiveFile();
}





void CMD_ServerReceiveFile::Handle(Packet& p, ClientConnector& con)
{
	Pack_C2S_File* p_C2S_File = (Pack_C2S_File*)&p;
	p_C2S_File->SetFilePath(p_C2S_File->fileName);
	FILE *fp = fopen(p_C2S_File->fileName, "wb");
	p_C2S_File->Save(con, fp, p_C2S_File->fileLength);
	fclose(fp);
	Pack_S2C_QueryReceivefileorNot irf(con.GetUser()->GetId(), p_C2S_File->fileName, p_C2S_File->fileLength);
	User* u = GetUser(p_C2S_File->ToId);
	SOCKET ToSocket = GetUserSocket(p_C2S_File->ToId);
	ClientConnector s2c_S2B(u, ToSocket);
	s2c_S2B.SendPacket(irf);
	FileName = p_C2S_File->fileName;
	FileLen = p_C2S_File->fileLength;
}

