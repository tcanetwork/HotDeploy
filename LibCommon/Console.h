#pragma once
#include <memory>
#include <vector>
#include <string>

class Console {
public:
	static void initialize( );
	static std::shared_ptr< class Console > get( );
public:
	Console( );
	virtual ~Console( );
public:
	void draw( );
	void draw( int x, int y, const char* str );
	void setCursorView( bool view );
	void setCursorPos( int x, int y );
	void setCursorPosAfterDraw( int x, int y );
	void getCursorPos( int& x, int& y );
	void getConsoleSize( int& x, int& y );
private:
	std::vector< std::vector< char > > _screen;
	int _width;
	int _height;
	int _cursor_x;
	int _cursor_y;
};

