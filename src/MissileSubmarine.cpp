#include "header.h"
#include "templates.h"

#include "MissileSubmarine.h"
#include "Missile.h"


MissileSubmarine::MissileSubmarine(Psysl5Engine* pEngine
	, int iCurrentScreenY, int iVel, int iCoolingTime)
	: Submarine(pEngine, iCurrentScreenY, iVel, iCoolingTime, 200)
{
	m_iDrawWidth = (*(GetEngine()->GetpM_imMissileSubmarine(m_iVel))).GetWidth(); /* width = 92 pixels*/
	m_iDrawHeight = (*(GetEngine()->GetpM_imMissileSubmarine(m_iVel))).GetHeight(); /* height = 23 pixels */
}


MissileSubmarine::~MissileSubmarine()
{
}


void MissileSubmarine::Draw()
{
	(*(GetEngine()->GetpM_imMissileSubmarine(m_iVel))).RenderImageWithMask(
		GetEngine()->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		(*(GetEngine()->GetpM_imMissileSubmarine(m_iVel))).GetWidth(), 
		(*(GetEngine()->GetpM_imMissileSubmarine(m_iVel))).GetHeight());
	StoreLastScreenPositionForUndraw();
}


void MissileSubmarine::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenX += m_iVel;
	BorderCollision();
	LaunchMissileRandomly(iCurrentTime);
	RedrawObjects();
}


void MissileSubmarine::LaunchMissile()
{
	GetEngine()->StoreObjectInVector(new Missile(GetEngine(), this));
}


void MissileSubmarine::LaunchMissileRandomly(int iCurrentTime)
{
	if (iCurrentTime - m_iTimeStamp < m_iCoolingTime)
		return;
	LaunchMissile();
	m_iTimeStamp = iCurrentTime;
	SetM_iCoolingTime(rand() % 1000 + 3000);
}