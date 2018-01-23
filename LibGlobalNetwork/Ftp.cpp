#include "Ftp.h"
#include "File.h"
#include <string>
#include "stringConverter.h"


const std::string IP = "160.16.140.103";
const std::string USER = "ftpuser";
const std::string PASS = "okurimasuyo";


Ftp::Ftp( ) {
}


Ftp::~Ftp( ) {
}

void Ftp::open( ) {	
	//Internet初期化
	_internet = InternetOpen( 
		"Agent",
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0 );

	//FTP接続
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
	//FTP内のdir内を検索
	convertString( dir, "\\", "/" );
	open( );
	WIN32_FIND_DATAA find_data = WIN32_FIND_DATAA( );
	HINTERNET find = FtpFindFirstFileA( _connect, ( dir + "*" ).c_str( ), &find_data, INTERNET_FLAG_NO_CACHE_WRITE, 0 );

	if ( find != NULL ) {
		do {
			if ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
				if ( strcmp( find_data.cFileName, "." ) != 0 && strcmp( find_data.cFileName, ".." ) != 0 ) {
					//ディレクトリ
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
	convertString( ftp_path  , "\\", "/" );
	convertString( local_path, "\\", "/" );
	{//ディレクトリ
		std::string dir = local_path.substr( 0, local_path.find_last_of( "/" ) ) + "/";
		std::shared_ptr< File >( )->createDir( dir, 0 );
	}
	std::shared_ptr< File >( )->deleteFile( local_path );

	open( );
	FtpGetFile(	_connect,
		ftp_path.c_str( ),
		local_path.c_str( ),
		FALSE,
		FILE_ATTRIBUTE_ARCHIVE,
		FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,
		0 );
	close( );
}

void Ftp::upload( std::string ftp_path, std::string local_path ) {
	convertString( ftp_path  , "/", "\\" );
	convertString( local_path, "/", "\\" );
	//FTPでアプリケーションを取得
	open( );
	FtpPutFile(	_connect,
		local_path.c_str( ),
		ftp_path.c_str( ),
		FTP_TRANSFER_TYPE_BINARY,
		0 );
	close( );
}

void Ftp::deleteFile( std::string ftp_path ) {
	convertString( ftp_path  , "/", "\\" );
	open( );
	FtpDeleteFileA( _connect, ftp_path.c_str( ) );
	close( );
}

void Ftp::close( ) {
	InternetCloseHandle( _connect );
	InternetCloseHandle( _internet );
}