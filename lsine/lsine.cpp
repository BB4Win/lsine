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
#include <bbapi.h>
#include "ine.h"
#include "lsapi.h"
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
            case PLUGIN_NAME:			return "LSine";
            case PLUGIN_VERSION:		return "0.0.1";
            case PLUGIN_AUTHOR:			return "Brian Hartvigsen";
            case PLUGIN_RELEASEDATE:	return "2006 Feb 10";
			case PLUGIN_LINK:			return "http://tresni.crackmonkey.us";
            case PLUGIN_EMAIL:			return "tresni@crackmonkey.us";
            default:					return "LSine 0.0.1";
        }
    };
};

HMODULE hMod;
ModuleInitExFunc initMod;
ModuleQuitFunc quitMod;

int beginPlugin(HINSTANCE h)
{
	LoadModules();
	return TRUE;
}

void endPlugin(HINSTANCE h)
{
	UnloadModules();
}

void loadModules()
{
	hMod = LoadLibrary("label.dll");
	initMod = (ModuleInitExFunc)GetProcAddress(hMod, "initModuleEx");
	quitMod = (ModuleQuitFunc)GetProcAddress(hMod, "quitModule");
	if (initMod && quitMod)
		initMod(GetBBWnd(), hMod, "label.dll");
}

void UnloadModules()
{
	quitMod(hMod);
}
//-- BEGIN Settings Handler Functions ---------------------------------

LSAPI FILE* LCOpen (LPCSTR szPath)
{
	return InteropNotEmulate.GetSettingsHandler()->LCOpen(szPath);
}

LSAPI BOOL LCClose (FILE * f)
{
	return InteropNotEmulate.GetSettingsHandler()->LCClose(f);
}

LSAPI BOOL LCReadNextCommand(FILE * pFile, LPSTR pszValue, size_t cchValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LCReadNextCommand(pFile, pszValue, (DWORD)cchValue);
}

LSAPI BOOL LCReadNextConfig(FILE * pFile, LPCSTR pszConfig, LPSTR pszValue, size_t cchValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LCReadNextConfig(pFile, pszConfig, pszValue, cchValue);
}

LSAPI BOOL LCReadNextLine(FILE * pFile, LPSTR pszValue, size_t cchValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LCReadNextCommand(pFile, pszValue, cchValue);
}

LSAPI int LCTokenize (LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
{
	return InteropNotEmulate.GetSettingsHandler()->LCTokenize(szString, lpszBuffers,dwNumBuffers, szExtraParameters);
}

LSAPI int GetRCInt(LPCSTR lpKeyName, int nDefault)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCInt(lpKeyName, nDefault);
}

LSAPI BOOL GetRCString(LPCSTR lpKeyName, LPSTR value, LPCSTR defStr, int maxLen)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCString(lpKeyName, value, defStr, maxLen);
}

LSAPI BOOL GetRCBool(LPCSTR lpKeyName, BOOL ifFound)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCBool(lpKeyName, ifFound);
}

LSAPI BOOL GetRCBoolDef(LPCSTR lpKeyName, BOOL bDefault)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCBoolDef(lpKeyName, bDefault);
}

LSAPI BOOL GetRCLine(LPCSTR szKeyName, LPSTR szBuffer, UINT nBufLen, LPCSTR szDefault)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCString(szKeyName, szBuffer, szDefault, nBufLen);
}

LSAPI COLORREF GetRCColor(LPCSTR lpKeyName, COLORREF colDef)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCColor(lpKeyName, colDef);
}

LSAPI BOOL LSGetVariable(LPCSTR pszKeyName, LPSTR pszValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LSGetVariable(pszKeyName, pszValue);
}

LSAPI BOOL LSGetVariableEx(LPCSTR pszKeyName, LPSTR pszValue, DWORD dwLength)
{
	return InteropNotEmulate.GetSettingsHandler()->LSGetVariableEx(pszKeyName, pszValue, dwLength);
}

