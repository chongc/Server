#include "stdafx.h"
#include "user.h"


User::User(const MyString& pId, const MyString& pUsername, const MyString& pDept)
	:id(pId), username(pUsername), dept(pDept)
{
	
}


void User::SetPassword(const MyString& pPassword)
{
	password = pPassword;
}

User::~User()
{

}

int User::Verify(const MyString& pwd)
{
	if (pwd==password)
		return LOGIN_OK;
	else
		return LOGIN_PASSWORDERR;
}