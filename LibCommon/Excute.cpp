#include "Excute.h"
#include <stdio.h>


Excute::Excute( std::string exe, std::shared_ptr< Log > log ) :
_log( log ),
_exe( exe ) {
	char buf[ 128 ];
	GetCurrentDirectory( 128, buf );
	_dir = buf;
	_dir = _exe.substr( 0, _exe.find_last_of( "\\" )  );
	_exe = buf;
	_exe += "\\" + exe;
	//ShellExcuteInfo
	_shell = SHELLEXECUTEINFOA( );
	_shell.cbSize = sizeof( _shell );
	_shell.hwnd = HWND( );
	_shell.lpVerb = "open";
	_shell.lpFile = _exe.c_str( );
	_shell.lpParameters = NULL;
	_shell.lpDirectory = _dir.c_str( );
	_shell.nShow = SW_SHOWNORMAL;
	_shell.fMask = SEE_MASK_NOCLOSEPROCESS;
	
}


Excute::~Excute( ) {
}

void Excute::open( ) {
	std::string message = "";
	FILE* fp = NULL;
	fopen_s( &fp, _exe.c_str( ), "r" );
	if ( fp == NULL ) {
		return;
	}
	fclose( fp );
	const char* dir = _shell.lpDirectory;
	ShellExecuteExA( &_shell );

	if ( _shell.hwnd ) {
		message = _exe + "を起動できませんでした";
	} else {
		message = _exe + "を起動しました";
	}
	_log->addMessage( message );
}

void Excute::close( ) {
	TerminateProcess( _shell.hProcess, 177 );
	CloseHandle( _shell.hProcess );
	
	std::string message = "アプリケーションを強制終了しました";
	_log->addMessage( message );
}