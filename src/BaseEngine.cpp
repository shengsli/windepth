#include "header.h"

#if defined(_MSC_VER)
#include <SDL_syswm.h>
#endif

#include <math.h>

#include "BaseEngine.h"
#include "DisplayableObject.h"

#include "templates.h"




/*
Constructor.
*/
BaseEngine::BaseEngine(int iUnusedSinceSDL2)
: m_pKeyStatus(NULL)
, m_iExitWithCode(-1)
, m_iTick(0)
, m_bNeedsRedraw(true)
, m_bWholeScreenUpdate(true)
, m_iNextTickToActOn(0)
, m_ppDisplayableObjects(NULL)
, m_iDrawableObjectsChanged(0)
, g_pMainFont(NULL)
, m_iMouseXClickedDown(0)
, m_iMouseYClickedDown(0)
, m_iMouseXClickedUp(0)
, m_iMouseYClickedUp(0)
, m_iCurrentMouseX(0)
, m_iCurrentMouseY(0)
, m_iCurrentButtonStates(0)
, m_bInsideDraw(false)
, m_iTimeOffset(0)
{
}


/*
Destructor - destroys moving objects
*/
BaseEngine::~BaseEngine(void)
{
	DestroyOldObjects();
}



/*
Initialise SDL and create a window of the correct size.
*/ 
int BaseEngine::Initialise(const char* strCaption, int iScreenWidth, int iScreenHeight, const char* szFontName, int iFontSize)
{
	m_iScreenWidth = iScreenWidth;
	m_iScreenHeight = iScreenHeight;

	// Initialize SDL's subsystems
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	// Register SDL_Quit to be called at exit; makes sure things are
	// cleaned up when we quit.
	atexit(SDL_Quit);

	// True type font stuff
	TTF_Init();
	atexit(TTF_Quit);

	m_oFontManager.LoadFonts();

	// Load the default font - used whenever NULL is passed
	if ( ( szFontName != NULL ) && ( strlen(szFontName) > 0 ) )
		g_pMainFont = m_oFontManager.GetFont( szFontName, iFontSize );

	// SDL2 replacement for the SDL1.2 stuff, to make the surfaces still work with software rendering, so it is still a bitmapped display
	// WARNING: This is definitely not the fastest performance way to do this by any means, but it means that the bitmapped display will still work
	// and all of the SDL1.2 drawing code works without changes.
	m_pSDL2Window = SDL_CreateWindow( strCaption, 
			10,40, /* If you hardcode these, allow room for the window border and title bar */
			//SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Use this to let it position the window for you.
			m_iScreenWidth,m_iScreenHeight,
			0/*SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL*/ );
	m_pSDL2Renderer = SDL_CreateRenderer( m_pSDL2Window,-1,0 );
	m_pSDL2Texture = SDL_CreateTexture( m_pSDL2Renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,m_iScreenWidth,m_iScreenHeight );

	// Foreground surface, for the moving objects on top of background.
	m_pActualScreen = SDL_CreateRGBSurface( 0, m_iScreenWidth, m_iScreenHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000 );
	// Background surface, for the static background stuff
	m_pBackgroundSurface = SDL_CreateRGBSurface( 0,m_iScreenWidth,m_iScreenHeight,32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000 );


	// Initialise key status
	m_pKeyStatus = new bool[1 + SDL12_SDLK_LAST];
	memset( m_pKeyStatus,0,sizeof( bool )*(1 + SDL12_SDLK_LAST) );

	// Call method to allow custom sub-class initialiation.
	GameInit();

	return 0; // success
}


/*
Initialise game-specific things.
Base class just calls InitialiseObjects() to create the moving objects
*/
int BaseEngine::GameInit()
{
	// Create the moving objects
	InitialiseObjects();
	// Set up the initial background
	SetupBackgroundBuffer();

	return 0; // success
}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
You can use the CreateObjectArray() and StoreObjectInArray() functions to create it.
*/
int BaseEngine::InitialiseObjects()
{
	// Destroy any existing objects
	DestroyOldObjects();

	// Create an array one element larger than the number of objects that you want.
	// e.g.: m_ppDisplayableObjects = new DisplayableObject*[MAXNUMBER+1];
	// or CreateObjectArray(MAXNUMBER);

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	//e.g. m_ppDisplayableObjects[0] = new MyDisplayableObject( this, m_iPlayer1StartX, m_iPlayer1StartY);
	//e.g. m_ppDisplayableObjects[1] = new MyDisplayableObject( this, m_iPlayer2StartX, m_iPlayer2StartY);
	// or StoreObjectInArray( 0, new MyDisplayableObject( this, m_iPlayer1StartX, m_iPlayer1StartY);
	// or StoreObjectInArray( 1, new MyDisplayableObject( this, m_iPlayer2StartX, m_iPlayer2StartY);

	// i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
	//e.g. m_ppDisplayableObjects[2] = NULL;
	// Note that CreateObjectArray will have zero'ed the last element (NULL) but you can set it if you wish

	// You should return the number of objects which you created.
	// e.g. return 2;

	return 0;
}

void BaseEngine::CreateObjectArray(int iNumberObjects)
{
	// Destroy any existing object array if it exists
	DestroyOldObjects();
	m_ppDisplayableObjects = new DisplayableObject*[iNumberObjects + 1];
	// Clear the array - set all elements to 0
	memset(m_ppDisplayableObjects, 0, sizeof(DisplayableObject*) * (iNumberObjects + 1));
}




void BaseEngine::StoreObjectInArray(int iIndex, DisplayableObject* pObject)
{
	m_ppDisplayableObjects[iIndex] = pObject;
}


/* Destroy any existing displayable objects */
void BaseEngine::DestroyOldObjects()
{
	// Record the fact that the drawable objects have changed.
	m_iDrawableObjectsChanged = 1;

	if (m_ppDisplayableObjects != NULL)
	{
		for (int i = 0; m_ppDisplayableObjects[i] != NULL; i++)
		{
			delete m_ppDisplayableObjects[i];
			m_ppDisplayableObjects[i] = NULL;
		}
		delete[] m_ppDisplayableObjects;
		m_ppDisplayableObjects = NULL;
	}
}





