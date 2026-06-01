/*****************************************************************************/
/*!	@file	keytypes.c
	@brief	Source file for key input control from stdin

	IO setting for console changes based on OS type (Win&Mac)

	@see	keytypes.hÅ@Source file for keytypes.h
	@par	Log
		- 2020/12/04 Ver. 1.0
			- Basic settings were programmed (by Keisuke SHIMA)
		 - 2024/05/01 Ver. 1.1
			- Changed getKeyName()&getKey for compiling by gcc. MACRO __GNUC__ is used to check the complier.
			- Fix warnings for GCC (MinGw) & Visual Studio (Win) (but there are two wornings about PASCAL...)
   @attention
		- Original codes programmed by Prof. Amano.
	@par Copyright
		Copyright &copy; 2020- Keisuke SHIMA All rights reserved.
******************************************************************************/
#include "keytypes.h"

const char* INPUTKEY_NAMES[] = {
	"others",
	"UP",
	"DOWN",
	"LEFT",
	"RIGHT",
	"ESC",
	"Back Space",
	"Delete",
	"SPACE",
	"Tab",
	"Enter/Return",
	"Home",
	"End",
	"Page Up",
	"Page Down",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"Ctrl + C",
	"End Of File"
};


#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

#include <conio.h>


KEYTYPE getKeyName(void) {
	KEYTYPE	key = KEY_OTHERS;
	char	c;

	c = _getch();
	switch (c & 0xff) {
	case 0xe0:
		c = _getch();
		switch (c) {
		case 0x48:
			key = KEY_UP;
			break;
		case 0x50:
			key = KEY_DOWN;
			break;
		case 0x4b:
			key = KEY_LEFT;
			break;
		case 0x4d:
			key = KEY_RIGHT;
			break;
		case 0x53:
			key = KEY_DELETE;
			break;
		case 0x47:
			key = KEY_HOME;
			break;
		case 0x4f:
			key = KEY_END;
			break;
		case 0x49:
			key = KEY_PAGEUP;
			break;
		case 0x51:
			key = KEY_PAGEDOWN;
			break;
#ifndef __GNUC__
		case 0x85:
			key = KEY_F11;
			break;
		case 0x86:
			key = KEY_F12;
			break;
#endif
		}
		break;
	case 3:
		key = KEY_STOP;
		break;
	case 0x1b:
		key = KEY_ESC;
		break;
	case 8:
		key = KEY_BACKSPACE;
		break;
	case 32:
		key = KEY_SPACE;
		break;
	case 9:
		key = KEY_TAB;
		break;
	case 10:
	case 13:
		key = KEY_ENTER;
		break;
	case 0:
		c = _getch();
		switch (c) {
		case 0x3b:
			key = KEY_F1;
			break;
		case 0x3c:
			key = KEY_F2;
			break;
		case 0x3d:
			key = KEY_F3;
			break;
		case 0x3e:
			key = KEY_F4;
			break;
		case 0x3f:
			key = KEY_F5;
			break;
		case 0x40:
			key = KEY_F6;
			break;
		case 0x41:
			key = KEY_F7;
			break;
		case 0x42:
			key = KEY_F8;
			break;
		case 0x43:
			key = KEY_F9;
			break;
		case 0x44:
			key = KEY_F10;
			break;
		}
		break;
	}

	return	key;
}


char getKey(void) {
	char	key = KEY_OTHERS;
	char	c;

	c = _getch();
	switch (c & 0xff) {
	case 0xe0:
		c = _getch();
		switch (c) {
		case 0x48:
			key = KEY_UP;
			break;
		case 0x50:
			key = KEY_DOWN;
			break;
		case 0x4b:
			key = KEY_LEFT;
			break;
		case 0x4d:
			key = KEY_RIGHT;
			break;
		case 0x53:
			key = KEY_DELETE;
			break;
		case 0x47:
			key = KEY_HOME;
			break;
		case 0x4f:
			key = KEY_END;
			break;
		case 0x49:
			key = KEY_PAGEUP;
			break;
		case 0x51:
			key = KEY_PAGEDOWN;
			break;
#ifndef __GNUC__
		case 0x85:
			key = KEY_F11;
			break;
		case 0x86:
			key = KEY_F12;
			break;
#endif
		}
		break;
	case 3:
		key = KEY_STOP;
		break;
	case 0x1b:
		key = KEY_ESC;
		break;
	case 8:
		key = KEY_BACKSPACE;
		break;
	case 32:
		key = KEY_SPACE;
		break;
	case 9:
		key = KEY_TAB;
		break;
	case 10:
	case 13:
		key = KEY_ENTER;
		break;
	case 0:
		c = _getch();
		switch (c) {
		case 0x3b:
			key = KEY_F1;
			break;
		case 0x3c:
			key = KEY_F2;
			break;
		case 0x3d:
			key = KEY_F3;
			break;
		case 0x3e:
			key = KEY_F4;
			break;
		case 0x3f:
			key = KEY_F5;
			break;
		case 0x40:
			key = KEY_F6;
			break;
		case 0x41:
			key = KEY_F7;
			break;
		case 0x42:
			key = KEY_F8;
			break;
		case 0x43:
			key = KEY_F9;
			break;
		case 0x44:
			key = KEY_F10;
			break;
		}
		break;
	default :
		key = c;
	}

	return	key;
}
// For CygWin--------------------------------------------
#elif __CYGWIN__		

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

