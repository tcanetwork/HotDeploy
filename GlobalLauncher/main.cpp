#include "Keyboard.h"
#include "GlobalDownloader.h"
#include "execute.h"
#include "Option.h"

bool isLoop( );

void main( ) {
	Keyboard::initialize( );
	std::shared_ptr< Option > option( new Option );
	std::shared_ptr< GlobalDownloader > downloader( new GlobalDownloader( option ) );
	std::shared_ptr< Execute > execute( new Execute( option ) );

	char id[ 20 ];
	sprintf_s( id, "Id:%d\n", option->getMachineId( ) );
	printf_s( id );

	while ( isLoop( ) ) {
		downloader->update( );
		if ( downloader->isDownloading( ) ) {
			if ( execute->isPlaying( ) ) {
				execute->close( );
			}
		} else {
			Sleep( 30 );
			if ( !execute->isPlaying( ) ) {
				execute->open( );
			}
		}
		system("cls");
		printf_s( "MachineId:%d\n", option->getMachineId( ) );
		printf_s( "GameId:%d\n", option->getMachineId( ) );
		printf_s( id );
		downloader->draw( );
	}
}


bool isLoop( ) {
	std::shared_ptr< Keyboard > keyboard = Keyboard::get( );
	keyboard->update( );
	return !keyboard->isHitKey( Keyboard::KEY_ESCAPE );
}