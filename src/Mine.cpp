#include "header.h"
#include "templates.h"

#include "Mine.h"


Mine::Mine(Psysl5Engine* pEngine, Submarine* pSubmarine)
	: SubWeapon(pEngine, pSubmarine)
	, m_iCoolingTime(200)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = 16;
	m_iDrawHeight = 16;
	m_iCurrentScreenX = m_iPreviousScreenX = m_pSubmarine->GetXCentre() - m_iDrawWidth / 2;
	m_iCurrentScreenY = m_iPreviousScreenY = m_pSubmarine->GetYCentre();
	m_iVel = 1;
	SetVisible(true);
}


Mine::~Mine()
{
}


void Mine::Draw()
{
	GetEngine()->DrawScreenOval(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0xC0C0C0
		);
	StoreLastScreenPositionForUndraw();
}


void Mine::DoUpdate(int iCurrentTime)
{
	/* rising to water surface */
	if (m_iCurrentScreenY > GetEngine()->GetM_iShallowSeaUpper() - m_iDrawHeight / 2)
	{
		m_iCurrentScreenY -= m_iVel;
		m_iTimeStamp = iCurrentTime;
	}
	/* last for a while then disappear */
	if (m_iCurrentScreenY <= GetEngine()->GetM_iShallowSeaUpper() - m_iDrawHeight / 2)
	{
		if (iCurrentTime - m_iTimeStamp < m_iCoolingTime)
			return;
		SetIsToRemove(true);
	}

	RedrawObjects();
}