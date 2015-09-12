#include "CUITextInput.h"
#include "Polycode.h"

CUITextInput::CUITextInput(bool multiLine, Number width, Number height, int customFontSize, const String &customFont,
	int customLineSpacing) : UITextInput(multiLine, width, height, customFontSize, customFont, customLineSpacing)
{
	Config *conf = CoreServices::getInstance()->getConfig();

	Color c;
	if (multiLine)
	{
		c.setColorHexFromString(conf->getStringValue("Polycode", "uiTextInputFontColorMultiLine"));
	}
	else
	{
		c.setColorHexFromString(conf->getStringValue("Polycode", "uiTextInputFontColor"));
	}

	setTextColor(c);
	blinkerRect->setColor(c);
}

CUITextInput::~CUITextInput()
{
}