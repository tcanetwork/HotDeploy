#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include "Log.h"

class Excute {
public:
	Excute( std::string exe, std::shared_ptr< Log > log );
	virtual ~Excute( );
public:
	void open( );
	void close( );
private:
	SHELLEXECUTEINFOA _shell;
	std::shared_ptr< Log > _log;
	std::string _exe;
	std::string _dir;
};

