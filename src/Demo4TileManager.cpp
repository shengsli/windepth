#include "header.h"

#include "Demo4TileManager.h"

Demo4TileManager::Demo4TileManager(void)
	: TileManager( 50,50 )
{
}

Demo4TileManager::~Demo4TileManager(void)
{
}


// Draw a tile at a location on the screen determined by the tile
// position and the base X and Y co-ordinates
// OVERRIDE THIS TO DRAW YOUR TILE!!!
// Draw a tile
void Demo4TileManager::DrawTileAt( 
	BaseEngine* pEngine, 
	SDL_Surface* pSurface, 
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY ) const
{
	// New implementation draws things of different sizes
	switch( GetValue(iMapX,iMapY) )
	{
	case 0:
	case 1:
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			pEngine->GetColour( GetValue(iMapX,iMapY) ),
			pSurface );
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			pEngine->GetColour( 0 ),
			pSurface );
		pEngine->DrawOval( 
			iStartPositionScreenX+GetValue(iMapX,iMapY)*2+5,
			iStartPositionScreenY+GetValue(iMapX,iMapY)*2+5, 
			iStartPositionScreenX + GetTileWidth() - GetValue(iMapX,iMapY)*2 -6,
			iStartPositionScreenY + GetTileHeight() - GetValue(iMapX,iMapY)*2 -6,
			pEngine->GetColour( GetValue(iMapX,iMapY) ),
			pSurface );
		break;
	}
}
