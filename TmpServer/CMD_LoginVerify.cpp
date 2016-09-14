#include "stdafx.h"
#include "CMD_LoginVerify.h"
#include "Pack_S2C_LoginResult.h"
#include "ClientConnector.h"
#include "main.h"
#include <iostream>
char loginResult;
CMD_LoginVerify::CMD_LoginVerify()
{
}


CMD_LoginVerify::~CMD_LoginVerify()
{
	
}
Command* CMD_LoginVerify::Create()
{
	
	return new CMD_LoginVerify();
}





void CMD_LoginVerify::Handle(Packet& p, ClientConnector& con)
{
	Pack_C2S_Login* p_c2s_lg = (Pack_C2S_Login*)&p;
	loginResult = VerifyLogin(p_c2s_lg->userId, p_c2s_lg->userPassword);
	if (loginResult == LOGIN_OK){
		
		User* u = GetUser(p_c2s_lg->userId);
		con.SetUser(u);
		std::cout << con.GetUser()->GetName() << "��¼�ɹ�" << std::endl;
		Pack_S2C_LoginResult r(loginResult, con.GetUser()->GetName(), con.GetUser()->GetDept());
		con.SendPacket(r);
	}
	else{
		
		if (loginResult == LOGIN_PASSWORDERR)
			std::cout << "�������������" << std::endl;
		if (loginResult == LOGIN_USERERR)
			std::cout << "�û��������ڣ�������" << std::endl;
		Pack_S2C_LoginResult r(loginResult);
		con.SendPacket(r);
	}
	

}
