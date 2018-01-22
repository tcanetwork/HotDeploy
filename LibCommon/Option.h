#pragma once
#include <vector>
#include <string>
#include <map>

class Option {
public:
	struct OptionData {
		std::string name;
		std::string exe_path;
		std::string ftp_dir;
		std::string local_dir;
	};
public:
	Option( );
	virtual ~Option( );
public:
	void drawGameList( ) const;
	void drawMachineInfo( ) const;
public:
	OptionData getData( int id ) const;
	std::map< int, OptionData >::const_iterator getDataBegin( ) const;
	std::map< int, OptionData >::const_iterator getDataEnd( ) const;
	int getMachineId( ) const;
	int getGameId( ) const;
	int getMaxId( );
public:
	void setGameId( int id );
private:
	void load( );
private:
	int _machine_id;
	int _game_id;
	std::map< int, OptionData > _data;
};

