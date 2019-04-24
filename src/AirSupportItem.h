#pragma once
#include "Item.h"
class AirSupportItem :
	public Item
{
public:
	AirSupportItem(Psysl5Engine* pEngine, Submarine* pSubmarine);
	~AirSupportItem();
	void DoUpdate(int iCurrentTime);
	void Trigger();
};

