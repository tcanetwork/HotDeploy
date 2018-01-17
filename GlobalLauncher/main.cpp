#include "Keyboard.h"
#include "GlobalDownloader.h"
#include "execute.h"
#include "Option.h"

bool isLoop( );

void main( ) {
	Keyboard::initialize( );
	std::shared_ptr< Option > option( new Option );
	std::shared_ptr< GlobalDownloader > downloader( new GlobalDownloader );
	std::shared_ptr< Execute > execute( new Execute( option->getData( option->getId( ) ).exe_path ) );

	while ( isLoop( ) ) {
		downloader->update( );
		if ( downloader->isWaitingDownload( ) ) {
			downloader->download( );
		}
	}
}


bool isLoop( ) {
	std::shared_ptr< Keyboard > keyboard = Keyboard::get( );
	keyboard->update( );
	return !keyboard->isHitKey( Keyboard::KEY_ESCAPE );
}