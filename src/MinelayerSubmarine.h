#pragma once
#include "Submarine.h"
class MinelayerSubmarine :
	public Submarine
{
public:
	MinelayerSubmarine(Psysl5Engine* pEngine, int iCurrentScreenY
		, int iVel, int iCoolingTime);
	~MinelayerSubmarine();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void LayMine();
	void LayMineRandomly(int iCurrentTime);
};