/*
Main loop.
1) Stores key presses, and calls call-back functions for press/release if necessary.
2) Calls GameAction
3) Calls GameRender
*/
int BaseEngine::MainLoop(void)
{
    SDL_Event event;
	int iKeyCode;

	// Main loop: loop until told not to
	while (m_iExitWithCode==-1)
	{
		// Render stuff
		m_iTick = SDL_GetTicks();

		// Poll for events, and handle the ones we care about.
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				//printf("Key down: %d, '%c'\n", SymbolToKeycode(event.key.keysym.sym), event.key.keysym.sym);
				m_pKeyStatus[SymbolToKeycode(event.key.keysym.sym)] = true;
				KeyDown(event.key.keysym.sym);
				break;

			case SDL_KEYUP:                  
				//printf("Key up: %d, '%c'\n", SymbolToKeycode(event.key.keysym.sym), event.key.keysym.sym);
				m_pKeyStatus[SymbolToKeycode(event.key.keysym.sym)] = false;
				KeyUp(event.key.keysym.sym);
				break;

			case SDL_MOUSEMOTION:
				m_iCurrentMouseX = event.motion.x;
				m_iCurrentMouseY = event.motion.y;
				MouseMoved( event.motion.x, event.motion.y );
				break;

			case SDL_MOUSEBUTTONDOWN:
				m_iMouseXClickedDown = event.button.x;
				m_iMouseYClickedDown = event.button.y;
				MouseDown( event.button.button, event.button.x, event.button.y );
				break;

			case SDL_MOUSEBUTTONUP:
				m_iMouseXClickedUp = event.button.x;
				m_iMouseYClickedUp = event.button.y;
				MouseUp( event.button.button, event.button.x, event.button.y );
				break;

			case SDL_QUIT:
				return 0;
			}
		}

		// Do any updates for movements
		GameAction();

		// Render the screen
		GameRender();
	}
	return 0;
}




/* Sub-class should actually do something, if relevant
e.g. move the player or any other sprites */
void BaseEngine::GameAction()
{
	// If too early to act then do nothing
	if ( !IsTimeToActWithSleep() )
		return;

	// Don't act for another 15 ticks
	SetTimeToAct( 15 );

	// Tell all objects to update themselves.
	// If they need the screen to redraw then they should say so, so that GameRender() will
	// call the relevant function later.
	UpdateAllObjects( GetTime() );
}



/*
Overridable function, if necessary, for doing all of the drawing.
Base class version checks whether a redraw is needed and does nothing if not. See Redraw() function.
If a redraw is needed then there is an option to either redraw the whole screen or just redraw the moving objects.
If redrawing the whole screen then:
1a) SetupBackgroundBuffer is called to draw the background to the background buffer.
1b) DrawScreen is called to copy the background buffer onto the screen, then draw the moving objects.
1c) SDL_UpdateRect() is called to actually redraw the whole screen.
If only redrawing the moving objects then:
2a) DrawChangingObjects() is called to remove the objects from their old positions, and redraw them in their new positions.
2b) GetUpdateRectanglesForChangingObjects() is called to calculate where on the screen needs redrawing.
2c) SDL_UpdateRects() is called to redraw the parts of the screen which changed.
*/
void BaseEngine::GameRender(void)
{
	if ( !m_bNeedsRedraw )
		return;

	// Just drawn so no more redraw needed
	m_bNeedsRedraw = false;
	// Note: the redraw flags must only be set early in this method, not at the end, since the drawing/moving of the moving objects may
	// potentially flag a win/lose condition and a state change, which will set the redraw flag again to force a full draw after the 
	// state changes.

	if ( m_bWholeScreenUpdate )
	{
		// Drawn whole screen now, so no need to draw it again. See note above about the importance of not resetting this after DrawChangingObjects()
		m_bWholeScreenUpdate = false;

		// Lock surface if needed
		if (SDL_MUSTLOCK(m_pActualScreen))
		{
			m_bInsideDraw = true;
			if (SDL_LockSurface(m_pActualScreen) < 0) 
				return;
		}

		// Draw the screen as it should appear now.
		// First draw the background
		//this->CopyBackgroundPixels( 100, 100, 100, 100 );
		CopyAllBackgroundBuffer();
		// Draw the text for the user
		DrawStrings(); // Single function in case people want to use that instead
		DrawStringsUnderneath();	// Draw the string underneath the other objects here
		// Then draw the changing objects
		DrawObjects();
		// Another option for where to put the draw strings code - if you want it on top of the moving objects
		DrawStringsOnTop();

		// Unlock if needed
		if (SDL_MUSTLOCK(m_pActualScreen)) 
			SDL_UnlockSurface(m_pActualScreen);
		m_bInsideDraw = false;

		SDL_UpdateTexture( m_pSDL2Texture,NULL,m_pActualScreen->pixels,m_pActualScreen->pitch );
		//SDL_RenderClear( m_pSDL2Renderer );
		SDL_RenderCopy( m_pSDL2Renderer,m_pSDL2Texture,NULL,NULL );
		SDL_RenderPresent( m_pSDL2Renderer );

		//SDL_UpdateRect(m_pActualScreen, 0, 0, m_iScreenWidth, m_iScreenHeight);    
	}
	else
	{
		// Here we assume that the background buffer has already been set up

		// Lock surface if needed
		if (SDL_MUSTLOCK(m_pActualScreen))
		{
			m_bInsideDraw = true;
			if (SDL_LockSurface(m_pActualScreen) < 0) 
				return;
		}

		// Remove objects from their old positions
		UndrawObjects();
		// Remove the old strings be re-drawing the background
		UnDrawStrings(); 
		// Draw the text for the user
		DrawStrings(); // Single function in case people want to use that instead
		DrawStringsUnderneath();	// Draw the string underneath the other objects here
		// Draw objects at their new positions
		DrawObjects();
		// Another option for where to put the draw strings code - if you want it on top of the moving objects
		DrawStringsOnTop(); 

		// Unlock if needed
		if (SDL_MUSTLOCK(m_pActualScreen)) 
			SDL_UnlockSurface(m_pActualScreen);
		m_bInsideDraw = false;

		// Copy the screen display into the window and display it
		SDL_UpdateTexture( m_pSDL2Texture,NULL,m_pActualScreen->pixels,m_pActualScreen->pitch );
		//SDL_RenderClear( m_pSDL2Renderer );
		SDL_RenderCopy( m_pSDL2Renderer,m_pSDL2Texture,NULL,NULL );
		SDL_RenderPresent( m_pSDL2Renderer );
	}
}







