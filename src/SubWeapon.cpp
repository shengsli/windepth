#include "header.h"
#include "templates.h"

#include "SubWeapon.h"
#include "WeaponExplosion.h"


SubWeapon::SubWeapon(Psysl5Engine* pEngine, Submarine* pSubmarine)
	: SubLaunchedObject(pEngine, pSubmarine)
{
}


SubWeapon::~SubWeapon()
{
}


void SubWeapon::Explode()
{
	GetEngine()->StoreObjectInVector(new WeaponExplosion(GetEngine(), this));
}