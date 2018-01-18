#include "Execute.h"
#include <stdio.h>
#include "stringConverter.h"
#include "File.h"
#include "Option.h"

Execute::Execute( std::shared_ptr< class Option > option ) :
_option( option ) {
	_shell = SHELLEXECUTEINFOA( );
	_shell.cbSize = sizeof( _shell );
	_shell.hwnd = HWND( );
	_shell.lpVerb = "open";
	_shell.lpParameters = NULL;
	//_shell.lpDirectory = _dir.c_str( );
	_shell.nShow = SW_SHOWNORMAL;
	_shell.fMask = SEE_MASK_NOCLOSEPROCESS;
	
}


Execute::~Execute( ) {
}

void Execute::open( ) {
	//ShellÝ’è
	char buf[ 128 ];
	GetCurrentDirectory( 128, buf );
	Option::OptionData data = _option->getData( _option->getGameId( ) );
	std::string exe = buf + ( std::string )"\\" + data.local_dir + data.exe_path;
	std::string dir = buf + ( std::string )"\\" + data.local_dir;
	convertString( exe, "/", "\\" );
	convertString( dir, "/", "\\" );
	//FilePath
	if ( _shell.lpFile != NULL ) {
		free( ( void* )_shell.lpFile );
	}
	_shell.lpFile = ( LPCSTR )malloc( exe.length( ) + 1 );
	memcpy_s( ( void* )_shell.lpFile, exe.length( ) + 1, exe.c_str( ), exe.length( ) + 1 );
	//Dir
	if ( _shell.lpDirectory != NULL ) {
		free( ( void* )_shell.lpDirectory );
	}
	_shell.lpDirectory = ( LPCSTR )malloc( dir.length( ) + 1 );
	memcpy_s( ( void* )_shell.lpDirectory, dir.length( ) + 1, dir.c_str( ), dir.length( ) + 1 );



	std::shared_ptr< File > file( new File );
	if ( file->getSize( _shell.lpFile ) <= 0 ) {
		return;
	}
	ShellExecuteExA( &_shell );
	_playing = true;
}

void Execute::close( ) {
	TerminateProcess( _shell.hProcess, 177 );
	CloseHandle( _shell.hProcess );
	_playing = false;
}

bool Execute::isPlaying( ) const {
	return _playing;
}