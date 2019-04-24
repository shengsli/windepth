#pragma once
#include "Psysl5Engine.h"
class SubmarineCommander
{
private:
	Psysl5Engine* m_pEngine;
	const int m_iNumOfWaterLevels;
	unsigned int m_uiNumOfSub;
public:
	SubmarineCommander(Psysl5Engine* pEngine);
	~SubmarineCommander();
	inline Psysl5Engine* GetEngine() { return m_pEngine; }
	void SendSubmarine();
	void SendMinelayerSubmarine(int iWaterLevel, int iVel);
	void SendMissileSubmarine(int iWaterLevel, int iVel);
	void SendStrategicSubmarine(int iWaterLevel, int iVel);
	int WaterLevel2Pixel(int iLevel);
	unsigned int GetM_uiNumOfSub() { return m_uiNumOfSub; }
	void IncM_uiNumOfSub(unsigned int uiNumOfSub) 
	{ 
		if (m_uiNumOfSub > 50)
			m_uiNumOfSub = 50;
		else
			m_uiNumOfSub += uiNumOfSub; 
	}
};

