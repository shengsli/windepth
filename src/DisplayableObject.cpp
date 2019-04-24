#include "header.h"


#include "BaseEngine.h"

#include "DisplayableObject.h"


// Constructor
DisplayableObject::DisplayableObject(BaseEngine* pEngine) :
	// Current position of object on the screen
	m_iCurrentScreenX(-1),
	m_iCurrentScreenY(-1),
	// Previous position of object on the screen
	m_iPreviousScreenX(-1),
	m_iPreviousScreenY(-1),
	// Offset within the drawing area to draw at:
	m_iStartDrawPosX(0),
	m_iStartDrawPosY(0),
	// Size of the thing inside the tile:
	m_iDrawWidth(10),
	m_iDrawHeight(10),
	// Store pointer to the game object, for later use
	m_pEngine(pEngine)
{
}


// Destructor
DisplayableObject::~DisplayableObject()
{
}

// Draw the object - override to implement the actual drawing of the correct object
// This just draws a magenta square at the current location
void DisplayableObject::Draw()
{
	// Draw the object
	for ( int iX = m_iCurrentScreenX + m_iStartDrawPosX ; iX < (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth) ; iX++ )
		for ( int iY = m_iCurrentScreenY + m_iStartDrawPosY ; iY < (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight) ; iY++ )
			m_pEngine->SafeSetScreenPixel( iX, iY, 0xff00ff );

	// Store the position at which the object was last drawn.
	StoreLastScreenPositionForUndraw();
}

void DisplayableObject::StoreLastScreenPositionForUndraw() 
{
	// Store the location that was last drawn at, so that it can be removed again
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;
}

void DisplayableObject::RedrawBackground()
{
	// Copy the background pixels back over.
	m_pEngine->CopyBackgroundPixels( m_iPreviousScreenX + m_iStartDrawPosX, 
		m_iPreviousScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight );
}




// Handle the update action, moving the object and/or handling any game logic
void DisplayableObject::DoUpdate( int iCurrentTime )
{
	// Override can handle player input or do some AI actions to redirect object and set the current screen position
	// Set m_iCurrentScreenX, m_iCurrentScreenY
}

