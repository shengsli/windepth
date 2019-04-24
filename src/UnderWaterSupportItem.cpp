#include "header.h"
#include "templates.h"

#include "UnderWaterSupportItem.h"
#include "UnderWaterMine.h"


UnderWaterSupportItem::UnderWaterSupportItem(Psysl5Engine* pEngine, Submarine* pSubmarine)
	: Item(pEngine, pSubmarine, 0x0000ff)
{
}


UnderWaterSupportItem::~UnderWaterSupportItem()
{
}


void UnderWaterSupportItem::DoUpdate(int iCurrentTime)
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
		if (iCurrentTime - m_iTimeStamp > m_iCoolingTime / 2)
			Blink(iCurrentTime);
		if (iCurrentTime - m_iTimeStamp < m_iCoolingTime)
			return;
		GetEngine()->SetM_bMineLaid(false);
		SetIsToRemove(true);
	}

	RedrawObjects();
}


void UnderWaterSupportItem::Trigger()
{
	for (int i = 1; i < 8; i++)
	{
		GetEngine()->IncM_iUnderWaterMineCounter();
		GetEngine()->StoreObjectInVector(
			new UnderWaterMine(
			GetEngine()
			, i * GetEngine()->GetScreenWidth() / 8 - (*(GetEngine()->GetpM_imUnderWaterMine())).GetWidth()/2
			, GetEngine()->GetScreenHeight() * (i%2 + 8) / 12));
	}
}