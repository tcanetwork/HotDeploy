#pragma once
#include <memory>

class GlobalDownloader {
public:
	GlobalDownloader( );
	virtual ~GlobalDownloader( );
public:
	void update( );
	void download( );
	bool isWaitingDownload( ) const;
private:
	void checkCommand( );
private:
	bool _wait_download;
	std::shared_ptr< class Ftp > _ftp;
};

