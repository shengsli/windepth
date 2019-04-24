#pragma once
#include "Explosion.h"
#include "BattleShip.h"
class ShipExplosion :
	public Explosion
{
public:
	ShipExplosion(Psysl5Engine* pEngine, BattleShip* pShip);
	~ShipExplosion();
	void Draw();
};

