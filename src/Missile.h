#pragma once
#include "SubWeapon.h"
#include "Submarine.h"
class Missile :
	public SubWeapon
{
private:
	double m_dTheta;
	//int m_iInitialX;
	//int m_iInitialY;
	bool m_bIsTurn;
	bool m_bIsInc;
	int m_iHorizontalDirection;
public:
	Missile(Psysl5Engine* pEngine, Submarine* pSubmarine);
	~Missile();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void RedrawBackground();

#if 0
	double GetMinX();
	double GetMaxX();
	double GetMinY();
	double GetMaxY();
	double GetWidthToCopy();
	double GetHeightToCopy();
	double GetValidWidth();
	double GetValidHeight();
#endif
};

