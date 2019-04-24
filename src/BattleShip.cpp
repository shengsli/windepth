#include "header.h"
#include "templates.h"

#include "BattleShip.h"
#include "SubWeapon.h"
#include "DepthCharge.h"
#include "Item.h"
#include "ShipExplosion.h"
#include <Windows.h>


BattleShip::BattleShip(Psysl5Engine* pEngine)
	: GameObject(pEngine)
	, m_iCoolingTime(500)
	, m_iReloadTime(600)
	, m_iBlinkPeriod(200)
	, m_iBlinkCounter(0)
	, m_iNumOfBlinks(3)
	, m_bIsBlink(false)
	, m_iHealthBarHeight(6)
	, m_iHealthBarWidth(90)
	, m_iHeightDiff(2)
	, m_uiColour(0x00ff00)
	, m_iHealthBlockWidth(m_iHealthBarWidth / m_iHealthBarHeight)
{
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iDrawWidth = (*(GetEngine()->GetpM_imBattleShip())).GetWidth(); /* 145 pixels */
	m_iDrawHeight = (*(GetEngine()->GetpM_imBattleShip())).GetHeight(); /* 42 pixels */
	m_iCurrentScreenX = m_iPreviousScreenX = (GetEngine()->GetScreenWidth() - m_iDrawWidth) / 2;
	m_iCurrentScreenY = m_iPreviousScreenY = GetEngine()->GetScreenHeight()/6 - m_iDrawHeight + 5;
	SetVisible(true);
}


BattleShip::~BattleShip()
{
}


void BattleShip::Draw()
{
	/* draw health bar */
	if (GetEngine()->GetM_iLives() < GetEngine()->GetM_iMaxLives())
	{
		if (GetEngine()->GetM_iLives() > GetEngine()->GetM_iMaxLives() * 2 / 3)
			m_uiColour = 0x00ff00;
		else if (GetEngine()->GetM_iLives() > GetEngine()->GetM_iMaxLives() / 3)
			m_uiColour = 0xffff00;
		else
			m_uiColour = 0xff0000;
		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + (m_iDrawWidth - m_iHealthBarWidth) / 2 - m_iHeightDiff,
			m_iCurrentScreenY - m_iHealthBarHeight - m_iHeightDiff,
			m_iCurrentScreenX + (m_iDrawWidth - m_iHealthBarWidth) / 2 + m_iHealthBarWidth + m_iHeightDiff - 1,
			m_iCurrentScreenY + m_iHeightDiff - 1,
			0x000000);
		for (int iOffset = 0; iOffset < GetEngine()->GetM_iLives(); iOffset++)
		{
			GetEngine()->DrawScreenRectangle(
				m_iCurrentScreenX + (m_iDrawWidth - m_iHealthBarWidth) / 2 + iOffset * m_iHealthBlockWidth,
				m_iCurrentScreenY - m_iHealthBarHeight,
				m_iCurrentScreenX + (m_iDrawWidth - m_iHealthBarWidth) / 2 + m_iHealthBarWidth / GetEngine()->GetM_iMaxLives() - 1 + iOffset * m_iHealthBlockWidth - 2,
				m_iCurrentScreenY - 1,
				m_uiColour);
		}
	}
	/* draw depth charge ammo */
	for (int iAmmoIdx = 0; iAmmoIdx < GetEngine()->GetM_iDepthChargeAmmo(); iAmmoIdx++)
	{
		(*(GetEngine()->GetpM_imOilDrum())).RenderImageWithMask(
			GetEngine()->GetForeground(),
			0, 0,
			m_iCurrentScreenX + 10 + iAmmoIdx * ((*(GetEngine()->GetpM_imOilDrum())).GetWidth() - 15), 
			m_iCurrentScreenY - 35,
			(*(GetEngine()->GetpM_imOilDrum())).GetWidth(),
			(*(GetEngine()->GetpM_imOilDrum())).GetHeight());
	}
	/* draw ship */
	if (IsVisible())
	{
		(*(GetEngine()->GetpM_imBattleShip())).RenderImageWithMask(
			GetEngine()->GetForeground(),
			0, 0,
			m_iCurrentScreenX, m_iCurrentScreenY,
			(*(GetEngine()->GetpM_imBattleShip())).GetWidth(),
			(*(GetEngine()->GetpM_imBattleShip())).GetHeight());
	}
	StoreLastScreenPositionForUndraw();
}


