#include "header.h"
#include "templates.h"

#include "Explosion.h"
#include "Submarine.h"


Explosion::Explosion(Psysl5Engine* pEngine, GameObject* pGameObject, int iExplosionFrames)
	: GameObject(pEngine)
	, m_pGameObject(pGameObject)
	, m_iExplosionOffset(0)
	, m_iExplosionFrames(iExplosionFrames)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imExplosionSpriteSheet())).GetWidth() / m_iExplosionFrames; /* 64 pixels */
	m_iDrawHeight = (*(GetEngine()->GetpM_imExplosionSpriteSheet())).GetHeight(); /* 64 pixels */
	m_iCurrentScreenX = m_iPreviousScreenX = m_pGameObject->GetXCentre();
	m_iCurrentScreenY = m_iPreviousScreenY = m_pGameObject->GetYCentre();
	SetVisible(true);
}


Explosion::~Explosion()
{
}


void Explosion::Draw()
{
	/* explosion animation */
	(*(GetEngine()->GetpM_imExplosionSpriteSheet())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		m_iDrawWidth*m_iExplosionOffset, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iDrawWidth,
		m_iDrawHeight);
	m_iExplosionOffset++;
	if (m_iExplosionOffset == m_iExplosionFrames)
		SetIsToRemove(true);
}


