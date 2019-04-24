#pragma once
#include "SubLaunchedObject.h"
class SubWeapon :
	public SubLaunchedObject
{
public:
	SubWeapon(Psysl5Engine* pEngine, Submarine* pSubmarine);
	~SubWeapon();
	void Explode();
};

