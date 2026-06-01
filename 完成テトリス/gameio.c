/*****************************************************************************/
/*!	@file	gameio.c
    @brief	IO settings and control functions for console game programming

    IO setting for console changes based on OS type (Win&Mac)

    @see	gameio.h　Header file for gameio.c
    @par	Log
        - 2020/11/30 Ver. 1.0
            - Basic settings were programmed (by Keisuke SHIMA)
         - 2020/12/01 Ver. 1.1
            - Change initialSettingGameIO for automatically setting (by Keisuke SHIMA)
            - Use global variable (gbl or Gbl) for working path of music.<BR>
              playSound() function can be used for playing music instead of PLAY_SOUND().
         - 2022/08/08 Ver. 2.0
            - Change changeCDirToWork() & getWorkingDirPath() for solving the error about working path name including 'Space'. 
            - Fix warnings for Windows by defining  _CRT_SECURE_NO_WARNINGS
         - 2024/04/23 Ver. 2.1
            - Changed getUserName(),playSoundOneTime() and playSound for compiling by gcc. MACRO __GNUC__ is used to check the complier.
            - Fixed errors for GCC
            - Changed mkdir() to solve the problem on gcc.
         - 2024/05/02 Ver. 2.2 (.h: Ver 1.2)
            - Revised for Borland C++
   @attention
        - This code supported by compiliers as follows: VS 2019-, MinGW, BorlandC++ and Xcode(Mac).
        - In CygWin, sounds cannot play. So some codes set are dummies (Ver 2.2).
        - Tiny C Compiler (TCC) is not supported (cannot compile; Ver 2.2).
        - In VS 2019- and BorlandC++, there are some warnings about 'PASCAL'. 
        - It should be noted that the SDL check must be unchecked in VS 2019 and current.
    @par Copyright
        Copyright &copy; 2013- Keisuke SHIMA All rights reserved.
******************************************************************************/
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "gameio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <direct.h>
#endif


//#define PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE 0x20016
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif
/*typedef struct _STARTUPINFOEXW {
    STARTUPINFOW StartupInfo;
    LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList;
} STARTUPINFOEXW, * LPSTARTUPINFOEXW;
*/

//For GCC
#ifdef __GNUC__
#define wcstombs(dest,source,size) sprintf(dest,source) //Directry change codes to gcc (not so good..)
#define mbstowcs(dest,source,size) sprintf(dest,source)
#elif __BORLANDC__
#define wcstombs(dest,source,size) sprintf(dest,source) //Directry change codes to gcc (not so good..)
#define mbstowcs(dest,source,size) sprintf(dest,source)
#endif

//! Global path for working folder (music etc)
char gblWorkPath[STR_MAX] = "";

//! Global flag for working folder
//! set=1
int isGblWorkPath = 0;

///////////////////////////////////////////
int changeCDirToWork(char pathName[])
{
    int retVal = -10;
    char tmp[STR_MAX];
    FILE* fp = NULL;
    char fname[STR_MAX];

    int countCode = 0;
    if ((fp = fopen(pathName, "r")) != NULL) {
        for (countCode = 0; fgets(tmp, STR_MAX, fp) != NULL; countCode++) {
            //       for( countCode=0; fscanf(fp,"%s",tmp)!=EOF; countCode++){
            int endlpos = strlen(tmp) - 1;
            tmp[endlpos] = '\0';     //Remove '\n' code from tmp
            if (strncmp(tmp, OS_CODE, OS_CODENUM) == 0) {
                strncpy(fname, &tmp[OS_CODENUM], STR_MAX - OS_CODENUM);
#ifdef __BORLANDC__
                chdir(fname);
#elif _WIN32
                _chdir(fname);
#else
                chdir(fname);
#endif
                //printf("%s\n",fname); //for debug
                retVal = 0;
            }
            else {
                retVal = -2;
            }
        }
    }
    else { //NR
        retVal = -1;
    }

    return retVal;
}

///////////////////////////////////////////
int getWorkingDirPath(char pathName[])
{
    int retVal = -10;
    char tmp[STR_MAX];
    FILE* fp = NULL;
    //    char fname[STR_MAX];

    int countCode = 0;
    if ((fp = fopen(pathName, "r")) != NULL) {
        for (countCode = 0; fgets(tmp, STR_MAX, fp) != NULL; countCode++) {
            //       for( countCode=0; fscanf(fp,"%s",tmp)!=EOF; countCode++){
            int endlpos = strlen(tmp) - 1;
            tmp[endlpos] = '\0';     //Remove '\n' code from tmp
            if (strncmp(tmp, OS_CODE, OS_CODENUM) == 0) {
                strncpy(gblWorkPath, &tmp[OS_CODENUM], STR_MAX - OS_CODENUM);
                isGblWorkPath = 1;  //setup flag
                //Directory change is bad for other application. no use
                //chdir(fname);
                //printf("GLOBAL=%s\n", gblWorkPath); //for debug
                retVal = 0;
            }
            else {
                retVal = -2;
            }
        }
    }
    else { //NR
        retVal = -1;
    }

    return retVal;
}

