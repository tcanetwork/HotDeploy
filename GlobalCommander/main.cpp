#include <memory>
#include <conio.h>
#include "GlobalCommander.h"
#include "Keyboard.h"
#include "Console.h"

const int FPS = 30;
const int FRAME_TIME = 1000 / FPS;
int before_time = 0;

bool isLoop( );
void wait( );

void main( ) {
	Keyboard::initialize( );
	std::shared_ptr< Console >( )->setCursorView( false );

	std::shared_ptr< GlobalCommander > commander( new GlobalCommander );
	int before = GetTickCount( );
	while ( isLoop( ) ) {
		commander->update( );

		wait( );
		commander->draw( );
	}
}


bool isLoop( ) {
	std::shared_ptr< Keyboard > keyboard = Keyboard::get( );
	keyboard->update( );
	return !keyboard->isHitKey( Keyboard::KEY_ESCAPE );
}

void wait( ) {
	int now = GetTickCount( );
	int wait_time = FRAME_TIME - ( now - before_time );
	if ( wait_time > 0 ) {
		Sleep( wait_time );
	}
}

void setCursorPos( int x, int y ) {
	HANDLE hCons = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition( hCons, pos );
}
