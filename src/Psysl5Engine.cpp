#include "header.h"
#include "templates.h"

#include "Psysl5Engine.h"
#include "BattleShip.h"
#include "Submarine.h"
#include "SubmarineCommander.h"
#include "UnderWaterMine.h"
#include "JPGImage.h"
#include "GameState.h"
#include "InitState.h"

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


Psysl5Engine::Psysl5Engine()
	: BaseEngine(50)
	, m_iDrawableObjectsChanged(0)
	, m_iInvincibleCodeCounter(0)
	, m_iDeactivateCodeCounter(0)
	, m_bInvincible(false)
	, m_oTiles(30, 100)
	, m_iShallowSeaUpper(100)
	, m_iDeepSeaUpper(200)
	, m_iLevel(0)
	, m_iMaxLives(6)
	, m_iLives(m_iMaxLives)
	, m_iDepthChargeAmmo(5)
	, m_uiScore(0)
	, m_iSubCounter(0)
	, m_bBombing(false)
	, m_bMineLaid(false)
	, m_iUnderWaterMineCounter(0)
	, m_iBackgroundX(0)
	, m_iBackgroundY(0)
{
	/* initialise state */
	ChangeState(InitState::Instance());
	/* cheat code */
	m_iInvincibleCode_vector = {
		SDLK_i, SDLK_n, SDLK_v, SDLK_i, SDLK_n, SDLK_c, SDLK_i, SDLK_b, SDLK_l, SDLK_e
	};
	m_iDeactivateCode_vector = {
		SDLK_d, SDLK_e, SDLK_a, SDLK_c, SDLK_t, SDLK_i, SDLK_v, SDLK_a, SDLK_t, SDLK_e
	};
	/* Load game object sprite */
	ImageData imTemp;
	imTemp.LoadImage("oil_drum.png");
	m_imOilDrum.ShrinkFrom(&imTemp, 4);
	
	imTemp.LoadImage("battleship.png");
	m_imBattleShip.ShrinkFrom(&imTemp, 8);

	/* submarines */
	imTemp.LoadImage("minelayer_submarine_left.png");
	m_imMinelayerSubmarine_left.ShrinkFrom(&imTemp, 9);
	imTemp.LoadImage("minelayer_submarine_right.png");
	m_imMinelayerSubmarine_right.ShrinkFrom(&imTemp, 9);

	imTemp.LoadImage("missile_submarine_left.png");
	m_imMissileSubmarine_left.ShrinkFrom(&imTemp, 9);
	imTemp.LoadImage("missile_submarine_right.png");
	m_imMissileSubmarine_right.ShrinkFrom(&imTemp, 9);

	imTemp.LoadImage("strategic_submarine_left.png");
	m_imStrategicSubmarine_left.ShrinkFrom(&imTemp, 9);
	imTemp.LoadImage("strategic_submarine_right.png");
	m_imStrategicSubmarine_right.ShrinkFrom(&imTemp, 9);

	/* explosion */
	imTemp.LoadImage("explosion_spritesheet.png");
	m_imExplosionSpriteSheet.ShrinkFrom(&imTemp, 2);

	imTemp.LoadImage("nuclear_explosion_spritesheet.png");
	m_imNuclearExplosionSpriteSheet.ShrinkFrom(&imTemp, 2);

	imTemp.LoadImage("weapon_explosion_spritesheet.png");
	m_imWeaponExplosionSpriteSheet.ShrinkFrom(&imTemp, 2);

	/* bomber */
	imTemp.LoadImage("bomber.png");
	m_imBomber.ShrinkFrom(&imTemp, 4);

	/* seagull*/
	imTemp.LoadImage("seagull_spritesheet.png");
	m_imSeagullSpritesheet.ShrinkFrom(&imTemp, 3);

	/* bomb */
	imTemp.LoadImage("bomb.png");
	m_imBomb.ShrinkFrom(&imTemp, 3);

	/* underwater mine */
	imTemp.LoadImage("underwater_mine.png");
	m_imUnderWaterMine.ShrinkFrom(&imTemp, 2);

	/* chain */
	imTemp.LoadImage("chain.png");
	m_imChain.ShrinkFrom(&imTemp, 1);
}


