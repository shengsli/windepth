#pragma once
#include "GameObject.h"
#include "Submarine.h"
class SubLaunchedObject :
	public GameObject
{
protected:
	Submarine* m_pSubmarine;
public:
	SubLaunchedObject(Psysl5Engine* pEngine, Submarine* pSubmarine);
	~SubLaunchedObject();
};

