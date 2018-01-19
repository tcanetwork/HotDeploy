#pragma once
#include <Windows.h>
#include <memory>
#include <string>

class Execute {
public:
	Execute( std::shared_ptr< class Option > option );
	virtual ~Execute( );
public:
	void open( );
	void close( );
	bool isPlaying( ) const;
private:
	void checkPath( );
	void checkDir( );
private:
	bool _playing;
	SHELLEXECUTEINFOA _shell;
	std::shared_ptr< class Option > _option;
};

