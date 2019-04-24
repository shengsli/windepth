#pragma once
#include "Item.h"
class LifeItem :
	public Item
{
public:
	LifeItem(Psysl5Engine* pEngine, Submarine* pSubmarine);
	~LifeItem();
	void Trigger();
};

