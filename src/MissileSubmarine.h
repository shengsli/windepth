#pragma once
#include "Submarine.h"
class MissileSubmarine :
	public Submarine
{
public:
	MissileSubmarine(Psysl5Engine* pEngine, int iCurrentScreenY
		, int iVel, int iCoolingTime);
	~MissileSubmarine();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void LaunchMissile();
	void LaunchMissileRandomly(int iCurrentTime);
};

