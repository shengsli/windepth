#include "header.h"
#include "templates.h"

#include "Item.h"


Item::Item(Psysl5Engine* pEngine, Submarine* pSubmarine, unsigned int uiColour)
	: SubLaunchedObject(pEngine, pSubmarine)
	, m_iCoolingTime(2000)
	, m_iBlinkPeriod(200)
	, m_uiColour(uiColour)
	, m_iBlink(1)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = 10;
	m_iDrawHeight = 10;
	m_iCurrentScreenX = m_iPreviousScreenX = m_pSubmarine->GetXCentre() - m_iDrawWidth / 2;
	m_iCurrentScreenY = m_iPreviousScreenY = m_pSubmarine->GetYCentre();
	m_iVel = 2;
}


Item::~Item()
{
}


void Item::Draw()
{
	GetEngine()->DrawScreenPolygon(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth + m_iDrawWidth / 2 - 1, m_iCurrentScreenY + m_iDrawHeight / 2 - 1,
		m_iCurrentScreenX + m_iDrawWidth - 1, m_iCurrentScreenY + m_iDrawHeight - 1,
		m_iCurrentScreenX, m_iCurrentScreenY + m_iDrawHeight - 1,
		m_iCurrentScreenX - m_iDrawWidth / 2, m_iCurrentScreenY + m_iDrawHeight / 2 - 1,
		m_uiColour
		);
	StoreLastScreenPositionForUndraw();
}


void Item::DoUpdate(int iCurrentTime)
{
	/* rising to water surface */
	if (m_iCurrentScreenY > GetEngine()->GetM_iShallowSeaUpper() - m_iDrawHeight / 2)
	{
		m_iCurrentScreenY -= m_iVel;
		m_iTimeStamp = iCurrentTime;
	}	
	/* last for a while then disappear */
	if (m_iCurrentScreenY <= GetEngine()->GetM_iShallowSeaUpper())
	{
		if (iCurrentTime - m_iTimeStamp > m_iCoolingTime/2)
			Blink(iCurrentTime);
		if (iCurrentTime - m_iTimeStamp < m_iCoolingTime)
			return;
		SetIsToRemove(true);
	}

	RedrawObjects();
}


void Item::RedrawBackground()
{
	m_pEngine->CopyBackgroundPixels(
		GetValidX(m_iPreviousScreenX + m_iStartDrawPosX - m_iDrawWidth/2),
		GetValidY(m_iPreviousScreenY + m_iStartDrawPosY),
		GetValidWidth(m_iPreviousScreenX + m_iStartDrawPosX - m_iDrawWidth / 2, m_iDrawWidth * 2),
		GetValidHeight(m_iPreviousScreenY + m_iStartDrawPosY, m_iDrawHeight * 2)
		);
}


void Item::Blink(int iCurrentTime)
{
	if (iCurrentTime - m_iBlinkTimeStamp < m_iBlinkPeriod)
		return;

	m_iBlink = -m_iBlink;
	m_uiColour += m_iBlink * 0x008800;

	m_iBlinkTimeStamp = iCurrentTime;
}


void Item::Trigger()
{
	/* implement the item's special function in sub-class */
}