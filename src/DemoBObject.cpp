#include "header.h"
#include "templates.h"

#include "DemoBObject.h"


DemoBObject::DemoBObject(BaseEngine* pEngine)
	: DisplayableObject(pEngine)
{
	m_iCurrentScreenX = m_iPreviousScreenX = 100;
	m_iCurrentScreenY = m_iPreviousScreenY = 100;
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = 100;
	m_iDrawHeight = 50;
	SetVisible(true);
}


DemoBObject::~DemoBObject()
{
}


void DemoBObject::Draw(void)
{
	GetEngine()->DrawScreenRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY, 
		m_iCurrentScreenX + m_iDrawWidth - 1, 
		m_iCurrentScreenY + m_iDrawHeight - 1, 
		0x00ff00);
	StoreLastScreenPositionForUndraw();
}

void DemoBObject::DoUpdate(int iCurrentTime)
{
	if (GetEngine()->IsKeyPressed(SDLK_UP))
		m_iCurrentScreenY -= 2;
	if (GetEngine()->IsKeyPressed(SDLK_DOWN))
		m_iCurrentScreenY += 2;
	if (GetEngine()->IsKeyPressed(SDLK_LEFT))
		m_iCurrentScreenX -= 2;
	if (GetEngine()->IsKeyPressed(SDLK_RIGHT))
		m_iCurrentScreenX += 2;
	RedrawObjects();
}
