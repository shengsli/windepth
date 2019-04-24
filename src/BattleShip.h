#pragma once
#include "GameObject.h"
#include "Psysl5Engine.h"
class BattleShip :
	public GameObject
{
private:
	int m_iTimeStamp;
	int m_iCoolingTime;
	int m_iReloadTime;
	int m_iBlinkTimeStamp;
	int m_iBlinkPeriod;
	int m_iBlinkCounter;
	const int m_iNumOfBlinks;
	bool m_bIsBlink;
	/* health bar */
	const int m_iHealthBarHeight;
	const int m_iHealthBarWidth;
	const int m_iHeightDiff;
	unsigned int m_uiColour;
	const int m_iHealthBlockWidth;
public:
	BattleShip(Psysl5Engine* pEngine);
	~BattleShip();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void ReloadDepthCharge();
	void ReloadDepthChargeWithCoolingTime(int iCurrentTime);
	void SubWeaponCollision();
	void Explode();
	void Blink(int iCurrentTime);
	bool IsBlink() { return m_bIsBlink; }
	void SetIsBlink(bool bIsBlink) { m_bIsBlink = bIsBlink; }
	void RedrawBackground();
};

