#pragma once
#include "GameObject.h"
class Bomber :
	public GameObject
{
private:
	int m_iVel;
	int m_iTimeStamp;
	int m_iCoolingTime;
	bool m_bIsTileSet;
	int m_iTileOffset;
public:
	Bomber(Psysl5Engine* pEngine);
	~Bomber();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void DropABomb();
	void DropBombs(int iCurrentTime);
	void BorderCollision();
};

