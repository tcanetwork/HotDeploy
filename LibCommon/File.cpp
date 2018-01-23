#include "File.h"
#include "stringConverter.h"
#include <stdio.h>
#include <direct.h>
#include <Shlwapi.h>
#include <ImageHlp.h>

#pragma comment( lib, "Shlwapi.lib" )
#pragma comment( lib, "imagehlp.lib" )

const std::string VERSION_DIR = "version/";

File::File( ) {
}

File::~File( ) {
}

int File::getSize( std::string path ) {
	struct stat buf = struct stat( );
	stat( path.c_str( ), &buf );
	return ( int )buf.st_size;
}

void File::load( std::string path, void* data, int size, FILETYPE type ) {
	FILE* fp = NULL;
	fopen_s( &fp, path.c_str( ), "rb" );

	if ( fp == NULL ) {
		return;
	}
	fread( data, 1, size, fp );
	fclose( fp );

	if ( type == FILETYPE_TEXT ) {
		( ( char* )data )[ size - 1 ] = '\0';
	}
}

void File::save( std::string path, const void* data, int size, FILETYPE type ) {
	createDir( path, 0 );
	FILE* fp = NULL;

	if ( fopen_s( &fp, path.c_str( ), "wb" ) != 0 ) {
		return;	
	}

	fwrite( data, 1, size, fp );
	fclose( fp );
}

void File::find( std::string path, std::vector< std::string >& data, bool deeply ) {
	WIN32_FIND_DATA find;	//検索しているデータ
	HANDLE handle;

	//ハンドル取得
	handle = FindFirstFile( ( path + "*" ).c_str( ), &find );
	do {
		//ディレクトリの判定
		if ( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
			if ( deeply ) {
				if ( strcmp( find.cFileName, "." ) != 0 && strcmp( find.cFileName, ".." ) != 0 ) {
					//ディレクトリ
					this->find( path + find.cFileName + "/", data, deeply );
				}
			}
		} else {
			data.push_back( path + find.cFileName );
		}
	} while ( FindNextFile( handle, &find ) );

	FindClose( handle );
}

void File::checkVersion( int *version ) {
	WIN32_FIND_DATA find;	//検索しているデータ
	HANDLE handle;

	//ハンドル取得
	handle = FindFirstFile( ( VERSION_DIR + "*" ).c_str( ), &find );
	do {
		//ディレクトリの判定
		if ( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
		} else {
			int find_version = atoi( find.cFileName );
			if ( find_version > *version ) {
				*version = find_version;
			}
		}
	} while ( FindNextFile( handle, &find ) );

	FindClose( handle );
}

void File::createDir( std::string path, std::string::size_type start_pos ) {
	convertString( path, "/", "\\" );
	std::size_t pos = path.find( "\\", start_pos );
	char buf[ 128 ];
	GetCurrentDirectory( 128, buf );
	//スラッシュがあった場合はディレクトリ生成
	while ( pos != std::string::npos ) {

		std::string dir = buf + ( std::string )"\\" + path.substr( 0, pos );
		_mkdir( dir.c_str( ) );

		pos = path.find( "\\", pos + 1 );
	}
}

void File::createDir( std::string dir ) {
	convertString( dir, "/", "\\" );
	char buf[ 128 ];
	GetCurrentDirectory( 128, buf );
	dir = buf + ( std::string )"\\" + dir;

	if ( !PathIsDirectory( dir.c_str( ) ) ) {
		MakeSureDirectoryPathExists( dir.c_str( ) );
	}
}

void File::deleteFile( std::string path ) {
	convertString( path, "/", "\\" );
	char buf[ 128 ];
	GetCurrentDirectory( 128, buf );
	path = buf + ( std::string )"\\" + path;
	DeleteFile( path.c_str( ) );
}