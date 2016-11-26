/*
 * VTFLib
 * Copyright (C) 2005-2010 Neil Jedrzejewski & Ryan Gregg

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later
 * version.
 */

// ============================================================
// NOTE: This file is commented for compatibility with Doxygen.
// ============================================================
/*!
	\file Error.h
	\brief Error handling class header.
*/

#ifndef VTFLIB_ERROR_H
#define VTFLIB_ERROR_H

#include "stdafx.h"

namespace VTFLib
{
	namespace Diagnostics
	{
		//! VTFLib Error handling class
		/*!
			The Error handling class allows you to aceess a text description 
			for the last error encountered.
		*/
		class VTFLIB_API CError
		{
		private:
			vlChar *cErrorMessage;

		public:
			CError();
			~CError();

			//! Clear the error message buffer.
			vlVoid Clear();

			//! Get the error message text.
			const vlChar *Get() const;

			//! Set the error message buffer.
			vlVoid SetFormatted(const vlChar *cFormat, ...) VTFLIB_PRINTF(2,3);
			vlVoid Set(const vlChar *cErrorMessage, vlBool bSystemError = vlFalse);
		};
	}
}

#endif
