#include "header.h"
#include "templates.h"

#include "Submarine.h"
#include "DepthCharge.h"
#include "LifeItem.h"
#include "AirSupportItem.h"
#include "UnderWaterSupportItem.h"
#include "SubExplosion.h"


Submarine::Submarine(Psysl5Engine* pEngine, int iCurrentScreenY
	, int iVel, int iCoolingTime, unsigned int iScore)
	: GameObject(pEngine)
	, m_iCoolingTime(iCoolingTime)
	, m_iScore(iScore)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = 92;
	m_iDrawHeight = 23;
	if (iVel > 0)
		m_iCurrentScreenX = m_iPreviousScreenX = -m_iDrawWidth;
	else if (iVel < 0)
		m_iCurrentScreenX = m_iPreviousScreenX = GetEngine()->GetScreenWidth();
	else
		puts("A submarine with velocity 0 is created");
	m_iCurrentScreenY = m_iPreviousScreenY = iCurrentScreenY;
	m_iVel = iVel;
	SetVisible(true);
}


Submarine::~Submarine()
{
}


void Submarine::Draw()
{
	GetEngine()->DrawScreenRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0xff0000
		);
	StoreLastScreenPositionForUndraw();
}


void Submarine::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenX += m_iVel;
	BorderCollision();
	RedrawObjects();
}


void Submarine::LaunchLifeItem()
{
	GetEngine()->StoreObjectInVector(new LifeItem(GetEngine(), this));
}


void Submarine::LaunchAirSupportItem()
{
	GetEngine()->StoreObjectInVector(new AirSupportItem(GetEngine(), this));
}


void Submarine::LaunchUnderWaterSupportItem()
{
	GetEngine()->StoreObjectInVector(new UnderWaterSupportItem(GetEngine(), this));
}


void Submarine::BorderCollision()
{
	if ((m_iCurrentScreenX < -m_iDrawWidth) || (m_iCurrentScreenX > GetEngine()->GetScreenWidth()))
		SetIsToRemove(true);
}


void Submarine::Explode(int iExplosionIdx)
{
	GetEngine()->StoreObjectInVector(new SubExplosion(GetEngine(), this, iExplosionIdx));
}


void Submarine::SetIsToRemove(bool bIsToRemove)
{
	if (!IsToRemove())
	{
		GetEngine()->DecM_iSubCounter();
		GameObject::SetIsToRemove(bIsToRemove);
	}
}