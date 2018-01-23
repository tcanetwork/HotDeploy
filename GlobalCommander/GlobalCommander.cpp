#include "GlobalCommander.h"
#include "Option.h"
#include "DataBase.h"
#include "Ntp.h"
#include "Console.h"
#include "Keyboard.h"
#include "stringConverter.h"

const int ACTIVE_TIME = 100;
const int CHECK_INTERVAL = 30 * 4;

GlobalCommander::GlobalCommander( ) :
_count( 0 ),
_next_input( false ) {
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
	int y = drawMachineList( 1 );
	y = _option->drawGameList( y + 1 );
	y = drawWaitCommand( y + 2 );
}

int GlobalCommander::drawMachineList( int y ) const {
	int x = 0;
	std::shared_ptr< Console > console = Console::get( );
	console->draw( x, y, "---------------------------Active Machine Id List---------------------------" );
	
	int size = ( int )_active_machine_id.size( );
	const int WIDTH = 10;
	x = -WIDTH;
	y++;
	for ( int i = 0; i < size; i++ ) {
		if ( i % 8 == 0 && i != 0 ) {
			x = -WIDTH;
			y++;
		}
		char buf[ 64 ];
		sprintf_s( buf, "ID:%2d", _active_machine_id[ i ] );
		console->draw( x += WIDTH, y, buf );
	}
	x = 0;
	y++;
	console->draw( x, y, "----------------------------------------------------------------------------" );
	return y + 1;
}

int GlobalCommander::drawWaitCommand( int y ) const {
	std::shared_ptr< Console > console = Console::get( );
	if ( _next_input ) {
		console->draw( 0, 0     , "入力中...   ※入力中はリストが更新されないので注意" );
	} else {
		console->draw( 0, 0     , "入力待機中  ※入力中はリストが更新されないので注意" );
	}
	console->draw( 0, y += 1, "Command Format : MachineId GameId" );
	console->draw( 0, y += 1, "Command Example: 1 2" );
	console->draw( 0, y += 2, "Command:" );
	console->setCursorPosAfterDraw( 8, y );
	return y;
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
	if ( !_next_input ) {
		if ( !key->isHitKeyAny( ) ) {
			return;
		}
		if ( key->isHitKey( Keyboard::KEY_RETURN ) ) {
			key->refleshInputKey( );
			return;
		}
		if ( key->isHitKey( Keyboard::KEY_ESCAPE ) ) {
			key->refleshInputKey( );
			return;
		}
		_next_input = true;
		return;
	}
	_next_input = false;
	//モード表示
	std::shared_ptr< Console > console = Console::get( );
	//入力
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
