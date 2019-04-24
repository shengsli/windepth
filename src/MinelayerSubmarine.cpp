#include "header.h"
#include "templates.h"

#include "MinelayerSubmarine.h"
#include "Mine.h"


MinelayerSubmarine::MinelayerSubmarine(Psysl5Engine* pEngine
	, int iCurrentScreenY, int iVel, int iCoolingTime)
	: Submarine(pEngine, iCurrentScreenY, iVel, iCoolingTime, 100)
{
	m_iDrawWidth = (*(GetEngine()->GetpM_imMinelayerSubmarine(m_iVel))).GetWidth(); /* width = 92 pixels*/
	m_iDrawHeight = (*(GetEngine()->GetpM_imMinelayerSubmarine(m_iVel))).GetHeight(); /* height = 23 pixels */
}


MinelayerSubmarine::~MinelayerSubmarine()
{
}


void MinelayerSubmarine::Draw()
{
	(*(GetEngine()->GetpM_imMinelayerSubmarine(m_iVel))).RenderImageWithMask(GetEngine()->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		(*(GetEngine()->GetpM_imMinelayerSubmarine(m_iVel))).GetWidth(), (*(GetEngine()->GetpM_imMinelayerSubmarine(m_iVel))).GetHeight());
	StoreLastScreenPositionForUndraw();
}


void MinelayerSubmarine::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenX += m_iVel;
	BorderCollision();
	LayMineRandomly(iCurrentTime);
	RedrawObjects();
}


void MinelayerSubmarine::LayMine()
{
	GetEngine()->StoreObjectInVector(new Mine(GetEngine(), this));
}


void MinelayerSubmarine::LayMineRandomly(int iCurrentTime)
{
	if (iCurrentTime - m_iTimeStamp < m_iCoolingTime)
		return;
	LayMine();
	m_iTimeStamp = iCurrentTime;
	SetM_iCoolingTime(rand() % 1000 + 5000);
}


