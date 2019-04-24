#pragma once
#include "GameObject.h"
class Explosion :
	public GameObject
{
protected:
	GameObject* m_pGameObject;
	int m_iExplosionOffset;
	const int m_iExplosionFrames;
public:
	Explosion(Psysl5Engine* pEngine, GameObject* pGameObject, int iExplosionFrames);
	~Explosion();
	void Draw();
};

