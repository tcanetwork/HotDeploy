#include "Option.h"
#include "File.h"
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
		int size = file->getSize( path );
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

				if ( line[ 0 ] == "ID" ) {
					if ( i != 0 ) {
						_data[ id ] = option;
						option = OptionData( );
					}
					id = std::atoi( line[ 1 ].c_str( ) );
				}
				if ( line[ 0 ] == "NAME" ) {
					option.name = line[ 1 ];
				}
				if ( line[ 0 ] == "EXE" ) {
					option.exe_path = line[ 1 ];
				}

				if ( line[ 0 ] == "DOWNLOAD" ) {
					option.download_dir = line[ 1 ];
				}
			}
			_data[ id ] = option;
			free( data );
		}
	}
	{//id
		std::string path = OPTION_FILE_DIR + "/" + OPTION_ID_FILE_PATH;
		int size = file->getSize( path ) - 1;
		if ( size > 0 ) {
			char* data = ( char* )malloc( size );
			file->load( path, data, size, File::FILETYPE_TEXT );
			_id = std::atoi( data );
			free( data );
		}
	}
}

int Option::getId( ) const {
	return _id;
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