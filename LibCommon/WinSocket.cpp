#include "WinSocket.h"



WinSocket::WinSocket( ) {
	//‰Šú‰»
	WSADATA data = WSADATA( );
	WSAStartup( MAKEWORD( 2, 0 ), &data );
}


WinSocket::~WinSocket( ) {
}

std::string WinSocket::getIp( ) const {
	char buf[ 128 ];
	gethostname( buf, 128 );
	std::string ip = gethostbyname( buf )->h_addr_list[ 0 ];
	return ip;
}