#include "header.h"
#include "templates.h"

#include "WeaponExplosion.h"


WeaponExplosion::WeaponExplosion(Psysl5Engine* pEngine, SubWeapon* pWeapon)
	: Explosion(pEngine, pWeapon, 16)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imWeaponExplosionSpriteSheet())).GetWidth() / 4;
	m_iDrawHeight = (*(GetEngine()->GetpM_imWeaponExplosionSpriteSheet())).GetHeight() / 4;
	m_iCurrentScreenX = m_iPreviousScreenX = pWeapon->GetXCentre() - m_iDrawWidth / 2;
	m_iCurrentScreenY = m_iPreviousScreenY = pWeapon->GetYCentre() - m_iDrawHeight / 2;
	SetVisible(true);
}


WeaponExplosion::~WeaponExplosion()
{
}


void WeaponExplosion::Draw()
{
	/* explosion animation */
	(*(GetEngine()->GetpM_imWeaponExplosionSpriteSheet())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		m_iDrawWidth * (m_iExplosionOffset % 4), m_iDrawHeight * (m_iExplosionOffset / 4),
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iDrawWidth,
		m_iDrawHeight);
	m_iExplosionOffset++;
	if (m_iExplosionOffset == m_iExplosionFrames)
		SetIsToRemove(true);
}


