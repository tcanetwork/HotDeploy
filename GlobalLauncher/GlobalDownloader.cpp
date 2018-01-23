#include "GlobalDownloader.h"
#include "Ftp.h"
#include "Ntp.h"
#include "Option.h"
#include "DataBase.h"
#include "stringConverter.h"
#include "Console.h"

const int CHECK_INTERVAL = 30 * 100;

GlobalDownloader::GlobalDownloader( std::shared_ptr< class Option > option ) :
_option( option ),
_download_id( -1 ),
_now_download( 0 ),
_count( 0 ) {
	_ftp = std::shared_ptr< Ftp >( new Ftp );
	_ntp = std::shared_ptr< Ntp >( new Ntp );
	_db  = std::shared_ptr< DataBase >( new DataBase );
	entryDataBase( );
}


GlobalDownloader::~GlobalDownloader( ) {
}


void GlobalDownloader::update( ) {
	if ( isDownloading( ) ) {
		download( );
	} else {
		if ( _count > CHECK_INTERVAL ) {
			refleshDataBase( );
			_count = 0;
		}
		_count++;
	}
}

bool GlobalDownloader::isDownloading( ) const {
	return _download_id >= 0;
}

void GlobalDownloader::download( ) {
	//FtpPath
	std::string ftp_path = _download_list[ _now_download ];
	//LocalPath
	std::string local_path = _option->getData( _download_id ).local_dir;
	std::vector< std::string > split = splitString( ftp_path, _option->getData( _download_id ).ftp_dir );
	if ( ( int )split.size( ) > 1 ) {
		local_path += splitString( ftp_path, _option->getData( _download_id ).ftp_dir )[ 1 ];
	} else {
		local_path += ftp_path;
	}

	//ダウンロード
	_ftp->download( ftp_path, local_path );

	//ダウンロード終了チェック
	_now_download++;
	if ( _now_download >= ( int )_download_list.size( ) ) {
		finishDownload( );
	}
}

void GlobalDownloader::finishDownload( ) {
	if ( ( int )_download_list.size( ) != 0 ) {
		_option->setGameId( _download_id );
	}
	_now_download = 0;
	_download_id = -1;
	_download_list.clear( );
}

void GlobalDownloader::draw( ) const {
	_option->drawMachineInfo( );
	if ( isDownloading( ) ) {
		int progress = _now_download * 100 / ( int )_download_list.size( );
		char buf[ 64 ];
		sprintf_s( buf, "ダウンロード中...%d%% ", progress );
		Console::get( )->draw( 0, 5, buf );
	}
}

bool GlobalDownloader::isEntryDataBase( ) const {
	std::vector< std::string > column = _db->selectColumn( TABLE_HOT_DEPLOY, COLUMN_MACHINE_ID );
	if ( ( int )column.size( ) > 0 ) {
		column.erase( column.begin( ) );
	}
	int size = ( int )column.size( );
	for ( int i = 0; i < size; i++ ) {
		if ( std::atoi( column[ i ].c_str( ) ) == _option->getMachineId( ) ) {
			return true;
		}
	}
	return false;
}


void GlobalDownloader::entryDataBase( ) {
	if ( !isEntryDataBase( ) ) {
		_db->add( TABLE_HOT_DEPLOY, COLUMN_MACHINE_ID, std::to_string( _option->getMachineId( ) ) );
		std::string condition = COLUMN_MACHINE_ID + "=" + std::to_string( _option->getMachineId( ) );
		_db->set( TABLE_HOT_DEPLOY, COLUMN_DOWNLOAD_ID, "-1", condition );
	}
	refleshDataBase( );
}

void GlobalDownloader::refleshDataBase( ) {
	_download_id = -1;
	//データベースに最終更新時間を入れる
	std::string condition = COLUMN_MACHINE_ID + "=" + std::to_string( _option->getMachineId( ) );
	_db->set( TABLE_HOT_DEPLOY, COLUMN_TIME, std::to_string( _ntp->getTime( ) ), condition );

	//データベースからダウンロードチェック
	std::vector< std::string > download_id = _db->selectColumn( TABLE_HOT_DEPLOY, COLUMN_DOWNLOAD_ID, condition );
	if ( ( int )download_id.size( ) > 1 ) {
		_download_id = std::atoi( download_id[ 1 ].c_str( ) );

		//データベースのダウンロードIDを削除
		_db->set( TABLE_HOT_DEPLOY, COLUMN_DOWNLOAD_ID, "-1", condition );
	}

	//FTPファイル一覧取得
	if ( isDownloading( ) ) {
		_ftp->getFindData( _option->getData( _download_id ).ftp_dir, _download_list );
		if ( ( int )_download_list.size( ) == 0 ) {
			finishDownload( );
		}
	}
}