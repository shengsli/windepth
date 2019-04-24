#pragma once
#include "AllyWeapon.h"
#include "BattleShip.h"
class DepthCharge :
	public AllyWeapon
{
private:
	BattleShip* m_pShip;
public:
	DepthCharge(Psysl5Engine* pEngine, BattleShip* pShip);
	~DepthCharge();
	void Draw();
	void DoUpdate(int iCurrentTime);
};

