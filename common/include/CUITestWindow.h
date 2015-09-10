#ifndef CUITESTWINDOW_H
#define CUITESTWINDOW_H

#include "PolyUIWindow.h"

namespace Polycode
{
	class UIButton;
	class UILabel;
	class UICheckBox;
	class UIColorBox;
	class UIColorPicker;
	class UIComboBox;
	class UIHSlider;
	class UIImageButton;
	class UIMenu;
}

using namespace Polycode;

class CUITestWindow : public UIWindow
{
public:
	CUITestWindow(String windowName, Number width, Number height);
	virtual ~CUITestWindow();

	virtual void handleEvent(Event *event);

private:
	UIButton*		m_pButton;
	UIButton*		m_pFocusedButton;
	UILabel*		m_pLabel;
	UICheckBox*		m_pCheckbox;
	UIColorBox*		m_pColourBox;
	UIComboBox*		m_pComboBox;
	UIHSlider*		m_pHSlider;
	UIImageButton*	m_pImageButton;
	UIMenu*			m_pMenu;
};

#endif // CUITESTWINDOW_H