/* Tell all displayable objects to update themselves. Calls DoUpdate on each displayable object. */
void BaseEngine::UpdateAllObjects( int iCurrentTime )
{
	m_iDrawableObjectsChanged = 0;
	if ( m_ppDisplayableObjects != NULL )
	{
		for ( int i = 0 ; m_ppDisplayableObjects[i] != NULL ; i++ )
		{
			m_ppDisplayableObjects[i]->DoUpdate(iCurrentTime);
			if ( m_iDrawableObjectsChanged )
				return; // Abort! Something changed in the array
		}
	}
}







/* Deinitialise everything - delete all resources */
void BaseEngine::Deinitialise(void)
{
	// Call method to allow custom sub-class cleanup
	CleanUp();

	// Free memory for key status record
	delete[] m_pKeyStatus;
}


/*
Overridable function, for adding custom clean-up in sub-classes.
*/
void BaseEngine::CleanUp(void)
{
}


/* Copy all of the background (e.g. tiles) to the screen display. e.g. removing any object drawn on top. */
void BaseEngine::CopyAllBackgroundBuffer()
{
/* Commented out because SDL_BlitSurface seems to fail since I went to SDL2
	SDL_Rect rect; rect.x = 0; rect.y = 0; rect.w = m_iScreenWidth; rect.h = m_iScreenHeight;
	
	if ( m_bInsideDraw )
		if (SDL_MUSTLOCK(m_pActualScreen)) 
			SDL_UnlockSurface(m_pActualScreen);

	::SDL_BlitSurface( m_pBackgroundSurface, &rect, m_pActualScreen, &rect );

	if ( m_bInsideDraw )
		if (SDL_MUSTLOCK(m_pActualScreen))
			SDL_LockSurface(m_pActualScreen);
*/

	memcpy( m_pActualScreen->pixels,m_pBackgroundSurface->pixels, sizeof( unsigned int ) * GetIntsPerScreenRow( m_pBackgroundSurface ) * m_iScreenHeight );
}

/* Copy some of the background onto the screen, e.g. removing an object which was drawn on top. */
void BaseEngine::CopyBackgroundPixels( int iX, int iY, int iWidth, int iHeight )
{
/* Commented out because SDL_BlitSurface seems to fail since I went to SDL2
	SDL_Rect rect; rect.x = iX; rect.y = iY; rect.w = iWidth; rect.h = iHeight;
	if ( m_bInsideDraw )
		if (SDL_MUSTLOCK(m_pActualScreen)) 
			SDL_UnlockSurface(m_pActualScreen);

	::SDL_BlitSurface( m_pBackgroundSurface, &rect, m_pActualScreen, &rect );

	if ( m_bInsideDraw )
		if (SDL_MUSTLOCK(m_pActualScreen))
			SDL_LockSurface(m_pActualScreen);
*/

	int iStart = iY * GetIntsPerScreenRow( m_pBackgroundSurface ) + iX;
	int iIncrement = GetIntsPerScreenRow( m_pBackgroundSurface ) - iWidth;
	unsigned int * puiSource = ((unsigned int *)m_pBackgroundSurface->pixels) + iStart;
	unsigned int * puiDest = ((unsigned int *)m_pActualScreen->pixels) + iStart;
	for ( int i = 0 ; i < iHeight ; i++ )
	{
		// Copy a line
		for ( int j = 0 ; j < iWidth ; j++ )
			*puiDest++ = *puiSource++;
		// Align on the next line
		puiSource += iIncrement;
		puiDest += iIncrement;
	}
}



// Determine whether a specific key is currently pressed
bool BaseEngine::IsKeyPressed(int iKeyCode) const
{
	return m_pKeyStatus[SymbolToKeycode(iKeyCode)];
}


/* Handle a key being released.
Note that the fact that the key has been released will have already been stored.
*/
void BaseEngine::KeyUp(int iKeyCode)
{
}

/* Handle a key being pressed
Note that the fact that the key has been pressed will have already been stored.
*/
void BaseEngine::KeyDown( int iKeyCode )
{
}

// Handle the fact that a mouse has moved
// Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish
void BaseEngine::MouseMoved( int iX, int iY )
{
	//printf( "BaseEngine::MouseMoved %d, %d\n", iX, iY );
	//DrawScreenThickLine( m_iMouseXClickedDown, m_iMouseYClickedDown, 
	//	iX, iY, 0xff0000, 1 );
}

// Handle the fact that a mouse button has been pressed
// Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish
void BaseEngine::MouseDown( int iButton, int iX, int iY )
{
	//printf( "BaseEngine::MouseDown %d, %d, %d\n", iButton, iX, iY );
}

// Handle the fact that a mouse button has been released
// Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish
void BaseEngine::MouseUp( int iButton, int iX, int iY )
{
	//printf( "BaseEngine::MouseUp %d, %d, %d\n", iButton, iX, iY );
}



















/* Fill the background - should be overridden to actually draw the correct thing. */
void BaseEngine::SetupBackgroundBuffer()
{
	// Put any special code to call different render functions for different states here
	FillBackground( 0xffff00 );
}

