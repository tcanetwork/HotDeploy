#include "Keyboard.h"
#include <conio.h>

static std::shared_ptr< class Keyboard > keyboard;

Keyboard::Keyboard( ) {
	_key = KEY_NONE;
}


Keyboard::~Keyboard( ) {
}

void Keyboard::initialize( ) {
	if ( keyboard == std::shared_ptr< Keyboard >( ) ) {
		keyboard = std::shared_ptr< Keyboard >( new Keyboard );
	}
}

std::shared_ptr< class Keyboard > Keyboard::get( ) {
	return keyboard;
}

bool Keyboard::isHitKey( KEY key ) const {
	return ( GetAsyncKeyState( key ) & 0x8000 ) != 0;
}

bool Keyboard::isHitKeyAny( ) const {
	return _kbhit( ) > 0;
}
