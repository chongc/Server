#pragma once

#include <WinSock.h>
#pragma comment(lib, "ws2_32.lib")
#include "user.h"
#include <map>
#include <vector>


char VerifyLogin(const MyString& UserID, const MyString& PWD);
User* GetUser(const MyString& UserID);

char IsOnline(const MyString&);
SOCKET GetUserSocket(const MyString&);

void UserAddContact(User&);



