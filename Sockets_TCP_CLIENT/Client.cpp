//TCP Client source file 

#include "CLIENT.h"
#include <vector>

using namespace std;

std::mutex mu;

//Globals
bool Connected = false;

void shared_print(string msg);
bool ConStatus(bool change);

void recvDATA(void * sock);

void main()
{
	//Locals
	long SUCCESSFUL;
	WSAData WinSockData;
	WORD DLLVersion;

	DLLVersion = MAKEWORD(2, 1);

	SUCCESSFUL = WSAStartup(DLLVersion, &WinSockData);

	string RESPONSE;
	string CONVERTER;
	char MESSAGE[250];

	SOCKADDR_IN ADDRESS;

	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, NULL);


	ADDRESS.sin_addr.s_addr = inet_addr("127.0.0.1");
	ADDRESS.sin_family = AF_INET;
	ADDRESS.sin_port = htons(444);

	cout << "\n\tConnecting you to the server..." << endl;
	connect(sock, (SOCKADDR*)&ADDRESS, sizeof(ADDRESS));

	

	if (ConStatus(false) == false)
		ConStatus(true);

	thread recev(recvDATA, (void *)sock);

	while (ConStatus(false) == true)
	{
		string msg;
		getline(cin, msg);
		if (msg != "")
			send(sock, msg.c_str(), sizeof(msg), NULL);
	}

	cout << "\n\n\t";
	system("PAUSE");
	recev.join();
	exit(1);
}

void recvDATA(void * sock)
{
	SOCKET MY_SOCKET = (*(SOCKET *)&sock);

	while ((ConStatus(false)))
	{
		char testbyte[1000000];
		char MESSAGE[10];
		string CONVERTER;
		string msg;
		if (recv(MY_SOCKET, testbyte, 1, MSG_PEEK) == SOCKET_ERROR)
		{
			closesocket(MY_SOCKET);
			shared_print("Connection lost, disconnected!");
			ConStatus(true);
			break;
		} else
			while (int sz = recv(MY_SOCKET, testbyte, sizeof(testbyte), MSG_PEEK))
			{
				//shared_print("Loop");
				memset(MESSAGE, 0, sizeof(MESSAGE));
				//shared_print(to_string(sz));
				if (recv(MY_SOCKET, MESSAGE, sizeof(MESSAGE), NULL) != SOCKET_ERROR)
				{
					msg.append(MESSAGE, sizeof(MESSAGE));
					//shared_print(msg);
					
					if (sz < 10)
						break;
				}
				else
				{
					closesocket(MY_SOCKET);
					shared_print("Connection lost, disconnected!");
					ConStatus(true);
					break;
				}
			}
		shared_print( msg);
		msg.clear();
	}
}

void shared_print(string msg)
{
	std::lock_guard<std::mutex> gaurd(mu); 
	cout << "\n\t" << msg.c_str() << endl;
}
bool ConStatus(bool change)
{
	std::lock_guard<std::mutex> gaurd(mu); 
	if (change == true)
	{
		if (Connected == false)
			Connected = true;
		else
			Connected = false;
		//cout << Connected << " SET" << endl;
	} else {
		//cout << Connected << " CHECK" << endl;
		return Connected;
	}
}