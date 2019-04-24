#include "header.h"
#include "templates.h"

#include "AllyWeapon.h"
#include "Submarine.h"
#include "StrategicSubmarine.h"
#include "Explosion.h"


AllyWeapon::AllyWeapon(Psysl5Engine* pEngine)
	: GameObject(pEngine)
{
}


AllyWeapon::~AllyWeapon()
{
}


void AllyWeapon::SubmarineCollision()
{
	/* loop through the game object pointer vector,
	   cast each pointer to submarine* type */
	for (int i = 0; i < GetEngine()->GetM_pGameObjects_vector().size(); i++)
	{
		Submarine* pSubmarine = dynamic_cast<Submarine*>(GetEngine()->GetM_pGameObjects_vector()[i]);
		if (pSubmarine != nullptr) /* if nullptr, collision check, else do nothing */
		{
			/* collision below */
			if ((abs(this->GetYCentre() - pSubmarine->GetYCentre())
				< (this->GetM_iDrawHeight() + pSubmarine->GetM_iDrawHeight()) / 2)
				&& (abs(this->GetXCentre() - pSubmarine->GetXCentre())
				< (this->GetM_iDrawWidth() + pSubmarine->GetM_iDrawWidth()) / 2))
			{
				pSubmarine->Explode(0);
				pSubmarine->SetIsToRemove(true);
				SetIsToRemove(true);
			}
			/* collision above */
		}
	}
}


void AllyWeapon::BorderCollision()
{
	/* if the object moves out of the window, SetIsToRemove(true) */
	if (m_iCurrentScreenY >= GetEngine()->GetScreenHeight() + m_iDrawHeight)
		SetIsToRemove(true);
}