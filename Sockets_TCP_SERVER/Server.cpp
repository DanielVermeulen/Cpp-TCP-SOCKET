
#include "SERVER.h"
#include "Clients.h"


//Main Globals
std::vector<Client> ConClients; //Stores all connected clients
Client DummyClient(NULL, "NULL"); 
int NumOfConnectionsATM = 0; //Intiger representation of client vector size
//Remember to specify the index size!!!!!!!!!!
string CLIENT_COMMANDS[3] = { "say", "login", "disconnect" }; // Commands that clients can run
string toSend, toPrint;
//mutex mu;

unsigned __stdcall ClientSession(void * data); // thread to handle the cleints 

Client GetClient(int index);
Client GetClient(string username);
void Inc_Dec_ClientCount(bool inc); //If inc is true we increase the NumOfConnectionATM by 1 else decrease by 1
int GetNumOfConnectionATM(); 
void AddClient(Client client);
void RemoveClient(int id); 
void shared_print(string msg);

//Server command processign 
bool ConvertToCommand(string);
Client ConvertToCommand(void * sock, string arry); 
bool RunCommand(string, string);
Client RunCommand(void * sock, string, string);

void main()
{
	//thread TCP(recvANDaccept);
	//main Locals
	long SUCESSFUL;
	WSAData WinSockData;
	WORD DLLVERSION;
	string CONVERTOR;

	//WORDS = objects of a data size that a processor natuarally handles (such as 16 or 32-bits)
	DLLVERSION = MAKEWORD(2, 1); //macro to create WORD value by concatenating its arguments

								 //Start Winsock DLL
	SUCESSFUL = WSAStartup(DLLVERSION, &WinSockData);

	//Create Socket Structure
	SOCKADDR_IN ADDRESS;
	int AddressSize = sizeof(ADDRESS);

	//Create Sockets 
	SOCKET sock_LISTEN; //Listen for incoming connections
	SOCKET sock_CONNECTION; //activates if connection is found

	//socket Arguments: AF_INET = internet domain (not Unix domain),
	//SOCK_STREAM = connection oriented TCP (NOT SOCK_DATAGRAM)
	sock_CONNECTION = socket(AF_INET, SOCK_STREAM, NULL);
	ADDRESS.sin_addr.s_addr = inet_addr("127.0.0.1"); //Set IP
	ADDRESS.sin_family = AF_INET;
	ADDRESS.sin_port = htons(444);

	sock_LISTEN = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sock_LISTEN, (SOCKADDR*)&ADDRESS, sizeof(ADDRESS));
	listen(sock_LISTEN, SOMAXCONN);
	shared_print("TCP Server started and listening on 127.0.0.1/24");

	//if connection found
	for (;;)
	{
		if (sock_CONNECTION = accept(sock_LISTEN, (SOCKADDR*)&ADDRESS, &AddressSize))
		{
			toPrint.clear();
			toPrint = "Incommin connection found, sending authentication request...";
			shared_print(toPrint);
			toPrint.clear();
			unsigned ClientSessionThread;
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &ClientSession, (void*)sock_CONNECTION, 0, &ClientSessionThread);
		} 
	}
}




unsigned __stdcall ClientSession(void * data)
{
	SOCKET client_socket = (SOCKET)data;
	char buffer[1024];
	string CONVERTER;
	
	toSend.clear();
	string toSend = "/authenticate";

	if (send(client_socket, toSend.c_str(), toSend.size(), NULL) != SOCKET_ERROR)
	{
		toPrint.clear();
		toPrint = "Authentication request sent!";
		shared_print(toPrint);
		toSend.clear();
		toPrint.clear();
	}
	else
		closesocket(client_socket);
	if (recv(client_socket, buffer, sizeof(buffer), NULL) != SOCKET_ERROR)
	{
		toPrint.clear();
		toPrint = "Client responded with: " + (string)buffer;
		shared_print(toPrint);
		toPrint.clear();
		CONVERTER = buffer;
		Client client = ConvertToCommand((void*)client_socket, CONVERTER);
		if (client.isValid())
		{
			while (client.isValid())
			{
				string msg;
				msg = client.recvMESSAGE();
				if (msg == "dc")
				{
					client.CloseSock();
					toPrint.clear();
					toPrint = (client.GetUsername()) + " has disconnected.";
					shared_print(toPrint);
					toPrint.clear();
					RemoveClient(client.GetID());
					return 0;
				}
				else {
					shared_print((client.GetUsername()) + ": " + msg);
				}
			}	
			closesocket(client_socket);
			//WSACleanup();
			return 0;
		} else {
			closesocket(client_socket);
			//WSACleanup();
			return 0;
		}
	} else {
		closesocket(client_socket);
		//WSACleanup();
		return 0;
	}
	closesocket(client_socket);
	//WSACleanup();
	return 0;
}

Client GetClient(int index)
{
	//std::lock_guard<std::mutex> gaurd(mu);
	return ConClients[index];
}

