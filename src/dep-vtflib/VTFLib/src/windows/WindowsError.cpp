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
#include <QTextStream>

using namespace VTFLib::Diagnostics;

#if defined(Q_OS_WIN32)

#ifdef UNICODE
typedef LPWSTR CERROR_PSTR;
#else
typedef LPSTR CERROR_PSTR;
#endif

vlVoid CError::Set(const vlChar *cErrorMessage, vlBool bSystemError)
{
    QString strBuffer;

	if(bSystemError)
	{
		LPVOID lpMessage;
		vlUInt uiLastError = GetLastError(); 

        if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, uiLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (CERROR_PSTR)&lpMessage, 0, NULL))
		{
            QTextStream stream(&strBuffer, QIODevice::WriteOnly);
            stream << "Error:\n" << cErrorMessage << "\n\nSystem Error: 0x" << QString::number(uiLastError, 16) << ": \n" << QString::fromLocal8Bit((const char *)lpMessage);

			LocalFree(lpMessage);
		}
		else
		{
            QTextStream stream(&strBuffer, QIODevice::WriteOnly);
            stream << "Error:\n" << cErrorMessage << "\n\nSystem Error: 0x" << QString::number(uiLastError, 16) << ".";
		}

		
	}
	else
	{
        QTextStream stream(&strBuffer, QIODevice::WriteOnly);
        stream << "Error:\n" << cErrorMessage;
	}

	this->Clear();
    this->cErrorMessage = new vlChar[strBuffer.length() + 1];
    strcpy(this->cErrorMessage, strBuffer.toLatin1().constData());
}

#endif
