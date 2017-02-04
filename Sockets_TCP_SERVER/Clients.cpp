#include "Clients.h"

Client::Client(void * cl, string user):Client_SOCKET((*(SOCKET *)&cl)), Username(user) {}


void Client::SendMESSAGE(string _message)
{
	if (send(Client_SOCKET, _message.c_str(), _message.size(), NULL) == 0)
		true;
		//closesocket(Client_SOCKET);
}

std::string Client::recvMESSAGE()
{
	//char buf;
	//int err = recv(Client_SOCKET, &buf, 1, MSG_PEEK);
	if (recv(Client_SOCKET, MSG, sizeof(MSG), NULL) != SOCKET_ERROR)// && err != SOCKET_ERROR)
		return MSG;
	else
	{
		return "dc";
	}
		
}

string Client::GetUsername()
{
	return	Username;
}

bool Client::isValid()
{
	char buf;
	int err = recv(Client_SOCKET, &buf, 1, MSG_PEEK);
	/*if (err == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}*/
	if (Username != "NULL" || err != SOCKET_ERROR)
		return true;
	else
		return false;
}

SOCKET Client::sock()
{
	return Client_SOCKET;
}

void Client::CloseSock()
{
	closesocket(Client_SOCKET);
}

int Client::GetID()
{
	return UserID;
}

void Client::SetID(int id)
{
	UserID = id;
}