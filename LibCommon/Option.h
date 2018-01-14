#pragma once
#include <vector>
#include <string>

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
	void load( );
private:
	std::vector< OptionData > _data;
};

