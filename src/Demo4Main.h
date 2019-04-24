#pragma once

#include "BaseEngine.h"
#include "TileManager.h"
#include "Demo4TileManager.h"

class Demo4Main :
	public BaseEngine
{
public:
	Demo4Main(void);
	~Demo4Main(void);

	virtual void SetupBackgroundBuffer();
	virtual int InitialiseObjects();
	virtual void DrawStrings();
	virtual void GameAction();
	virtual void MouseDown(int iButton, int iX, int iY);
	virtual void KeyDown(int iKeyCode);
	virtual void UndrawObjects();
	virtual void DrawObjects();

	// Get a reference to the current tile manager
	Demo4TileManager& GetTileManager() { return m_oTiles; }

private:
	Demo4TileManager m_oTiles;

public:
	// State number
	enum State { stateInit, stateMain, statePaused };

	// Work out what this means yourself
	int m_iPauseStarted;

private:
	State m_state;
};

