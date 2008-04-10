/*
CommandHandler.h
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
#ifndef __COMMAND_HANDLER_H__
#define __COMMAND_HANDLER_H__
#pragma once

#include "bbapi.h"

class IneCommandHandler
{
public:
	int CommandTokenize (LPCSTR szString, LPSTR * lpszBuffers, DWORD dwNumBuffers, LPSTR szExtraParameters)
	{
		return BBTokenize(szString, lpszBuffers, dwNumBuffers, szExtraParameters);
	}

	void CommandParse(LPCSTR pszCommand, LPSTR pszOutCommand, LPSTR pszOutArgs, size_t cchOutCommand, size_t cchOutArgs)
	{
		//TODO: Implement CommandParse
		Log("LSine", __FUNCTION__" Not Implemented");
	}

	HINSTANCE LSExecuteEx(HWND Owner, LPCSTR szOperation, LPCSTR szCommand, LPCSTR szArgs, LPCSTR szDirectory, int nShowCmd)
	{
		if (szCommand[0] == '@')
		{
			SendMessage(GetBBWnd(), BB_BROADCAST, NULL, (LPARAM)szCommand);
			return HINSTANCE(33);
		}
		else if (szCommand[0] == '!')
		{
			return ParseBangCommand(Owner, szCommand, szArgs) ? HINSTANCE(33) : HINSTANCE(32);
		}
		else
			return BBExecute(Owner, szOperation, szCommand, szArgs, szDirectory, nShowCmd, true);
	}

	HWND GetLitestepWnd(void)
	{
		return GetBBWnd();
	}

	BOOL LSGetLitestepPath(LPSTR pszPath, size_t cchPath)
	{
		GetBlackboxPath(pszPath, (int)cchPath);
		return TRUE;
	}

	BOOL LSGetImagePath(LPSTR pszPath, size_t cchPath)
	{
		GetBlackboxPath(pszPath, (int)cchPath);
		strncat(pszPath, "images//", cchPath - lstrlen(pszPath));
		return TRUE;
	}

	void VarExpansion(LPSTR pszExpandedString, LPCSTR pszTemplate)
	{
		//TODO: Implement VarExpansion
		Log("LSine", __FUNCTION__" Not Implemented");
	}

	void VarExpansionEx(LPSTR pszExpandedString, LPCSTR pszTemplate, size_t cchExpandedString)
	{
		//TODO: Implement VarExpansionEx
		Log("LSine", __FUNCTION__" Not Implemented");
	}

	BOOL GetToken(LPCSTR szString, LPSTR szToken, LPCSTR * szNextToken, BOOL useBrackets)
	{
		//TODO: Implement GetToken
		Log("LSine", __FUNCTION__" Not Implemented");
		return FALSE;
	}

	void SetDesktopArea(int left, int top, int right, int bottom)
	{
		//TODO: Implement SetDesktopArea
		Log("LSine", __FUNCTION__" Not Implemented");
	}

	BOOL match(LPCSTR pattern, LPCSTR text)
	{
		//TODO: Implement match
		Log("LSine", __FUNCTION__" Not Implemented");
		return FALSE;
	}

	int matche(LPCSTR pattern, LPCSTR text)
	{
		//TODO: Implement matche
		Log("LSine", __FUNCTION__" Not Implemented");
		return 0;
	}

	BOOL is_valid_pattern(LPCSTR p, LPINT error_type)
	{
		//TODO: Implement is_valid_pattern
		Log("LSine", __FUNCTION__" Not Implemented");
		return FALSE;
	}

	BOOL LSLog(int nLevel, LPCSTR pszModule, LPCSTR pszMessage)
	{
		Log(pszModule, pszMessage);
		return TRUE;
	}

	HRESULT EnumLSData(UINT uInfo, FARPROC pfnCallback, LPARAM lParam)
	{
		//TODO: Implement EnumLSData
		Log("LSine", __FUNCTION__" Not Implemented");
		return NULL;
	}
};

#endif