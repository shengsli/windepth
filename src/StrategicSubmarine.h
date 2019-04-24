#pragma once
#include "Submarine.h"
class StrategicSubmarine :
	public Submarine
{
public:
	StrategicSubmarine(Psysl5Engine* pEngine, int iCurrentScreenY
		, int iVel, int iCoolingTime);
	~StrategicSubmarine();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void SetIsToRemove(bool bIsToRemove);
	void LaunchItemAtPercent(int iPercent);
};

