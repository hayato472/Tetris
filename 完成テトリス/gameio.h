/*****************************************************************************/
/*!	@file	gameio.h
    @brief	IO settings and control functions for console game programming

    IO setting for console changes based on OS type (Win&Mac)

    @see	gameio.cÅ@Source file for gameio.h
    @par	Log
        - 2020/11/30 Ver. 1.0
            - Basic settings were programmed (by Keisuke SHIMA)
        - 2020/12/01 Ver. 1.1
            - Change initialSettingGameIO for automatically setting (by Keisuke SHIMA)
            - Use global variable (gbl or Gbl) for working path of music.<BR>
              playSound() function can be used for playing music instead of PLAY_SOUND().
        - 2024/04/22 Ver 1.2
            - Include stdio.h
   @attention
        - This code supported by compiliers as follows: VS 2019-, MinGW, BorlandC++ and Xcode(Mac).
        - In CygWin, sounds cannot play. So some codes set are dummies (Ver 1.2).
        - Tiny C Compiler (TCC) is not supported (cannot compile; Ver 1.2).
        - In VS 2019- and BorlandC++, there are some warnings about 'PASCAL'. 
        - It should be noted that the SDL check must be unchecked in VS 2019 and current.
    @par Copyright
        Copyright &copy; 2013- Keisuke SHIMA All rights reserved.
******************************************************************************/
#pragma once
#ifndef __INC_GAMEIO_H
#define __INC_GAMEIO_H

#ifndef GAMEIO_FILEPATH
#define GAMEIO_FILEPATH "GameIODat"
#define GAMEIO_FILENAME "dir.dat"
#endif
#ifndef STR_MAX
#define STR_MAX 1024
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//----------------------------------------------------------------------------
/** @brief Sleep Function (Win&Mac)

    Wait program for msec
    @param [in] msec(unsigned long): wait time
    @attention Using Sleep() for Win, usleep for Mac
*/
void timeSleep(unsigned long msec);

//----------------------------------------------------------------------------
/** @brief Initial setting function (Win&Mac)

    Setup current directory, and console mode
    @return int: 0: success, -1: file open error, -2: file type error
    @attention Current folder path is automatically set for all data (music etc) by GAMEIO_FILENAME 
*/
int initialSettingGameIO( void );

//----------------------------------------------------------------------------
/** @brief Initial path setting function (Win&Mac)
    @param [in] pathName(char[]): current folder path for all data (music etc)
    @return int: 0: success, -1: file open error, -2: file type error
*/
int initialSettingPathGameIO(char pathName[]);

//----------------------------------------------------------------------------
/** @brief Current directory change by file(pathName[])

    Current directory is includes all data files (music etc)
    Called by initialSetting()
    @param [in] pathName(char*): current folder path for all data (music etc)
    @return int: 0: success, -1: file open error, -2: file type error
*/
int changeCDirToWork( char pathName[] );

//----------------------------------------------------------------------------
/** @brief Get UserName on the computer
    
    @param [out] name(char[]): Username (Login name)
    @param [in] size(int): maximum length of username
*/
int getUserName(char name[], int size);

//----------------------------------------------------------------------------
/** @brief Sound play func (wav)

    sndPlaySound used for Win & Mac (wav only)
    @param [in] name(char*) sound file name (in preset folder by GAMEIO_FILENAME)
    @return 0: Success
    @attention Codes for Windows should be changed for supporting mp3 files for future
               Mac OS only supports one time playing
*/
int playSound( char fname[]);

//----------------------------------------------------------------------------
/** @brief Sound play func (wav) just one time

    sndPlaySound used for Win & Mac (Win: wav only)
    @param [in] name(char*) sound file name (in preset folder by GAMEIO_FILENAME)
    @return 0: Success
    @attention Codes for Windows should be changed for supporting mp3 files for future
*/
int playSoundOneTime(char fname[]);

//----------------------------------------------------------------------------
/** @brief Sound stop func (wav) for Mac

    Stop all music.
*/
void stopSound( void );

//*****************************************************************************
//****  Global variable  ******************************************************
//! Global path for working folder (music etc)
extern char gblWorkPath[STR_MAX];

//! Global flag for working folder
//! set=1
extern int isGblWorkPath;
//*****************************************************************************

//For Windows-------------------------------------------
#ifdef _WIN32

#include <lmcons.h>
#include <Windows.h>
#include <io.h>
#include <tchar.h>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")

//----------------------------------------------------------------------------
/** @def PLAY_SOUND(name)
    @brief Sound control macro (wav file) for Win
    @param [in] name(char*) sound file name
    @attention wav file can be accepted only<BR>
               music will be looped until called STOP_SOUND()
*/
#define PLAY_SOUND(name)	sndPlaySound(_T(name), SND_ASYNC|SND_LOOP)
#define STOP_SOUND()    sndPlaySound(NULL,SND_ASYNC)
#define OS_CODE "_WIN32"
#define OS_CODENUM  6       //Length of OS_CODE
#define PATH_DRIVE  "C:"
#define PATH_SLASH  "\\"


//----------------------------------------------------------------------------
/** @brief Setup console terminal for Windows (stand alone exe)

    @param [in] stream(FILE*): stdout (display)
    @attention stdout must set for stream
*/
BOOL enable_virtual_terminal_processing(FILE* stream);

//For MacOS----------------------------------------------
#elif __MACH__
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


//----------------------------------------------------------------------------
/** @brief Sound control macro (wav & mp3) for Mac

    Using system call for mac
    @param [in] name(char*) sound file name
    @return no mean
*/
#define PLAY_SOUND(name) playSound(name)
#define STOP_SOUND() system("killall afplay");
#define OS_CODE "__MACH__"
#define OS_CODENUM  8       //Length of OS_CODE
#define PATH_DRIVE  ""
#define PATH_SLASH  "/"

#elif __CYGWIN__
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PLAY_SOUND(name) playSound(name)
#define STOP_SOUND() system("killall afplay");
#define OS_CODE "__MACH__"
#define OS_CODENUM  8       //Length of OS_CODE
#define PATH_DRIVE  ""
#define PATH_SLASH  "/"

#endif

#endif

