#include "header.h"
#include "templates.h"

#include "Seagull.h"
#include "Bomber.h"


Seagull::Seagull(Psysl5Engine* pEngine)
	: GameObject(pEngine)
	, m_iExplosionOffset(0)
	, m_iExplosionFrames(9)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetM_imSeagullSpritesheet())).GetWidth() / m_iExplosionFrames;
	m_iDrawHeight = (*(GetEngine()->GetM_imSeagullSpritesheet())).GetHeight();
	m_iCurrentScreenX = m_iPreviousScreenX = -m_iDrawWidth;
	m_iCurrentScreenY = m_iPreviousScreenY = 0;
	m_iVel = 4;
	SetVisible(true);
}


Seagull::~Seagull()
{
}


void Seagull::Draw()
{
	/* explosion animation */
	(*(GetEngine()->GetM_imSeagullSpritesheet())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		m_iDrawWidth*m_iExplosionOffset, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iDrawWidth,
		m_iDrawHeight);
	m_iExplosionOffset++;
	if (m_iExplosionOffset == m_iExplosionFrames)
		m_iExplosionOffset = 0;
	StoreLastScreenPositionForUndraw();
}


void Seagull::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenX += m_iVel;
	BorderCollision();
	RedrawObjects();
}


void Seagull::BorderCollision()
{
	if (m_iCurrentScreenX > GetEngine()->GetScreenWidth())
	{
		GetEngine()->StoreObjectInVector(new Bomber(GetEngine()));
		SetIsToRemove(true);
	}
}