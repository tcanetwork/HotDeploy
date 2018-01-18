#pragma once
#include "../MySQL/include/mysql.h"
#include <string>
#include <vector>



class DataBase {
public:
	DataBase( );
	virtual ~DataBase( );
public:
	//取得 vector[ column(列) ][ row(行) ]
	std::vector< std::vector< std::string > > selectTable( std::string table );      //指定したtable取得
	std::vector< std::string > selectColumn( std::string table, std::string column, std::string condition = "" );//table内の指定した列を取得
	std::string selectField( std::string table, std::string column, int row = 0 );       //列,行にあるものを取得
	//追加
	void add( std::string table, std::string column, std::string value );
	void addColumn( std::string table, std::string column, std::string type );
	//変更
	void set( std::string table, std::string column, std::string value, std::string condition = "" );
	void copy( std::string to_table, std::string from_table, std::string to_column, std::string from_column );
	//削除
	void erase( std::string table, std::string column, std::string value );
	void erase( std::string table, std::string column );
private:
	void open( );
	void close( );
private:
	MYSQL* _mysql;
	MYSQL_RES* _res;
};
