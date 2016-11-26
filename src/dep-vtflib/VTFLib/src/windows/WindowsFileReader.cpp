/*
 * VTFLib
 * Copyright (C) 2005-2010 Neil Jedrzejewski & Ryan Gregg

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later
 * version.
 */

#include "../VTFLib.h"
#include "../FileReader.h"
#include <QScopedArrayPointer>

using namespace VTFLib;
using namespace VTFLib::IO::Readers;

#if defined(Q_OS_WIN32)
vlBool CFileReader::Open()
{
	this->Close();

#ifdef UNICODE
    size_t requiredLength = mbstowcs(NULL, this->cFileName, 0);
    QScopedArrayPointer<wchar_t> strBuffer(new wchar_t[requiredLength]);
    mbstowcs(strBuffer.data(), this->cFileName, requiredLength);
    wchar_t* str = strBuffer.data();
#else
    const char* str = this->cFileName;
#endif;

    this->hFile = CreateFile(str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(this->hFile == INVALID_HANDLE_VALUE)
	{
		this->hFile = NULL;

		LastError.Set("Error opening file.", vlTrue);

		return vlFalse;
	}

	return vlTrue;
}

vlVoid CFileReader::Close()
{
	if(this->hFile != NULL)
	{
		CloseHandle(this->hFile);
		this->hFile = NULL;
	}
}

vlSSize CFileReader::GetStreamSize() const
{
	if(this->hFile == NULL)
	{
		return 0;
	}

	return GetFileSize(this->hFile, NULL);
}

vlSSize CFileReader::GetStreamPointer() const
{
	if(this->hFile == NULL)
	{
		return 0;
	}

	return (vlSSize)SetFilePointer(this->hFile, 0, NULL, FILE_CURRENT);
}

vlSSize CFileReader::Seek(vlOffset lOffset, VLSeekMode uiMode)
{
	if(this->hFile == NULL)
	{
		return 0;
	}

	return (vlSSize)SetFilePointer(this->hFile, lOffset, NULL, (vlUInt)uiMode);
}
#endif

vlBool CFileReader::Read(vlChar &cChar)
{
	if(this->hFile == NULL)
	{
		return vlFalse;
	}

	vlULong ulBytesRead = 0;

	if(!ReadFile(this->hFile, &cChar, 1, &ulBytesRead, NULL))
	{
		LastError.Set("ReadFile() failed.", vlTrue);
	}

	return ulBytesRead == 1;
}

vlSize CFileReader::Read(vlVoid *vData, vlSize uiBytes)
{
	if(this->hFile == NULL)
	{
		return 0;
	}

	vlULong ulBytesRead = 0;

	if(!ReadFile(this->hFile, vData, uiBytes, &ulBytesRead, NULL))
	{
		LastError.Set("ReadFile() failed.", vlTrue);
	}

	return (vlSize)ulBytesRead;
}
