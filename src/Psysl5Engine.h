#pragma once
#include "BaseEngine.h"
#include "Psysl5TileManager.h"
#include "JPGImage.h"
#include <vector>
#include <iostream>
using namespace std;
class GameState;
class GameObject;
class BattleShip;
class SubmarineCommander;
class Psysl5Engine :
	public BaseEngine
{
public:
	int m_iPauseStarted;
	vector<int> m_iInvincibleCode_vector; /* vector storing cheating code */
	vector<int> m_iDeactivateCode_vector; /* vector storing deactivating cheating mode code */
	int m_iInvincibleCodeCounter; /* counter for cheating mode */
	int m_iDeactivateCodeCounter; /* counter for deactivating cheating mode */
	bool m_bInvincible; /* bool for cheating mode */
	Font* m_pFont; /* font pointer for 12px size font */
private:
	int m_iDrawableObjectsChanged; /* flag for game object pointer vector */
	vector<GameObject*> m_pGameObjects_vector; /* vector storing game object pointer */
	BattleShip* m_pShip;
	SubmarineCommander* m_pCommander;
	Psysl5TileManager m_oTiles;
	const int m_iShallowSeaUpper; /*  y coordinate of shallow sea upper bound */
	const int m_iDeepSeaUpper; /* y coordinate of deep sea upper bound */
	int m_iLevel;
	vector<GameState*> m_pGameStates_vector; /* use as a stack of states */
	/* game object ImageData */
	ImageData m_imOilDrum;
	ImageData m_imBattleShip;
	ImageData m_imMinelayerSubmarine_left;
	ImageData m_imMinelayerSubmarine_right;
	ImageData m_imMissileSubmarine_left;
	ImageData m_imMissileSubmarine_right;
	ImageData m_imStrategicSubmarine_left;
	ImageData m_imStrategicSubmarine_right;
	ImageData m_imExplosionSpriteSheet;
	ImageData m_imNuclearExplosionSpriteSheet;
	ImageData m_imWeaponExplosionSpriteSheet;
	ImageData m_imBomber;
	ImageData m_imSeagullSpritesheet;
	ImageData m_imBomb;
	ImageData m_imUnderWaterMine;
	ImageData m_imChain;
	/* ship variables */
	int m_iLives;
	const int m_iMaxLives;
	int m_iDepthChargeAmmo;
	unsigned int m_uiScore;
	int m_iSubCounter;
	bool m_bBombing; /* true if bomber is bombing or air support item appears, false otherwise */
	bool m_bMineLaid; /* true if underwater mines or underwater spuuort item appears, false otherwise */
	int m_iUnderWaterMineCounter; /* counter of existing under water mines on screen */
	/* background position */
	int m_iBackgroundX;
	int m_iBackgroundY;
public:
	Psysl5Engine();
	~Psysl5Engine();
	void SetupBackgroundBuffer();
	int InitialiseObjects();
	void DrawStrings();
	void DrawStringsOnTop();
	void UnDrawStrings();
	/* functions manipulating the vector of game object pointers */
	int GetM_iDrawableObjectsChanged() { return m_iDrawableObjectsChanged; }
	void SetM_iDrawableObjectsChanged(int iDrawableObjectsChanged) { m_iDrawableObjectsChanged = iDrawableObjectsChanged; }
	void StoreObjectInVector(GameObject* pObject);
	void RemoveObjectsFromVector();
	void DestroyOldObjects();
	void UpdateAllObjects(int iCurrentTime);
	void UndrawObjects();
	void DrawObjects();
	void NotifyAllObjects(int iSignalNumber);
	int NotifyAllObjectsGetCountNonZero(int iSignalNumber);
	int NotifyAllObjectsGetSum(int iSignalNumber);
	int NotifyAllObjectsGetMax(int iSignalNumber);
	int NotifyAllObjectsGetMin(int iSignalNumber);

	void GameAction();
	void KeyDown(int iKeyCode);
	vector<GameObject*> GetM_pGameObjects_vector() { return m_pGameObjects_vector; }
	inline BattleShip* GetM_pShip() const { return m_pShip; }
	int GetM_iShallowSeaUpper() { return  m_iShallowSeaUpper; }
	int GetM_iDeepSeaUpper() { return  m_iDeepSeaUpper; }
	int GetM_iLevel() { return m_iLevel; }
	void IncM_iLevel() { m_iLevel++; }
	/* ImageData pointer accessor  */
	ImageData* GetpM_imOilDrum() { return &m_imOilDrum; }
	ImageData* GetpM_imBattleShip() { return &m_imBattleShip; }
	ImageData* GetpM_imMinelayerSubmarine(int iVel)
	{
		if (iVel > 0) return &m_imMinelayerSubmarine_right;
		else return &m_imMinelayerSubmarine_left;
	}
	ImageData* GetpM_imMissileSubmarine(int iVel)
	{
		if (iVel > 0) return &m_imMissileSubmarine_right;
		else return &m_imMissileSubmarine_left;
	}
	ImageData* GetpM_imStrategicSubmarine(int iVel)
	{
		if (iVel > 0) return &m_imStrategicSubmarine_right;
		else return &m_imStrategicSubmarine_left;
	}
	ImageData* GetpM_imExplosionSpriteSheet() { return &m_imExplosionSpriteSheet; }
	ImageData* GetpM_imNuclearExplosionSpriteSheet() { return &m_imNuclearExplosionSpriteSheet; }
	ImageData* GetpM_imWeaponExplosionSpriteSheet() { return &m_imWeaponExplosionSpriteSheet; }
	ImageData* GetpM_imBomber() { return &m_imBomber; }
	ImageData* GetM_imSeagullSpritesheet() { return &m_imSeagullSpritesheet; }
	ImageData* GetpM_imBomb() { return &m_imBomb; }
	ImageData* GetpM_imUnderWaterMine() { return &m_imUnderWaterMine; }
	ImageData* GetpM_imChain() { return &m_imChain; }
	/* background */
	void DrawStateInitBackground();
	void DrawStateMainBackground();
	void DrawStateLoseBackground();
	int GetM_iBackgroundX() { return m_iBackgroundX; }
	int GetM_iBackgroundY() { return m_iBackgroundY; }
	void SetM_iBackgroundX(int iBackgroundX) { m_iBackgroundX = iBackgroundX; }
	void SetM_iBackgroundY(int iBackgroundY) { m_iBackgroundY = iBackgroundY; }
	void ShakeBackground()
	{
		m_iBackgroundX = rand() % 10 - 5;
		m_iBackgroundY = rand() % 10 - 5;
	}
	/* Rotation transform */
	double RotateX(double x0, double y0, double v, double w, double theta)
	{
		return (v - x0)*cos(theta) - (w - y0)*sin(theta) + x0;
	}
	double RotateY(double x0, double y0, double v, double w, double theta)
	{
		return (v - x0)*sin(theta) + (w - y0)*cos(theta) + y0;
	}
	void DrawScreenPolygonWithAngle(
		double x0, double y0, double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4, double fX5, double fY5, 
		unsigned int uiColour,
		double theta)
	{
		DrawScreenPolygon(
			RotateX(x0, y0, fX1, fY1, theta), RotateY(x0, y0, fX1, fY1, theta),
			RotateX(x0, y0, fX2, fY2, theta), RotateY(x0, y0, fX2, fY2, theta),
			RotateX(x0, y0, fX3, fY3, theta), RotateY(x0, y0, fX3, fY3, theta),
			RotateX(x0, y0, fX4, fY4, theta), RotateY(x0, y0, fX4, fY4, theta),
			RotateX(x0, y0, fX5, fY5, theta), RotateY(x0, y0, fX5, fY5, theta),
			uiColour);
	}
	/* lives */
	int GetM_iLives() { return m_iLives; }
	void SetM_iLives(int iLives) { m_iLives = iLives; }
	void IncM_iLives() 
	{
		/* increase lives by one if less than max lives */
		if (m_iLives < m_iMaxLives)
			m_iLives++;
	}
	void DecM_iLives() { m_iLives--; }
	int GetM_iMaxLives() { return m_iMaxLives; }
	/* depth charge ammo */
	int GetM_iDepthChargeAmmo() { return m_iDepthChargeAmmo; }
	void SetM_iDepthChargeAmmo(int iDepthChargeAmmo) { m_iDepthChargeAmmo = iDepthChargeAmmo; }
	void IncM_iDepthChargeAmmo() { m_iDepthChargeAmmo++; }
	void DecM_iDepthChargeAmmo() { m_iDepthChargeAmmo--; }
	/* score */
	void IncM_uiScore(unsigned int uiScore) { m_uiScore += uiScore; }
	unsigned int GetM_uiScore() { return m_uiScore; }
	/* submarine counter for level difficulty */
	void IncM_iSubCounter() { m_iSubCounter++; }
	void DecM_iSubCounter() { m_iSubCounter--; }
	int GetM_iSubCounter() { return m_iSubCounter; }
	/* invincible mode for cheating */
	void CompareCode(int iKeyCode);
	void SetInvincibleMode(bool bInvincible)
	{
		m_bInvincible = bInvincible;
		if (bInvincible)
			puts("Invincible mode activated.");
		else
			puts("Invincible mode deactivated.");
	}
	/* bombing */
	bool IsM_bBombing() { return m_bBombing; }
	void SetM_bBombing(bool bBombing) { m_bBombing = bBombing; }
	/* underwater mine counter */
	bool IsM_bMineLaid() { return m_bMineLaid; }
	void SetM_bMineLaid(bool bMineLaid) { m_bMineLaid = bMineLaid; }
	int GetM_iUnderWaterMineCounter() { return m_iUnderWaterMineCounter; }
	void IncM_iUnderWaterMineCounter() { m_iUnderWaterMineCounter++; }
	void DecM_iUnderWaterMineCounter() { m_iUnderWaterMineCounter--; }
	/* initialise variables in initialised list */
	void InitialiseVariables();
	SubmarineCommander* GetM_pCommander() { return m_pCommander; }
	Psysl5TileManager& GetM_oTiles() { return m_oTiles; }
	/* states */
	void ChangeState(GameState* pGameState);
	void PushState(GameState* pGameState);
	void PopState();
	void HandleEvents(); // remove pointer to game class
	void Clean();
	/* mouse */
	void ResetMousePositionClickedDown()
	{
		m_iMouseXClickedDown = -1;
		m_iMouseYClickedDown = -1;
	}
	bool IsMouseDown()
	{
		/* return true if click mouse, false otherwise */
		bool bTemp = ((m_iMouseXClickedDown == m_iCurrentMouseX)
			&& (m_iMouseYClickedDown == m_iCurrentMouseY));
		ResetMousePositionClickedDown();
		return bTemp;
	}
};