Client GetClient(string username)
{
	//std::lock_guard<std::mutex> gaurd(mu);
	for (int i = 0; i < NumOfConnectionsATM; i++)
	{
		string user = ConClients[i].GetUsername();
		if (user == username)
		{
			return ConClients[i];
		}
	}
	return DummyClient;
}

void Inc_Dec_ClientCount(bool inc) //If inc is true we increase the NumOfConnectionATM by 1 else decrease by 1
{
	//std::lock_guard<std::mutex> gaurd(mu);
	if (inc)
		NumOfConnectionsATM++;
	else
		NumOfConnectionsATM--;
}

int GetNumOfConnectionATM()
{
	//std::lock_guard<std::mutex> gaurd(mu);
	return NumOfConnectionsATM;
}

void AddClient(Client client) {
	ConClients.push_back(client);
	NumOfConnectionsATM++;
}

void RemoveClient(int id) {
	ConClients.erase(ConClients.begin() + (id - 1));
	NumOfConnectionsATM--;
}

void shared_print(string msg)
{
	//std::lock_guard<std::mutex> gaurd(mu);
	cout << "\n\t" << msg.c_str() << endl;
}

//bool ConvertToCommand(string arry)
//{
//	if (arry.substr(0, 1) == "/")
//	{
//		int endpos = arry.find(' ');
//		string ToCompair = arry.substr(1, endpos - 1);
//		//REMEMBER TO SPECIFY AMOUT OF COMMANDS
//		for (int i = 0; i < 3; i++)
//		{
//			if (ToCompair == CLIENT_COMMANDS[i])
//			{
//				string args = arry.substr((endpos + 1));
//				if (RunCommand(ToCompair, args))
//					return true;
//				else
//					return false;
//			}
//
//		}
//
//	}
//	return false;
//}
Client ConvertToCommand(void * sock, string arry)
{
	if (arry.substr(0, 1) == "/")
	{
		int endpos = arry.find(' ');
		string ToCompair = arry.substr(1, endpos - 1);
		//REMEMBER TO SPECIFY AMOUT OF COMMANDS
		for (int i = 0; i < 3; i++)
		{
			if (ToCompair == CLIENT_COMMANDS[i])
			{
				string args = arry.substr((endpos + 1));
				
				return RunCommand((SOCKET*)sock, ToCompair, args);
			}

		}

	}
	return DummyClient;
}


//bool RunCommand(string Input, string args)
//{
//	if (Input != "NULL")
//
//		string command; string command = Input;
//
//	//Setup array to store command arguments
//	string args_ = args;
//	string ArgsArry[10];
//
//	string delimiter = " ";
//	size_t pos = 0;
//	string token;
//	int i = 0;
//	if (command != "say")
//	{
//		//Find all arguments
//		while ((pos = args_.find(delimiter)) != string::npos)
//		{
//			token = args_.substr(0, pos);
//			ArgsArry[i] = token;
//			args_.erase(0, pos + delimiter.length());
//			//cout << ArgsArry[i] << i << endl;
//			i++;
//		}
//		ArgsArry[i] = args_;
//		//cout << ArgsArry[i] << i << endl;
//	}
//
//
//	if (command == "say")
//	{
//		cout << args << endl;
//	}
//	if (command == "login")
//	{
//		cout << "Logging you in as " << ArgsArry[0] << ", with password: " << ArgsArry[1] << endl;
//	}
//}
Client RunCommand(void * sock, string Input, string args)
{
	if (Input != "NULL")

	string command; string command = Input;

	SOCKET client_socket = (SOCKET)sock;

	//Setup array to store command arguments
	string args_ = args;
	string ArgsArry[10];

	string delimiter = " ";
	size_t pos = 0;
	string token;
	int i = 0;
	if (command != "say")
	{
		//Find all arguments
		while ((pos = args_.find(delimiter)) != string::npos)
		{
			token = args_.substr(0, pos);
			ArgsArry[i] = token;
			args_.erase(0, pos + delimiter.length());
			//cout << ArgsArry[i] << i << endl;
			i++;
		}
		ArgsArry[i] = args_;
		//cout << ArgsArry[i] << i << endl;
	}


	if (command == "say")
	{
		cout << args << endl;
	}
	if (command == "login")
	{
		string user = ArgsArry[0];
		string pass = ArgsArry[1];
		if (pass == "")
		{
			toSend.clear();
			send(client_socket, toSend.c_str(), toSend.size(), NULL);
			toSend.clear();
			Sleep(10);
			toPrint.clear();
			toPrint = "Invalid password, socket closed.";
			toPrint.clear();
			shared_print(toPrint);
			
			return DummyClient;
		} else {
			Client _Client(((void *)client_socket), user);
			AddClient(_Client);
			_Client.SetID(ConClients.size());
			toSend.clear();
			toSend = "Welkome " + user + ", you have been authenticated, and you are now connected!";		
			_Client.SendMESSAGE(toSend);
			toSend.clear();
			
			toPrint.clear();
			toPrint = user + " has been authenticated sucessfully!";
			shared_print(toPrint);
			toPrint.clear();
			
			return _Client;
		}
	}
}
