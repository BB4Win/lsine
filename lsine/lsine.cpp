/*
lsine.cpp
This work is part of the Litestep Interop Not Emulate Project

Copyright (c) 2008, Brian Hartvigsen
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
#include "lsine.h"
#include "ine.h"
#include "lsapi.h"
#include "bang.h"

extern "C"
{
	DLL_EXPORT int beginPlugin(HINSTANCE h);
	DLL_EXPORT void endPlugin(HINSTANCE h);
	DLL_EXPORT LPCSTR pluginInfo(int x)
    {
        switch (x)
        {
            case PLUGIN_NAME:			return "LSine";
            case PLUGIN_VERSION:		return "0.0.1";
            case PLUGIN_AUTHOR:			return "Brian Hartvigsen";
            case PLUGIN_RELEASEDATE:	return "2008 April 10";
			case PLUGIN_LINK:			return "http://tresni.crackmonkey.us";
            case PLUGIN_EMAIL:			return "tresni@crackmonkey.us";
            default:					return "LSine 0.0.1";
        }
    };
};

int beginPlugin(HINSTANCE h)
{
	_hMod = h;
	InteropNotEmulate.GetModuleHandler()->LoadModules();
	return TRUE;
}

void endPlugin(HINSTANCE h)
{
	InteropNotEmulate.GetModuleHandler()->UnloadModules();
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

LSAPI HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance, int xoffset, int yoffset)
{
	return InteropNotEmulate.GetImageHandler()->BitmapToRegion(hBmp, cTransparentColor, cTolerance, xoffset, yoffset);
}

LSAPI HBITMAP BitmapFromIcon(HICON hIcon)
{
	return InteropNotEmulate.GetImageHandler()->BitmapFromIcon(hIcon);
}

LSAPI HBITMAP LoadLSImage(LPCSTR szFile, LPCSTR szImage)
{
	return InteropNotEmulate.GetImageHandler()->LoadLSImage(szFile, szImage);
}

LSAPI HICON LoadLSIcon (LPCSTR pszIconPath, LPCSTR pszFile)
{
	return InteropNotEmulate.GetImageHandler()->LoadLSIcon(pszIconPath, pszFile);
}

LSAPI void GetLSBitmapSize(HBITMAP hBitmap, LPINT nWidth, LPINT nHeight)
{
	return InteropNotEmulate.GetImageHandler()->GetLSBitmapSize(hBitmap, nWidth, nHeight);
}

LSAPI void TransparentBltLS (HDC dc, int nXDest, int nYDest, int nWidth, int nHeight, HDC tempDC, int nXSrc, int nYSrc, COLORREF colorTransparent)
{
	return InteropNotEmulate.GetImageHandler()->TransparentBltLS(dc, nXDest, nYDest, nWidth, nHeight, tempDC, nXSrc, nYSrc, colorTransparent);
}

LSAPI void Frame3D(HDC dc, RECT rect, COLORREF TopColor, COLORREF BottomColor, int Width)
{
	return InteropNotEmulate.GetImageHandler()->Frame3D(dc, rect, TopColor, BottomColor, Width);
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

LSAPI BOOL WINAPI LSLog(int nLevel, LPCSTR pszModule, LPCSTR pszMessage)
{
	return InteropNotEmulate.GetCommandHandler()->LSLog(nLevel, pszModule, pszMessage);
}

LSAPI BOOL WINAPIV LSLogPrintf(int nLevel, LPCSTR pszModule, LPCSTR pszFormat, ...)
{
	va_list args;
	char log_max[MAX_LINE_LENGTH];
	ZeroMemory(log_max, sizeof(log_max));

	va_start(args, pszFormat);
	_vsnprintf(log_max, MAX_LINE_LENGTH - 1, pszFormat, args);
	va_end(args);
	
	return InteropNotEmulate.GetCommandHandler()->LSLog(nLevel, pszModule, log_max);
}

//-- BEGIN Display  Handler Functions ----------------------------------

LSAPI int LSGetSystemMetrics(int i)
{
	return InteropNotEmulate.GetDisplayHandler()->LSGetSystemMetrics(i);
}

LSAPI HMONITOR LSMonitorFromWindow(HWND h, DWORD d)
{
	return InteropNotEmulate.GetDisplayHandler()->LSMonitorFromWindow(h, d);
}

LSAPI HMONITOR LSMonitorFromRect(LPCRECT r, DWORD d)
{
	return InteropNotEmulate.GetDisplayHandler()->LSMonitorFromRect(r, d);
}

LSAPI HMONITOR LSMonitorFromPoint(POINT p, DWORD d)
{
	return InteropNotEmulate.GetDisplayHandler()->LSMonitorFromPoint(p, d);
}

LSAPI BOOL LSGetMonitorInfo(HMONITOR h, LPMONITORINFO m)
{
	return InteropNotEmulate.GetDisplayHandler()->LSGetMonitorInfo(h, m);
}

LSAPI BOOL LSEnumDisplayMonitors(HDC h, LPCRECT r, MONITORENUMPROC p, LPARAM l)
{
	return InteropNotEmulate.GetDisplayHandler()->LSEnumDisplayMonitors(h, r, p, l);
}

LSAPI BOOL LSEnumDisplayDevices(PVOID p, DWORD d, PDISPLAY_DEVICE dd, DWORD l)
{
	return InteropNotEmulate.GetDisplayHandler()->LSEnumDisplayDevices(p, d, dd, l);
}

//-- BEGIN Unknown Handler Functions ----------------------------------
//TODO: Implement These ;)

LSAPI void GetResStr(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText)
{
	Log("LSine", __FUNCTION__" Not Implemented");
}

LSAPI void GetResStrEx(HINSTANCE hInstance, UINT uIDText, LPSTR pszText, size_t cchText, LPCSTR pszDefText, ...)
{
	Log("LSine", __FUNCTION__" Not Implemented");
}

LSAPI HRESULT EnumLSData(UINT uInfo, FARPROC pfnCallback, LPARAM lParam)
{
	Log("LSine", __FUNCTION__" Not Implemented");
	return NULL;
}