#include "header.h"
#include "templates.h"

#include "LifeItem.h"
#include "BattleShip.h"


LifeItem::LifeItem(Psysl5Engine* pEngine, Submarine* pSubmarine)
	: Item(pEngine, pSubmarine, 0x00ff00)
{
}


LifeItem::~LifeItem()
{
}

void LifeItem::Trigger()
{
	GetEngine()->IncM_iLives();
}
