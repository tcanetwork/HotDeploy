#include "FpsController.h"
#include <Windows.h>

const int UPDATE_INTERVAL = 30;
const int FRAME_TIME = 1000 / FPS;//  ˆê•b/ˆê•bŠÔ‚ÌXV‰ñ”

#define FPS_SKIP 0

FpsController::FpsController( ) :
_count( UPDATE_INTERVAL ) {
	_fps = FPS;
	init( );
}


FpsController::~FpsController( ) {
}

void FpsController::init( ) {
	_start_time = GetTickCount( );
	_count = 0;
}

void FpsController::update( ) {
	if ( _count >= UPDATE_INTERVAL ) {
		int now_time = GetTickCount( ) - _start_time;
		int frame_time = now_time / UPDATE_INTERVAL;
		_fps = 1000.0 / frame_time;
		init( );
	}

	_count++;
	wait( );
}

void FpsController::wait( ) {
	int now_time = GetTickCount( ) - _start_time;
	_wait_time = FRAME_TIME * _count - now_time;
	if ( _wait_time > 0 ) {
		Sleep( _wait_time );
		_wait_time = 0;
	}
}


bool FpsController::isOverFps( ) const {
#if FPS_SKIP
	return _wait_time < -FRAME_TIME;
#else
	return false;
#endif
}

double FpsController::getFps( ) const {
	return _fps;
}
