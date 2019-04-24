#include "header.h"
#include "templates.h"

#include "ShipExplosion.h"
#include "LoseState.h"


ShipExplosion::ShipExplosion(Psysl5Engine* pEngine, BattleShip* pShip)
	: Explosion(pEngine, pShip, 25)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imNuclearExplosionSpriteSheet())).GetWidth() / 5;
	m_iDrawHeight = (*(GetEngine()->GetpM_imNuclearExplosionSpriteSheet())).GetHeight() / 5;
	m_iCurrentScreenX = m_iPreviousScreenX = pShip->GetXCentre() - m_iDrawWidth/2;
	m_iCurrentScreenY = m_iPreviousScreenY = pShip->GetYCentre() - m_iDrawHeight / 2 - pShip->GetM_iDrawHeight()/2;
	SetVisible(true);
}


ShipExplosion::~ShipExplosion()
{
}


void ShipExplosion::Draw()
{
	/* explosion animation */
	(*(GetEngine()->GetpM_imNuclearExplosionSpriteSheet())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		m_iDrawWidth * ((m_iExplosionOffset/4) % 5), m_iDrawHeight * ((m_iExplosionOffset/4) / 5),
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iDrawWidth,
		m_iDrawHeight);
	m_iExplosionOffset++;
	if (m_iExplosionOffset == m_iExplosionFrames*4)
		GetEngine()->ChangeState(LoseState::Instance());
}