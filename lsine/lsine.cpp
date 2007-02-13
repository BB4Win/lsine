/*
lsine.cpp
This work is part of the Litestep Interop Not Emulate Project

Copyright (c) 2007, Brian Hartvigsen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Litestep Interop Not Emulate Project nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "lsapi.h"
#include "ine.h"
#include <bbapi.h>
#include "bang.h"

typedef int (*ModuleInitExFunc) (HWND, HINSTANCE, LPCSTR);
typedef int (*ModuleQuitFunc) (HINSTANCE);

extern "C"
{
	DLL_EXPORT int beginPlugin(HINSTANCE h);
	DLL_EXPORT void endPlugin(HINSTANCE h);
	DLL_EXPORT LPCSTR    pluginInfo(int x)
    {
        switch (x)
        {
            case PLUGIN_NAME:			return "LS4BB";
            case PLUGIN_VERSION:		return "0.0.1";
            case PLUGIN_AUTHOR:			return "Brian Hartvigsen";
            case PLUGIN_RELEASEDATE:	return "2006 Feb 10";
			case PLUGIN_LINK:			return "http://tresni.crackmonkey.us";
            case PLUGIN_EMAIL:			return "tresni@crackmonkey.us";
            default:					return "LS4BB 0.0.1";
        }
    };
};

HMODULE hMod;
ModuleInitExFunc initMod;
ModuleQuitFunc quitMod;

int beginPlugin(HINSTANCE h)
{
	hMod = LoadLibrary("beep.dll");
	initMod = (ModuleInitExFunc)GetProcAddress(hMod, "initModuleEx");
	quitMod = (ModuleQuitFunc)GetProcAddress(hMod, "quitModule");
	if (initMod && quitMod)
		initMod(GetBBWnd(), hMod, "beep.dll");
	return TRUE;
}

void endPlugin(HINSTANCE h)
{
	quitMod(hMod);
}

// TODO: Add these calls to InteropNotEmualte
LSAPI FILE* LCOpen (LPCSTR szPath)
{
	return FileOpen(szPath);
}

LSAPI BOOL LCClose (FILE * f)
{
	return FileClose(f);
}

// TODO: Clean up RC reading, look for colons
LSAPI BOOL LCReadNextCommand(FILE * pFile, LPSTR pszValue, size_t cchValue)
{
	return ReadNextCommand(pFile, pszValue, (DWORD)cchValue);
}

LSAPI BOOL LCReadNextConfig(FILE * pFile, LPCSTR pszConfig, LPSTR pszValue, size_t cchValue)
{
	char line[MAX_LINE_LENGTH];
	bool findConfig = false;
	do
	{
		if (!LCReadNextCommand(pFile, line, MAX_LINE_LENGTH))
			break;

		if (!_strnicmp(pszConfig, line, lstrlen(pszConfig)))
		{
			findConfig = true;
			strncpy(pszValue, line + lstrlen(pszConfig), cchValue);
			break;
		}
	} while(findConfig);

	return findConfig;
}

LSAPI BOOL LCReadNextLine(FILE * pFile, LPSTR pszValue, size_t cchValue)
{
	return LCReadNextCommand(pFile, pszValue, cchValue);
}

LSAPI int LCTokenize (LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
{
	return BBTokenize(szString, lpszBuffers,dwNumBuffers, szExtraParameters);
}

LSAPI int GetRCInt(LPCSTR lpKeyName, int nDefault)
{
	return ReadInt("litestep.rc", lpKeyName, nDefault);
}

LSAPI BOOL GetRCString(LPCSTR lpKeyName, LPSTR value, LPCSTR defStr, int maxLen)
{
	strncpy(value, ReadString("litestep.rc", lpKeyName, (LPSTR)defStr), maxLen);
	return true;
}

LSAPI BOOL GetRCBool(LPCSTR lpKeyName, BOOL ifFound)
{
	return GetRCBoolDef(lpKeyName, ifFound);
}

LSAPI BOOL GetRCBoolDef(LPCSTR lpKeyName, BOOL bDefault)
{
	return ReadBool("litestep.rc", lpKeyName, bDefault == TRUE ? true : false);
}

LSAPI BOOL GetRCLine(LPCSTR szKeyName, LPSTR szBuffer, UINT nBufLen, LPCSTR szDefault)
{
	return GetRCString(szKeyName, szBuffer, szDefault, nBufLen);
}

LSAPI COLORREF GetRCColor(LPCSTR lpKeyName, COLORREF colDef)
{
	char color[8];
	sprintf(color, "#%02X%02X%02X", GetRValue(colDef), GetGValue(colDef), GetBValue(colDef));
	return ReadColor("litestep.rc", lpKeyName, color);
}

// TODO: Implement proper variable support.
LSAPI BOOL LSGetVariable(LPCSTR pszKeyName, LPSTR pszValue)
{
	strcpy(pszValue, ReadString("litestep.rc", pszKeyName, ""));
	if (pszValue == 0)
		return FALSE;
	return TRUE;
}

LSAPI BOOL LSGetVariableEx(LPCSTR pszKeyName, LPSTR pszValue, DWORD dwLength)
{
	strncpy(pszValue, ReadString("litestep.rc", pszKeyName, ""), dwLength);
	if (pszValue == 0)
		return FALSE;
	return TRUE;
}

LSAPI void LSSetVariable(LPCSTR pszKeyName, LPCSTR pszValue)
{
	char *keyName = new char[lstrlen(pszKeyName) + 2];
	strcpy(keyName, pszKeyName);

	if (pszKeyName[lstrlen(pszKeyName) - 1] != ':')
		strcat(keyName, ":");

	WriteString("litestep.rc", pszKeyName, (LPSTR)pszValue);
}

LSAPI BOOL AddBangCommand(LPCSTR pszCommand, BangCommand pfnBangCommand)
{
	return InteropNotEmulate.AddBang(pszCommand, new Bang(pfnBangCommand, pszCommand));
}

LSAPI BOOL AddBangCommandEx(LPCSTR pszCommand, BangCommandEx pfnBangCommand)
{
	return InteropNotEmulate.AddBang(pszCommand, new Bang(pfnBangCommand, pszCommand));
}

LSAPI BOOL RemoveBangCommand(LPCSTR pszCommand)
{
	return InteropNotEmulate.RemoveBang(pszCommand);
}

LSAPI BOOL ParseBangCommand (HWND hCaller, LPCSTR pszCommand, LPCSTR pszArgs)
{
	return InteropNotEmulate.ParseBang(hCaller, pszCommand, pszArgs);
}

// TODO: All this!
LSAPI HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance, int xoffset, int yoffset)
{
	return NULL;
}

LSAPI HBITMAP BitmapFromIcon (HICON hIcon)
{
	return NULL;
}

LSAPI HBITMAP LoadLSImage(LPCSTR szFile, LPCSTR szImage)
{
	return NULL;
}

LSAPI HICON LoadLSIcon (LPCSTR szImage, LPCSTR szFile)
{
	return NULL;
}

LSAPI void GetLSBitmapSize(HBITMAP hBitmap, LPINT nWidth, LPINT nHeight) {}
LSAPI void TransparentBltLS (HDC dc, int nXDest, int nYDest, int nWidth, int nHeight, HDC tempDC, int nXSrc, int nYSrc, COLORREF colorTransparent) {}

LSAPI int CommandTokenize (LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
{
	return 0;
}

LSAPI void CommandParse(LPCSTR pszCommand, LPSTR pszOutCommand, LPSTR pszOutArgs, size_t cchOutCommand, size_t cchOutArgs) {}

LSAPI HINSTANCE LSExecute(HWND Owner, LPCSTR szCommand, int nShowCmd)
{
	return NULL;
}

LSAPI HINSTANCE LSExecuteEx(HWND Owner, LPCSTR szOperation, LPCSTR szCommand, LPCSTR szArgs, LPCSTR szDirectory, int nShowCmd)
{
	return NULL;
}

LSAPI HWND GetLitestepWnd(void)
{
	return GetBBWnd();
}

LSAPI BOOL WINAPI LSGetLitestepPath(LPSTR pszPath, size_t cchPath)
{
	GetBlackboxPath(pszPath, (int)cchPath);
	return TRUE;
}

LSAPI BOOL WINAPI LSGetImagePath(LPSTR pszPath, size_t cchPath)
{
	GetBlackboxPath(pszPath, (int)cchPath);
	strncat(pszPath, "images//", cchPath - lstrlen(pszPath));
	return TRUE;
}

LSAPI void VarExpansion(LPSTR pszExpandedString, LPCSTR pszTemplate){}
LSAPI void VarExpansionEx(LPSTR pszExpandedString, LPCSTR pszTemplate, size_t cchExpandedString){}

LSAPI BOOL GetToken(LPCSTR szString, LPSTR szToken, LPCSTR * szNextToken, BOOL useBrackets)
{
	return FALSE;
}

LSAPI void Frame3D(HDC dc, RECT rect, COLORREF TopColor, COLORREF BottomColor, int Width){}
LSAPI void SetDesktopArea(int left, int top, int right, int bottom){}

LSAPI BOOL match(LPCSTR pattern, LPCSTR text)
{
	return FALSE;
}

LSAPI int matche(LPCSTR pattern, LPCSTR text)
{
	return 0;
}

LSAPI BOOL is_valid_pattern(LPCSTR p, LPINT error_type)
{
	return FALSE;
}

LSAPI void GetResStr(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText){}
LSAPI void GetResStrEx(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText, ...){}

LSAPI int LSGetSystemMetrics(int)
{
	return 0;
}

LSAPI HMONITOR LSMonitorFromWindow(HWND, DWORD)
{
	return NULL;
}

LSAPI HMONITOR LSMonitorFromRect(LPCRECT, DWORD)
{
	return NULL;
}

LSAPI HMONITOR LSMonitorFromPoint(POINT, DWORD)
{
	return NULL;
}

LSAPI BOOL LSGetMonitorInfo(HMONITOR, LPMONITORINFO)
{
	return NULL;
}

LSAPI BOOL LSEnumDisplayMonitors(HDC, LPCRECT, MONITORENUMPROC, LPARAM)
{
	return NULL;
}

LSAPI BOOL LSEnumDisplayDevices(PVOID, DWORD, PDISPLAY_DEVICE, DWORD)
{
	return NULL;
}

LSAPI BOOL WINAPI LSLog(int nLevel, LPCSTR pszModule, LPCSTR pszMessage)
{
	return FALSE;
}

LSAPI BOOL WINAPIV LSLogPrintf(int nLevel, LPCSTR pszModule, LPCSTR pszFormat, ...)
{
	return FALSE;
}

LSAPI int GetRCCoordinate(LPCSTR pszKeyName, int nDefault, int nMaxVal)
{
	return 0;
}

LSAPI int ParseCoordinate(LPCSTR szString, int nDefault, int nMaxVal)
{
	return 0;
}

LSAPI HRESULT EnumLSData(UINT uInfo, FARPROC pfnCallback, LPARAM lParam)
{
	return NULL;
}