/* Fill the background with a specific colour */
void BaseEngine::FillBackground(unsigned int uiPixelColour)
{
	SDL_Rect rect; rect.x = 0; rect.y = 0; rect.w = m_iScreenWidth; rect.h = m_iScreenHeight;
	::SDL_FillRect( m_pBackgroundSurface, &rect, uiPixelColour );
//	int iStart = 0;
//	unsigned int * puiDest = m_puiScreenBuffer;
//	for ( int i = 0 ; i < (m_iScreenHeight * m_iIntsPerScreenRow) ; i++ )
//		*puiDest++ = uiPixelColour;
}


/* Draw text labels.
 You can do everything in this one if you wish, ignoring the three below, or you can split the contents.
*/
void BaseEngine::DrawStrings()
{
	// First clear the area where they were originally drawn - i.e. undraw previous text
	// e.g. CopyBackgroundBuffer()
	// Then draw the new labels/strings
	//e.g. m_pFont->DrawString(m_pActualScreen, 100, 180, "The DrawScreen method needs to be overridden", 0xff00ff);
}



/* You probably need to override these or the DrawStrings() above: 
Draw text labels for the screen */

/* Redraw the background over where you are drawing the strings.*/
void BaseEngine::UnDrawStrings() 
{ 
	// Example:
	// 	CopyBackgroundPixels( 0/*X*/, 0/*Y*/, GetScreenWidth(), 100/*Height*/ );
}

/* Draw the string that moving objects should be drawn on top of */
void BaseEngine::DrawStringsUnderneath() 
{
	// Example:
	// DrawScreenString( 150,10,"Example text",0xffffff,NULL );
}

/* Draw any string which should appear on top of moving objects - i.e. objects move behind these */
void BaseEngine::DrawStringsOnTop() 
{
	// Example:
	// DrawScreenString( 150,60,"Example text",0xffffff,NULL );
}







/*
Redraws the background behind each of the displayable objects, in their old positions.
*/
void BaseEngine::UndrawObjects()
{
	m_iDrawableObjectsChanged = 0;
	// This effectively un-draws the old positions of the objects
	if ( m_ppDisplayableObjects != NULL )
	{
		for ( int i = 0 ; m_ppDisplayableObjects[i] != NULL ; i++ )
		{
			m_ppDisplayableObjects[i]->RedrawBackground();
			if ( m_iDrawableObjectsChanged )
				return; // Abort! Something changed in the array
		}
	}
}


/*
Draws draws the displayable objects in their new positions.
*/
void BaseEngine::DrawObjects()
{
	m_iDrawableObjectsChanged= 0;
	// And this re-draws the new positions
	if ( m_ppDisplayableObjects != NULL )
	{
		for ( int i = 0 ; m_ppDisplayableObjects[i] != NULL ; i++ )
		{
			m_ppDisplayableObjects[i]->Draw();
			if ( m_iDrawableObjectsChanged )
				return; // Abort! Something changed in the array
		}
	}
}





/*
Gets the specified displayable object.
You may need to dynamic_cast the resulting pointer to the correct type.
*/
DisplayableObject* BaseEngine::GetDisplayableObject( int iIndex )
{
	if ( m_ppDisplayableObjects != NULL )
	{
		return m_ppDisplayableObjects[iIndex];
	}
	else
		return NULL;
}


// Get a colour code for a specific index
// Used to avoid creating arbitrary colours and instead have a set of useful values.
unsigned int BaseEngine::GetColour(int iColourIndex) const
{
	switch ( iColourIndex )
	{
	case 0: return 0x000000;
	case 1: return 0xff0000;
	case 2: return 0x00ff00;
	case 3: return 0x0000ff;
	case 4: return 0xff00ff;
	case 5: return 0xffff00;
	case 6: return 0x00ffff;
	case 7: return 0xffffff;
	case 8:	return 0xffc0c0;
	case 9: return 0xc0ffc0;
	case 10: return 0xc0c0ff;
	case 11: return 0xc00000;
	case 12: return 0x00c000;
	case 13: return 0x0000c0;
	case 14: return 0xc000c0;
	case 15: return 0xc0c000;
	case 16: return 0x00c0c0;
	case 17: return 0xc0c0c0;
	case 18: return 0xff8080;
	case 19: return 0x80ff80;
	case 20: return 0x8080ff;
	case 21: return 0x800000;
	case 22: return 0x008000;
	case 23: return 0x000080;
	case 24: return 0x800080;
	case 25: return 0x808000;
	case 26: return 0x008080;
	case 27: return 0x808080;
	case 28: return 0xc08080;
	case 29: return 0x80c080;
	case 30: return 0x8080c0;
	case 31: return 0x400000;
	case 32: return 0x004000;
	case 33: return 0x000040;
	case 34: return 0x400040;
	case 35: return 0x404000;
	case 36: return 0x004040;
	case 37: return 0x404040;
	case 38: return 0xc04040;
	case 39: return 0x40c040;
	case 40: return 0x4040c0;
	}
	return 0xcccccc; // Anything else is grey
}


// Draw a string in the specified font
void BaseEngine::DrawString(int iX, int iY, const char* pText, 
							unsigned int uiColour, Font* pFont, SDL_Surface* pTarget )
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	if ( pFont == NULL )
		pFont = g_pMainFont;

	if ( m_bInsideDraw )
		if (SDL_MUSTLOCK(m_pActualScreen)) 
			SDL_UnlockSurface(m_pActualScreen);

	SDL_Color color = { (uiColour&0xff0000)>>16, (uiColour&0xff00)>>8, (uiColour&0xff), 0 };

	if ( ( pFont != NULL ) && ( pFont->GetFont() != NULL ) )
	{
		SDL_Surface *sText = TTF_RenderText_Solid( pFont->GetFont(), pText, color );
		SDL_Rect rcDest = {iX,iY,0,0};

		SDL_BlitSurface( sText, NULL, pTarget, &rcDest );
		SDL_FreeSurface( sText );
	}

	if ( m_bInsideDraw )
		if (SDL_MUSTLOCK(m_pActualScreen))
			SDL_LockSurface(m_pActualScreen);
}