///////////////////////////////////////////
int initialSettingPathGameIO(char pathName[])
{
    int retVal = -1;
    char tmp[STR_MAX];
    char pname[STR_MAX];

    getUserName(tmp, STR_MAX);
    sprintf(pname, "%s%sUsers%s%s%sDocuments%s%s", PATH_DRIVE, PATH_SLASH, PATH_SLASH, tmp, PATH_SLASH, PATH_SLASH, GAMEIO_FILEPATH);
    //printf("Path: %s", pname); //for debug
#ifdef __CYGWIN__
    mkdir(pname, 0755);
#elif __MACH__
    mkdir(pname, 0755);
#elif __GNUC__
    mkdir(pname);
#elif _WIN32
    _mkdir(pname);
#else
    mkdir(pname, 0755);
#endif
    //chdir(pname);
    sprintf(tmp, "%s%s%s", pname, PATH_SLASH, pathName);
    //    printf("Path: %s\n", tmp); //for debug
        //getcwd(pname,STR_MAX); //for debug
    //    printf("%s\n",pname);

        //Directory change is bad for other application. no use
    retVal = changeCDirToWork(tmp);
    retVal = getWorkingDirPath(tmp);
    return retVal;

}

//For Windows-------------------------------------------
#ifdef _WIN32

///////////////////////////////////////////
void timeSleep(unsigned long msec)
{
	Sleep(msec);
}

///////////////////////////////////////////
int initialSettingGameIO( void )
{
    enable_virtual_terminal_processing(stdout);

    return initialSettingPathGameIO(GAMEIO_FILENAME);
}

///////////////////////////////////////////
BOOL enable_virtual_terminal_processing(FILE* stream)
{
    HANDLE handle = (HANDLE)_get_osfhandle(_fileno(stream));
    DWORD mode = 0;
    if (!GetConsoleMode(handle, &mode)) {
        return FALSE;
    }
    if (!SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        return FALSE;
    }
    return TRUE;
}
///////////////////////////////////////////
int getUserName(char name[], int size)
{
    TCHAR szUserName[UNLEN + 1];
    DWORD dwUNLen = UNLEN;

    if (!GetUserName((LPTSTR)szUserName, &dwUNLen)) {
        printf("Error: %u", GetLastError());
        return 1;
    }
    //_tprintf(TEXT("Username: %s\n"), szUserName);

    wcstombs(name, szUserName, size);
    return 0;
}

///////////////////////////////////////////
int playSound(char fname[])
{
    TCHAR tmpT[STR_MAX];
    char tmp[STR_MAX];
    int retVal = -1;
    if (isGblWorkPath) {
        sprintf(tmp, "%s%s%s", gblWorkPath, PATH_SLASH, fname);
        //printf("Global=%s\n", tmp); //for debug

        mbstowcs(tmpT, tmp, STR_MAX);
        sndPlaySound(tmpT, SND_ASYNC | SND_LOOP);

        retVal = 0;
    } 
    else {
        retVal = -1;
    }
    return retVal;
}

///////////////////////////////////////////
int playSoundOneTime(char fname[])
{
    TCHAR tmpT[STR_MAX];
    char tmp[STR_MAX];
    int retVal = -1;
    if (isGblWorkPath) {
        sprintf(tmp, "%s%s%s", gblWorkPath, PATH_SLASH, fname);
        //printf("Global=%s\n", tmp); //for debug

       mbstowcs(tmpT, tmp, STR_MAX);
        sndPlaySound(tmpT, SND_ASYNC );
        retVal = 0;
    }
    else {
        retVal = -1;
    }
    return retVal;
}


///////////////////////////////////////////
void stopSound(void)
{
    sndPlaySound(NULL, SND_ASYNC);
}

//For CygWin (cannot support CygWin for sound play (ver 2.2...))------------------------------------
#elif __CYGWIN__
#include <unistd.h>

///////////////////////////////////////////
void timeSleep(unsigned long msec)
{
    unsigned long tmp = msec * 1000;  //Convert to micro sec from msec

    usleep((unsigned int)tmp);
}

///////////////////////////////////////////
int playSound(char fname[])
{
    char tmp[STR_MAX];
    sprintf(tmp, "afplay %s%s&", gblWorkPath, fname);
    //    sprintf(tmp, "cat %s%s  ", gblWorkPath, fname);
//    printf("gblWorkPath: %s\n", gblWorkPath); //for debug
//    printf("Music: %s\n",tmp);
//    getchar();
//    system(tmp);

    return 0;
}

///////////////////////////////////////////
void stopSound(void)
{
 //   system("killall afplay"); //dummy
}

///////////////////////////////////////////
int initialSettingGameIO(void)
{
//    return initialSettingPathGameIO(GAMEIO_FILENAME);
    return 1;
}

///////////////////////////////////////////
int getUserName(char name[], int size)
{
    strcpy(name, getlogin());

    return 0;
}


//For MacOS-------------------------------------------
#elif __MACH__
#include <unistd.h>

///////////////////////////////////////////
void timeSleep(unsigned long msec)
{
    unsigned long tmp=msec*1000;  //Convert to micro sec from msec
    
    usleep((unsigned int)tmp);
}

///////////////////////////////////////////
int playSound( char fname[] )
{
    char tmp[STR_MAX];
    sprintf(tmp,"afplay %s%s&",gblWorkPath, fname);
    //printf("Music: %s\n",tmp);
    system(tmp);
    
    return 0;
}

///////////////////////////////////////////
void stopSound( void )
{
    system("killall afplay");
}

///////////////////////////////////////////
int initialSettingGameIO( void )
{
    return initialSettingPathGameIO(GAMEIO_FILENAME);
}

///////////////////////////////////////////
int getUserName(char name[], int size)
{
    strcpy(name, getlogin());

    return 0;
}

#endif

