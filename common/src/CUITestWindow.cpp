#include "CUITestWindow.h"
#include "PolycodeUI.h"
#include "CalliperApp.h"

#define OFFSET_X padding
#define OFFSET_Y (topPadding + titlebarRect->getHeight() + 5)

CUITestWindow::CUITestWindow(String windowName, Number width, Number height) :
	UIWindow(windowName, width, height)
{
	m_pButton = new UIButton("Button", 100, 19);
	m_pButton->setPosition(OFFSET_X, OFFSET_Y);
	addChild(m_pButton);

	m_pFocusedButton = new UIButton("Focused", 100, 19);
	m_pFocusedButton->setPosition(OFFSET_X + 110, OFFSET_Y);
	m_pFocusedButton->hasFocus = true;
	addChild(m_pFocusedButton);

	m_pLabel = new UILabel("Label", 12);
	m_pLabel->setPosition(OFFSET_X + 220, OFFSET_Y + 2);
	addChild(m_pLabel);

	m_pCheckbox = new UICheckBox("Check Box", false);
	m_pCheckbox->setPosition(OFFSET_X, OFFSET_Y + (1 * 28));
	addChild(m_pCheckbox);

	m_pComboBox = new UIComboBox(globalApp->GetGlobalMenu(), 150);
	m_pComboBox->setPosition(OFFSET_X + 110, OFFSET_Y + (1 * 28));
	m_pComboBox->addComboItem("Item One");
	m_pComboBox->addComboItem("Item Two");
	m_pComboBox->addComboItem("Item Three");
	m_pComboBox->setSelectedIndex(0);
	addChild(m_pComboBox);
	m_pComboBox->toggleDropDown();

	// Leave this until we've worked out how it works.
	//m_pColourBox = new UIColorBox(globalApp->GetGlobalColorPicker(), Color(0,0,0,255), 16, 16);
	//m_pColourBox->setPosition(OFFSET_X, OFFSET_Y + (3 * 28));
	//addChild(m_pColourBox);
}

CUITestWindow::~CUITestWindow()
{

}

void CUITestWindow::handleEvent(Event* event)
{
	UIWindow::handleEvent(event);
}