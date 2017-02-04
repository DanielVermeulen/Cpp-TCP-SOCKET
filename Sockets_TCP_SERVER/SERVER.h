//TCP Server Header file
//NOTE!!! You need to add ws_32.lib to the LINKER settings. Like so:
#pragma once 
#pragma comment(lib, "Ws2_32.lib")

//SRD Console header files
#include <sdkddkver.h>	
#include <conio.h>
#include <stdio.h>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//SOCKET header file
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>

#include <vector>
//#include <thread>
//#include <mutex>
#include <process.h>


#define SCK_VERSION2 0x0202

using namespace std;
