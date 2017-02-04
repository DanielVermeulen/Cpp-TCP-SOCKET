#pragma once

#ifndef CLIENT_H_
#define CLIENT_H_


#include "SERVER.h"


class Client
{
public:
	//Constructors
	Client(void * _Client, string _ClientUsername);

	char MSG[250];

	//Member functions
	SOCKET sock();
	void SendMESSAGE(string _message);
	string recvMESSAGE();
	string GetUsername();
	void Kick();
	void Ban();
	bool isValid();
	void CloseSock();
	int GetID();
	void SetID(int id);
private:
	//Data Members
	SOCKET Client_SOCKET;
	int UserID;
	string Username = 0;
	WORD Password = 0;
};

#endif // !CLIENT_H_