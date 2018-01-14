#include "File.h"
#include <stdio.h>
#include <direct.h>

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

void File::load( std::string path, void* data, int size ) {
	FILE* fp = NULL;
	fopen_s( &fp, path.c_str( ), "rb" );
	if ( fp == NULL ) {
		return;
	}
	fread( data, 1, size, fp );
	fclose( fp );
}

void File::save( std::string path, const void* data, int size ) {
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
	std::string::size_type pos = path.find( "/", start_pos );
	//スラッシュがあった場合はディレクトリ生成
	if ( pos != std::string::npos ) {

		std::string dir = path.substr( 0, pos );
		_mkdir( dir.c_str( ) );

		std::string::size_type next_pos = path.find( "/", pos + 1 );
		if ( next_pos != std::string::npos ) {
			createDir( path, pos + 1 );
		}
	}
}