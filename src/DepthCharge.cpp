#include "header.h"
#include "templates.h"

#include "DepthCharge.h"
#include "Submarine.h"


DepthCharge::DepthCharge(Psysl5Engine* pEngine, BattleShip* pShip)
	: AllyWeapon(pEngine)
	, m_pShip(pShip)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imOilDrum())).GetWidth(); /* 37 pixels */
	m_iDrawHeight = (*(GetEngine()->GetpM_imOilDrum())).GetHeight(); /* 31 pixels */
	m_iCurrentScreenX = m_iPreviousScreenX = m_pShip->GetXCentre() - m_iDrawWidth / 2;
	m_iCurrentScreenY = m_iPreviousScreenY = m_pShip->GetYCentre() + m_iDrawHeight / 2;
	SetVisible(true);
}


DepthCharge::~DepthCharge()
{
}


void DepthCharge::Draw()
{
	(*(GetEngine()->GetpM_imOilDrum())).RenderImageWithMask(GetEngine()->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		(*(GetEngine()->GetpM_imOilDrum())).GetWidth(), (*(GetEngine()->GetpM_imOilDrum())).GetHeight());
	StoreLastScreenPositionForUndraw();
}


void DepthCharge::DoUpdate(int iCurrentTime)
{
	BorderCollision();
	SubmarineCollision();
	m_iCurrentScreenY++;
	RedrawObjects();
}


