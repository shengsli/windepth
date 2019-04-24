#pragma once
#include "GameObject.h"
#include "Psysl5Engine.h"
class Submarine :
	public GameObject
{
protected:
	Psysl5Engine* m_pEngine;
	int m_iTimeStamp;
	int m_iCoolingTime;
	const int m_iScore;
public:
	Submarine(Psysl5Engine* pEngine, int iCurrentScreenY, int iVel, int iCoolingTime, unsigned int iScore);
	~Submarine();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void SetM_iCoolingTime(int iCoolingTime) { m_iCoolingTime = iCoolingTime; }
	void LaunchLifeItem();
	void LaunchAirSupportItem();
	void LaunchUnderWaterSupportItem();
	void LaunchItemAtPercent(int iPercent);
	void BorderCollision();
	int GetM_iScore() { return m_iScore; }
	void Explode(int iExplosionIdx);
	virtual void SetIsToRemove(bool bIsToRemove);
};

