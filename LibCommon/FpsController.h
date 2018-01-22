#pragma once

const int FPS = 30;

class FpsController {
public:
	FpsController( );
	virtual ~FpsController( );
public:
	void update( );
	bool isOverFps( ) const;
	double getFps( ) const;
private:
	void init( );
	void wait( );
private:
	int _start_time;
	int _count;
	int _wait_time;
	double _fps;
};