// Draw a triangle, as two vertical sided regions.
void BaseEngine::DrawTriangle(
		double fX1, double fY1,
		double fX2, double fY2,
		double fX3, double fY3, 
		unsigned int uiColour, 
		SDL_Surface* pTarget )
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	// Ensure order is 1 2 3 from left to right
	if ( fX1 > fX2 ) { Swap( fX1,fX2 ); Swap( fY1,fY2 ); } // Bigger of 1 and 2 is in position 2
	if ( fX2 > fX3 ) { Swap( fX2,fX3 ); Swap( fY2,fY3 ); } // Bigger of new 2 and 3 is in position 3
	if ( fX1 > fX2 ) { Swap( fX1,fX2 ); Swap( fY1,fY2 ); } // Bigger of 1 and new 2 is in position 2

	if ( fX1 == fX2 )
		DrawVerticalSidedRegion( fX1, fX3, fY1, fY3, fY2, fY3, uiColour, pTarget );
	else if ( fX2 == fX3 )
		DrawVerticalSidedRegion( fX1, fX3, fY1, fY2, fY1, fY3, uiColour, pTarget );
	else
	{
		// Split into two triangles. Find position on line 1-3 to split at
		double dSplitPointY = (double)fY1 + 
			(   ( (double)((fX2-fX1)*(fY3-fY1)) )
			/ (double)(fX3-fX1)   );
		DrawVerticalSidedRegion( fX1, fX2, fY1, fY2, fY1, dSplitPointY, uiColour, pTarget );
		DrawVerticalSidedRegion( fX2, fX3, fY2, fY3, dSplitPointY, fY3, uiColour, pTarget );
	}
}


// Draw a vertical sided region.
// If two points are the same then it is a triangle.
// To do an arbitrary triangle, just draw two next to each other, one for left and one for right.
void BaseEngine::DrawVerticalSidedRegion(
         double fX1, double fX2,// X positions
         double fY1a, double fY2a, // Start y positions for x1 and x2
         double fY1b, double fY2b, // End y positions for x1 and x2
         unsigned int uiColour,
         SDL_Surface* pTarget)
{
     if ( pTarget == NULL )
         pTarget = m_pActualScreen;

     // Ensure X1<  X2, otherwise steps will go wrong!
     // Switch the points if x and y are wrong way round
     if ( fX2<  fX1 ) { Swap(fX1,fX2); Swap(fY1a,fY2a); Swap(fY1b,fY2b);  }

     int iXStart = (int)(fX1+0.5);
     int iXEnd = (int)(fX2+0.5);

     // If integer x positions are the same then avoid floating point inaccuracy problems by a special case
     if ( iXStart == iXEnd )
     {
         int iYStart = (int)(fY1a+0.5);
         int iYEnd = (int)(fY2a+0.5);
         for ( int iY = iYStart ; iY<= iYEnd ; iY++ )
             SafeSetPixel( iXStart, iY, uiColour, pTarget );
     }
     else
     {
         // Draw left hand side
         int iYStart = (int)(fY1a+0.5);
         int iYEnd = (int)(fY1b+0.5);
         if ( iYStart>  iYEnd ) Swap( iYStart, iYEnd );
         //printf( "Firstline %d to %d (%f to %f)\n", iYStart, iYEnd, fY1a, fY1b );
         for ( int iY = iYStart ; iY<= iYEnd ; iY++ )
             SafeSetPixel( iXStart, iY, uiColour, pTarget );

         // Draw the middle
         for ( int iX = iXStart+1 ; iX<  iXEnd ; iX++ )
         {
             double fYStart = fY1a + ( (((double)iX)-fX1)*(fY2a-fY1a)) /(fX2-fX1);
             double fYEnd = fY1b + ((((double)iX)-fX1)*(fY2b-fY1b))/(fX2-fX1);
             if ( fYEnd<  fYStart ) Swap( fYStart, fYEnd );
             int iYStart = (int)(fYStart+0.5);
             int iYEnd = (int)(fYEnd+0.5);
             //printf( "Line from %d to %d (%f to %f)\n", iYStart, iYEnd, fYStart, fYEnd );
             for ( int iY = iYStart ; iY<= iYEnd ; iY++ )
                 SafeSetPixel( iX, iY, uiColour, pTarget );
         }

         // Draw right hand side
         iYStart = (int)(fY2a+0.5);
         iYEnd = (int)(fY2b+0.5);
         if ( iYStart>  iYEnd ) Swap( iYStart, iYEnd );
         //printf( "Last line %d to %d (%f to %f)\n", iYStart, iYEnd, fY2a, fY2b );
         for ( int iY = iYStart ; iY<= iYEnd ; iY++ )
             SafeSetPixel( iXEnd, iY, uiColour, pTarget );
     }
}



// Draw a rectangle on the specified surface
void BaseEngine::DrawRectangle(int iX1, int iY1, int iX2, int iY2, 
									  unsigned int uiColour, SDL_Surface* pTarget)
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	if ( iX2 < iX1 ) { int t = iX1; iX1 = iX2; iX2 = t; }
	if ( iY2 < iY1 ) { int t = iY1; iY1 = iY2; iY2 = t; }

	for ( int iX = iX1 ; iX <= iX2 ; iX++ )
		for ( int iY = iY1 ; iY <= iY2 ; iY++ )
			SafeSetPixel( iX, iY, uiColour, pTarget );
}

