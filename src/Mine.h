#pragma once
#include "SubWeapon.h"
#include "Submarine.h"
class Mine :
	public SubWeapon
{
private:
	int m_iTimeStamp;
	int m_iCoolingTime;
public:
	Mine(Psysl5Engine* pEngine, Submarine* pSubmarine);
	~Mine();
	void Draw();
	void DoUpdate(int iCurrentTime);
};

