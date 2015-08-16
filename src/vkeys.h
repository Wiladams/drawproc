// Virtual key codes 
// these are typically associated with the Windows APIs
// but they are fairly universal


typedef enum VKEYS {
	/*
		Virtual Keys, Standard Set
	*/
	KC_LBUTTON = 0x01,
	KC_RBUTTON = 0x02,
	KC_CANCEL = 0x03,
	KC_MBUTTON = 0x04,

	KC_XBUTTON1 = 0x05,
	KC_XBUTTON2 = 0x06,

/*
			*0x07:unassigned
*/

	KC_BACK = 0x08,
	KC_TAB = 0x09,

/*
*0x0A - 0x0B:reserved
*/

	KC_CLEAR = 0x0C,
	KC_RETURN = 0x0D,

	KC_SHIFT = 0x10,
	KC_CONTROL = 0x11,
	KC_MENU = 0x12,
	KC_PAUSE = 0x13,
	KC_CAPITAL = 0x14,

	KC_KANA = 0x15,  //HANGEUL        0x15  //old name - should be here for compatibility

	KC_HANGUL = 0x15,
	KC_JUNJA = 0x17,
	KC_FINAL = 0x18,
	KC_HANJA = 0x19,
	KC_KANJI = 0x19,

	KC_ESCAPE = 0x1B,

	KC_CONVERT = 0x1C,
	KC_NONCONVERT = 0x1D,
	KC_ACCEPT = 0x1E,
	KC_MODECHANGE = 0x1F,


	KC_SPACE = 0x20,
	KC_PRIOR = 0x21,
	KC_NEXT = 0x22,
	KC_END = 0x23,
	KC_HOME = 0x24,
	KC_LEFT = 0x25,
	KC_UP = 0x26,
	KC_RIGHT = 0x27,
	KC_DOWN = 0x28,
	KC_SELECT = 0x29,
	KC_PRINT = 0x2A,
	KC_EXECUTE = 0x2B,
	KC_SNAPSHOT = 0x2C,
	KC_INSERT = 0x2D,
	KC_DELETE = 0x2E,
	KC_HELP = 0x2F,


	// 0 - 9 are the same as ASCII '0' - '9' (0x30 - 0x39)
	KC_0 = 0x30,
	KC_1 = 0x31,
	KC_2 = 0x32,
	KC_3 = 0x33,
	KC_4 = 0x34,
	KC_5 = 0x35,
	KC_6 = 0x36,
	KC_7 = 0x37,
	KC_8 = 0x38,
	KC_9 = 0x39,
	
	//		* 0x40:unassigned

	// A - Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	KC_A = 0x41,
	KC_B = 0x42,
	KC_C = 0x43,
	KC_D = 0x44,
	KC_E = 0x45,
	KC_F = 0x46,
	KC_G = 0x47,
	KC_H = 0x48,
	KC_I = 0x49,
	KC_J = 0x4A,
	KC_K = 0x4B,
	KC_L = 0x4C,
	KC_M = 0x4D,
	KC_N = 0x4E,
	KC_O = 0x4F,
	KC_P = 0x50,
	KC_Q = 0x51,
	KC_R = 0x52,
	KC_S = 0x53,
	KC_T = 0x54,
	KC_U = 0x55,
	KC_V = 0x56,
	KC_W = 0x57,
	KC_X = 0x58,
	KC_Y = 0x59,
	KC_Z = 0x5A,


	KC_LWIN = 0x5B,
	KC_RWIN = 0x5C,
	KC_APPS = 0x5D,

	// 0x5E:reserved

	KC_SLEEP = 0x5F,


	KC_NUMPAD0 = 0x60,
	KC_NUMPAD1 = 0x61,
	KC_NUMPAD2 = 0x62,
	KC_NUMPAD3 = 0x63,
	KC_NUMPAD4 = 0x64,
	KC_NUMPAD5 = 0x65,
	KC_NUMPAD6 = 0x66,
	KC_NUMPAD7 = 0x67,
	KC_NUMPAD8 = 0x68,
	KC_NUMPAD9 = 0x69,
	KC_MULTIPLY = 0x6A,
	KC_ADD = 0x6B,
	KC_SEPARATOR = 0x6C,
	KC_SUBTRACT = 0x6D,
	KC_DECIMAL = 0x6E,
	KC_DIVIDE = 0x6F,

	KC_F1 = 0x70,
	KC_F2 = 0x71,
	KC_F3 = 0x72,
	KC_F4 = 0x73,
	KC_F5 = 0x74,
	KC_F6 = 0x75,
	KC_F7 = 0x76,
	KC_F8 = 0x77,
	KC_F9 = 0x78,
	KC_F10 = 0x79,
	KC_F11 = 0x7A,
	KC_F12 = 0x7B,
	KC_F13 = 0x7C,
	KC_F14 = 0x7D,
	KC_F15 = 0x7E,
	KC_F16 = 0x7F,
	KC_F17 = 0x80,
	KC_F18 = 0x81,
	KC_F19 = 0x82,
	KC_F20 = 0x83,
	KC_F21 = 0x84,
	KC_F22 = 0x85,
	KC_F23 = 0x86,
	KC_F24 = 0x87,

		/*
			*0x88 - 0x8F:unassigned
		*/

	KC_NUMLOCK = 0x90,
	KC_SCROLL = 0x91,

	/*
	*NEC PC - 9800 kbd definitions
	*/

	KC_OEM_NEC_EQUAL = 0x92,   // '=' key on numpad

	/*
	*Fujitsu / OASYS kbd definitions
	*/

	KC_OEM_FJ_JISHO = 0x92,   // 'Dictionary' key
	KC_OEM_FJ_MASSHOU = 0x93,   // 'Unregister word' key
	KC_OEM_FJ_TOUROKU = 0x94,   // 'Register word' key
	KC_OEM_FJ_LOYA = 0x95,   // 'Left OYAYUBI' key
	KC_OEM_FJ_ROYA = 0x96,   // 'Right OYAYUBI' key

			/*
*0x97 - 0x9F:unassigned
	*/

	/*
		*L* & R* -left and right Alt, Ctrl and Shift virtual keys.
			* Used only as parameters to GetAsyncKeyState() and GetKeyState().
			* No other API or message will distinguish left and right keys in this way.
	*/

	KC_LSHIFT = 0xA0,
	KC_RSHIFT = 0xA1,
	KC_LCONTROL = 0xA2,
	KC_RCONTROL = 0xA3,
	KC_LMENU = 0xA4,
	KC_RMENU = 0xA5,


	KC_BROWSER_BACK = 0xA6,
	KC_BROWSER_FORWARD = 0xA7,
	KC_BROWSER_REFRESH = 0xA8,
	KC_BROWSER_STOP = 0xA9,
	KC_BROWSER_SEARCH = 0xAA,
	KC_BROWSER_FAVORITES = 0xAB,
	KC_BROWSER_HOME = 0xAC,


	KC_VOLUME_MUTE = 0xAD,
	KC_VOLUME_DOWN = 0xAE,
	KC_VOLUME_UP = 0xAF,
	KC_MEDIA_NEXT_TRACK = 0xB0,
	KC_MEDIA_PREV_TRACK = 0xB1,
	KC_MEDIA_STOP = 0xB2,
	KC_MEDIA_PLAY_PAUSE = 0xB3,
	KC_LAUNCH_MAIL = 0xB4,
	KC_LAUNCH_MEDIA_SELECT = 0xB5,
	KC_LAUNCH_APP1 = 0xB6,
	KC_LAUNCH_APP2 = 0xB7,


		/*
			0xB8 - 0xB9:reserved
*/

KC_OEM_1 = 0xBA,   // ',:' for US
KC_OEM_PLUS = 0xBB,   // '+' any country
KC_OEM_COMMA = 0xBC,   // ',' any country
KC_OEM_MINUS = 0xBD,   // '-' any country
KC_OEM_PERIOD = 0xBE,   // '.' any country
KC_OEM_2 = 0xBF,   // '/?' for US
KC_OEM_3 = 0xC0,   // '`~' for US

/*
	0xC1 - 0xD7:reserved
		*/

		/*
			0xD8 - 0xDA:unassigned
*/

KC_OEM_4 = 0xDB,  //  '[{' for US
KC_OEM_5 = 0xDC,  //  '\|' for US
KC_OEM_6 = 0xDD,  //  ']}' for US
KC_OEM_7 = 0xDE,  //  ''"' for US
KC_OEM_8 = 0xDF,

			/*
0xE0:reserved
	*/

	/*
		Various extended or enhanced keyboards
			*/
			KC_OEM_AX = 0xE1,  //  'AX' key on Japanese AX kbd
			KC_OEM_102 = 0xE2, //  "<>" or "\|" on RT 102 - key kbd.
			KC_ICO_HELP = 0xE3,  //Help key on ICO
			KC_ICO_00 = 0xE4,  //  00 key on ICO
			KC_PROCESSKEY = 0xE5,
			KC_ICO_CLEAR = 0xE6,
			KC_PACKET = 0xE7,

		/*
			0xE8:unassigned
*/

/*
	Nokia / Ericsson definitions
*/
KC_OEM_RESET = 0xE9,
KC_OEM_JUMP = 0xEA,
KC_OEM_PA1 = 0xEB,
KC_OEM_PA2 = 0xEC,
KC_OEM_PA3 = 0xED,
KC_OEM_WSCTRL = 0xEE,
KC_OEM_CUSEL = 0xEF,
KC_OEM_ATTN = 0xF0,
KC_OEM_FINISH = 0xF1,
KC_OEM_COPY = 0xF2,
KC_OEM_AUTO = 0xF3,
KC_OEM_ENLW = 0xF4,
KC_OEM_BACKTAB = 0xF5,

KC_ATTN = 0xF6,
KC_CRSEL = 0xF7,
KC_EXSEL = 0xF8,
KC_EREOF = 0xF9,
KC_PLAY = 0xFA,
KC_ZOOM = 0xFB,
KC_NONAME = 0xFC,
KC_PA1 = 0xFD,
KC_OEM_CLEAR = 0xFE,

};



