#include "Option.h"
#include "File.h"
#include "Console.h"
#include "stringConverter.h"
#include <memory>

const std::string OPTION_FILE_DIR = "Option";
const std::string OPTION_DATA_FILE_PATH       = OPTION_FILE_DIR + "/" + "setting.txt";
const std::string OPTION_MACHINE_ID_FILE_PATH = OPTION_FILE_DIR + "/" + "MachineId.txt";
const std::string OPTION_GAME_ID_FILE_PATH    = OPTION_FILE_DIR + "/" + "GameId.txt";

Option::Option( ) {
	load( );
}


Option::~Option( ) {
}

void Option::load( ) {
	std::shared_ptr< File > file;
	{//Setting
		int size = file->getSize( OPTION_DATA_FILE_PATH ) + 1;
		if ( size > 0 ) {
			char* data = ( char* )malloc( size );
			file->load( OPTION_DATA_FILE_PATH, data, size, File::FILETYPE_TEXT );
			std::vector< std::string > lines = splitString( data, "\r\n" );

			int size = ( int )lines.size( );
			int id = 0;
			OptionData option = OptionData( );
			for ( int i = 0; i < size; i++ ) {
				std::vector< std::string > line = splitString( lines[ i ], ":" );
				if ( ( int )line.size( ) != 2 ) {
					continue;
				}

				if ( line[ 0 ] == "GameId" ) {
					if ( i != 0 ) {
						_data[ id ] = option;
						option = OptionData( );
					}
					id = std::atoi( line[ 1 ].c_str( ) );
				}

				if ( line[ 0 ] == "Name" ) {
					option.name = line[ 1 ];
				}
				if ( line[ 0 ] == "ExePath" ) {
					option.exe_path = line[ 1 ];
				}
				if ( line[ 0 ] == "FtpDir" ) {
					option.ftp_dir = line[ 1 ];
					convertString( option.ftp_dir, "\\", "/" );
				}
				if ( line[ 0 ] == "LocalDir" ) {
					option.local_dir = line[ 1 ];
					convertString( option.local_dir, "\\", "/" );
				}
			}
			_data[ id ] = option;
			free( data );
		}
	}
	{//MachineId
		_machine_id = -1;

		int size = file->getSize( OPTION_MACHINE_ID_FILE_PATH ) + 1;
		if ( size > 0 ) {
			char* data = ( char* )malloc( size );
			file->load( OPTION_MACHINE_ID_FILE_PATH, data, size, File::FILETYPE_TEXT );
			_machine_id = std::atoi( data );
			free( data );
		}
	}
	{//GameId
		_game_id = -1;
		int size = file->getSize( OPTION_GAME_ID_FILE_PATH ) + 1;
		if ( size > 0 ) {
			char* data = ( char* )malloc( size );
			file->load( OPTION_GAME_ID_FILE_PATH, data, size, File::FILETYPE_TEXT );
			_game_id = std::atoi( data );
			free( data );
		}
	}
}

std::map< int, Option::OptionData >::const_iterator Option::getDataBegin( ) const {
	return _data.begin( );
}

std::map< int, Option::OptionData >::const_iterator Option::getDataEnd( ) const {
	return _data.end( );
}

void Option::setGameId( int id ) {
	_game_id = id;
	std::string data = std::to_string( _game_id ).c_str( );
	std::shared_ptr< File >( )->save( OPTION_GAME_ID_FILE_PATH, data.c_str( ), ( int )data.length( ), File::FILETYPE_TEXT );
}

int Option::getGameId( ) const {
	return _game_id;
}

int Option::getMachineId( ) const {
	return _machine_id;
}

Option::OptionData Option::getData( int id ) const {
	if ( _data.find( id ) == _data.end( ) ) {
		return OptionData( );
	}
	return _data.at( id );
}

int Option::getMaxId( ) {
	return ( int )_data.size( );
}

void Option::drawGameList( ) const {
	std::shared_ptr< Console > console = Console::get( );


	int x = 0;
	int y = 7;

	console->draw( x, y++, "-------------------------------Game Id List--------------------------------" );
	
	int count = 0;
	x = -25;
	std::map< int, OptionData >::const_iterator ite = _data.begin( );
	std::map< int, OptionData >::const_iterator end = _data.end( );
	while ( ite != end ) {
		//Id Name
		int game_id = ( *ite ).first;
		OptionData data = (*ite).second;
		//•¶Žš—ñˆ—
		char buf[ 64 ];
		sprintf_s( buf, "ID:%2d  %-18s", game_id, data.name.c_str( ) );
		//•`‰æ
		console->draw( x += 25, y, buf );

		ite++;
		count++;
		if ( count % 3 == 0 && ite != end ) {
			y++;
			x = -25;
		}
	}
	x = 0;
	y++;

	console->draw( x, y, "---------------------------------------------------------------------------" );
}

void Option::drawMachineInfo( ) const {
	std::shared_ptr< Console > console = Console::get( );
	//MachineId
	char machine_id[ 16 ];
	sprintf_s( machine_id, "MachineId:%02d", _machine_id );
	//GameId
	char game_id[ 16 ];
	sprintf_s( game_id, "GameId:%02d", _game_id );

	//draw
	console->draw( 0, 0, machine_id );
	console->draw( 0, 1, game_id    );
}
