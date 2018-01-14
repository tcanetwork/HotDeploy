#pragma once
#include <string>
#include <memory>
#include <Windows.h>
#include <WinUser.h>


class Keyboard {
public:
	enum KEY {
		KEY_NONE = 0x00,

		KEY_BACK = VK_BACK,
		KEY_TAB,

		KEY_RETURN = VK_RETURN,

		KEY_SHIFT  = VK_SHIFT,
		KEY_CTRL   = VK_CONTROL,
		KEY_ALT    = VK_MENU,
		KEY_ESCAPE = VK_ESCAPE,
		KEY_SPACE  = VK_SPACE,

		KEY_PGUP = VK_PRIOR,
		KEY_PGDN,
		KEY_END,
		KEY_HOME,

		KEY_LEFT,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN,

		KEY_SELECT,
		KEY_PRINT,
		KEY_EXECUTE_,
		KEY_PRINTSCREEN,
		KEY_INSERT,
		KEY_DELETE,
		KEY_HELP,

		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,

		KEY_A = 65,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,

		KEY_LWIN,
		KEY_RWIN,

		KEY_NUMPAD0 = VK_NUMPAD0,
		KEY_NUMPAD1,
		KEY_NUMPAD2,
		KEY_NUMPAD3,
		KEY_NUMPAD4,
		KEY_NUMPAD5,
		KEY_NUMPAD6,
		KEY_NUMPAD7,
		KEY_NUMPAD8,
		KEY_NUMPAD9,
		KEY_MULTIPLY,
		KEY_ADD,
		KEY_SEPARATOR,
		KEY_SUBTRACT,
		KEY_DECIMAL,
		KEY_DIVIDE,


		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,


		KEY_NUMLOCK = VK_NUMLOCK,
		KEY_SCROLL,

		KEY_LSHIFT      = VK_LSHIFT,
		KEY_RSHIFT  ,
		KEY_LCONTROL,
		KEY_RCONTROL,
		KEY_LALT,
		KEY_RALT,
	};
public:
	static void initialize( );
	static std::shared_ptr< class Keyboard > get( );
public:
	Keyboard( );
	virtual ~Keyboard( );
public:
	void update( );
public:
	bool isHitKey( KEY key ) const;
private:
	void checkInputKey( );
private:
	KEY _key;
};

