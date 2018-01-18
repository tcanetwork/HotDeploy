#pragma once

//#define _WINSOCKAPI_
#define _WINSOCK2API_
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#pragma comment( lib, "ws2_32.lib" )

#include <string>

class WinSocket {
public:
	WinSocket( );
	virtual ~WinSocket( );
public:
	std::string getIp( ) const;
};

