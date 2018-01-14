#include "Keyboard.h"


void main( ) {
	Keyboard::initialize( );
	int key = ( int )'D';
	std::shared_ptr< Keyboard > keyboard = Keyboard::get( );

	while ( !keyboard->isHitKey( Keyboard::KEY_ESCAPE ) ) {
		keyboard->update( );
	}
	int check = 0;
}