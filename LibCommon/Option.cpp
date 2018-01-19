#include "Option.h"
#include "File.h"
#include "Console.h"
#include "stringConverter.h"
#include <memory>

const std::string OPTION_FILE_DIR = "Option";
const std::string OPTION_DATA_FILE_PATH = "setting.txt";
const std::string OPTION_ID_FILE_PATH = "id.txt";

Option::Option( ) {
	load( );
}


Option::~Option( ) {
}

void Option::load( ) {
	std::shared_ptr< File > file( new File );
	{//Setting
		std::string path = OPTION_FILE_DIR + "/" + OPTION_DATA_FILE_PATH;
		int size = file->getSize( path ) + 1;
		if ( size > 0 ) {
			char* data = ( char* )malloc( size );
			file->load( path, data, size, File::FILETYPE_TEXT );
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
	{//id
		_machine_id = -1;
		_game_id = -1;

		std::string path = OPTION_FILE_DIR + "/" + OPTION_ID_FILE_PATH;
		int size = file->getSize( path ) + 1;
		if ( size > 0 ) {
			char* data = ( char* )malloc( size );
			file->load( path, data, size, File::FILETYPE_TEXT );
			std::vector< std::string > lines = splitString( data, "\r\n" );
			//àÍçsÇ∏Ç¬èàóù
			int size = ( int )lines.size( );
			for ( int i = 0; i < size; i++ ) {
				std::vector< std::string > line = splitString( lines[ i ], ":" );
				if ( ( int )line.size( ) != 2 ) {
					continue;
				}
				if ( line[ 0 ] == "MachineId" ) {
					_machine_id = std::atoi( line[ 1 ].c_str( ) );	
				}
				if ( line[ 0 ] == "GameId" ) {
					_game_id = std::atoi( line[ 1 ].c_str( ) );	
				}
			}
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
	std::shared_ptr< Console > drawer;


	int x = 0;
	int y = 8;

	drawer->draw( x, y, "-------------------------------Game Id List--------------------------------" );
	int count = 0;
	y++;
	std::map< int, OptionData >::const_iterator ite = _data.begin( );
	std::map< int, OptionData >::const_iterator end = _data.end( );
	while ( ite != end ) {
		int game_id = ( *ite ).first;
		OptionData data = (*ite).second;
		char buf[ 64 ];
		sprintf_s( buf, "ID:%2d  %-18s", game_id, data.name.c_str( ) );
		drawer->draw( x, y, buf );

		x += 25;
		ite++;
		count++;
		if ( count % 3 == 0 && ite != end ) {
			y++;
			x = 0;
		}
	}
	x = 0;
	y++;

	drawer->draw( x, y, "---------------------------------------------------------------------------" );
}
