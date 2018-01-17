#pragma once
#include <vector>
#include <string>
#include <map>

class Option {
public:
	struct OptionData {
		std::string name;
		std::string exe_path;
		std::string download_dir;
	};
public:
	Option( );
	virtual ~Option( );
public:
	int getId( ) const;
	OptionData getData( int id ) const;
	int getMaxId( );
private:
	void load( );
private:
	int _id;
	std::map< int, OptionData > _data;
};

