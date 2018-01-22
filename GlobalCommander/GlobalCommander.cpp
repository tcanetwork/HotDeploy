#include "GlobalCommander.h"
#include "Option.h"
#include "DataBase.h"
#include "Ntp.h"
#include "Console.h"
#include "Keyboard.h"
#include "stringConverter.h"

const int ACTIVE_TIME = 30;
const int CHECK_INTERVAL = 120;

GlobalCommander::GlobalCommander( ) :
_count( 0 ) {
	_ntp    = std::shared_ptr< Ntp      >( new Ntp      );
	_option = std::shared_ptr< Option   >( new Option   );
	_db     = std::shared_ptr< DataBase >( new DataBase );
}


GlobalCommander::~GlobalCommander( ) {
}


void GlobalCommander::update( ) {
	checkCommand( );

	if ( _count == 0 ) {
		checkActiveMachine( );
	}
	_count = ( _count + 1 ) % CHECK_INTERVAL;
}

void GlobalCommander::draw( ) const {
	drawMachineList( );
	_option->drawGameList( );
	drawWaitCommand( );
}

void GlobalCommander::drawMachineList( ) const {
	int x = 0;
	int y = 0;
	std::shared_ptr< Console > console = Console::get( );
	console->draw( x, y++, "-----------------------------Active Machine Id-----------------------------" );
	
	int size = ( int )_active_machine_id.size( );
	x = -10;
	for ( int i = 0; i < size; i++ ) {
		if ( i % 8 == 0 && i != 0 ) {
			x = -10;
			y++;
		}
		char buf[ 64 ];
		sprintf_s( buf, "ID:%2d", _active_machine_id[ i ] );
		console->draw( x += 10, y, buf );
	}
	x = 0;
	y++;
	console->draw( x, y, "---------------------------------------------------------------------------" );
}


void GlobalCommander::checkActiveMachine( ) {
	_active_machine_id.clear( );
	unsigned int now = _ntp->getTime( );
	std::vector< std::vector< std::string > > table = _db->selectTable( TABLE_HOT_DEPLOY );
	
	std::vector< std::string > time;
	std::vector< std::string > machine_id;
	int column_num = ( int )table.size( );
	for ( int i = 0; i < column_num; i++ ) {
		if ( ( int )table[ i ].size( ) == 0 ) {
			continue;
		}
		//MachineId
		if ( table[ i ][ 0 ] == COLUMN_MACHINE_ID ) {
			machine_id = table[ i ];
			machine_id.erase( machine_id.begin( ) );
			continue;
		}
		//time
		if ( table[ i ][ 0 ] == COLUMN_TIME ) {
			time = table[ i ];
			time.erase( time.begin( ) );
			continue;
		}
	}

	int row_num = ( int )machine_id.size( );
	for ( int i = 0; i < row_num; i++ ) {
		int diff_time = now - std::atoi( time[ i ].c_str( ) );
		if ( diff_time > ACTIVE_TIME ) {
			continue;
		}
		_active_machine_id.push_back( std::atoi( machine_id[ i ].c_str( ) ) );
	}
}

void GlobalCommander::checkCommand( ) {
	std::shared_ptr< Keyboard > key = Keyboard::get( );
	if ( !key->isHitKeyAny( ) ) {
		return;
	}
	if ( key->isHitKey( Keyboard::KEY_RETURN ) ) {
		return;
	}
	if ( key->isHitKey( Keyboard::KEY_ESCAPE ) ) {
		return;
	}
	std::shared_ptr< Console > console = Console::get( );
	//“ü—Í
	int machine_id = 0;
	int game_id = 0;
	console->setCursorView( true );
	scanf_s( "%d %d", &machine_id, &game_id );
	console->setCursorView( false );
	std::string condition = COLUMN_MACHINE_ID + "=" + std::to_string( machine_id );

	//Active Machine Check
	int machine_num = ( int )_active_machine_id.size( );
	bool active = false;
	for ( int i = 0; i < machine_num; i++ ) {
		if ( _active_machine_id[ i ] == machine_id ) {
			active = true;
			break;
		}
	}
	if ( !active ) {
		return;
	}

	_db->set( TABLE_HOT_DEPLOY, COLUMN_DOWNLOAD_ID, std::to_string( game_id ), condition );
}

void GlobalCommander::drawWaitCommand( ) const {
	std::shared_ptr< Console > console = Console::get( );
	console->draw( 0, 20, "Command Format: MachineId GameId" );
	console->draw( 0, 22, "Command:" );
	console->setCursorPosAfterDraw( 8, 22 );
}