char getKey(void)
{
	char	key = KEY_OTHERS;
	struct termios	tio, savetio;
	int	savefc;
	char	c, kbf[4];

	tcgetattr(STDIN_FILENO, &savetio);
	tio = savetio;
	tio.c_lflag &= ~ICANON;
	tio.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &tio);

	c = getchar();
	switch (c) {
	case EOF:
		key = KEY_EOF;
		break;
	case 27:
		savefc = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
		kbf[0] = getchar();
		kbf[1] = getchar();
		kbf[2] = getchar();
		kbf[3] = getchar();
		fcntl(STDIN_FILENO, F_SETFL, savefc);
		switch (kbf[0]) {
		case EOF:
			key = KEY_ESC;
			break;
		case 91:
			switch (kbf[1]) {
			case 65:
				key = KEY_UP;
				break;
			case 66:
				key = KEY_DOWN;
				break;
			case 68:
				key = KEY_LEFT;
				break;
			case 67:
				key = KEY_RIGHT;
				break;
			case 51:
				switch (kbf[2]) {
				case 126:
					key = KEY_DELETE;
					break;
				default:
					switch (kbf[3]) {
					case 126:
						switch (kbf[2]) {
						case 49:
							key = KEY_F17;
							break;
						case 50:
							key = KEY_F18;
							break;
						case 51:
							key = KEY_F19;
							break;
						}
						break;
					}
					break;
				}
				break;
			case 49:
				switch (kbf[3]) {
				case 126:
					switch (kbf[2]) {
					case 53:
						key = KEY_F5;
						break;
					case 55:
						key = KEY_F6;
						break;
					case 56:
						key = KEY_F7;
						break;
					case 57:
						key = KEY_F8;
						break;
					}
					break;
				}
				break;
			case 50:
				switch (kbf[3]) {
				case 126:
					switch (kbf[2]) {
					case 48:
						key = KEY_F9;
						break;
					case 49:
						key = KEY_F10;
						break;
					case 51:
						key = KEY_F11;
						break;
					case 52:
						key = KEY_F12;
						break;
					case 53:
						key = KEY_F13;
						break;
					case 54:
						key = KEY_F14;
						break;
					case 56:
						key = KEY_F15;
						break;
					case 57:
						key = KEY_F16;
						break;
					}
					break;
				}
				break;

				/*  can't checked in macOS  */
			case 72:
				key = KEY_HOME;
				break;
			case 70:
				key = KEY_END;
				break;
			case 53:
				key = KEY_PAGEUP;
				break;
			case 54:
				key = KEY_PAGEDOWN;
				break;
			}
			break;
		case 79:
			switch (kbf[1]) {
			case 80:
				key = KEY_F1;
				break;
			case 81:
				key = KEY_F2;
				break;
			case 82:
				key = KEY_F3;
				break;
			case 83:
				key = KEY_F4;
				break;
			}
			break;
		}
		break;
	case 127:
		key = KEY_BACKSPACE;
		break;
	case 9:
		key = KEY_TAB;
		break;
	case 32:
		key = KEY_SPACE;
		break;
	case 10:
	case 13:
		key = KEY_ENTER;
		break;
	default:
		key = c;
		break;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &savetio);

	return	key;
}

