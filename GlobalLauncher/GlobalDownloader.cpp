#include "GlobalDownloader.h"
#include "Ftp.h"



GlobalDownloader::GlobalDownloader( ) :
_wait_download( false ) {
	_ftp = std::shared_ptr< Ftp >( new Ftp );
}


GlobalDownloader::~GlobalDownloader( ) {
}


void GlobalDownloader::update( ) {
	checkCommand( );
}

bool GlobalDownloader::isWaitingDownload( ) const {
	return _wait_download;
}

void GlobalDownloader::checkCommand( ) {

}

void GlobalDownloader::download( ) {
}
