/*****************************************************************************/
/*!	@file	keytypes.h
	@brief	Header file for key input control from stdin

	IO setting for console changes based on OS type (Win&Mac)

	@see	keytypes.cÅ@Source file for keytypes.h
	@par	Log
		- 2020/12/04 Ver. 1.0
			- Basic settings were programmed (by Keisuke SHIMA)
		- 2024/05/01 Ver. 1.1
			- Change settings to be compatible with CygWin
   @attention
		- Original codes programmed by Prof. Amano.
	@par Copyright
		Copyright &copy; 2020- Keisuke SHIMA All rights reserved.
******************************************************************************/
#ifndef __INC_KEY_TYPES_H
#define __INC_KEY_TYPES_H

//! Definition of data type for key input
typedef enum {
	KEY_OTHERS = 0,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ESC,
	KEY_BACKSPACE,
	KEY_DELETE,
	KEY_SPACE,
	KEY_TAB,
	KEY_ENTER,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
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
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,
	KEY_F17,
	KEY_F18,
	KEY_F19,
	KEY_STOP,
	KEY_EOF
} KEYTYPE;

//! Grobal variable for understanding key name input (use getKeyName() together)
extern const char* INPUTKEY_NAMES[];

//----------------------------------------------------------------------------
/** @brief Get key function from stdin

	@return char: Input key (special keys are named as KEYTYPE) 
	@attention getch() is used for Win, getchar() and terminal setting are used for Mac
*/
char getKey(void);

//----------------------------------------------------------------------------
/** @brief Get key with name function from stdin

	@return KEYTYPE: Input KEYTYPE (named as KEYTYPE)
	@attention getch() is used for Win, getchar() and terminal setting are used for Mac.<BR>
			   No defined keys in KEYTYPE cannot return.
*/
KEYTYPE getKeyName(void);

#endif

