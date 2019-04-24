#pragma once
#include "AllyWeapon.h"
#include <forward_list>
class Chain;
class UnderWaterMine :
	public AllyWeapon
{
private:
	const int m_iMinScreenY;
	int m_iChainCounter;
	std::forward_list<Chain*> m_pChains_forward_list;
public:
	UnderWaterMine(Psysl5Engine* pEngine, int iCurrentScreenX, int iMinScreenY);
	~UnderWaterMine();
	void Draw();
	void DoUpdate(int iCurrentTime);
	int GetM_iMinScreenY() { return m_iMinScreenY; }
	void SetIsToRemove(bool bIsToRemove);
};