// Draw an oval on the specified surface
void BaseEngine::DrawOval(int iX1, int iY1, int iX2, int iY2, 
									unsigned int uiColour, SDL_Surface* pTarget)
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	if ( iX2 < iX1 ) { int t = iX1; iX1 = iX2; iX2 = t; }
	if ( iY2 < iY1 ) { int t = iY1; iY1 = iY2; iY2 = t; }

	double fCentreX = ((double)(iX2+iX1))/2.0;
	double fCentreY = ((double)(iY2+iY1))/2.0;
	double fXFactor = (double)((iX2-iX1) * (iX2-iX1))/4.0;
	double fYFactor = (double)((iY2-iY1) * (iY2-iY1))/4.0;
	double fDist;
	
	for ( int iX = iX1 ; iX <= iX2 ; iX++ )
		for ( int iY = iY1 ; iY <= iY2 ; iY++ )
		{
			fDist = ((double)iX - fCentreX) * ((double)iX - fCentreX)/fXFactor
				+ ((double)iY - fCentreY) * ((double)iY - fCentreY)/fYFactor;
			if ( fDist <= 1.0 )
				SafeSetPixel( iX, iY, uiColour, pTarget );
		}
}

// Draw an oval on the specified surface
void BaseEngine::DrawHollowOval(	int iX1, int iY1, int iX2, int iY2, 
									int iX3, int iY3, int iX4, int iY4, 
									unsigned int uiColour, SDL_Surface* pTarget)
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	if ( iX2 < iX1 ) Swap( iX1, iX2 );
	if ( iY2 < iY1 ) Swap( iY1, iY2 );
	if ( iX4 < iX3 ) Swap( iX3, iX4 );
	if ( iY4 < iY3 ) Swap( iY3, iY4 );

	double fCentreX1 = ((double)(iX2+iX1))/2.0;
	double fCentreY1 = ((double)(iY2+iY1))/2.0;
	double fXFactor1 = (double)((iX2-iX1) * (iX2-iX1))/4.0;
	double fYFactor1 = (double)((iY2-iY1) * (iY2-iY1))/4.0;
	double fCentreX2 = ((double)(iX4+iX3))/2.0;
	double fCentreY2 = ((double)(iY4+iY3))/2.0;
	double fXFactor2 = (double)((iX4-iX3) * (iX4-iX3))/4.0;
	double fYFactor2 = (double)((iY4-iY3) * (iY4-iY3))/4.0;
	double fDist1, fDist2;
	
	for ( int iX = iX1 ; iX <= iX2 ; iX++ )
		for ( int iY = iY1 ; iY <= iY2 ; iY++ )
		{
			fDist1 = ((double)iX - fCentreX1) * ((double)iX - fCentreX1)/fXFactor1
				+ ((double)iY - fCentreY1) * ((double)iY - fCentreY1)/fYFactor1;
			fDist2 = ((double)iX - fCentreX2) * ((double)iX - fCentreX2)/fXFactor2
				+ ((double)iY - fCentreY2) * ((double)iY - fCentreY2)/fYFactor2;
			if ( ( fDist1 <= 1.0 ) && ( fDist2 >= 1.0 ) )
				SafeSetPixel( iX, iY, uiColour, pTarget );
		}
}


// Draw a line on the specified surface
void BaseEngine::DrawLine(double fX1, double fY1, double fX2, double fY2, 
						unsigned int uiColour, SDL_Surface* pTarget)
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	int iX1 = (int)(fX1+0.5);
	int iX2 = (int)(fX2+0.5);
	int iY1 = (int)(fY1+0.5);
	int iY2 = (int)(fY2+0.5);

	int iSteps = (iX2-iX1);
	if ( iSteps < 0 ) iSteps = -iSteps;
	if ( iY2 > iY1 ) iSteps += (iY2-iY1); else iSteps += (iY1-iY2);
	iSteps+=2;

	double fXStep = ((double)(fX2-fX1))/iSteps;
	double fYStep = ((double)(fY2-fY1))/iSteps;

	for ( int i = 0 ; i <= iSteps ; i++ )
	{
		SafeSetPixel( (int)(0.5 + fX1 + fXStep*i), (int)(0.5 + fY1 + fYStep*i), uiColour, pTarget );
	}
}



// Draw a thick line on the specified surface
void BaseEngine::DrawThickLine( double fX1, double fY1, double fX2, double fY2, 
							 unsigned int uiColour, int iThickness, SDL_Surface* pTarget)
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	if ( iThickness < 2 )
	{ // Go to the quicker draw function
		DrawLine(fX1, fY1, fX2, fY2, uiColour, pTarget);
		return;
	}

	double fAngle1 = GetAngle( fX1, fY1, fX2, fY2 );
	double fAngle1a = fAngle1 - ((5 * M_PI) / 4.0);
	double fAngle1b = fAngle1 + ((5 * M_PI) / 4.0);
	double fRectX1 = fX1 + iThickness * cos(fAngle1a) * 0.5;
	double fRectY1 = fY1 + iThickness * sin(fAngle1a) * 0.5;
	double fRectX2 = fX1 + iThickness * cos(fAngle1b) * 0.5;
	double fRectY2 = fY1 + iThickness * sin(fAngle1b) * 0.5;

	double fAngle2 = fAngle1 + M_PI;
	double fAngle2a = fAngle2 - ((5 * M_PI) / 4.0);
	double fAngle2b = fAngle2 + ((5 * M_PI) / 4.0);
	double fRectX3 = fX2 + iThickness * cos(fAngle2a) * 0.5;
	double fRectY3 = fY2 + iThickness * sin(fAngle2a) * 0.5;
	double fRectX4 = fX2 + iThickness * cos(fAngle2b) * 0.5;
	double fRectY4 = fY2 + iThickness * sin(fAngle2b) * 0.5;

	DrawTriangle( fRectX1, fRectY1, fRectX2, fRectY2, fRectX3, fRectY3, uiColour, pTarget );
	DrawTriangle( fRectX3, fRectY3, fRectX4, fRectY4, fRectX1, fRectY1, uiColour, pTarget );
}



