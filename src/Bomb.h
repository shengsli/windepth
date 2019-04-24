#pragma once
#include "AllyWeapon.h"
#include "Bomber.h"
class Bomb :
	public AllyWeapon
{
public:
	Bomb(Psysl5Engine* pEngine, Bomber* pBomber);
	~Bomb();
	void Draw();
	void DoUpdate(int iCurrentTime);
};

