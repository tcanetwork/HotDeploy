#include "Keyboard.h"
#include "GlobalDownloader.h"
#include "execute.h"
#include "Option.h"
#include "Console.h"
#include "FpsController.h"

bool isLoop( );

void main( ) {
	Keyboard::initialize( );
	Console::initialize( );

	std::shared_ptr< Console > console = Console::get( );
	std::shared_ptr< FpsController > fps( new FpsController );
	std::shared_ptr< Option > option( new Option );
	std::shared_ptr< GlobalDownloader > downloader( new GlobalDownloader( option ) );
	std::shared_ptr< Execute > execute( new Execute( option ) );

	console->setCursorView( false );
	while ( isLoop( ) ) {
		downloader->update( );
		if ( downloader->isDownloading( ) ) {
			if ( execute->isPlaying( ) ) {
				execute->close( );
			}
		} else {
			if ( !execute->isPlaying( ) ) {
				execute->open( );
			}
		}
		downloader->draw( );
		console->draw( );
		fps->update( );
	}
	execute->close( );
}


bool isLoop( ) {
	std::shared_ptr< Keyboard > keyboard = Keyboard::get( );
	keyboard->update( );
	return !keyboard->isHitKey( Keyboard::KEY_ESCAPE );
}