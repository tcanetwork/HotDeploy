#pragma once
#include <string>
#include <vector>
#include <windows.h>

class File {
public:
	enum FILETYPE {
		FILETYPE_BINARY,
		FILETYPE_TEXT
	};
public:
	File( );
	virtual ~File( );
public:
	int getSize( std::string path );
	void load( std::string path, void* data, int size, FILETYPE type );
	void save( std::string path, const void* data, int size, FILETYPE type );
	void find( std::string path, std::vector< std::string >& data, bool deeply = true );
	void checkVersion( int *version );
	void createDir( std::string path, std::string::size_type start_pos );
};

