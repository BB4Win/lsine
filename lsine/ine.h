/*
ine.h
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

#include "bbapi.h"
#include <map>
#include "bang.h"

typedef std::map<LPCSTR, Bang*> BangMap;
//#define CHECKKEYNAME(x, y)		LPSTR y = MakeBBSettingName(x);
//#define FREEKEYNAME(x, y)		FreeBBSettingName(x, y);
#define CHECKKEYNAME(x, y)		char* y = (LPSTR)x;
#define FREEKEYNAME(x, y)

class IneBangHandler
{
	BangMap bangs;
public:
	BOOL AddBang(LPCSTR command, Bang* bang)
	{
		BangMap::iterator it = bangs.find(command);
		if (it != bangs.end())
			bangs.erase(it);

		bangs.insert(BangMap::value_type(command, bang));
		return TRUE;
	}

	BOOL RemoveBang(LPCSTR command)
	{
		BangMap::iterator it = bangs.find(command);
		if (it != bangs.end())
		{
			bangs.erase(it);
			return TRUE;
		}

		return FALSE;
	}

	BOOL ParseBang(HWND caller, LPCSTR command, LPCSTR args)
	{
		BangMap::iterator it = bangs.find(command);
		if (it != bangs.end())
		{
			it->second->Execute(caller, args);
			return TRUE;
		}
		return FALSE;
	}
};

class IneSettingsHandler
{
private:
	char _RcPath[MAX_PATH];

	LPSTR MakeBBSettingName(LPCSTR keyName)
	{
		int len = lstrlen(keyName);
		char *key;
		if (keyName[len] != ':')
		{
			 key = new char[len + 2];
			sprintf(key, "%s:", keyName);
		}
		else
		{
			key = (LPSTR)keyName;
		}

		return key;
	}

	void FreeBBSettingName(LPCSTR keyName, LPSTR key)
	{
		if (keyName != key)
			delete[] key;
	}

public:
	IneSettingsHandler()
	{
		DWORD len = ::GetModuleFileName(NULL, _RcPath, MAX_PATH);
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

			CHECKKEYNAME(pszConfig, config);
			if (!_strnicmp(config, line, lstrlen(config)))
			{
				findConfig = TRUE;
				strncpy(pszValue, line, cchValue);
				break;
			}
			FREEKEYNAME(pszConfig, config);
		} while(!findConfig);

		return findConfig;
	}

	int LCTokenize(LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
	{
		return BBTokenize(szString, lpszBuffers,dwNumBuffers, szExtraParameters);
	}

	int GetRCInt(LPCSTR lpKeyName, int nDefault)
	{
		CHECKKEYNAME(lpKeyName, key);
		int retValue = ReadInt(_RcPath, key, nDefault);
		FREEKEYNAME(lpKeyName, key);
		return retValue;
	}

	BOOL GetRCString(LPCSTR lpKeyName, LPSTR value, LPCSTR defStr, int maxLen)
	{
		CHECKKEYNAME(lpKeyName, key);
		strncpy(value, ReadString(_RcPath, key, (LPSTR)defStr), maxLen);
		FREEKEYNAME(lpKeyName, key);
		return TRUE;
	}

	BOOL GetRCBool(LPCSTR lpKeyName, BOOL ifFound)
	{
		char value[MAX_LINE_LENGTH];
		GetRCString(lpKeyName, value, "not found", MAX_LINE_LENGTH);
		if (strcmp(value, "not found") == 0)
			return !ifFound;
		else
			return ifFound;
	}

	BOOL GetRCBoolDef(LPCSTR lpKeyName, BOOL bDefault)
	{
		CHECKKEYNAME(lpKeyName, key);
		BOOL retValue = ReadBool(_RcPath, key, bDefault == TRUE ? true : false);
		FREEKEYNAME(lpKeyName, key);

		return retValue;
	}

	COLORREF GetRCColor(LPCSTR lpKeyName, COLORREF colDef)
	{
		char color[8];
		sprintf(color, "#%02X%02X%02X", GetRValue(colDef), GetGValue(colDef), GetBValue(colDef));

		CHECKKEYNAME(lpKeyName, key);
		COLORREF retValue =  ReadColor(_RcPath, key, color);
		FREEKEYNAME(lpKeyName, key);

		return retValue;
	}

	BOOL LSGetVariable(LPCSTR pszKeyName, LPSTR pszValue)
	{
		CHECKKEYNAME(pszKeyName, key);
		strcpy(pszValue, ReadString(_RcPath, pszKeyName, ""));
		FREEKEYNAME(pszKeyName, key);

		if (pszValue == 0)
			return FALSE;
		return TRUE;
	}

	BOOL LSGetVariableEx(LPCSTR pszKeyName, LPSTR pszValue, DWORD dwLength)
	{
		CHECKKEYNAME(pszKeyName, key);
		strncpy(pszValue, ReadString(_RcPath, key, ""), dwLength);
		FREEKEYNAME(pszKeyName, key);

		if (pszValue == 0)
			return FALSE;
		return TRUE;
	}

	void LSSetVariable(LPCSTR pszKeyName, LPCSTR pszValue)
	{
		CHECKKEYNAME(pszKeyName, key);
		WriteString("litestep.rc", pszKeyName, (LPSTR)pszValue);
		FREEKEYNAME(pszKeyName, key);
	}
};


class Ine
{
	IneBangHandler* Bang;
	IneSettingsHandler *Settings;

public:
	Ine()
	{
		Bang = new IneBangHandler();
		Settings = new IneSettingsHandler();
	}

	IneBangHandler* GetBangHandler()
	{
		return Bang;
	}

	IneSettingsHandler* GetSettingsHandler()
	{
		return Settings;
	}
};

static Ine InteropNotEmulate;