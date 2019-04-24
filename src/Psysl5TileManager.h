#pragma once
#include "TileManager.h"
#include "JPGImage.h"
class Psysl5TileManager :
	public TileManager
{
private:
	ImageData m_imSeagull;
	const int m_iNumOfRows;
	const int m_iNumOfCols;
	const int m_iTileHeight;
	const int m_iTileWidth;
public:
	Psysl5TileManager(int iTileHeight, int iTileWidth);
	~Psysl5TileManager();
	void DrawTileAt(
		BaseEngine* pEngine,
		SDL_Surface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const;
	int GetM_iNumOfRows() { return m_iNumOfRows; }
	int GetM_iNumOfCols() { return m_iNumOfCols; }
	int GetM_iTileHeight() { return m_iTileHeight; }
	int GetM_iTileWidth() { return m_iTileWidth; }
};

