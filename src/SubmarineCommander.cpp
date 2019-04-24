#include "header.h"
#include "templates.h"

#include "SubmarineCommander.h"
#include "Psysl5Engine.h"
#include "Submarine.h"
#include "MinelayerSubmarine.h"
#include "MissileSubmarine.h"
#include "StrategicSubmarine.h"


SubmarineCommander::SubmarineCommander(Psysl5Engine* pEngine)
	: m_pEngine(pEngine)
	, m_iNumOfWaterLevels(16)
	, m_uiNumOfSub(15)
{
}


SubmarineCommander::~SubmarineCommander()
{
}


void SubmarineCommander::SendSubmarine()
{
	if (GetEngine()->GetM_iSubCounter() < GetM_uiNumOfSub() 
		&& GetEngine()->GetTime() % 50 < 5)
	{
		if (rand() % 100 < 90)
		{
			switch (rand() % 2)
			{
			case 0:
				SendMinelayerSubmarine(rand() % m_iNumOfWaterLevels, (rand() % 2) * 2 - 1);
				break;
			case 1:
				SendMissileSubmarine((rand() % 8) + 7, ((rand() % 2) * 2 - 1) * 2);
				break;
			default:
				SendMinelayerSubmarine(rand() % m_iNumOfWaterLevels, (rand() % 2) * 2 - 1);
				break;
			}
		}
		else
			SendStrategicSubmarine((rand() % 8) + 4, (rand() % 2) * 2 - 1);
	}
}


void SubmarineCommander::SendMinelayerSubmarine(int iWaterLevel, int iVel)
{
	GetEngine()->IncM_iSubCounter();
	GetEngine()->StoreObjectInVector(new MinelayerSubmarine(GetEngine()
		, WaterLevel2Pixel(iWaterLevel), iVel, 300));
}


void SubmarineCommander::SendMissileSubmarine(int iWaterLevel, int iVel)
{
	GetEngine()->IncM_iSubCounter();
	GetEngine()->StoreObjectInVector(new MissileSubmarine(GetEngine()
		, WaterLevel2Pixel(iWaterLevel), iVel, 300));
}


void SubmarineCommander::SendStrategicSubmarine(int iWaterLevel, int iVel)
{
	GetEngine()->IncM_iSubCounter();
	GetEngine()->StoreObjectInVector(new StrategicSubmarine(GetEngine()
		, WaterLevel2Pixel(iWaterLevel), iVel, 300));
}


int SubmarineCommander::WaterLevel2Pixel(int iLevel)
{
	return iLevel * 25 + GetEngine()->GetM_iDeepSeaUpper();
}


