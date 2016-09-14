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
		std::cout << con.GetUser()->GetName() << "登录成功" << std::endl;
		Pack_S2C_LoginResult r(loginResult, con.GetUser()->GetName(), con.GetUser()->GetDept());
		con.SendPacket(r);
	}
	else{
		
		if (loginResult == LOGIN_PASSWORDERR)
			std::cout << "密码错误，请重试" << std::endl;
		if (loginResult == LOGIN_USERERR)
			std::cout << "用户名不存在，请重试" << std::endl;
		Pack_S2C_LoginResult r(loginResult);
		con.SendPacket(r);
	}
	

}
