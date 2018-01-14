#include "Ftp.h"
#include <string>
#include <Shlwapi.h>
#include <ImageHlp.h>

#pragma comment( lib, "Shlwapi.lib" )
#pragma comment( lib, "imagehlp.lib" )

const std::string IP = "160.16.140.103";
const std::string USER = "ftpuser";
const std::string PASS = "okurimasuyo";


Ftp::Ftp( ) {
}


Ftp::~Ftp( ) {
}

void Ftp::open( ) {	
	//Internet������
	_internet = InternetOpen( 
		"Agent",
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0 );

	//FTP�ڑ�
	_connect = InternetConnect(
		_internet,
		IP.c_str( ),
		INTERNET_DEFAULT_FTP_PORT,
		USER.c_str( ),
		PASS.c_str( ),
		INTERNET_SERVICE_FTP,
		0,
		0 );
}

void Ftp::getFindData( std::string dir, std::vector< std::string >& data ) {
	//FTP����dir��������
	open( );
	WIN32_FIND_DATAA find_data = WIN32_FIND_DATAA( );
	HINTERNET find = FtpFindFirstFileA( _connect, ( dir + "*" ).c_str( ), &find_data, INTERNET_FLAG_NO_CACHE_WRITE, 0 );

	if ( find != NULL ) {
		do {
			if ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
				if ( strcmp( find_data.cFileName, "." ) != 0 && strcmp( find_data.cFileName, ".." ) != 0 ) {
					//�f�B���N�g��
					getFindData( dir + find_data.cFileName + "/", data );
				}
			} else {
				data.push_back( dir + find_data.cFileName );
			}
		} while ( InternetFindNextFile( find, &find_data ) );
	}

	close( );
}

void Ftp::download( std::string ftp_path, std::string local_path ) {
	//�f�B���N�g��
	std::string dir;
	{
		char buf[ 128 ];
		GetCurrentDirectory( 128, buf );
		std::string str1 = "\\";
		std::string str2 = "/";
		std::string::size_type pos = max( ( int )local_path.find_last_of( str1 ), ( int )local_path.find_last_of( str2 ) );
		dir = ( std::string )buf + "/" + local_path.substr( 0, pos ) + "/";
		pos = dir.find( str2 );
		while ( pos != std::string::npos ) {
			dir.replace( pos, str2.length( ), str1 );
			pos = dir.find( str2 );
		}
		pos = ftp_path.find( str1 );
		while ( pos != std::string::npos ) {
			ftp_path.replace( pos, str1.length( ), str2 );
			pos = ftp_path.find( str1 );
		}
		pos = local_path.find( str1 );
		while ( pos != std::string::npos ) {
			local_path.replace( pos, str1.length( ), str2 );
			pos = local_path.find( str1 );
		}
	}
	if ( !PathIsDirectory( dir.c_str( ) ) ) {
		MakeSureDirectoryPathExists( dir.c_str( ) );
	}

	remove( local_path.c_str( ) );

	open( );
	FtpGetFile(	_connect,
		ftp_path.c_str( ),
		local_path.c_str( ),
		FALSE,
		FILE_ATTRIBUTE_ARCHIVE,
		FTP_TRANSFER_TYPE_BINARY,
		0 );
	close( );
}

void Ftp::upload( std::string ftp_path, std::string local_path ) {
	//FTP�ŃA�v���P�[�V�������擾
	open( );
	FtpPutFile(	_connect,
		local_path.c_str( ),
		ftp_path.c_str( ),
		FTP_TRANSFER_TYPE_BINARY,
		0 );
	close( );
}

void Ftp::deleteFile( std::string ftp_path ) {
	open( );
	FtpDeleteFileA( _connect, ftp_path.c_str( ) );
	close( );
}

void Ftp::close( ) {
	InternetCloseHandle( _connect );
	InternetCloseHandle( _internet );
}