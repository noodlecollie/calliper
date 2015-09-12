#ifndef CUITEXTINPUT_H
#define CUITEXTINPUT_H

#include "PolyUITextInput.h"

using namespace Polycode;

class CUITextInput : public UITextInput
{
public:
	CUITextInput(bool multiLine, Number width, Number height, int customFontSize = -1, const String &customFont = "", int customLineSpacing = -1);
	virtual ~CUITextInput();
};

#endif // CUITEXTINPUT_H