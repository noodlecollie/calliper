/*
 * VTFLib
 * Copyright (C) 2005-2010 Neil Jedrzejewski & Ryan Gregg
 *               2014      Mathias Panzenböck

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later
 * version.
 */

#include "../Error.h"
#include <QString>

using namespace VTFLib::Diagnostics;

#if defined(Q_OS_WIN32)

#ifdef UNICODE
typedef LPWSTR CERROR_PSTR;
#else
typedef LPSTR CERROR_PSTR;
#endif

vlVoid CError::Set(const vlChar *cErrorMessage, vlBool bSystemError)
{
	vlChar cBuffer[2048];
	if(bSystemError)
	{
		LPVOID lpMessage;
		vlUInt uiLastError = GetLastError(); 

        if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, uiLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (CERROR_PSTR)&lpMessage, 0, NULL))
		{
            snprintf(cBuffer, sizeof(cBuffer), "Error:\n%s\n\nSystem Error: 0x%.8x:\n%s", cErrorMessage, uiLastError, QString::fromLocal8Bit((const char *)lpMessage).toLatin1().constData());

			LocalFree(lpMessage);
		}
		else
		{
			snprintf(cBuffer, sizeof(cBuffer), "Error:\n%s\n\nSystem Error: 0x%.8x.", cErrorMessage, uiLastError); 
		}

		
	}
	else
	{
		snprintf(cBuffer, sizeof(cBuffer), "Error:\n%s", cErrorMessage); 
	}

	this->Clear();
	this->cErrorMessage = new vlChar[strlen(cBuffer) + 1];
	strcpy(this->cErrorMessage, cBuffer);
}

#endif
