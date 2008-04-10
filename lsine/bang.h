/*
bang.h
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

#ifndef __BANG_H
#define __BANG_H
#pragma once

#include <string>
#include "lsapidefines.h"
class Bang
{
	BangCommand* bang;
	BangCommandEx* bangEx;
	std::string szCommand;

public:
	Bang(BangCommand* b, LPCSTR command)
		: bangEx(NULL),
		szCommand(command)
	{
		bang = b;
	}

	Bang(BangCommandEx* b, LPCSTR command)
		: bang(NULL),
		szCommand(command)
	{
		bangEx = b;
	}

	void Execute(HWND hCaller, LPCSTR pszArgs)
	{
		if(bang != NULL)
			bang(hCaller, pszArgs);
		else
			bangEx(hCaller, szCommand.c_str(), pszArgs);
			
	}
};

#endif