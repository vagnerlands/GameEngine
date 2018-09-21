#pragma once
//========================================================================
// ZipFile.h : API to use Zip files
//
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code: http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

//========================================================================
//  Content References in Game Coding Complete 3rd Edition
// 

// --------------------------------------------------------------------------
// File:        ZipFile.h
//
// Purpose:     The declaration of a quick'n dirty ZIP file reader class.
//              Original code from Javier Arevalo.
//              Get zlib from http://www.cdrom.com/pub/infozip/zlib/
//
// class ZipFile - Chapter 7, page 186
// --------------------------------------------------------------------------

#include <map>
#include <stdio.h>

typedef std::map<std::string, int> ZipContentsMap;		// maps path to a zip content id

class ZipFile
{
  public:
    ZipFile() { m_nEntries=0; m_pFile=NULL; m_pDirData=NULL; }
    virtual ~ZipFile() { End(); fclose(m_pFile); }

    bool Init(const char* resFileName);
    void End();

    int GetNumFiles()const { return m_nEntries; }
    void GetFilename(int i, char *pszDest) const;
    int GetFileLen(int i) const;
    bool ReadFile(int i, void *pBuf);

	// Added to show multi-threaded decompression
	bool ReadLargeFile(int i, void *pBuf, void (*callback)(int, bool &));

	int Find(const char *path) const;

	ZipContentsMap m_ZipContentsMap;

  private:
    struct TZipDirHeader;
    struct TZipDirFileHeader;
    struct TZipLocalHeader;

    FILE *m_pFile;		// Zip file
    char *m_pDirData;	// Raw data buffer.
    int  m_nEntries;	// Number of entries.

    // Pointers to the dir entries in pDirData.
    const TZipDirFileHeader **m_papDir;   
};


