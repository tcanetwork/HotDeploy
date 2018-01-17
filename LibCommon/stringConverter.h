#pragma once

#include <Windows.h>
#include <string>
#include <vector>


static void convertString( std::string& str, std::string from, std::string to ) {
	std::string::size_type pos = str.find( from );
	while ( pos != std::string::npos ) {
		str.replace( pos, from.length( ), to );
		pos = str.find( from );
	}
}

static std::vector< std::string > splitString( std::string str, std::string div ) {
	std::vector< std::string > result;
	size_t end_pos = str.find( div );
	size_t start_pos = 0;
	if ( end_pos == std::string::npos ) {
		result.push_back( str );
		return result;
	}

	while ( end_pos != std::string::npos ) {
		result.push_back( str.substr( start_pos, end_pos - start_pos ) );
		start_pos = end_pos + div.length( );
		end_pos = str.find( div, start_pos );
	}
	result.push_back( str.substr( start_pos, str.length( ) - start_pos ) );
	return result;
}