LSAPI void LSSetVariable(LPCSTR pszKeyName, LPCSTR pszValue)
{
	return InteropNotEmulate.GetSettingsHandler()->LSSetVariable(pszKeyName, pszValue);
}

LSAPI int GetRCCoordinate(LPCSTR pszKeyName, int nDefault, int nMaxVal)
{
	return InteropNotEmulate.GetSettingsHandler()->GetRCCoordinate(pszKeyName, nDefault, nMaxVal);
}

LSAPI int ParseCoordinate(LPCSTR szString, int nDefault, int nMaxVal)
{
	return InteropNotEmulate.GetSettingsHandler()->ParseCoordinate(szString, nDefault, nMaxVal);
}

//-- BEGIN Bang Handler Functions -------------------------------------

LSAPI BOOL AddBangCommand(LPCSTR pszCommand, BangCommand pfnBangCommand)
{
	return InteropNotEmulate.GetBangHandler()->AddBang(pszCommand, new Bang(pfnBangCommand, pszCommand));
}

LSAPI BOOL AddBangCommandEx(LPCSTR pszCommand, BangCommandEx pfnBangCommand)
{
	return InteropNotEmulate.GetBangHandler()->AddBang(pszCommand, new Bang(pfnBangCommand, pszCommand));
}

LSAPI BOOL RemoveBangCommand(LPCSTR pszCommand)
{
	return InteropNotEmulate.GetBangHandler()->RemoveBang(pszCommand);
}

LSAPI BOOL ParseBangCommand (HWND hCaller, LPCSTR pszCommand, LPCSTR pszArgs)
{
	return InteropNotEmulate.GetBangHandler()->ParseBang(hCaller, pszCommand, pszArgs);
}

//-- BEGIN Image Handler Functions ------------------------------------

// TODO: All this!
LSAPI HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance, int xoffset, int yoffset)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}