// Draw a polygon on the specified surface
void BaseEngine::DrawPolygon( 
		int iPoints, double* pXArray, double* pYArray,
		unsigned int uiColour, SDL_Surface* pTarget)
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	if ( iPoints == 1 )
	{
		SafeSetPixel( pXArray[0], pYArray[0], uiColour, pTarget );
		return;
	}

	if ( iPoints == 2 )
	{
		DrawLine( pXArray[0], pYArray[0], pXArray[1], pYArray[1], uiColour, pTarget );
		return;
	}

/*	if ( iPoints == 3 )
	{
		printf( "Draw triangle for points 0, 1, 2 of %d available\n", iPoints );
		DrawTriangle( pXArray[0], pYArray[0], pXArray[1], pYArray[1], pXArray[2], pYArray[2],
				uiColour, pTarget );
		return;
	}
*/

	// Otherwise attempt to eliminate a point by filling the polygon, then call this again
	double fXCentre, fYCentre; //fX1, fX2, fX3, fY1, fY2, fY3;
	int i2, i3;
	double fAngle1, fAngle2, fAngle3;

	for ( int i1 = 0 ; i1 < iPoints ; i1++ )
	{
		i2 = i1 + 1; if ( i2 >= iPoints ) i2 -= iPoints;
		i3 = i1 + 2; if ( i3 >= iPoints ) i3 -= iPoints;
		fXCentre = (pXArray[i1] + pXArray[i2] + pXArray[i3]) / 3.0;
		fYCentre = (pYArray[i1] + pYArray[i2] + pYArray[i3]) / 3.0;
		fAngle1 = GetAngle( fXCentre, fYCentre, pXArray[i1], pYArray[i1] );
		fAngle2 = GetAngle( fXCentre, fYCentre, pXArray[i2], pYArray[i2] );
		fAngle3 = GetAngle( fXCentre, fYCentre, pXArray[i3], pYArray[i3] );
		// Now work out the relative angle positions and make sure all are positive
		fAngle2 -= fAngle1; if ( fAngle2 < 0 ) fAngle2 += 2*M_PI;
		fAngle3 -= fAngle1; if ( fAngle3 < 0 ) fAngle3 += 2*M_PI;
		if ( fAngle2 < fAngle3 )
		{ // Then points are in clockwise order so central one can be eliminated as long as we don't
			// fill an area that we shouldn't
			bool bPointIsWithinTriangle = false;
			if ( iPoints > 3 )
			{ // Need to check that there isn't a point within the area - for convex shapes
				double fLineAngle12 = GetAngle( pXArray[i1], pYArray[i1], pXArray[i2], pYArray[i2] );
				if ( fLineAngle12 < 0 )
					fLineAngle12 += M_PI * 2.0;
				double fLineAngle23 = GetAngle( pXArray[i2], pYArray[i2], pXArray[i3], pYArray[i3] );
				if ( fLineAngle23 < 0 )
					fLineAngle23 += M_PI * 2.0;
				double fLineAngle31 = GetAngle( pXArray[i3], pYArray[i3], pXArray[i1], pYArray[i1] );
				if ( fLineAngle31 < 0 )
					fLineAngle31 += M_PI * 2.0;

				for ( int i = i3+1 ; i != i1 ; i++ )
				{
					if ( i >= iPoints )
					{
						i = 0;
						if ( i1 == 0 )
							break; // From the for loop - finished
					}

					// Otherwise we need to work out whether point i is to right of line  i3 to i1
					double fPointAngle1 = GetAngle( pXArray[i1], pYArray[i1], pXArray[i], pYArray[i] );
					if ( fPointAngle1 < 0 )
						fPointAngle1 += M_PI * 2.0;
					fPointAngle1 -= fLineAngle12;
					if ( fPointAngle1 < 0 )
						fPointAngle1 += M_PI * 2.0;

					double fPointAngle2 = GetAngle( pXArray[i2], pYArray[i2], pXArray[i], pYArray[i] );
					if ( fPointAngle2 < 0 )
						fPointAngle2 += M_PI * 2.0;
					fPointAngle2 -= fLineAngle23;
					if ( fPointAngle2 < 0 )
						fPointAngle2 += M_PI * 2.0;

					double fPointAngle3 = GetAngle( pXArray[i3], pYArray[i3], pXArray[i], pYArray[i] );
					if ( fPointAngle3 < 0 )
						fPointAngle3 += M_PI * 2.0;
					fPointAngle3 -= fLineAngle31;
					if ( fPointAngle3 < 0 )
						fPointAngle3 += M_PI * 2.0;

					if ( ( fPointAngle1 < M_PI ) && ( fPointAngle2 < M_PI ) && ( fPointAngle3 < M_PI ) )
						bPointIsWithinTriangle = true;
				}
			}

			if ( !bPointIsWithinTriangle )
			{// If not then try the next position
				//printf( "Draw for points %d, %d, %d of %d available\n", i1, i2, i3, iPoints );
				DrawTriangle( pXArray[i1], pYArray[i1], pXArray[i2], pYArray[i2], 
							pXArray[i3], pYArray[i3], /*GetColour(iPoints)*/uiColour, pTarget );
				// Remove the point i2 and then recurse			
				for ( int i = i2 ; i < (iPoints-1) ; i++ )
				{
					//printf( "\tCopy point %d to %d\n", i+1, i );
					pXArray[i] = pXArray[i+1];
					pYArray[i] = pYArray[i+1];
				}
				if ( iPoints > 3 )
					DrawPolygon( iPoints - 1, pXArray, pYArray, uiColour, pTarget );
				return; // Done
			}
		}
	}
}

