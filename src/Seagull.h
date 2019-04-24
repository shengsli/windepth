#pragma once
#include "GameObject.h"
class Seagull :
	public GameObject
{
private:
	int m_iExplosionOffset;
	const int m_iExplosionFrames;
public:
	Seagull(Psysl5Engine* pEngine);
	~Seagull();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void BorderCollision();
};

