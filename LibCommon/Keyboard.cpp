#include "Keyboard.h"
#include <conio.h>

static std::shared_ptr< class Keyboard > keyboard;

//-------------static-------------//
void Keyboard::initialize( ) {
	if ( keyboard == std::shared_ptr< Keyboard >( ) ) {
		keyboard = std::shared_ptr< Keyboard >( new Keyboard );
	}
}

std::shared_ptr< class Keyboard > Keyboard::get( ) {
	return keyboard;
}

//--------------------------------//

Keyboard::Keyboard( ) {
	for ( int i = 0; i < 256; i++ ) {
		_key[ i ] = 0;
	}
}

Keyboard::~Keyboard( ) {
}

bool Keyboard::isHitKey( KEY key ) const {
	return _key[ key ] > 0;
}

bool Keyboard::isPushKey( KEY key ) const {
	return _key[ key ] == 1;
}

bool Keyboard::isHoldKey( KEY key ) const {
	return _key[ key ] > 1;
}

bool Keyboard::isHitKeyAny( ) const {
	return _kbhit( ) > 0;
}

void Keyboard::update( ) {
	for ( int i = 0; i < 256; i++ ) {
		if ( ( GetAsyncKeyState( i ) & 0x8000 ) != 0 ) {
			_key[ i ]++;
			if ( _key[ i ] > 200 ) {
				_key[ i ] = 2;
			}
		} else {
			_key[ i ] = 0;
		}
	}
}

void Keyboard::refleshInputKey( ) {
	_getch( );
}