KEYTYPE getKeyName(void)
{
	KEYTYPE	key = KEY_OTHERS;
	struct termios	tio, savetio;
	int	savefc;
	char	c, kbf[4];

	tcgetattr(STDIN_FILENO, &savetio);
	tio = savetio;
	tio.c_lflag &= ~ICANON;
	tio.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &tio);

	c = getchar();
	switch (c) {
	case EOF:
		key = KEY_EOF;
		break;
	case 27:
		savefc = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
		kbf[0] = getchar();
		kbf[1] = getchar();
		kbf[2] = getchar();
		kbf[3] = getchar();
		fcntl(STDIN_FILENO, F_SETFL, savefc);
		switch (kbf[0]) {
		case EOF:
			key = KEY_ESC;
			break;
		case 91:
			switch (kbf[1]) {
			case 65:
				key = KEY_UP;
				break;
			case 66:
				key = KEY_DOWN;
				break;
			case 68:
				key = KEY_LEFT;
				break;
			case 67:
				key = KEY_RIGHT;
				break;
			case 51:
				switch (kbf[2]) {
				case 126:
					key = KEY_DELETE;
					break;
				default:
					switch (kbf[3]) {
					case 126:
						switch (kbf[2]) {
						case 49:
							key = KEY_F17;
							break;
						case 50:
							key = KEY_F18;
							break;
						case 51:
							key = KEY_F19;
							break;
						}
						break;
					}
					break;
				}
				break;
			case 49:
				switch (kbf[3]) {
				case 126:
					switch (kbf[2]) {
					case 53:
						key = KEY_F5;
						break;
					case 55:
						key = KEY_F6;
						break;
					case 56:
						key = KEY_F7;
						break;
					case 57:
						key = KEY_F8;
						break;
					}
					break;
				}
				break;
			case 50:
				switch (kbf[3]) {
				case 126:
					switch (kbf[2]) {
					case 48:
						key = KEY_F9;
						break;
					case 49:
						key = KEY_F10;
						break;
					case 51:
						key = KEY_F11;
						break;
					case 52:
						key = KEY_F12;
						break;
					case 53:
						key = KEY_F13;
						break;
					case 54:
						key = KEY_F14;
						break;
					case 56:
						key = KEY_F15;
						break;
					case 57:
						key = KEY_F16;
						break;
					}
					break;
				}
				break;

				/*  can't checked in macOS  */
			case 72:
				key = KEY_HOME;
				break;
			case 70:
				key = KEY_END;
				break;
			case 53:
				key = KEY_PAGEUP;
				break;
			case 54:
				key = KEY_PAGEDOWN;
				break;
			}
			break;
		case 79:
			switch (kbf[1]) {
			case 80:
				key = KEY_F1;
				break;
			case 81:
				key = KEY_F2;
				break;
			case 82:
				key = KEY_F3;
				break;
			case 83:
				key = KEY_F4;
				break;
			}
			break;
		}
		break;
	case 127:
		key = KEY_BACKSPACE;
		break;
	case 9:
		key = KEY_TAB;
		break;
	case 32:
		key = KEY_SPACE;
		break;
	case 10:
	case 13:
		key = KEY_ENTER;
		break;
	default:
		//		key = c;
		break;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &savetio);

	return	key;
}



// For Mac OS--------------------------------------------
#elif __MACH__		

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

char getKey(void)
{
	char	key = KEY_OTHERS;
	struct termios	tio, savetio;
	int	savefc;
	char	c, kbf[4];

	tcgetattr(STDIN_FILENO, &savetio);
	tio = savetio;
	tio.c_lflag &= ~ICANON;
	tio.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &tio);

	c = getchar();
	switch (c) {
	case EOF:
		key = KEY_EOF;
		break;
	case 27:
		savefc = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
		kbf[0] = getchar();
		kbf[1] = getchar();
		kbf[2] = getchar();
		kbf[3] = getchar();
		fcntl(STDIN_FILENO, F_SETFL, savefc);
		switch (kbf[0]) {
		case EOF:
			key = KEY_ESC;
			break;
		case 91:
			switch (kbf[1]) {
			case 65:
				key = KEY_UP;
				break;
			case 66:
				key = KEY_DOWN;
				break;
			case 68:
				key = KEY_LEFT;
				break;
			case 67:
				key = KEY_RIGHT;
				break;
			case 51:
				switch (kbf[2]) {
				case 126:
					key = KEY_DELETE;
					break;
				default:
					switch (kbf[3]) {
					case 126:
						switch (kbf[2]) {
						case 49:
							key = KEY_F17;
							break;
						case 50:
							key = KEY_F18;
							break;
						case 51:
							key = KEY_F19;
							break;
						}
						break;
					}
					break;
				}
				break;
			case 49:
				switch (kbf[3]) {
				case 126:
					switch (kbf[2]) {
					case 53:
						key = KEY_F5;
						break;
					case 55:
						key = KEY_F6;
						break;
					case 56:
						key = KEY_F7;
						break;
					case 57:
						key = KEY_F8;
						break;
					}
					break;
				}
				break;
			case 50:
				switch (kbf[3]) {
				case 126:
					switch (kbf[2]) {
					case 48:
						key = KEY_F9;
						break;
					case 49:
						key = KEY_F10;
						break;
					case 51:
						key = KEY_F11;
						break;
					case 52:
						key = KEY_F12;
						break;
					case 53:
						key = KEY_F13;
						break;
					case 54:
						key = KEY_F14;
						break;
					case 56:
						key = KEY_F15;
						break;
					case 57:
						key = KEY_F16;
						break;
					}
					break;
				}
				break;

				/*  can't checked in macOS  */
			case 72:
				key = KEY_HOME;
				break;
			case 70:
				key = KEY_END;
				break;
			case 53:
				key = KEY_PAGEUP;
				break;
			case 54:
				key = KEY_PAGEDOWN;
				break;
			}
			break;
		case 79:
			switch (kbf[1]) {
			case 80:
				key = KEY_F1;
				break;
			case 81:
				key = KEY_F2;
				break;
			case 82:
				key = KEY_F3;
				break;
			case 83:
				key = KEY_F4;
				break;
			}
			break;
		}
		break;
	case 127:
		key = KEY_BACKSPACE;
		break;
	case 9:
		key = KEY_TAB;
		break;
	case 32:
		key = KEY_SPACE;
		break;
	case 10:
	case 13:
		key = KEY_ENTER;
		break;
	default:
		key = c;
		break;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &savetio);

	return	key;
}

