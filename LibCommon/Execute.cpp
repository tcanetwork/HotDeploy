#include "Execute.h"
#include <stdio.h>
#include "stringConverter.h"
#include "File.h"

Execute::Execute( std::string exe_path ) :
_exe( exe_path ) {
	convertString( _exe, "/", "\\" );

	//char buf[ 128 ];
	//GetCurrentDirectory( 128, buf );//\\
	//ShellExecuteInfo
	_shell = SHELLEXECUTEINFOA( );
	_shell.cbSize = sizeof( _shell );
	_shell.hwnd = HWND( );
	_shell.lpVerb = "open";
	_shell.lpFile = _exe.c_str( );
	_shell.lpParameters = NULL;
	//_shell.lpDirectory = _dir.c_str( );
	_shell.nShow = SW_SHOWNORMAL;
	_shell.fMask = SEE_MASK_NOCLOSEPROCESS;
	
}


Execute::~Execute( ) {
}

void Execute::open( ) {
	std::shared_ptr< File > file( new File );
	if ( file->getSize( _exe ) <= 0 ) {
		return;
	}
	ShellExecuteExA( &_shell );
}

void Execute::close( ) {
	TerminateProcess( _shell.hProcess, 177 );
	CloseHandle( _shell.hProcess );
	
	std::string message = "アプリケーションを強制終了しました";
	_log->addMessage( message );
}