LSAPI HBITMAP BitmapFromIcon (HICON hIcon)
{
/*
This function's code is a part of the LiteStep Shell Source code.

Copyright (C) 1997-2005 The LiteStep Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/ 
	ICONINFO infoIcon;

	if (GetIconInfo(hIcon, &infoIcon))
	{
		HDC hDC;
		HBITMAP hOldBMP;
		HBRUSH hBrush;

		hDC = CreateCompatibleDC(NULL);
		hOldBMP = (HBITMAP)SelectObject(hDC, infoIcon.hbmColor);
		hBrush = CreateSolidBrush(RGB (255, 0, 255));
		DrawIconEx(hDC, 0, 0, hIcon, 0, 0, 0, hBrush, DI_NORMAL);
		DeleteObject(hBrush);
		DeleteObject(infoIcon.hbmMask);
		SelectObject(hDC, hOldBMP);
		DeleteDC(hDC);

		return infoIcon.hbmColor;
	}

	return NULL;
}

LSAPI HBITMAP LoadLSImage(LPCSTR szFile, LPCSTR szImage)
{
	const char* appearance = "%s:";
	const char* color = "%s.color:";
	const char* colorTo = "%s.colorTo:";
	const char* borderColor = "borderColor:";
	const char* borderWidth = "borderWidth:";

	char dimensions[MAX_LINE_LENGTH];
	char styleItem[MAX_LINE_LENGTH];
	char styleString[MAX_LINE_LENGTH];
	char style[MAX_LINE_LENGTH];
	char sPath[MAX_PATH];

	StyleItem si;
	int width;
	int height;

	char *tokens[1];
	tokens[0] = dimensions;

	BBTokenize(szFile, tokens, 1, styleItem);
	char *w = strtok(dimensions, "x");
	char *h = dimensions;

	width = atoi(w);
	height = atoi(h);

	strncpy(sPath, stylePath(), MAX_PATH - 1);

	_snprintf(styleString, MAX_LINE_LENGTH, appearance, styleItem);
	strncpy(style, ReadString(sPath, styleString, ""), MAX_LINE_LENGTH - 1);
	ParseItem(style, &si);

	_snprintf(styleString, MAX_LINE_LENGTH, color, styleItem);
	si.Color = ReadColor(sPath, styleString, "#FFFFFF");

	_snprintf(styleString, MAX_LINE_LENGTH, colorTo, styleItem);
	si.ColorTo = ReadColor(sPath, styleString, "#000000");

	_snprintf(styleString, MAX_LINE_LENGTH, borderColor, styleItem);
	si.borderColor = ReadColor(sPath, styleString, "#000000");

	_snprintf(styleString, MAX_LINE_LENGTH, borderWidth, styleItem);
	si.borderWidth = ReadInt(sPath, styleString, 1);

	HDC hDC = CreateCompatibleDC(NULL);
	BITMAPINFO alpha;
	ZeroMemory( &alpha.bmiHeader, sizeof(BITMAPINFOHEADER) );
	alpha.bmiHeader.biWidth=width;      // Set size you need
	alpha.bmiHeader.biHeight=height;    // Set size you need
	alpha.bmiHeader.biPlanes=1;
	alpha.bmiHeader.biBitCount=32;      // Can be 8, 16, 32 bpp or other number
	alpha.bmiHeader.biSizeImage=0;
	alpha.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	alpha.bmiHeader.biClrUsed= 0;
	alpha.bmiHeader.biClrImportant= 0;
	VOID *pvBits;

	HBITMAP bufferBitmap = CreateDIBSection( hDC,
												&alpha,
												DIB_RGB_COLORS,
												&pvBits,
												NULL,
												0 );
	HBITMAP oldBMP = (HBITMAP) SelectObject(hDC, bufferBitmap);
	RECT r = {0,0,width, height};
	MakeStyleGradient(hDC, &r, &si, false);
	bufferBitmap = (HBITMAP) SelectObject(hDC, oldBMP);
	ReleaseDC(NULL, hDC);
	DeleteObject(oldBMP);
	return bufferBitmap;
}

LSAPI HICON LoadLSIcon (LPCSTR pszIconPath, LPCSTR pszFile)
{
	HICON hIcon = ExtractIcon(NULL, pszIconPath, 0);
	if (hIcon == NULL)
		hIcon = ExtractIcon(NULL, pszFile, 0);
	return hIcon;
}

LSAPI void GetLSBitmapSize(HBITMAP hBitmap, LPINT nWidth, LPINT nHeight)
{
/*
This function's code is a part of the LiteStep Shell Source code.

Copyright (C) 1997-2005 The LiteStep Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/ 
	BITMAP hbmBitmap;

	if (!GetObject(hBitmap, sizeof(hbmBitmap), (LPSTR) & hbmBitmap))
	{
		*nWidth = 0;
		*nHeight = 0;
	}
	else
	{
		*nWidth = hbmBitmap.bmWidth;
		*nHeight = hbmBitmap.bmHeight;
	}
}

LSAPI void TransparentBltLS (HDC dc, int nXDest, int nYDest, int nWidth, int nHeight, HDC tempDC, int nXSrc, int nYSrc, COLORREF colorTransparent)
{
	Log("LSine", __FUNCTION__" Not Implemented");
}

LSAPI void Frame3D(HDC dc, RECT rect, COLORREF TopColor, COLORREF BottomColor, int Width)
{
	Log("LSine", __FUNCTION__" Not Implemented");
}

//-- BEGIN Command Handler Functions ----------------------------------

LSAPI int CommandTokenize (LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
{
	return InteropNotEmulate.GetCommandHandler()->CommandTokenize(szString, lpszBuffers, dwNumBuffers, szExtraParameters);
}

LSAPI void CommandParse(LPCSTR pszCommand, LPSTR pszOutCommand, LPSTR pszOutArgs, size_t cchOutCommand, size_t cchOutArgs)
{
	return InteropNotEmulate.GetCommandHandler()->CommandParse(pszCommand, pszOutCommand, pszOutArgs, cchOutCommand, cchOutArgs);
}

LSAPI HINSTANCE LSExecute(HWND Owner, LPCSTR szCommand, int nShowCmd)
{
	return LSExecuteEx(Owner, "", szCommand, "", "", nShowCmd);
}

LSAPI HINSTANCE LSExecuteEx(HWND Owner, LPCSTR szOperation, LPCSTR szCommand, LPCSTR szArgs, LPCSTR szDirectory, int nShowCmd)
{
	return InteropNotEmulate.GetCommandHandler()->LSExecuteEx(Owner, szOperation, szCommand, szArgs, szDirectory, nShowCmd);
}

LSAPI HWND GetLitestepWnd(void)
{
	return InteropNotEmulate.GetCommandHandler()->GetLitestepWnd();
}

LSAPI BOOL WINAPI LSGetLitestepPath(LPSTR pszPath, size_t cchPath)
{
	return InteropNotEmulate.GetCommandHandler()->LSGetLitestepPath(pszPath, cchPath);
}

LSAPI BOOL WINAPI LSGetImagePath(LPSTR pszPath, size_t cchPath)
{
	return InteropNotEmulate.GetCommandHandler()->LSGetImagePath(pszPath, cchPath);
}

LSAPI void VarExpansion(LPSTR pszExpandedString, LPCSTR pszTemplate)
{
	InteropNotEmulate.GetCommandHandler()->VarExpansion(pszExpandedString, pszTemplate);
}

LSAPI void VarExpansionEx(LPSTR pszExpandedString, LPCSTR pszTemplate, size_t cchExpandedString)
{
	InteropNotEmulate.GetCommandHandler()->VarExpansionEx(pszExpandedString, pszTemplate, cchExpandedString);
}

LSAPI BOOL GetToken(LPCSTR szString, LPSTR szToken, LPCSTR * szNextToken, BOOL useBrackets)
{
	return InteropNotEmulate.GetCommandHandler()->GetToken(szString, szToken, szNextToken, useBrackets);
}

LSAPI void SetDesktopArea(int left, int top, int right, int bottom)
{
	InteropNotEmulate.GetCommandHandler()->SetDesktopArea(left, top, right, bottom);
}

LSAPI BOOL match(LPCSTR pattern, LPCSTR text)
{
	return InteropNotEmulate.GetCommandHandler()->match(pattern, text);
}

LSAPI int matche(LPCSTR pattern, LPCSTR text)
{
	return InteropNotEmulate.GetCommandHandler()->matche(pattern, text);
}

LSAPI BOOL is_valid_pattern(LPCSTR p, LPINT error_type)
{
	return InteropNotEmulate.GetCommandHandler()->is_valid_pattern(p, error_type);
}

//TODO: Implement These ;)

LSAPI void GetResStr(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText)
{
	Log("LSine", __FUNCTION__" Not Implemented");
}

LSAPI void GetResStrEx(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText, ...)
{
	Log("LSine", __FUNCTION__" Not Implemented");
}

LSAPI int LSGetSystemMetrics(int)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return 0;
}

LSAPI HMONITOR LSMonitorFromWindow(HWND, DWORD)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}

LSAPI HMONITOR LSMonitorFromRect(LPCRECT, DWORD)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}

LSAPI HMONITOR LSMonitorFromPoint(POINT, DWORD)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}

LSAPI BOOL LSGetMonitorInfo(HMONITOR, LPMONITORINFO)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}

LSAPI BOOL LSEnumDisplayMonitors(HDC, LPCRECT, MONITORENUMPROC, LPARAM)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}

LSAPI BOOL LSEnumDisplayDevices(PVOID, DWORD, PDISPLAY_DEVICE, DWORD)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}

LSAPI BOOL WINAPI LSLog(int nLevel, LPCSTR pszModule, LPCSTR pszMessage)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return FALSE;
}

LSAPI BOOL WINAPIV LSLogPrintf(int nLevel, LPCSTR pszModule, LPCSTR pszFormat, ...)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return FALSE;
}

LSAPI HRESULT EnumLSData(UINT uInfo, FARPROC pfnCallback, LPARAM lParam)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}