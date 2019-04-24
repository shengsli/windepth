#include "header.h"
#include "templates.h"

#include "SubExplosion.h"


SubExplosion::SubExplosion(Psysl5Engine* pEngine, Submarine* pSubmarine, int iExplosionIdx)
	: Explosion(pEngine, pSubmarine, 40)
	, m_iExplosionRangeWidth(70)
	, m_iExplosionRangeHeight(35)
	, m_iExplosionIdx(iExplosionIdx+1)
	, m_iSubScore(pSubmarine->GetM_iScore())
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imExplosionSpriteSheet())).GetWidth() / m_iExplosionFrames;
	m_iDrawHeight = (*(GetEngine()->GetpM_imExplosionSpriteSheet())).GetHeight();
	m_iCurrentScreenX = m_iPreviousScreenX = pSubmarine->GetXCentre() - m_iDrawWidth/2;
	m_iCurrentScreenY = m_iPreviousScreenY = pSubmarine->GetYCentre() - m_iDrawHeight/2;
	SetVisible(true);

	GetEngine()->IncM_uiScore(m_iSubScore * m_iExplosionIdx);
}


SubExplosion::~SubExplosion()
{
}


void SubExplosion::DoUpdate(int iCurrentTime)
{
	if (m_iExplosionIdx > 2)
		GetEngine()->ShakeBackground();
	SubmarineCollision();
	m_iExplosionOffset++;
	if (m_iExplosionOffset == m_iExplosionFrames)
		SetIsToRemove(true);
}


void SubExplosion::Draw()
{
	/* explosion animation */
	(*(GetEngine()->GetpM_imExplosionSpriteSheet())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		m_iDrawWidth*m_iExplosionOffset, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iDrawWidth,
		m_iDrawHeight);
}


void SubExplosion::SubmarineCollision()
{
	for (int i = 0; i < GetEngine()->GetM_pGameObjects_vector().size(); i++)
	{
		Submarine* pSubmarine = dynamic_cast<Submarine*>(GetEngine()->GetM_pGameObjects_vector()[i]);
		if (pSubmarine != nullptr)
		{
			if (m_iExplosionOffset < 8 || m_iExplosionOffset > 15) /* 6 - 15 */
				return;

			/* collision */
			if ((abs(this->GetYCentre() - pSubmarine->GetYCentre()) < m_iExplosionRangeHeight)
				&& (abs(this->GetXCentre() - pSubmarine->GetXCentre()) < m_iExplosionRangeWidth))
			{
				pSubmarine->Explode(m_iExplosionIdx);
				pSubmarine->SetIsToRemove(true);
			}
		}
	}
}


void SubExplosion::DrawScore()
{
	char buffer[16];
	sprintf(buffer, "x %d", m_iExplosionIdx);
	GetEngine()->DrawBackgroundString(m_iCurrentScreenX, m_iCurrentScreenY, buffer, 0x000000);
}