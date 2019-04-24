#include "header.h"
#include "templates.h"

#include "Bomber.h"
#include "Bomb.h"


Bomber::Bomber(Psysl5Engine* pEngine)
	: GameObject(pEngine)
	, m_iCoolingTime(500)
	, m_bIsTileSet(false)
	, m_iTileOffset(0)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imBomber())).GetWidth();
	m_iDrawHeight = (*(GetEngine()->GetpM_imBomber())).GetHeight();
	m_iCurrentScreenX = m_iPreviousScreenX = -m_iDrawWidth;
	m_iCurrentScreenY = m_iPreviousScreenY = 0;
	m_iVel = 3;
	SetVisible(true);
}


Bomber::~Bomber()
{
}


void Bomber::Draw()
{
	(*(GetEngine()->GetpM_imBomber())).RenderImageWithMask(
		GetEngine()->GetForeground(),
		0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY,
		(*(GetEngine()->GetpM_imBomber())).GetWidth(), (*(GetEngine()->GetpM_imBomber())).GetHeight());
	StoreLastScreenPositionForUndraw();
}


void Bomber::DoUpdate(int iCurrentTime)
{
	int iMapX = m_iCurrentScreenX / GetEngine()->GetM_oTiles().GetM_iTileWidth();

	if (iMapX >= 0 && iMapX < GetEngine()->GetM_oTiles().GetM_iNumOfCols() && m_iCurrentScreenX)
	{
		m_bIsTileSet = false;
		if (m_iCurrentScreenX >  GetEngine()->GetM_oTiles().GetM_iTileWidth() * m_iTileOffset
			&& m_iCurrentScreenX < GetEngine()->GetM_oTiles().GetM_iTileWidth() * (m_iTileOffset + 1))
		{
			if (!m_bIsTileSet)
			{
				GetEngine()->GetM_oTiles().SetValue(iMapX, 0, rand() % 100);
				GetEngine()->SetupBackgroundBuffer();
				GetEngine()->Redraw(true);
				m_bIsTileSet = true;
				m_iTileOffset++;
			}
		}
	}

	m_iCurrentScreenX += m_iVel;
	BorderCollision();
	DropBombs(iCurrentTime);
	RedrawObjects();
}


void Bomber::DropABomb()
{
	GetEngine()->StoreObjectInVector(new Bomb(GetEngine(), this));
}


void Bomber::DropBombs(int iCurrentTime)
{
	/* avoid droping any bomb out of windows */
	if (GetXCentre() < (*(GetEngine()->GetpM_imBomb())).GetWidth() 
		|| GetXCentre() > GetEngine()->GetScreenWidth() - (*(GetEngine()->GetpM_imBomb())).GetWidth())
		return;

	if (m_iCurrentScreenX % 90 == 1)
		DropABomb();
}


void Bomber::BorderCollision()
{
	if (m_iCurrentScreenX > GetEngine()->GetScreenWidth())
	{
		GetEngine()->SetM_bBombing(false);
		SetIsToRemove(true);
	}
}