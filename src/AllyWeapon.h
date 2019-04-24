#pragma once
#include "GameObject.h"
class AllyWeapon :
	public GameObject
{
public:
	AllyWeapon(Psysl5Engine* pEngine);
	~AllyWeapon();
	void SubmarineCollision();
	void BorderCollision();
};

