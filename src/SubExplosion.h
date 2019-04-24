#pragma once
#include "Explosion.h"
#include "Submarine.h"
class SubExplosion :
	public Explosion
{
private:
	const int m_iExplosionRangeWidth;
	const int m_iExplosionRangeHeight;
	int m_iExplosionIdx;
	int m_iSubScore;
public:
	SubExplosion(Psysl5Engine* pEngine, Submarine* pShip, int iIdx);
	~SubExplosion();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void SubmarineCollision();
	void DrawScore();
};

