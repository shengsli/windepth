#include "header.h"

#include "BaseEngine.h"

#include "Demo4Object.h"

#include "Demo4Main.h"

#include "JPGImage.h"

#include "TileManager.h"


Demo4Main::Demo4Main(void)
: BaseEngine( 50 )
, m_state(stateInit) // NEW
{
}

Demo4Main::~Demo4Main(void)
{
}

void Demo4Main::SetupBackgroundBuffer()
{
	// NEW SWITCH
	switch( m_state )
	{
	case stateInit: // Reload the level data
		FillBackground( 0xffff00 );
		{
			char* data[] = {
				"bbbbbbbbbbbbbbb",
				"baeaeadadaeaeab",
				"babcbcbcbcbibeb",
				"badadgdadhdadhb",
				"bgbcbcbcbibcbeb",
				"badadadadadadab",
				"bfbcbibcbcbcbeb",
				"bahadadhdadadab",
				"bfbcbcbibcbibeb",
				"badadadadadadab",
				"bbbbbbbbbbbbbbb" };

			// Specify how many tiles wide and high
			m_oTiles.SetSize( 15, 11 ); 
			// Set up the tiles
			for ( int x = 0 ; x < 15 ; x++ )
				for ( int y = 0 ; y < 11 ; y++ )
					m_oTiles.SetValue( x, y, data[y][x]-'a' );

			for ( int y = 0 ; y < 11 ; y++ )
			{
				for ( int x = 0 ; x < 15 ; x++ )
					printf("%d ", m_oTiles.GetValue(x,y) );
				printf("\n" );
			}

			// Specify the screen x,y of top left corner
			m_oTiles.SetBaseTilesPositionOnScreen( 25, 40 );
		}
		return;

	case stateMain:
		FillBackground( 0 );
		// Tell it to draw tiles from x1,y1 to x2,y2 in tile array,
		// to the background of this screen
		m_oTiles.DrawAllTiles( this, 
			this->GetBackground(), 
			0, 0, 14, 10 );
		break; // Drop out to the complicated stuff
	case statePaused:
		FillBackground( 0 );
		m_oTiles.DrawAllTiles( this, 
			this->GetBackground(), 
			0, 0, 14, 10 );
		break;
	} // End switch
}

int Demo4Main::InitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	DrawableObjectsChanged();

	// Destroy any existing objects
	DestroyOldObjects();

	// Creates an array one element larger than the number of objects that you want.
	CreateObjectArray(5);

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	StoreObjectInArray( 0, new Demo4Object(this, 1, 1) );
	StoreObjectInArray( 1, new Demo4Object(this, 9, 9) );
	StoreObjectInArray( 2, new Demo4Object(this, 13, 9) );
	StoreObjectInArray( 3, new Demo4Object(this, 9, 5) );
	StoreObjectInArray( 4, new Demo4Object(this, 13, 5) );

	// i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
	StoreObjectInArray( 5, NULL);

	// NOTE: We also need to destroy the objects, but the method at the 
	// top of this function will destroy all objects pointed at by the 
	// array elements so we can ignore that here.

	return 0;
}







/* Draw text labels */
void Demo4Main::DrawStrings()
{
	// NEW SWITCH
	switch( m_state )
	{
	case stateInit:
		CopyBackgroundPixels( 0/*X*/, 280/*Y*/, GetScreenWidth(), 40/*Height*/ );
		DrawScreenString( 100, 300, "Initialised and waiting for SPACE", 0x0, NULL );
		break;
	case stateMain:
		CopyBackgroundPixels( 0/*X*/, 0/*Y*/, GetScreenWidth(), 30/*Height*/ );
		DrawScreenString( 250, 10, "Running", 0xffffff, NULL );
		break;
	case statePaused:
		CopyBackgroundPixels( 0/*X*/, 280/*Y*/, GetScreenWidth(), 40/*Height*/ );
		DrawScreenString( 200, 300, "Paused. Press SPACE to continue", 0xffffff, NULL );
		break;
	}
}


/* Overridden GameAction to ensure that objects use the modified time */
void Demo4Main::GameAction()
{
	// If too early to act then do nothing
	if ( !IsTimeToAct() ) // No sleep, will just keep polling constantly - this is a difference from the base class
		return;

	// Don't act for another 1 tick - this is a difference from the base class
	SetTimeToAct( 1 );

	// NEW SWITCH - different from the base class
	switch( m_state )
	{
	case stateInit:
	case statePaused:
		break;
	case stateMain:
		// Only tell objects to move when not paused etc
		UpdateAllObjects( GetModifiedTime() );
		break;
	}
}


// Override to add a node at specified point
void Demo4Main::MouseDown( int iButton, int iX, int iY )
{
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is pressed
*/
void Demo4Main::KeyDown(int iKeyCode)
{
	// NEW SWITCH on current state
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		SetExitWithCode( 0 );
		break;
	case SDLK_SPACE: // SPACE Pauses
		switch( m_state )
		{
		case stateInit:
			// Go to state main
			m_state = stateMain;
			// Force redraw of background
			SetupBackgroundBuffer();
			// Redraw the whole screen now
			Redraw(true);
			break;
		case stateMain:
			// Go to state paused
			m_state = statePaused;
			// Work out what this does. 
			m_iPauseStarted = GetTime();
			// Force redraw of background
			SetupBackgroundBuffer();
			// Redraw the whole screen now
			Redraw(true);
			break;
		case statePaused:
			// Go to state main
			m_state = stateMain;
			// Work out what this does. It will be hard to notice any difference without these, but there is one. Hint: watch the positions and sizes of the objects
			IncreaseTimeOffset(m_iPauseStarted - GetTime());
			// Force redraw of background
			SetupBackgroundBuffer();
			// Redraw the whole screen now
			Redraw(true);
			break;
		} // End switch on current state
		break; // End of case SPACE
	}
}

void Demo4Main::UndrawObjects()
{
	if (m_state != stateInit) // Not in initialise state
		BaseEngine::UndrawObjects();
}

void Demo4Main::DrawObjects()
{
	if (m_state != stateInit) // Not in initialise state
		BaseEngine::DrawObjects();
}