KEYTYPE getKeyName(void)
{
	KEYTYPE	key = KEY_OTHERS;
	struct termios	tio, savetio;
	int	savefc;
	char	c, kbf[4];

	tcgetattr(STDIN_FILENO, &savetio);
	tio = savetio;
	tio.c_lflag &= ~ICANON;
	tio.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &tio);

	c = getchar();
	switch (c) {
	case EOF:
		key = KEY_EOF;
		break;
	case 27:
		savefc = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
		kbf[0] = getchar();
		kbf[1] = getchar();
		kbf[2] = getchar();
		kbf[3] = getchar();
		fcntl(STDIN_FILENO, F_SETFL, savefc);
		switch (kbf[0]) {
		case EOF:
			key = KEY_ESC;
			break;
		case 91:
			switch (kbf[1]) {
			case 65:
				key = KEY_UP;
				break;
			case 66:
				key = KEY_DOWN;
				break;
			case 68:
				key = KEY_LEFT;
				break;
			case 67:
				key = KEY_RIGHT;
				break;
			case 51:
				switch (kbf[2]) {
				case 126:
					key = KEY_DELETE;
					break;
				default:
					switch (kbf[3]) {
					case 126:
						switch (kbf[2]) {
						case 49:
							key = KEY_F17;
							break;
						case 50:
							key = KEY_F18;
							break;
						case 51:
							key = KEY_F19;
							break;
						}
						break;
					}
					break;
				}
				break;
			case 49:
				switch (kbf[3]) {
				case 126:
					switch (kbf[2]) {
					case 53:
						key = KEY_F5;
						break;
					case 55:
						key = KEY_F6;
						break;
					case 56:
						key = KEY_F7;
						break;
					case 57:
						key = KEY_F8;
						break;
					}
					break;
				}
				break;
			case 50:
				switch (kbf[3]) {
				case 126:
					switch (kbf[2]) {
					case 48:
						key = KEY_F9;
						break;
					case 49:
						key = KEY_F10;
						break;
					case 51:
						key = KEY_F11;
						break;
					case 52:
						key = KEY_F12;
						break;
					case 53:
						key = KEY_F13;
						break;
					case 54:
						key = KEY_F14;
						break;
					case 56:
						key = KEY_F15;
						break;
					case 57:
						key = KEY_F16;
						break;
					}
					break;
				}
				break;

				/*  can't checked in macOS  */
			case 72:
				key = KEY_HOME;
				break;
			case 70:
				key = KEY_END;
				break;
			case 53:
				key = KEY_PAGEUP;
				break;
			case 54:
				key = KEY_PAGEDOWN;
				break;
			}
			break;
		case 79:
			switch (kbf[1]) {
			case 80:
				key = KEY_F1;
				break;
			case 81:
				key = KEY_F2;
				break;
			case 82:
				key = KEY_F3;
				break;
			case 83:
				key = KEY_F4;
				break;
			}
			break;
		}
		break;
	case 127:
		key = KEY_BACKSPACE;
		break;
	case 9:
		key = KEY_TAB;
		break;
	case 32:
		key = KEY_SPACE;
		break;
	case 10:
	case 13:
		key = KEY_ENTER;
		break;
	default:
//		key = c;
		break;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &savetio);

	return	key;
}


#endif

