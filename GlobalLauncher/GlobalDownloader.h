#pragma once
#include <memory>
#include <vector>
#include <string>

class GlobalDownloader {
public:
	GlobalDownloader( std::shared_ptr< class Option > option );
	virtual ~GlobalDownloader( );
public:
	void update( );
	void draw( ) const;
	bool isDownloading( ) const;
private:
	void download( );
	void finishDownload( );
	void entryDataBase( );
	void refleshTimeDataBase( );
	void checkDownloadDataBase( );
	bool isEntryDataBase( ) const;
private:
	int _count;
	int _download_id;
	int _now_download;
	std::vector< std::string > _download_list;
	std::shared_ptr< class Ftp > _ftp;
	std::shared_ptr< class Ntp > _ntp;
	std::shared_ptr< class DataBase > _db;
	std::shared_ptr< class Option > _option;
};

