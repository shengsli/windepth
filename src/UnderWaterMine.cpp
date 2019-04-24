#include "header.h"
#include "templates.h"

#include "UnderWaterMine.h"
#include "Chain.h"


UnderWaterMine::UnderWaterMine(Psysl5Engine* pEngine, int iCurrentScreenX, int iMinScreenY)
	: AllyWeapon(pEngine)
	, m_iMinScreenY(iMinScreenY)
	, m_iChainCounter(0)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imUnderWaterMine())).GetWidth();
	m_iDrawHeight = (*(GetEngine()->GetpM_imUnderWaterMine())).GetHeight();
	m_iCurrentScreenX = m_iPreviousScreenX = iCurrentScreenX;
	m_iCurrentScreenY = m_iPreviousScreenY = GetEngine()->GetScreenHeight() + m_iDrawHeight;
	m_iVel = -1;
	SetVisible(true);
}


UnderWaterMine::~UnderWaterMine()
{
}


void UnderWaterMine::Draw()
{
	(*(GetEngine()->GetpM_imUnderWaterMine())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		(*(GetEngine()->GetpM_imUnderWaterMine())).GetWidth(), 
		(*(GetEngine()->GetpM_imUnderWaterMine())).GetHeight());
	StoreLastScreenPositionForUndraw();
}


void UnderWaterMine::DoUpdate(int iCurrentTime)
{
	if (m_iCurrentScreenY > m_iMinScreenY)
	{
		m_iCurrentScreenY += m_iVel;
		if (m_iCurrentScreenY < GetEngine()->GetScreenHeight() - m_iDrawHeight - 
			(*(GetEngine()->GetpM_imChain())).GetHeight() * (m_iChainCounter))
		{
			Chain* pChain = new Chain(GetEngine(), this, m_iChainCounter);
			GetEngine()->StoreObjectInVector(pChain);
			m_pChains_forward_list.push_front(pChain);
			m_iChainCounter++;
		}
	}
	SubmarineCollision();
	RedrawObjects();
}


void UnderWaterMine::SetIsToRemove(bool bIsToRemove)
{
	if (!IsToRemove())
	{
		for (Chain* pChain : m_pChains_forward_list)
			pChain->SetM_bIsDrop(true);

		GetEngine()->DecM_iUnderWaterMineCounter();

		if (GetEngine()->GetM_iUnderWaterMineCounter() == 0)
			GetEngine()->SetM_bMineLaid(false);
		
		GameObject::SetIsToRemove(bIsToRemove);
	}
}