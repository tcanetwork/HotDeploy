#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include "Log.h"

class Execute {
public:
	Execute( std::string exe_path );
	virtual ~Execute( );
public:
	void open( );
	void close( );
private:
	SHELLEXECUTEINFOA _shell;
	std::shared_ptr< Log > _log;
	std::string _exe;
	std::string _dir;
};

