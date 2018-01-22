#pragma once
#include <memory>
#include <vector>

class GlobalCommander {
public:
	GlobalCommander( );
	virtual ~GlobalCommander( );
public:
	void update( );
	void draw( ) const;
private:
	void checkCommand( );
	void checkActiveMachine( );
	void drawMachineList( ) const;
	void drawWaitCommand( ) const;
private:
	std::shared_ptr< class DataBase > _db;
	std::shared_ptr< class Ntp      > _ntp;
	std::shared_ptr< class Option   > _option;
	std::vector< int > _active_machine_id;
	int _count;
};

