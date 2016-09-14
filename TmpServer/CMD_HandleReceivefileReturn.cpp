#include "stdafx.h"
#include "CMD_HandleReceivefileReturn.h"
#include "Pack_C2S_QueryReceivefileResult.h"
#include "ClientConnector.h"
#include "main.h"
#include "Pack_C2S_File.h"
#include <iostream>
#include "Pack_S2C_QueryReceivefileorNot.h"

CMD_HandleReceivefileReturn::CMD_HandleReceivefileReturn()
{
}


CMD_HandleReceivefileReturn::~CMD_HandleReceivefileReturn()
{
}

Command* CMD_HandleReceivefileReturn::Create()
{
	
	return new CMD_HandleReceivefileReturn();
}




void CMD_HandleReceivefileReturn::Handle(Packet& p, ClientConnector& con)
{
	Pack_C2S_QueryReceivefileResult *p_c2s_qrfr = (Pack_C2S_QueryReceivefileResult *)&p;
	User* u = GetUser(p_c2s_qrfr->ToId);
	
	if (p_c2s_qrfr->result == USER_ACCEP)
	{
		extern MyString FileName;
		extern UINT32 FileLen;
		Pack_C2S_File pf(p_c2s_qrfr->ToId, FileName, FileLen);
		std::cout << "正在下载文件" << std::endl;
		con.SendPacket(pf);
		FILE *fp = fopen(pf.fileName, "rb");
		pf.Send(con, fp, pf.fileLength);
		fclose(fp);
		std::cout << "文件下载成功" << std::endl;
	}
	else{

		SOCKET ToSocket = GetUserSocket(p_c2s_qrfr->ToId);
		ClientConnector s2c_S2A(u, ToSocket);
		s2c_S2A.SendPacket(*p_c2s_qrfr);
	}

}
