#include "stdafx.h"
#include "Command.h"
#include "Packet.h"
#include "CMD_LoginVerify.h"
#include "CMD_HandleQueryUserstatus.h"
#include "CMD_HandleTextMessage.h"
#include "CMD_ServerReceiveFile.h"
#include "CMD_HandleReceivefileReturn.h"
//#include "CMD_ClientReceivedFile.h"
#include "CMD_GetContacts.h"
std::map<int, COMMAND_CREATOR> Command::commandMap;
char Command::Type = 0;
Command::Command()
{
}


Command::~Command()
{
}

void Command::Initialize()
{
	Command::commandMap[PACKET_C2S_LOGIN] = CMD_LoginVerify::Create;
	Command::commandMap[PACKET_C2S_QUREYUSERSTATUS] = CMD_HandleQueryUserstatus::Create;
	Command::commandMap[PACKET_C2S_TEXTMESSAGE] = CMD_HandleTextMessage::Create;
	Command::commandMap[PACKET_C2S_FILE] = CMD_ServerReceiveFile::Create;
	Command::commandMap[PACKET_C2S_QUERYRECEIVEFILERESULT] = CMD_HandleReceivefileReturn::Create;
	//Command::commandMap[PACKET_C2S_FILEFINISH] = CMD_ClientReceivedFile::Create;
	Command::commandMap[PACKET_C2S_GETCONTACTS] = CMD_GetContacts::Create;

}

Command* Command::GetCommand(char type)
{
	Type = type;
	COMMAND_CREATOR creator = Command::commandMap[type];
	return creator();

}
