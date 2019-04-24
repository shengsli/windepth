#include "header.h"
#include "templates.h"

#include "Bomb.h"


Bomb::Bomb(Psysl5Engine* pEngine, Bomber* pBomber)
	: AllyWeapon(pEngine)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imBomb())).GetWidth(); /* 10 pixels */
	m_iDrawHeight = (*(GetEngine()->GetpM_imBomb())).GetHeight(); /* 33 pixels */
	m_iCurrentScreenX = m_iPreviousScreenX = pBomber->GetXCentre() - m_iDrawWidth / 2;
	m_iCurrentScreenY = m_iPreviousScreenY = pBomber->GetYCentre() + m_iDrawHeight / 2;
	m_iVel = 2;
	SetVisible(true);
}


Bomb::~Bomb()
{
}


void Bomb::Draw()
{
	(*(GetEngine()->GetpM_imBomb())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		(*(GetEngine()->GetpM_imBomb())).GetWidth(), 
		(*(GetEngine()->GetpM_imBomb())).GetHeight());
	StoreLastScreenPositionForUndraw();
}


void Bomb::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenY += m_iVel;
	BorderCollision();
	SubmarineCollision();
	RedrawObjects();
}