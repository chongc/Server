#include "stdafx.h"
#include "CMD_GetContacts.h"
#include "Pack_C2S_GetContacts.h"
#include "main.h"
#include "Pack_S2C_ContactsReturn.h"
CMD_GetContacts::CMD_GetContacts()
{
}


CMD_GetContacts::~CMD_GetContacts()
{
}

Command* CMD_GetContacts::Create()
{
	
	return new CMD_GetContacts();
}



void CMD_GetContacts::Handle(Packet& p, ClientConnector& con)
{
	Pack_C2S_GetContacts * p_c2s_gContacts = (Pack_C2S_GetContacts*)&p;
	User* u = GetUser(p_c2s_gContacts->userId);
	Pack_S2C_ContactsReturn cr(u->GetContact());
	con.SendPacket(cr);
}

