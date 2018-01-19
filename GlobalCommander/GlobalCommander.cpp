#include "GlobalCommander.h"
#include "Option.h"
#include "DataBase.h"
#include "Ntp.h"
#include "Console.h"


GlobalCommander::GlobalCommander( ) {
	_ntp    = std::shared_ptr< Ntp      >( new Ntp      );
	_option = std::shared_ptr< Option   >( new Option   );
	_db     = std::shared_ptr< DataBase >( new DataBase );
	_active_machine_id.push_back( 1 );
	_active_machine_id.push_back( 3 );
	_active_machine_id.push_back( 4 );
	_active_machine_id.push_back( 2 );
	_active_machine_id.push_back( 4 );
	_active_machine_id.push_back( 7 );
	_active_machine_id.push_back( 8 );
	_active_machine_id.push_back( 29 );
}


GlobalCommander::~GlobalCommander( ) {
}


void GlobalCommander::update( ) {
}

void GlobalCommander::draw( ) const {
	_option->drawGameList( );
	drawMachineList( );
}

void GlobalCommander::drawMachineList( ) const {
	int x = 0;
	int y = 1;
	std::shared_ptr< Console > drawer;
	drawer->draw( x, y, "-----------------------------Active Machine Id-----------------------------" );
	y++;
	int size = ( int )_active_machine_id.size( );
	for ( int i = 0; i < size; i++ ) {
		if ( i % 8 == 0 && i != 0 ) {
			x = 0;
			y++;
		}
		char buf[ 64 ];
		sprintf_s( buf, "ID:%2d     ", _active_machine_id[ i ] );
		drawer->draw( x, y, buf );
		x += 10;
	}
	x = 0;
	y++;
	drawer->draw( x, y, "---------------------------------------------------------------------------" );
}
