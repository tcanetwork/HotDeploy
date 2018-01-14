#include "Log.h"
//#include "DxLib.h"
#define DXLIB 0

Log::Log( ) {
	_idx = 0;
	_idx_once = 0;
	_log = { };
	_log_once = { };
}


Log::~Log( ) {
}

void Log::draw( ) {
#if DXLIB
	int x = 10;
	int y = 0;
	for ( int i = 0; i < MAX_MESSAGE; i++ ) {
		int idx = ( _idx_once + i ) % MAX_MESSAGE;
		if ( _log_once[ idx ] == "" ) {
			continue;
		}
		
		DrawString( x, y, _log_once[ idx ].c_str( ), GetColor( 255, 255, 255 ) );
		_log_once[ idx ] = "";
		y += 20;
	}
	_idx_once = 0;

	x = 300;
	y = 0;
	for ( int i = 0; i < MAX_MESSAGE; i++ ) {
		int idx = ( _idx + i ) % MAX_MESSAGE;
		if ( _log[ idx ] == "" ) {
			continue;
		}
		DrawString( x, y, _log[ idx ].c_str( ), GetColor( 255, 255, 255 ) );
		y += 20;
	}
#endif
}

void Log::addMessage( std::string message ) {
	_log[ _idx ] = message;
	_idx = ( _idx + 1 ) % MAX_MESSAGE;
}

void Log::addOnceMessage( std::string message ) {
	_log_once[ _idx_once ] = message;
	_idx_once = ( _idx_once + 1 ) % MAX_MESSAGE;
}