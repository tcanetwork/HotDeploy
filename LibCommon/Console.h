#pragma once
#include <memory>
#include <array>
#include <string>

const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 24;

class Console {
public:
	static void initialize( );
	static std::shared_ptr< class Console > get( );
public:
	Console( );
	virtual ~Console( );
public:
	void update( );
	void draw( );
	void draw( int x, int y, const char* str );
	void setCursorView( bool view );
	void setCursorPos( int x, int y );
	void setCursorPosAfterDraw( int x, int y );
	void getCursorPos( int& x, int& y );
	void setConsoleSize( int x, int y );
private:
	void initArray( );
private:
	std::array< std::array< char, CONSOLE_WIDTH + 1 >, CONSOLE_HEIGHT > _screen;
	int _cursor_x;
	int _cursor_y;
};