Psysl5Engine::~Psysl5Engine()
{
	DestroyOldObjects();
}


void Psysl5Engine::SetupBackgroundBuffer()
{
	m_pGameStates_vector.back()->SetupBackgroundBuffer(this);
}


int Psysl5Engine::InitialiseObjects()
{
	/* destroy all objects in game objects vector, 
	   then create submarine commander and store battleship pointer in vector */
	DestroyOldObjects();
	DrawableObjectsChanged();

	m_pCommander = new SubmarineCommander(this);
	m_pShip = new BattleShip(this);
	StoreObjectInVector(m_pShip);
	
	return 0;
}


void Psysl5Engine::DrawStrings()
{
}


void Psysl5Engine::DrawStringsOnTop()
{
	m_pGameStates_vector.back()->DrawStringsOnTop(this);
}


void Psysl5Engine::UnDrawStrings()
{
	m_pGameStates_vector.back()->UnDrawStrings(this);
}


void Psysl5Engine::GameAction()
{
	// If too early to act then do nothing
	if (!IsTimeToAct())
		return;
	// Don't act for another 1 tick - this is a difference from the base class
	SetTimeToAct(15);

	m_pGameStates_vector.back()->GameAction(this);
}


void Psysl5Engine::KeyDown(int iKeyCode)
{
	m_pGameStates_vector.back()->KeyDown(iKeyCode, this);
}


void Psysl5Engine::StoreObjectInVector(GameObject* pObject)
{
	m_pGameObjects_vector.push_back(pObject);
}


void Psysl5Engine::RemoveObjectsFromVector()
{
	/* If IsToRemove() returns true, delete memory and remove it from the vector.
	   Otherwise, do nothing. */
	m_pGameObjects_vector.erase(remove_if(m_pGameObjects_vector.begin()
		, m_pGameObjects_vector.end(), 
		[](GameObject* pObject)
		{
			if (pObject->IsToRemove())
			{
				delete pObject;
				pObject = nullptr;
				return true;
			}
			else
				return false;
		}
		), m_pGameObjects_vector.end());
}


void Psysl5Engine::DestroyOldObjects()
{
	/* deallocate memory pointed by all pointers stored in vector, 
	   set all pointers to nullptr,
	   destroy all elements and leave the vector with a size of zero */
	m_iDrawableObjectsChanged = 1;
	if (!m_pGameObjects_vector.empty())
	{
		for (int i = 0; i < m_pGameObjects_vector.size(); i++)
		{
			delete m_pGameObjects_vector[i];
			m_pGameObjects_vector[i] = NULL;
		}
		m_pGameObjects_vector.clear();
	}
}


void Psysl5Engine::UpdateAllObjects(int iCurrentTime)
{
	/* loop through the vector, each game object pointer call DoUpdate() */
	m_iDrawableObjectsChanged = 0;
	for (int i = 0; i < m_pGameObjects_vector.size(); i++)
	{
		m_pGameObjects_vector[i]->DoUpdate(iCurrentTime);
		if (m_iDrawableObjectsChanged)
			return; // Abort! Something changed
	}
}


void Psysl5Engine::UndrawObjects()
{
	m_pGameStates_vector.back()->UndrawObjects(this);
}


void Psysl5Engine::DrawObjects()
{
	m_pGameStates_vector.back()->DrawObjects(this);
}


void Psysl5Engine::NotifyAllObjects(int iSignalNumber)
{
	for (int i = 0; i < m_pGameObjects_vector.size(); i++)
	{
		m_pGameObjects_vector[i]->Notify(iSignalNumber);
	}
}

/* Send a specified notification value to all displayable objects and count the number which give a non-zero response. */
int Psysl5Engine::NotifyAllObjectsGetCountNonZero(int iSignalNumber)
{
	int iReturn = 0;
	for (int i = 0; i < m_pGameObjects_vector.size(); i++)
	{
		if (m_pGameObjects_vector[i]->Notify(iSignalNumber) != 0)
			iReturn++;
	}
	return iReturn;
}

