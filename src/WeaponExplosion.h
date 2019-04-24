#pragma once
#include "Explosion.h"
#include "SubWeapon.h"
class WeaponExplosion :
	public Explosion
{
public:
	WeaponExplosion(Psysl5Engine* pEngine, SubWeapon* pGameObject);
	~WeaponExplosion();
	void Draw();
};

