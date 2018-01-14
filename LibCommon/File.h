#pragma once
#include <string>
#include <vector>
#include <windows.h>

class File {
public:
	File( );
	virtual ~File( );
public:
	int getSize( std::string path );
	void load( std::string path, void* data, int size );
	void save( std::string path, const void* data, int size );
	void find( std::string path, std::vector< std::string >& data, bool deeply = true );
	void checkVersion( int *version );
	void createDir( std::string path, std::string::size_type start_pos );
};

