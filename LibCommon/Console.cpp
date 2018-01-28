#include "Console.h"
#include <Windows.h>
#include <stdio.h>
#include <string>

static std::shared_ptr< Console > console;

void Console::initialize( ) {
	if ( console == std::shared_ptr< Console >( ) ) {
		console = std::shared_ptr< Console >( new Console );
	}
}

std::shared_ptr< class Console > Console::get( ) {
	return console;
}

Console::Console( ) {
	_cursor_x = 0;
	_cursor_y = 0;
	getConsoleSize( _width, _height );
	_screen.resize( _height );
	for ( int i = 0; i < _height; i++ ) {
		_screen[ i ].resize( _width + 1 );
		for ( int j = 0; j < _width; j++ ) {
			_screen[ i ][ j ] = ' ';
		}
		_screen[ i ][ _width ] = '\0';
	}
}


Console::~Console( ) {
}

void Console::draw( ) {
	std::string str;
	setCursorPos( 0, 0 );
	for ( int i = 0; i < _height; i++ ) {
		_screen[ i ][ _width - 1 ] = '\n';
		str += _screen[ i ].data( );
		//std::vector< char > str = _screen[ i ];
		for ( int j = 0; j < _width; j++ ) {
			_screen[ i ][ j ] = ' ';
		}
	}
	printf_s( "%s", str.c_str( ) );
	setCursorPos( _cursor_x, _cursor_y );
}

void Console::draw( int x, int y, const char* str ) {
	if ( y >= _height ) {
		return;
	}
	if ( x >= _width ) {
		return;
	}
	int length = ( int )strnlen_s( str, 128 );
	if ( x + length >= _width ) {
		length = _width - x;
	}
	memcpy_s( &_screen[ y ][ x ], length, str, length );
}

void Console::setCursorView( bool view ) {
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo( hCons, &info );
	info.bVisible = view;
	SetConsoleCursorInfo( hCons, &info );
}


void Console::setCursorPos( int x, int y ) {
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition( hCons, pos );
}

void Console::setCursorPosAfterDraw( int x, int y ) {
	_cursor_x = x;
	_cursor_y = y;
}

void Console::getCursorPos( int& x, int& y ) {
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_SCREEN_BUFFER_INFO info = CONSOLE_SCREEN_BUFFER_INFO( );
	GetConsoleScreenBufferInfo( hCons, &info );
	x = info.dwCursorPosition.X;
	y = info.dwCursorPosition.Y;
}

void Console::getConsoleSize( int& x, int& y ) {
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_SCREEN_BUFFER_INFO info = CONSOLE_SCREEN_BUFFER_INFO( );
	GetConsoleScreenBufferInfo( hCons, &info );
	x = info.srWindow.Right;
	y = info.srWindow.Bottom - 1;
}