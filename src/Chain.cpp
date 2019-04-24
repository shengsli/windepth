#include "header.h"
#include "templates.h"

#include "Chain.h"


Chain::Chain(Psysl5Engine* pEngine, UnderWaterMine* pUnderWaterMine, int iOffset)
	: GameObject(pEngine)
	, m_iOffset(iOffset)
	, m_bIsDrop(false)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imChain())).GetWidth();
	m_iDrawHeight = (*(GetEngine()->GetpM_imChain())).GetHeight();
	m_iCurrentScreenX = m_iPreviousScreenX = pUnderWaterMine->GetXCentre();
	m_iCurrentScreenY = m_iPreviousScreenY = GetEngine()->GetScreenHeight();
	m_iVel = -1;
	m_iMinScreenY = pUnderWaterMine->GetM_iMinScreenY() + pUnderWaterMine->GetM_iDrawHeight() + m_iDrawHeight * iOffset;
	SetVisible(true);
}


Chain::~Chain()
{
}


void Chain::Draw()
{
	(*(GetEngine()->GetpM_imChain())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		(*(GetEngine()->GetpM_imChain())).GetWidth(),
		(*(GetEngine()->GetpM_imChain())).GetHeight());
	StoreLastScreenPositionForUndraw();
}


void Chain::DoUpdate(int iCurrentTime)
{
	if (m_bIsDrop)
		m_iCurrentScreenY -= m_iVel;
	else if (m_iCurrentScreenY > m_iMinScreenY)
		m_iCurrentScreenY += m_iVel;
	BorderCollision();
	RedrawObjects();
}


void Chain::BorderCollision()
{
	if (m_iCurrentScreenY >= GetEngine()->GetScreenHeight() + m_iDrawHeight)
		SetIsToRemove(true);
}


