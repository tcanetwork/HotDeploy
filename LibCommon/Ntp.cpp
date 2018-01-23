#include "Ntp.h"

const char* HOST = "ntp.jst.mfeed.ad.jp";
const unsigned short PORT = 123;
const unsigned long NTP_TIME = 2208988800U;
const int BIT_NUM = 32;

const timeval TIMEOUT = { 1, 0 };//受信待機 秒,ms

//-------------- Option ---------------//
const UINT32 LI = 0;     //警告( 0:なし )
const UINT32 VN = 3;     //version( 3:通常のNTP )
const UINT32 MODE = 3;   //動作モード( 3:クライアント )
const UINT32 STRATUM = 1;//階層(サーバーによる)
const UINT32 POLL = 0;   //間隔?
const UINT32 PREC = 0;   //精度

const UINT32 OPTION = htonl(//計32bit
	( LI      << 30 ) |//2bit
	( VN      << 27 ) |//3bit
	( MODE    << 24 ) |//3bit
	( STRATUM << 16 ) |//8bit
	( POLL    << 8  ) |//8bit
	( PREC & 0xff ) ); //8bit

Ntp::Ntp( ) :
_connect( false ),
_before_time( 0 ) {
	open( );
}


Ntp::~Ntp( ) {
	closesocket( _socket );
	WSACleanup( );
}

void Ntp::open( ) {
	
	{//初期化
		WSADATA data = WSADATA( );
		WSAStartup( MAKEWORD( 2, 0 ), &data );
	}

	//ソケット生成
	_socket = ( int )socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( _socket < 0 ) {
		return;
	}

    // ホストの情報を取得
	hostent *servhost = gethostbyname( HOST );
    if( servhost == NULL){
        perror( HOST );
		return;
    }

    //構造体に情報を格納
	sockaddr_in addr = sockaddr_in( );
    addr.sin_family = AF_INET;
	addr.sin_addr = *( IN_ADDR* )servhost->h_addr_list[ 0 ];
	addr.sin_port = htons( PORT );

	if ( connect( _socket, ( sockaddr* )&addr, sizeof( addr ) ) < 0 ) {
		return;
	}
	_connect = true;
}

unsigned long Ntp::getTime( ) {
	unsigned long time = 0;
	if ( _before_time > 0 ) {
		time = getTimeCalc( );
	} else {
		time = getTimeServer( );
	}
	return time;
}

//-----------------------------------------------------------------
// NTP サーバからの時刻取得
//-----------------------------------------------------------------
unsigned long Ntp::getTimeServer( ) {
	if ( !_connect ) {
		open( );
		if ( _connect ) {
			return getTimeServer( );
		}
		return 0;
	}

	PACKET data = PACKET( );
	data.option = OPTION;
	data.delay = htonl( 1 << 16 );
	data.disp  = htonl( 1 << 16 );
	//Option送信
	int send_byte = send( _socket, ( char* )&data, sizeof( PACKET ), 0 );
	if ( send_byte <= 0 ) {
		return 0;
	}

	
	//待機
	fd_set read = fd_set( );
	FD_ZERO( &read );
	FD_SET( _socket, &read );
	if ( select( _socket + 1, &read, NULL, NULL, &TIMEOUT ) != 1 ) {
		OutputDebugString( "Ntp:タイムアウト\n" );
		return 0;
	}
	FD_CLR( _socket, &read );

	//受信
	int recv_byte = recv( _socket, ( char* )&data, sizeof( PACKET ), 0 );
	if ( recv_byte <= 0 ) {
		return 0;
	}

	//時間計算
	unsigned long time_s = ntohl( data.timestamp_transmit_s ) - NTP_TIME;
	//計算用変数に格納
	_before_count = GetTickCount( ) / 1000;
	_before_time = time_s;
	return time_s;
}

//-----------------------------------------------------------------
// 時刻計算
//-----------------------------------------------------------------
unsigned long Ntp::getTimeCalc( ) const {
	int count = GetTickCount( ) / 1000;
	unsigned long time = _before_count - _before_count + count;
	return _before_time;
}
