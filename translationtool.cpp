#include "translationtool.h"
#include <QKeyEvent>
#include "translationhandle.h"
#include "mapdocument.h"
#include "uiscene.h"

TranslationTool::TranslationTool(MapDocument *document) : BaseTool(TranslationTool::staticName(), document)
{
    m_bInMove = false;
    m_pHandle = NULL;
}

TranslationTool::~TranslationTool()
{
	if ( m_bActive )
	{
		deactivate();
	}
}

QString TranslationTool::staticName()
{
	return QString("TranslationTool");
}

void TranslationTool::vActivate()
{
    m_pHandle = new TranslationHandle(m_pDocument->uiScene()->root());
    m_pHandle->setObjectName("_translationHandle");
    updateHandleState();
    m_bInMove = false;
}

void TranslationTool::vDeactivate()
{
    if ( m_bInMove )
        endMove();

    delete m_pHandle;
    m_pHandle = NULL;
    BaseTool::vDeactivate();
}

void TranslationTool::vKeyPress(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyPress(e);
}

void TranslationTool::vKeyRelease(QKeyEvent *e)
{
    if ( m_bInMove )
        return;

    BaseTool::vKeyRelease(e);
}

void TranslationTool::vMousePress(QMouseEvent *e)
{

}

void TranslationTool::vMouseMove(QMouseEvent *e)
{
    if ( !m_bInMove )
    {
        BaseTool::vMouseMove(e);
        return;
    }
}

void TranslationTool::vMouseRelease(QMouseEvent *)
{
    if ( m_bInMove )
        endMove();
}

void TranslationTool::vSelectedSetChanged()
{
    if ( m_bInMove )
        endMove();

    updateTableFromSet();
    BaseTool::vSelectedSetChanged();
}

void TranslationTool::endMove()
{
    m_bInMove = false;
}

void TranslationTool::updateHandleState()
{
    Q_ASSERT(m_pHandle);
    m_pHandle->setPosition(m_pDocument->selectedSetCentroid());
    m_pHandle->setHidden(m_pDocument->selectedSet().count() < 1);
}

void TranslationTool::updateTableFromSet()
{
    updateHandleState();
}
