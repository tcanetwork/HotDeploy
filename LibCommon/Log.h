#pragma once
#include <string>
#include <array>

class Log {
public:
	Log( );
	virtual ~Log( );
public:
	void draw( );
	void addMessage( std::string message );
	void addOnceMessage( std::string message );
private:
	static const int MAX_MESSAGE = 20;
private:
	std::array< std::string, MAX_MESSAGE > _log;
	std::array< std::string, MAX_MESSAGE > _log_once;
	int _idx;
	int _idx_once;
};