/* Added in 2014 since it was used in the GroundMovement playback program so may be useful elsewhere too, but students can ignore this */
void BaseEngine::DrawShortenedArrow( int iX1,int iY1,int iX2,int iY2,
	int iShortenedStart,int iShortenedEnd,
	unsigned int uiColour,int iThickness,
	int iHeadSize,SDL_Surface* pTarget )
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	//iShortenedStart += 10; // Test
	//iShortenedEnd += 10; // Test

	double dAngle1 = GetAngle( iX1,iY1,iX2,iY2 );
	double dAngle2 = dAngle1 + M_PI;

	double dX1 = iX1 + iShortenedStart * cos( dAngle1 );
	double dY1 = iY1 + iShortenedStart * sin( dAngle1 );
	double dX2 = iX2 + iShortenedEnd * cos( dAngle2 );
	double dY2 = iY2 + iShortenedEnd * sin( dAngle2 );

	// First draw the line
	if ( iThickness < 2 )
	{ // Go to the quicker draw function
		DrawLine( dX1,dY1,dX2,dY2,uiColour,pTarget );
	}
	else
	{
		double dX1l = iX1 + iShortenedStart * cos( dAngle1 );
		double dY1l = iY1 + iShortenedStart * sin( dAngle1 );
		double dX2l = iX2 + (iShortenedEnd + iThickness*1.5) * cos( dAngle2 );
		double dY2l = iY2 + (iShortenedEnd + iThickness*1.5) * sin( dAngle2 );

		DrawThickLine( dX1l,dY1l,dX2l,dY2l,uiColour,iThickness,pTarget );
	}

	// Now draw the arrow head.
	// Need three points - one is end of line and others are at 60 degrees from it.

	DrawTriangle(
		dX2,dY2,
		dX2 + iHeadSize * cos( dAngle2 + M_PI / 6.0 ),dY2 + iHeadSize * sin( dAngle2 + M_PI / 6.0 ),
		dX2 + iHeadSize * cos( dAngle2 - M_PI / 6.0 ),dY2 + iHeadSize * sin( dAngle2 - M_PI / 6.0 ),
		uiColour,pTarget );
}

/* Added in 2014 since it was used in the GroundMovement playback program so may be useful elsewhere too, but students can ignore this */
void BaseEngine::DrawShortenedLine( int iX1,int iY1,int iX2,int iY2,
	int iShortenedStart,int iShortenedEnd,
	unsigned int uiColour,int iThickness,SDL_Surface* pTarget )
{
	if ( pTarget == NULL )
		pTarget = m_pActualScreen;

	double dAngle1 = GetAngle( iX1,iY1,iX2,iY2 );
	double dAngle2 = dAngle1 + M_PI;

	// First draw the line
	if ( iThickness < 2 )
	{ // Go to the quicker draw function
		double dX1 = iX1 + iShortenedStart * cos( dAngle1 );
		double dY1 = iY1 + iShortenedStart * sin( dAngle1 );
		double dX2 = iX2 + iShortenedEnd * cos( dAngle2 );
		double dY2 = iY2 + iShortenedEnd * sin( dAngle2 );

		DrawLine( dX1,dY1,dX2,dY2,uiColour,pTarget );
	}
	else
	{
		double dX1l = iX1 + iShortenedStart * cos( dAngle1 );
		double dY1l = iY1 + iShortenedStart * sin( dAngle1 );
		double dX2l = iX2 + iShortenedEnd * cos( dAngle2 );
		double dY2l = iY2 + iShortenedEnd * sin( dAngle2 );

		if ( iX1 == iX2 )
			printf( "Draw shortened line %d,%d to %d,%d shortened by %d,%d is %f,%f %f,%f\n",
			iX1,iY1,iX2,iY2,iShortenedStart,iShortenedEnd,
			dX1l,dY1l,dX2l,dY2l );

		DrawThickLine( dX1l,dY1l,dX2l,dY2l,uiColour,iThickness,pTarget );
	}
}

void BaseEngine::NotifyAllObjects( int iSignalNumber )
{
	if ( m_ppDisplayableObjects != NULL )
	{
		for ( int i = 0; m_ppDisplayableObjects[i] != NULL; i++ )
		{
			m_ppDisplayableObjects[i]->Notify( iSignalNumber );
		}
	}
}

/* Send a specified notification value to all displayable objects and count the number which give a non-zero response. */
int BaseEngine::NotifyAllObjectsGetCountNonZero( int iSignalNumber )
{
	int iReturn = 0;
	if ( m_ppDisplayableObjects != NULL )
	{
		for ( int i = 0; m_ppDisplayableObjects[i] != NULL; i++ )
		{
			if ( m_ppDisplayableObjects[i]->Notify( iSignalNumber ) != 0 )
				iReturn++;
		}
	}
	return iReturn;
}

/* Send a specified notification value to all displayable objects and return the sum of the returned values. */
int BaseEngine::NotifyAllObjectsGetSum( int iSignalNumber )
{
	int iReturn = 0;
	if ( m_ppDisplayableObjects != NULL )
	{
		for ( int i = 0; m_ppDisplayableObjects[i] != NULL; i++ )
		{
			iReturn += m_ppDisplayableObjects[i]->Notify( iSignalNumber );
		}
	}
	return iReturn;
}

/* Send a specified notification value to all displayable objects and return the largest of the returned values. */
int BaseEngine::NotifyAllObjectsGetMax( int iSignalNumber )
{
	int iReturn = INT_MIN;
	if ( m_ppDisplayableObjects != NULL )
	{
		for ( int i = 0; m_ppDisplayableObjects[i] != NULL; i++ )
		{
			int ival = m_ppDisplayableObjects[i]->Notify( iSignalNumber );
			if ( ival > iReturn )
				iReturn = ival;
		}
	}
	return iReturn;
}

/* Send a specified notification value to all displayable objects and return the smallest of the returned values. */
int BaseEngine::NotifyAllObjectsGetMin( int iSignalNumber )
{
	int iReturn = INT_MAX;
	if ( m_ppDisplayableObjects != NULL )
	{
		for ( int i = 0; m_ppDisplayableObjects[i] != NULL; i++ )
		{
			int ival = m_ppDisplayableObjects[i]->Notify( iSignalNumber );
			if ( ival < iReturn )
				iReturn = ival;
		}
	}
	return iReturn;
}

