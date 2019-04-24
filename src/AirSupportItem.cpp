#include "header.h"
#include "templates.h"

#include "AirSupportItem.h"
#include "BattleShip.h"
#include "Seagull.h"


AirSupportItem::AirSupportItem(Psysl5Engine* pEngine, Submarine* pSubmarine)
	: Item(pEngine, pSubmarine, 0xffff00)
{
}


AirSupportItem::~AirSupportItem()
{
}


void AirSupportItem::DoUpdate(int iCurrentTime)
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
		GetEngine()->SetM_bBombing(false);
		SetIsToRemove(true);
	}

	RedrawObjects();
}


void AirSupportItem::Trigger()
{
	GetEngine()->StoreObjectInVector(new Seagull(GetEngine()));
}