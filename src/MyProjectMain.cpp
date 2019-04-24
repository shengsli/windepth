#include "header.h"

// This is a basic implementation, without fancy stuff

#include "BaseEngine.h"

#include "MyProjectMain.h"

#include "JPGImage.h"

#include "TileManager.h"

#include "DisplayableObject.h"

// This draws and moves the simple rectangle on the screen
class SimpleShape : public DisplayableObject
{
private:
	double m_dSX;
	double m_dSY;
	double m_dX;
	double m_dY;

public:
	// Constructor has to set up all of the position and size members
	SimpleShape(MyProjectMain* pEngine )
		: DisplayableObject( pEngine )
	{
		// The ball coordinate will be the centre of the ball
		// Because we start drawing half the size to the top-left.
		m_iStartDrawPosX = -50;
		m_iStartDrawPosY = -25;
		// Record the ball size as both height and width
		m_iDrawWidth = 100;
		m_iDrawHeight = 50;
		// Just put it somewhere initially
		m_dX = m_iPreviousScreenX = m_iCurrentScreenX = m_iDrawWidth;
		m_dY = m_iPreviousScreenY = m_iCurrentScreenY = m_iDrawHeight;
		// Speed
		m_dSX = 1;
		m_dSY = 1;
		// And make it visible
		SetVisible(true);
	}

	// Draw the shape - just draws a rectangle
	void Draw()
	{
		GetEngine()->DrawScreenRectangle( 
			m_iCurrentScreenX - m_iDrawWidth/2 + 1,
			m_iCurrentScreenY - m_iDrawHeight/2 + 1,
			m_iCurrentScreenX + m_iDrawWidth/2 -1,
			m_iCurrentScreenY + m_iDrawHeight/2 -1,
			0xffff00 );

		// This will store the position at which the object was drawn
		// so that the background can be drawn over the top.
		// This will then remove the object from the screen.
		StoreLastScreenPositionForUndraw();
	}

	// Called frequently, this should move the item
	// In this case we also accept cursor key presses to change the speed
	// Space will set the speed to zero
	void DoUpdate( int iCurrentTime )
	{
		// Change speed if player presses a key
		if ( GetEngine()->IsKeyPressed( SDLK_UP ) )
			m_dSY -= 0.001;
		if ( GetEngine()->IsKeyPressed( SDLK_DOWN ) )
			m_dSY += 0.001;
		if ( GetEngine()->IsKeyPressed( SDLK_LEFT ) )
			m_dSX -= 0.001;
		if ( GetEngine()->IsKeyPressed( SDLK_RIGHT ) )
			m_dSX += 0.001;
		if ( GetEngine()->IsKeyPressed( SDLK_SPACE ) )
			m_dSX = m_dSY = 0;

		// Alter position for speed
		m_dX += m_dSX;
		m_dY += m_dSY;

		// Check for bounce off the edge
		if ( (m_dX+m_iStartDrawPosX) < 0 )
		{
			m_dX = - m_iStartDrawPosX;
			if ( m_dSX < 0 )
				m_dSX = -m_dSX;
		}
		if ( (m_dX+m_iStartDrawPosX+m_iDrawWidth) > (GetEngine()->GetScreenWidth()-1) )
		{
			m_dX = GetEngine()->GetScreenWidth() -1 - m_iStartDrawPosX - m_iDrawWidth;
			if ( m_dSX > 0 )
				m_dSX = -m_dSX;
		}
		if ( (m_dY+m_iStartDrawPosY) < 0 )
		{
			m_dY = -m_iStartDrawPosY;
			if ( m_dSY < 0 )
				m_dSY = -m_dSY;
		}
		if ( (m_dY+m_iStartDrawPosY+m_iDrawHeight) > (GetEngine()->GetScreenHeight()-1) )
		{
			m_dY = GetEngine()->GetScreenHeight() -1 - m_iStartDrawPosY - m_iDrawHeight;
			if ( m_dSY > 0 )
				m_dSY = -m_dSY;
		}

		// Set current position - you NEED to set the current positions
		m_iCurrentScreenX = (int)(m_dX+0.5);
		m_iCurrentScreenY = (int)(m_dY+0.5);

		printf("Position %f, %f\n", m_dX, m_dY );

		// Ensure that the object gets redrawn on the display, if something changed
		RedrawObjects();
	}
};


/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void MyProjectMain::SetupBackgroundBuffer()
{
	FillBackground( 0x000000 );

	for ( int iX = 0 ; iX < GetScreenWidth() ; iX++ )
		for ( int iY = 0 ; iY < this->GetScreenHeight() ; iY++ )
			switch( rand()%100 )
			{
			case 0: SetBackgroundPixel( iX, iY, 0xFF0000 ); break;
			case 1: SetBackgroundPixel( iX, iY, 0x00FF00 ); break;
			case 2: SetBackgroundPixel( iX, iY, 0x0000FF ); break;
			case 3: SetBackgroundPixel( iX, iY, 0xFFFF00 ); break;
			case 4: SetBackgroundPixel( iX, iY, 0x00FFFF ); break;
			case 5: SetBackgroundPixel( iX, iY, 0xFF00FF ); break;
			}
}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int MyProjectMain::InitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	DrawableObjectsChanged();

	// Destroy any existing objects
	DestroyOldObjects();

	// Create an array one element larger than the number of objects that you want.
	m_ppDisplayableObjects = new DisplayableObject*[2];

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	// i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
	m_ppDisplayableObjects[0] = new SimpleShape(this);
	m_ppDisplayableObjects[1] = NULL;

	return 0;
}








/* Draw text labels */
void MyProjectMain::DrawStrings()
{
	// Build the string to print
	char buf[128];
	sprintf( buf, "Changing text %6d %6d", rand(), rand() );
	// Clear the top of the screen, since we about to draw text on it.
	CopyBackgroundPixels( 0, 0, GetScreenWidth(), 35 );
	// Then draw the strings
	DrawScreenString( 150, 10, buf, 0xffffff, NULL );
}


/* Overridden GameAction which can be modified */
void MyProjectMain::GameAction()
{
	// If too early to act then do nothing
	if ( !IsTimeToActWithSleep() )
		return;

	// Don't act for another 15 ticks
	SetTimeToAct( 15 );

	// Tell all objects to update themselves
	UpdateAllObjects( GetTime() );
}


// Override to handle a mouse press
void MyProjectMain::MouseDown( int iButton, int iX, int iY )
{
	// Redraw the background
	SetupBackgroundBuffer();
	Redraw(true); // Force total redraw
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is currently pressed
*/
void MyProjectMain::KeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		SetExitWithCode( 0 );
		break;
	case SDLK_SPACE: // SPACE Pauses
		break;
	}
}
