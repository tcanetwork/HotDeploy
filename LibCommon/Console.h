#pragma once

class Console {
public:
	Console( );
	virtual ~Console( );
public:
	void draw( int x, int y, const char* str );
	void setCursorView( bool view );
	void setCursorPos( int x, int y );
};

