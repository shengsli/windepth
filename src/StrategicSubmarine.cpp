#include "header.h"
#include "templates.h"

#include "StrategicSubmarine.h"


StrategicSubmarine::StrategicSubmarine(Psysl5Engine* pEngine
	, int iCurrentScreenY, int iVel, int iCoolingTime)
	: Submarine(pEngine, iCurrentScreenY, iVel, iCoolingTime, 1000)
{
	m_iDrawWidth = (*(GetEngine()->GetpM_imStrategicSubmarine(m_iVel))).GetWidth(); /* width = 92 pixels*/
	m_iDrawHeight = (*(GetEngine()->GetpM_imStrategicSubmarine(m_iVel))).GetHeight(); /* height = 23 pixels */
}


StrategicSubmarine::~StrategicSubmarine()
{
}


void StrategicSubmarine::Draw()
{
	(*(GetEngine()->GetpM_imStrategicSubmarine(m_iVel))).RenderImageWithMask(
		GetEngine()->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		(*(GetEngine()->GetpM_imStrategicSubmarine(m_iVel))).GetWidth(),
		(*(GetEngine()->GetpM_imStrategicSubmarine(m_iVel))).GetHeight());
	StoreLastScreenPositionForUndraw();
}


void StrategicSubmarine::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenX += m_iVel;
	BorderCollision();
	RedrawObjects();
}


void StrategicSubmarine::SetIsToRemove(bool bIsToRemove)
{
	if (!IsToRemove())
	{
		GetEngine()->DecM_iSubCounter();
		LaunchItemAtPercent(100);
		GameObject::SetIsToRemove(bIsToRemove);
	}
}


void StrategicSubmarine::LaunchItemAtPercent(int iPercent)
{
	if (GetXCentre() < 15 || GetXCentre() > GetEngine()->GetScreenWidth() - 15)
		return; /* avoid launch item at borders (hard coded) */
	if (rand() % 100 > iPercent)
		return;

	switch (rand()%3)
	{
	case 0:
		LaunchLifeItem();
		break;
	case 1:
		if (!GetEngine()->IsM_bBombing())
		{
			LaunchAirSupportItem();
			GetEngine()->SetM_bBombing(true);
		}
		break;
	case 2:
		if (!GetEngine()->IsM_bMineLaid() && GetEngine()->GetM_iUnderWaterMineCounter() == 0)
		{
			LaunchUnderWaterSupportItem();
			GetEngine()->SetM_bMineLaid(true);
		}
		break;
	default:
		LaunchLifeItem();
		break;
	}
}