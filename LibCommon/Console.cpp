#include "Console.h"
#include <Windows.h>
#include <stdio.h>



Console::Console( ) {
}


Console::~Console( ) {
}

void Console::draw( int x, int y, const char* str ) {
	setCursorPos( x, y );
	printf_s( str );
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