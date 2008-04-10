/*
SettingsHandler.h
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
#ifndef __SETTINGSHANDLER_H__
#define __SETTINGSHANDLET_H__
#pragma once

#include "lsine.h"

class IneSettingsHandler
{
private:
	char _RcPath[MAX_PATH];

public:
	IneSettingsHandler()
	{
		DWORD len = ::GetModuleFileName(_hMod, _RcPath, MAX_PATH);
		for (;_RcPath[len] != '\\'; len--)
			_RcPath[len] = 0;
	
		strcpy(_RcPath, ConfigFileExists("step.rc", _RcPath));
	}

	FILE* LCOpen(LPCSTR path)
	{
		if (path == 0)
			return ::FileOpen(_RcPath);
		else
			return ::FileOpen(path);
	}

	BOOL LCClose(FILE* f)
	{
		return ::FileClose(f);
	}

	BOOL LCReadNextCommand(FILE* f, LPSTR v, size_t s)
	{
		return ReadNextCommand(f, v, (DWORD)s);
	}

	BOOL LCReadNextConfig(FILE * pFile, LPCSTR pszConfig, LPSTR pszValue, size_t cchValue)
	{
		char line[MAX_LINE_LENGTH];
		BOOL findConfig = FALSE;
		do
		{
			if (!ReadNextCommand(pFile, line, MAX_LINE_LENGTH))
				break;

			if (!_strnicmp(pszConfig, line, lstrlen(pszConfig)))
			{
				findConfig = TRUE;
				strncpy(pszValue, line, cchValue);
				break;
			}
		} while(!findConfig);

		return findConfig;
	}

	int LCTokenize(LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
	{
		return BBTokenize(szString, lpszBuffers,dwNumBuffers, szExtraParameters);
	}

	int GetRCInt(LPCSTR lpKeyName, int nDefault)
	{
		return ReadInt(_RcPath, lpKeyName, nDefault);
	}

	BOOL GetRCString(LPCSTR lpKeyName, LPSTR value, LPCSTR defStr, int maxLen)
	{
		strncpy(value, ReadString(_RcPath, lpKeyName, (LPSTR)(defStr != NULL ? defStr : "")), maxLen);
		return TRUE;
	}

	BOOL GetRCBool(LPCSTR lpKeyName, BOOL ifFound)
	{
		char value[MAX_LINE_LENGTH];
		GetRCString(lpKeyName, value, "not found", MAX_LINE_LENGTH);
		if (_stricmp(value, "not found") && 
			_stricmp(value, "off") && 
			_stricmp(value, "no") &&
			_stricmp(value, "false"))
			return ifFound;

		return !ifFound;
	}

	BOOL GetRCBoolDef(LPCSTR lpKeyName, BOOL bDefault)
	{
		return ReadBool(_RcPath, lpKeyName, bDefault == TRUE ? true : false);
	}

	COLORREF GetRCColor(LPCSTR lpKeyName, COLORREF colDef)
	{
		char color[8];
		sprintf(color, "#%02X%02X%02X", GetRValue(colDef), GetGValue(colDef), GetBValue(colDef));

		return ReadColor(_RcPath, lpKeyName, color);
	}

	BOOL LSGetVariable(LPCSTR pszKeyName, LPSTR pszValue)
	{
		strcpy(pszValue, ReadString(_RcPath, pszKeyName, ""));

		if (pszValue == 0)
			return FALSE;
		return TRUE;
	}

	BOOL LSGetVariableEx(LPCSTR pszKeyName, LPSTR pszValue, DWORD dwLength)
	{
		strncpy(pszValue, ReadString(_RcPath, pszKeyName, ""), dwLength);

		if (pszValue == 0)
			return FALSE;
		return TRUE;
	}

	void LSSetVariable(LPCSTR pszKeyName, LPCSTR pszValue)
	{
		WriteString(_RcPath, pszKeyName, (LPSTR)pszValue);
	}

	int GetRCCoordinate(LPCSTR pszKeyName, int nDefault, int nMaxVal)
	{
		char strval[MAX_LINE_LENGTH];
		if (!ReadString(_RcPath, strval, NULL))
			return nDefault;

		return ParseCoordinate(strval, nDefault, nMaxVal);
	}

	int ParseCoordinate(LPCSTR szString, int nDefault, int nMaxVal)
	{
		bool center = false;
		bool percentail = false;
		bool negative = false;
		int val = 0;

		if (szString[0] == '-')
		{
			negative = true;
			szString++;
		}
		else if (szString[0] == '+')
			szString++;

		if (IsInString(szString, "%"))
			percentail = true;

		if (IsInString(szString, "c"))
			center = true;

		for (;*szString >= '0' && *szString <= '9'; szString++)
			val = val * 10 + (*szString - '0');

		if (percentail)
			val = nMaxVal * val / 100;

		if (center)
		{
			if (negative)
				val = nMaxVal / 2 - val;
			else
				val = nMaxVal / 2 + val;
		}
		else if (negative)
			val = nMaxVal - val;

		return val;
	}

};
#endif