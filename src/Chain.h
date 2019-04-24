#pragma once
#include "GameObject.h"
#include "UnderWaterMine.h"
class Chain :
	public GameObject
{
private:
	int m_iOffset;
	int m_iMinScreenY;
	bool m_bIsDrop;
public:
	Chain(Psysl5Engine* pEngine, UnderWaterMine* pUnderWaterMine, int iOffset);
	~Chain();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void BorderCollision();
	void SetM_bIsDrop(bool bIsDrop) { m_bIsDrop = bIsDrop; }
};

