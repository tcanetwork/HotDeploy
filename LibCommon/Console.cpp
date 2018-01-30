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

Console::Console( ) :
_cursor_x( 0 ),
_cursor_y( 0 ) {
	initArray( );
}


Console::~Console( ) {
}

void Console::update( ) {
	setConsoleSize( CONSOLE_WIDTH, CONSOLE_HEIGHT );
}

void Console::draw( ) {
	std::string str;
	setCursorPos( 0, 0 );
	for ( int i = 0; i < CONSOLE_HEIGHT; i++ ) {
		_screen[ i ][ CONSOLE_WIDTH - 1 ] = '\n';
		str += _screen[ i ].data( );
		for ( int j = 0; j < CONSOLE_WIDTH; j++ ) {
			_screen[ i ][ j ] = ' ';
		}
	}
	printf_s( "%s", str.c_str( ) );
	setCursorPos( _cursor_x, _cursor_y );
}

void Console::draw( int x, int y, const char* str ) {
	if ( y >= CONSOLE_HEIGHT ) {
		return;
	}
	if ( x >= CONSOLE_WIDTH ) {
		return;
	}
	int length = ( int )strnlen_s( str, 128 );
	if ( x + length >= CONSOLE_WIDTH ) {
		length = CONSOLE_WIDTH - x;
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
	if ( _cursor_x >= CONSOLE_WIDTH ) {
		return;
	}
	if ( _cursor_y >= CONSOLE_HEIGHT ) {
		return;
	}
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

void Console::setConsoleSize( int x, int y ) {
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	CONSOLE_SCREEN_BUFFER_INFOEX info = CONSOLE_SCREEN_BUFFER_INFOEX( );
	GetConsoleScreenBufferInfoEx( hCons, &info );
	info.dwMaximumWindowSize.X = x;
	info.dwMaximumWindowSize.Y = x;
	info.dwSize.X = x;
	info.dwSize.Y = y;
	info.srWindow.Top    = 0;
	info.srWindow.Left   = 0;
	info.srWindow.Right  = x;
	info.srWindow.Bottom = y;
	info.wAttributes = TRUE;
	SetConsoleScreenBufferInfoEx( hCons, &info );
}

void Console::initArray( ) {
	//サイズ変更
	for ( int i = 0; i < CONSOLE_HEIGHT; i++ ) {
		for ( int j = 0; j < CONSOLE_WIDTH; j++ ) {
			_screen[ i ][ j ] = ' ';
		}
		_screen[ i ][ CONSOLE_WIDTH ] = '\0';
	}
}
