#pragma once
#include "Item.h"
class UnderWaterSupportItem :
	public Item
{
public:
	UnderWaterSupportItem(Psysl5Engine* pEngine, Submarine* pSubmarine);
	~UnderWaterSupportItem();
	void DoUpdate(int iCurrentTime);
	void Trigger();
};

