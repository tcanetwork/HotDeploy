#include <memory>
#include <conio.h>
#include "GlobalCommander.h"
#include "Keyboard.h"
#include "Console.h"
#include "FpsController.h"

bool isLoop( );

void main( ) {
	Keyboard::initialize( );
	Console::initialize( );
	std::shared_ptr< Console > console = Console::get( );
	std::shared_ptr< GlobalCommander > commander( new GlobalCommander );
	std::shared_ptr< FpsController > fps( new FpsController );
	int count = 0;
	while ( isLoop( ) ) {
		commander->update( );

		console->setCursorView( false );
		commander->draw( );
		console->draw( );
		if ( count % 20 == 0 ) {
			console->setCursorView( true );
		}
		fps->update( );
		count++;
	}
}


bool isLoop( ) {
	std::shared_ptr< Keyboard > keyboard = Keyboard::get( );
	keyboard->update( );
	return !keyboard->isHitKey( Keyboard::KEY_ESCAPE );
}