void BattleShip::DoUpdate(int iCurrentTime)
{
	/* blink and temporarily invincible */
	if (IsBlink())
		Blink(iCurrentTime);
	/* keyboard input */
	if (GetEngine()->IsKeyPressed(SDLK_LEFT) || GetEngine()->IsKeyPressed(SDLK_a))
		m_iCurrentScreenX -= 4;
	if (GetEngine()->IsKeyPressed(SDLK_RIGHT) || GetEngine()->IsKeyPressed(SDLK_d))
		m_iCurrentScreenX += 4;
	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenX > GetEngine()->GetScreenWidth() - m_iDrawWidth)
		m_iCurrentScreenX = GetEngine()->GetScreenWidth() - m_iDrawWidth;
	if (GetEngine()->IsKeyPressed(SDLK_SPACE) || GetEngine()->IsMouseDown())
	{
		if (iCurrentTime - m_iTimeStamp < m_iCoolingTime)
			return;
		if (GetEngine()->GetM_iDepthChargeAmmo() > 0)
		{
			GetEngine()->StoreObjectInVector(new DepthCharge(GetEngine(), this));
			GetEngine()->DecM_iDepthChargeAmmo();
			m_iTimeStamp = iCurrentTime;
		}
	}

	SubWeaponCollision();
	ReloadDepthChargeWithCoolingTime(iCurrentTime);
	RedrawObjects();
}


void BattleShip::ReloadDepthCharge()
{
	GetEngine()->IncM_iDepthChargeAmmo();
}


void BattleShip::ReloadDepthChargeWithCoolingTime(int iCurrentTime)
{
	if (GetEngine()->GetM_iDepthChargeAmmo() < 5) {
		if (iCurrentTime - m_iTimeStamp < m_iReloadTime)
			return;
		ReloadDepthCharge();
		m_iTimeStamp = iCurrentTime;
	}
}


void BattleShip::SubWeaponCollision()
{
	for (int i = 0; i < GetEngine()->GetM_pGameObjects_vector().size(); i++)
	{
		SubWeapon* pSubWeapon = dynamic_cast<SubWeapon*>(GetEngine()->GetM_pGameObjects_vector()[i]);
		if (pSubWeapon != nullptr)
		{
			/* first collision below */
			if (pSubWeapon->GetYCentre() < 120)
			{
				/* second collision below */
				if ((abs(this->GetYCentre() - pSubWeapon->GetYCentre())
					< (this->GetM_iDrawHeight() + pSubWeapon->GetM_iDrawHeight()) / 2)
					&& (abs(this->GetXCentre() - pSubWeapon->GetXCentre())
					< (this->GetM_iDrawWidth() + pSubWeapon->GetM_iDrawWidth()) / 2))
				{
					/* invincible mode for cheating below */
					if (!GetEngine()->m_bInvincible)
					{
						if (!IsBlink())
						{
							GetEngine()->DecM_iLives();
							SetIsBlink(true);
						}
					}
					/* invincible mode for cheating above */
					pSubWeapon->Explode();
					pSubWeapon->SetIsToRemove(true);
					if (GetEngine()->GetM_iLives() == 0)
					{
						Explode();
						SetIsToRemove(true);
					}
				}
				/* second collision above */
			}
			/* first collision above */
		}
		else
		{
			Item* pItem = dynamic_cast<Item*>(GetEngine()->GetM_pGameObjects_vector()[i]);
			if (pItem != nullptr)
			{
				/* first collision below */
				if (pItem->GetYCentre() < 120)
				{
					/* second collision below */
					if ((abs(this->GetYCentre() - pItem->GetYCentre())
						< (this->GetM_iDrawHeight() + pItem->GetM_iDrawHeight()) / 2)
						&& (abs(this->GetXCentre() - pItem->GetXCentre())
						< (this->GetM_iDrawWidth() + pItem->GetM_iDrawWidth()) / 2))
					{
						pItem->Trigger();
						pItem->SetIsToRemove(true);
					}
					/* second collision above */
				}
				/* first collision above */
			}
		}
	}
}


void BattleShip::Explode()
{
	GetEngine()->StoreObjectInVector(new ShipExplosion(GetEngine(), this));
}


void BattleShip::Blink(int iCurrentTime)
{
	if (iCurrentTime - m_iBlinkTimeStamp < m_iBlinkPeriod)
		return;
	/* blink and temporarily invincible*/
	SetVisible(!IsVisible());
	if (IsVisible()) m_iBlinkCounter++;
	if (m_iBlinkCounter == m_iNumOfBlinks)
	{
		SetVisible(true);
		m_iBlinkCounter = 0;
		SetIsBlink(false);
	}
	/* blink and temporarily invincible*/
	m_iBlinkTimeStamp = iCurrentTime;
}


void BattleShip::RedrawBackground()
{
	m_pEngine->CopyBackgroundPixels(
		GetValidX(m_iPreviousScreenX + m_iStartDrawPosX),
		GetValidY(m_iPreviousScreenY + m_iStartDrawPosY - m_iHealthBarHeight - m_iHeightDiff - 35),
		GetValidWidth(m_iPreviousScreenX + m_iStartDrawPosX, m_iDrawWidth + m_iHealthBarHeight),
		GetValidHeight(m_iPreviousScreenY + m_iStartDrawPosY - m_iHealthBarHeight - 35, m_iDrawHeight + m_iHealthBarHeight + m_iHeightDiff + 35)
		);
}
