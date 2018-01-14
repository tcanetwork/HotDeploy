#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <vector>
#include <WinInet.h>

#pragma comment( lib, "wininet" )

class Ftp {
public:
	Ftp( );
	virtual ~Ftp( );
public:
	void open( );
	void getFindData( std::string dir, std::vector< std::string >& data );
	void download( std::string ftp_path, std::string local_path );
	void upload( std::string ftp_path, std::string local_path );
	void deleteFile( std::string ftp_path );
	void close( );
private:
	HINTERNET _internet;
	HINTERNET _connect;
};

