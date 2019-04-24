#include "header.h"
#include "templates.h"

#include "Missile.h"
#include <algorithm>
#include "BattleShip.h"

using namespace std;


Missile::Missile(Psysl5Engine* pEngine, Submarine* pSubmarine)
	: SubWeapon(pEngine, pSubmarine)
	, m_dTheta(0)
	, m_bIsTurn(true)
	, m_bIsInc(true)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = 5;
	m_iDrawHeight = 20;
	m_iCurrentScreenX = m_iPreviousScreenX = m_pSubmarine->GetXCentre() - m_iDrawWidth / 2;
	m_iCurrentScreenY = m_iPreviousScreenY = m_pSubmarine->GetYCentre();
	m_iVel = 2;

#if 1
	/* The missile tracks the battleship: launch a missile to left or right */
	BattleShip* pShip = dynamic_cast<BattleShip*>(GetEngine()->GetM_pGameObjects_vector()[0]);
	if (pShip != nullptr)
	{
		if (pShip->GetXCentre() < m_iCurrentScreenX)
			m_iHorizontalDirection = 1;
		else if (pShip->GetXCentre() > m_iCurrentScreenX)
			m_iHorizontalDirection = -1;
	}
#endif

	SetVisible(true);
}


Missile::~Missile()
{
}


void Missile::Draw()
{
	GetEngine()->DrawScreenPolygonWithAngle(
		GetXCentre(), GetYCentre(),
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth / 2, m_iCurrentScreenY - m_iDrawWidth / 2,
		m_iCurrentScreenX + m_iDrawWidth - 1, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1, m_iCurrentScreenY + m_iDrawHeight - 1,
		m_iCurrentScreenX, m_iCurrentScreenY + m_iDrawHeight - 1,
		0xC0C0C0,
		m_dTheta
	);
	StoreLastScreenPositionForUndraw();
}


void Missile::DoUpdate(int iCurrentTime)
{
	m_iCurrentScreenX += m_iVel*sin(m_dTheta);
	m_iCurrentScreenY -= m_iVel*cos(m_dTheta);

	if (m_bIsTurn)
	{
		if (m_bIsInc)
		{
			m_dTheta -= m_iHorizontalDirection * M_PI / 180;
			if (abs(m_dTheta) > M_PI / 3)
			{
				m_bIsInc = false;
			}
		}
		else
		{
			m_dTheta += m_iHorizontalDirection * M_PI / 180;
			if (abs(m_dTheta) < M_PI / 180)
			{
				m_dTheta = 0;
				m_bIsTurn = false;
			}
		}
	}

	RedrawObjects();
	if (m_iCurrentScreenY < -m_iDrawHeight)
		SetIsToRemove(true);
}


void Missile::RedrawBackground()
{
	m_pEngine->CopyBackgroundPixels(
		GetValidX(GetXCentre() - m_iDrawHeight / 2 - m_iDrawWidth / 2),
		GetValidY(GetYCentre() - m_iDrawHeight / 2 - m_iDrawWidth / 2),
		GetValidWidth(GetXCentre() - m_iDrawHeight / 2 - m_iDrawWidth / 2, m_iDrawHeight + m_iDrawWidth),
		GetValidHeight(GetYCentre() - m_iDrawHeight / 2 - m_iDrawWidth / 2, m_iDrawHeight + m_iDrawWidth)
		);
}


