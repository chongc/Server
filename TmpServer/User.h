#pragma once


#include "MyString.h"
#include "MyArray.h"
#include "Contact.h"



class User
{
public:
	User(){};
	User(const MyString& pId, const MyString& pUsername, const MyString& pDept);
	MyString& GetId(){ return id; };
	MyString& GetName(){ return username; };
	MyString& GetDept(){ return dept; };
	~User();
	void SetPassword(const MyString& pPassword);
	int Verify(const MyString& pwd);
	
	MyString& GetPassWord(){ return password; }
	MyArray<Contact*>& GetContact(){ return ContactList; }
	void SetContact(Contact* c){ ContactList.Add(c); };
private:
	MyString id;
	MyString password;
	MyString username;
	MyString dept;
	MyArray<Contact*> ContactList;

};

//
//class NETAPI Contact{
//public:
//	Contact()=default;
//	Contact(User* u, MyString& nick){ 
//		user=u;
//		nickname = nick;
//	};
//	~Contact() = default;
//public:
//	User* user;
//	MyString nickname;
//};
