/*
ImageHandler.h
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
#ifndef __IMAGEHANDLER_H__
#define __IMAGEHANDLER_H__
#pragma once

#include "lsine.h"

class IneImageHandler
{
public:
	// TODO: All this!
	HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance, int xoffset, int yoffset)
	{
		Log("LSine", __FUNCTION__" Not Implemented");
		return NULL;
	}

	HBITMAP BitmapFromIcon (HICON hIcon)
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

	HBITMAP LoadLSImage(LPCSTR szFile, LPCSTR szImage)
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

		//TODO: Do we need to handle ParentRelative?

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

	HICON LoadLSIcon (LPCSTR pszIconPath, LPCSTR pszFile)
	{
		HICON hIcon = ExtractIcon(NULL, pszIconPath, 0);
		if (hIcon == NULL)
			hIcon = ExtractIcon(NULL, pszFile, 0);
		return hIcon;
	}

	void GetLSBitmapSize(HBITMAP hBitmap, LPINT nWidth, LPINT nHeight)
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

	void TransparentBltLS (HDC dc, int nXDest, int nYDest, int nWidth, int nHeight, HDC tempDC, int nXSrc, int nYSrc, COLORREF colorTransparent)
	{
		Log("LSine", __FUNCTION__" Not Implemented");
	}

	void Frame3D(HDC dc, RECT rect, COLORREF TopColor, COLORREF BottomColor, int Width)
	{
		TopColor = ReadColor(stylePath(), "borderColor:", "#000000");
		CreateBorder(dc, &rect, TopColor, Width);
	}
};
#endif