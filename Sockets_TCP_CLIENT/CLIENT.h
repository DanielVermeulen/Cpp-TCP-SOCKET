//TCP CLIENT header file

//Must include "Ws2_32.lib".
#pragma once
#pragma comment(lib, "Ws2_32.lib")

//Standard Header file
#include <sdkddkver.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>

//Threading Header file
#include <thread>
#include <mutex>

#define SCK_VERSION 0x0202