#include "DataBase.h"
#include <string>
#include <vector>

//�ʐM�K�i:IP:�|�[�g
const char* SQL_HOST     = "160.16.140.103";//�ڑ���  
const char* SQL_USER     = "sqluser";       //mysql���[�U�[��
const char* SQL_PASSWORD = "pR3zwa6fR+";    //�p�X���[�h
const char* SQL_DATABASE = "test";     //�f�[�^�x�[�X��

#pragma comment( lib, "../MySQL/lib/libmysql.lib" )

DataBase::DataBase( ) {
	open( );
}


DataBase::~DataBase( ) {
	close( );
}

void DataBase::open( ) {
	//������
	_mysql = mysql_init( NULL );

	//MySQL&DataBase�ɐڑ�
	if ( mysql_real_connect( _mysql, SQL_HOST, SQL_USER, SQL_PASSWORD, SQL_DATABASE, MYSQL_PORT, NULL, 0 ) == NULL ) {
		//�ڑ��ł��Ȃ�����
		OutputDebugString( "MySQL�̐ڑ��Ɏ��s\n" );
		return;
	}
}

void DataBase::close( ) {
	//�ڑ���ؒf
	mysql_free_result( _res );
	mysql_close( _mysql );
}

std::vector< std::vector< std::string > > DataBase::selectTable( std::string table ) {
	std::vector< std::vector< std::string > > result;
	//-----�N�G������------//
	std::string query = "select * from " + table;
	
	//-----�N�G�����M-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//���M�Ɏ��s
		OutputDebugString( mysql_error( _mysql ) );
		return result;
	}

	//MySQL����f�[�^���擾
	_res = mysql_store_result( _mysql );
	if ( _res == NULL ) {
		//�擾�ł��Ȃ�
		OutputDebugString( mysql_error( _mysql ) );
		return result;
	}

	//�s��
	int num_rows = ( int )mysql_num_rows( _res );
	//��
	int num_column = ( int )mysql_num_fields( _res );
	//�t�B�[���h�擾( �Z�� )
	MYSQL_FIELD* field = mysql_fetch_fields( _res );

	result.resize( num_column );
	//MYSQL_FIELD tes;
	for ( int i = 0; i < num_column; i++ ) {
		result[ i ].resize( num_rows + 1 );
		result[ i ][ 0 ] = field[ i ].name;
	}

	//���E(�s)�擾
	MYSQL_ROW row = mysql_fetch_row( _res );
	int row_count = 0;
	while ( row != NULL ) {
		row_count++;

		for ( int i = 0; i < num_column; i++ ) {
			result[ i ][ row_count ] = row[ i ];
		}

		row = mysql_fetch_row( _res );
	}

	return result;
}

std::vector< std::string > DataBase::selectColumn( std::string table, std::string column, std::string condition ) {
	std::vector< std::string > result;
	//-----�N�G������------//
	std::string query = "select " + column + " from " + table;
	if ( condition != "" ) {
		query += " where " + condition;
	}
	
	//-----�N�G�����M-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//���M�Ɏ��s
		OutputDebugString( mysql_error( _mysql ) );
		return result;
	}

	//MySQL����f�[�^���擾
	_res = mysql_store_result( _mysql );
	if ( _res == NULL ) {
		//�擾�ł��Ȃ�
		OutputDebugString( mysql_error( _mysql ) );
		return result;
	}

	//�s��
	int num_rows = ( int )mysql_num_rows( _res );
	//��
	int num_column = ( int )mysql_num_fields( _res );
	//�t�B�[���h�擾( �Z�� )
	MYSQL_FIELD* field = mysql_fetch_fields( _res );

	result.resize( num_column );
	//MYSQL_FIELD tes;
	for ( int i = 0; i < num_column; i++ ) {
		result.resize( num_rows + 1 );
		result[ 0 ] = field[ i ].name;
	}

	//���E(�s)�擾
	MYSQL_ROW row = mysql_fetch_row( _res );
	int row_count = 0;
	while ( row != NULL ) {
		row_count++;
		if ( row[ 0 ] != NULL ) {
			result[ row_count ] = row[ 0 ];
		} else {
			result[ row_count ] = "";
		}
		row = mysql_fetch_row( _res );
	}

	return result;
}

std::string DataBase::selectField( std::string table, std::string column, int row ) {
	std::vector< std::string > tmp = selectColumn( table, column );
	if ( row >= ( int )tmp.size( ) ) {
		return "";
	}
	return tmp[ row ];
}

void DataBase::add( std::string table, std::string column, std::string value ) {
	//INSERT INTO �e�[�u���� (��) VALUES (�l)

	//-----�N�G������------//
	std::string query = "insert into " + table + " ("+ column + ") VALUES ('" + value + "')";
	
	//-----�N�G�����M-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//���M�Ɏ��s
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}

void DataBase::addColumn( std::string table, std::string column, std::string type ) {
	
	//INSERT INTO �e�[�u���� (��) VALUES (�l)

	//-----�N�G������------//
	std::string query = "alter table " + table + " add column " + column + " " + type;
	
	//-----�N�G�����M-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//���M�Ɏ��s
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}

void DataBase::set( std::string table, std::string column, std::string value, std::string condition ) {
	//UPDATE �e�[�u���� SET �� = �f�[�^, �� = �f�[�^ ,.... ;

	//-----�N�G������------//
	std::string query = "update " + table + " set " + column + " ='" + value + "'";//
	if ( condition != "" ) {
		query += " where " + condition;
	}
	//-----�N�G�����M-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//���M�Ɏ��s
		OutputDebugString( mysql_error( _mysql ) );
		return ;
	}
}

void DataBase::copy( std::string to_table, std::string from_table, std::string to_column, std::string from_column ) {
	//update table set ( column ) = ( select column from table )
	std::string query = "insert into " + to_table + " select * from " + from_table;
	//std::string query = "update " + to_table + " set " + to_column + " = select " + from_column + " from " + from_table;

	//-----�N�G�����M-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//���M�Ɏ��s
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}

void DataBase::erase( std::string table, std::string column, std::string condition ) {
	//DELETE FROM `test`.`Matching` WHERE  `IP`='a' LIMIT 1;
	//-----�N�G������------//
	std::string query = "delete from " + table + " WHERE " + condition;

	//-----�N�G�����M-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//���M�Ɏ��s
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}

void DataBase::erase( std::string table, std::string column ) {
	//DELETE FROM `test`.`Matching` WHERE  `IP`='a' LIMIT 1;
	//-----�N�G������------//
	std::string query = "alter table " + table + " drop column " + column;

	//-----�N�G�����M-----//
	if ( mysql_query( _mysql, query.c_str( ) ) != 0 ) {
		//���M�Ɏ��s
		OutputDebugString( mysql_error( _mysql ) );
		return;
	}
}