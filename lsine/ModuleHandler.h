/*
ModuleHandler.h
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
#ifndef __MODULE_HANDLER_H__
#define __MODULE_HANDLER_H__
#pragma once

#include "lsine.h"

typedef int (*ModuleInitExFunc) (HWND, HINSTANCE, LPCSTR);
typedef int (*ModuleQuitFunc) (HINSTANCE);

struct ModuleList {
	HMODULE hMod;
//	ModuleInitExFunc initMod;
	ModuleQuitFunc quitMod;
	ModuleList* next;
};

class IneModuleHandler {
private:
	ModuleList *modTree;
	ModuleList *modBranch;

	void innerUnload(ModuleList* mod)
	{
		if (mod)
		{
			if (mod->next)
				innerUnload(mod->next);

			mod->quitMod(mod->hMod);
		}
	}
public:
	IneModuleHandler() : modTree(NULL), modBranch(NULL)
	{
	}

	void LoadModules()
	{
		char _RcPath[MAX_PATH + 1];
		ZeroMemory(&_RcPath, MAX_PATH + 1);

		DWORD len = ::GetModuleFileName(_hMod, _RcPath, MAX_PATH);
		for (;_RcPath[len] != '\\'; len--)
			_RcPath[len] = 0;

		strcpy(_RcPath, ConfigFileExists("litestep.rc", _RcPath));

		FILE* fp = FileOpen(_RcPath);
		char buffer[MAX_LINE_LENGTH];

		while (ReadNextCommand(fp, buffer, MAX_LINE_LENGTH))
		{
			if (buffer[0] == '!' || buffer[0] == '#' || buffer[0] == ';')
				continue;
			
			HMODULE hMod = LoadLibrary(buffer);
			ModuleInitExFunc initMod = (ModuleInitExFunc)GetProcAddress(hMod, "initModuleEx");
			ModuleQuitFunc quitMod = (ModuleQuitFunc)GetProcAddress(hMod, "quitModule");

			if (initMod && quitMod)
			{

				if (initMod(GetBBWnd(), hMod, buffer) == 0)
				{
					if (modTree == NULL)
					{
						modTree = new ModuleList;
						modTree->hMod = hMod;
						//modTree->initMod = initMod;
						modTree->quitMod = quitMod;
						modTree->next = NULL;
						modBranch = modTree;
					}
					else
					{
						modBranch = modBranch->next = new ModuleList;
						modBranch->hMod = hMod;
						modBranch->quitMod = quitMod;
						modBranch->next = NULL;
					}
				}
			}
		}
	}

	void UnloadModules()
	{
		innerUnload(modTree);
	}

};

#endif