/* Send a specified notification value to all displayable objects and return the sum of the returned values. */
int Psysl5Engine::NotifyAllObjectsGetSum(int iSignalNumber)
{
	int iReturn = 0;
	for (int i = 0; i < m_pGameObjects_vector.size(); i++)
	{
		iReturn += m_pGameObjects_vector[i]->Notify(iSignalNumber);
	}
	return iReturn;
}

/* Send a specified notification value to all displayable objects and return the largest of the returned values. */
int Psysl5Engine::NotifyAllObjectsGetMax(int iSignalNumber)
{
	int iReturn = INT_MIN;
	for (int i = 0; i < m_pGameObjects_vector.size(); i++)
	{
		int ival = m_pGameObjects_vector[i]->Notify(iSignalNumber);
		if (ival > iReturn)
			iReturn = ival;
	}
	return iReturn;
}

/* Send a specified notification value to all displayable objects and return the smallest of the returned values. */
int Psysl5Engine::NotifyAllObjectsGetMin(int iSignalNumber)
{
	int iReturn = INT_MAX;
	for (int i = 0; i < m_pGameObjects_vector.size(); i++)
	{
		int ival = m_pGameObjects_vector[i]->Notify(iSignalNumber);
		if (ival < iReturn)
			iReturn = ival;
	}
	return iReturn;
}


void Psysl5Engine::CompareCode(int iKeyCode)
{
	if (m_bInvincible == false)
	{
		if (m_iInvincibleCodeCounter > m_iInvincibleCode_vector.size())
			m_iInvincibleCodeCounter = 0;
		if (iKeyCode == m_iInvincibleCode_vector[m_iInvincibleCodeCounter])
		{
			m_iInvincibleCodeCounter++;
			if (m_iInvincibleCodeCounter == m_iInvincibleCode_vector.size())
				SetInvincibleMode(true);
		}
		else
			m_iInvincibleCodeCounter = 0;
	}
	else
	{
		if (m_iDeactivateCodeCounter > m_iDeactivateCode_vector.size())
			m_iDeactivateCodeCounter = 0;
		if (iKeyCode == m_iDeactivateCode_vector[m_iDeactivateCodeCounter])
		{
			m_iDeactivateCodeCounter++;
			if (m_iDeactivateCodeCounter == m_iDeactivateCode_vector.size())
				SetInvincibleMode(false);
		}
		else
			m_iDeactivateCodeCounter = 0;
	}
}


void Psysl5Engine::InitialiseVariables()
{
	/* initialise score, submarine counter */
	m_iLevel = 0;
	m_iLives = m_iMaxLives;
	m_iDepthChargeAmmo = 5;
	m_uiScore = 0;
	m_iSubCounter = 0;
	m_bBombing = false;
	m_bMineLaid = false;
	m_iUnderWaterMineCounter = 0;
}

/*************************************************************************************************/
/* states */
void Psysl5Engine::ChangeState(GameState* pGameState)
{
	// cleanup the current state
	if (!m_pGameStates_vector.empty()) {
		m_pGameStates_vector.back()->Clean(this);
		m_pGameStates_vector.pop_back();
	}

	// store and init the new state
	m_pGameStates_vector.push_back(pGameState);
	m_pGameStates_vector.back()->Init(this);
}


void Psysl5Engine::PushState(GameState* pGameState)
{
	// pause current state
	if (!m_pGameStates_vector.empty()) {
		m_pGameStates_vector.back()->Pause();
	}

	// store and init the new state
	m_pGameStates_vector.push_back(pGameState);
	m_pGameStates_vector.back()->Init(this);
}


void Psysl5Engine::PopState()
{
	// cleanup the current state
	if (!m_pGameStates_vector.empty()) {
		m_pGameStates_vector.back()->Clean(this);
		m_pGameStates_vector.pop_back();
	}

	// resume previous state
	if (!m_pGameStates_vector.empty()) {
		m_pGameStates_vector.back()->Resume();
	}
}


void Psysl5Engine::Clean()
{
	while (!m_pGameStates_vector.empty()) {
		m_pGameStates_vector.back()->Clean(this);
		m_pGameStates_vector.pop_back();
	}
	SDL_Quit();
}