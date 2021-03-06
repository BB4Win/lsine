/*
BangHandler.h
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
#ifndef __BANG_HANDLER_H__
#define __BANG_HANDLER_H__
#pragma once

#include "lsine.h"
#include <string>
#include <algorithm>
#include <map>
#include "bang.h"

typedef std::map<std::string, Bang*> BangMap;

class IneBangHandler
{
	BangMap bangs;
public:
	BOOL AddBang(LPCSTR command, Bang* bang)
	{
		std::string c = command;
		std::transform(c.begin(), c.end(), c.begin(), tolower);
		BangMap::iterator it = bangs.find(c);
		if (it != bangs.end())
			bangs.erase(it);
		

		bangs.insert(BangMap::value_type(c, bang));
		return TRUE;
	}

	BOOL RemoveBang(LPCSTR command)
	{
		std::string c = command;
		std::transform(c.begin(), c.end(), c.begin(), tolower);
		BangMap::iterator it = bangs.find(c);
		if (it != bangs.end())
		{
			bangs.erase(it);
			return TRUE;
		}

		return FALSE;
	}

	BOOL ParseBang(HWND caller, LPCSTR command, LPCSTR args)
	{
		std::string c = command;
		std::transform(c.begin(), c.end(), c.begin(), tolower);
		BangMap::iterator it = bangs.find(c);
		if (it != bangs.end())
		{
			it->second->Execute(caller, args);
			return TRUE;
		}
		return FALSE;
	}
};
#endif