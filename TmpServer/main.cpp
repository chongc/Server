//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "stdafx.h"
#include "main.h"
#include "ClientConnector.h"
#include "MainConnector.h"
extern std::vector <ClientConnector*> Connectors;
std::vector<User*> Users;
std::map<int, Contact>ContactsMap;
static void Init()
{
	static User u1("", "赖秋生", "市场部");
	static User u2("002", "陶建风", "研发部");
	static User u3("003", "李双峰", "研发部");
	static User u9("009", "丁浩", "研发部");

	u1.SetPassword("");
	u2.SetPassword("");
	u3.SetPassword("");
	u9.SetPassword("");
	Users.push_back(&u1);
	Users.push_back(&u2);
	Users.push_back(&u3);
	Users.push_back(&u9);


	static Contact c1("", "赖秋生", "市场部");
	static Contact c2("002", "陶建风", "研发部");
	static Contact c3("003", "李双峰", "研发部");
	static Contact c4("004", "王伟", "研发部");
	static Contact c5("005", "王治扬", "研发部");
	static Contact c6("006", "彭碧芳", "市场部");
	static Contact c7("007", "伍河山", "研发部");
	static Contact c8("008", "李进", "研发部");
	static Contact c9("009", "丁浩", "研发部");

	ContactsMap[0] = c1;
	ContactsMap[1] = c2;
	ContactsMap[2] = c3;
	ContactsMap[3] = c4;
	ContactsMap[4] = c5;
	ContactsMap[5] = c6;
	ContactsMap[6] = c7;
	ContactsMap[7] = c8;
	ContactsMap[8] = c9;
	
	
	UserAddContact(u1);
	UserAddContact(u2);
	UserAddContact(u3);
	UserAddContact(u9);
	
}


int main()
{
	Init();
	int iResult = MainConnector::MainStart();


	return iResult;
}







char VerifyLogin(const MyString& UserID, const MyString& PWD)
{
	char ret = LOGIN_USERERR;
	std::vector<User*>::iterator iter = Users.begin();
	for (; iter != Users.end(); iter++){
		if(UserID == (*iter)->GetId()){
			ret = (*iter)->Verify(PWD);
			break;
		}
	}

	return ret;

}

User* GetUser(const MyString& UserID)
{
	
	std::vector<User*>::iterator iter = Users.begin();
	for (; iter != Users.end(); iter++){
		if (strcmp(UserID, (*iter)->GetId()) == 0){
			break;
		}
	}
	return *iter;
}

SOCKET GetUserSocket(const MyString& UserID)
{
	SOCKET socket;
	std::vector<ClientConnector*>::iterator iter = Connectors.begin();
	for (; iter != Connectors.end(); iter++){
		if ((*iter)->GetUser()->GetId()== UserID){
			socket = (*iter)->GetSocket();
			break;
		}
	}
	return socket;
}
char IsOnline(const MyString& UserID)
{
	char Status = USERSTATUS_OFFLINE;
	bool Online = false;
	std::vector<ClientConnector*>::iterator iter = Connectors.begin();
	for (; iter != Connectors.end(); iter++){
		if ((*iter)->GetUser() == NULL)continue;
		if ((*iter)->GetUser()->GetId()==UserID){
			Online = true;
			break;
		}
	}
	if (Online == true){
		Status = USERSTATUS_ONLINE;
	}
	else{
		Status = USERSTATUS_OFFLINE;
	}

	return Status;
}

void UserAddContact(User& u)
{
	
	for (unsigned int i = 0; i < ContactsMap.size(); i++){
		if (u.GetId() == ContactsMap[i].GetId()) continue;
		u.SetContact(&ContactsMap[i]);
	}

}