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


void Keyboard::update( ) {
	_key = KEY_NONE;
	if ( _kbhit( ) ) {
		_key = ( KEY )_getch( );
	}
}

bool Keyboard::isHitKey( KEY key ) const {
	return _key == key;
}