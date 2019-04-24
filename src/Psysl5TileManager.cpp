#include "header.h"
#include "templates.h"

#include "Psysl5TileManager.h"
#include "JPGImage.h"


Psysl5TileManager::Psysl5TileManager(int iTileHeight, int iTileWidth)
	: TileManager(iTileHeight, iTileWidth)
	, m_iNumOfRows(3)
	, m_iNumOfCols(8)
	, m_iTileHeight(iTileHeight)
	, m_iTileWidth(iTileWidth)
{
	/* tile manager */
	SetSize(m_iNumOfCols, m_iNumOfRows);
	SetBaseTilesPositionOnScreen(0, 0);
	for (int iRowIdx = 0; iRowIdx < m_iNumOfRows; iRowIdx++)
		for (int iColIdx = 0; iColIdx < m_iNumOfCols; iColIdx++)
			SetValue(iColIdx, iRowIdx, rand() % 100);

	ImageData imTemp;
	imTemp.LoadImage("black_seagull.png");
	m_imSeagull.ShrinkFrom(&imTemp, 12);
}


Psysl5TileManager::~Psysl5TileManager()
{
}

void Psysl5TileManager::DrawTileAt(
	BaseEngine* pEngine,
	SDL_Surface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const
{
	if (GetValue(iMapX, iMapY) < 50)
		m_imSeagull.RenderImageWithMask(
		pSurface,
		0, 0,
		iStartPositionScreenX + GetValue(iMapX, iMapY),
		iStartPositionScreenY,
		m_imSeagull.GetWidth(),
		m_imSeagull.GetHeight());
}