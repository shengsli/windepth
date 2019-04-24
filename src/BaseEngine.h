#ifndef BASEGAMEENGINE_H
#define BASEGAMEENGINE_H

// Prototype the class, so we can use pointers of type DisplayableObject*
class DisplayableObject;

#include "FontManager.h"

#include <assert.h>

class BaseEngine
{
public:
	/* Constructor. */
	BaseEngine(int iUnused = 0);

	/* Virtual destructor */
	virtual ~BaseEngine(void);

	//
	// Internal functions
	//

	/* Initialise the SDL and create a window of the designated size */
	int Initialise(const char* strCaption, int iScreenWidth, int iScreenHeight, const char* szFontName, int iFontSize);

	/* Deinitialise everything required. */
	void Deinitialise(void);

	/* The main game loop - will call the draw and gamel logic functions in turn */
	int MainLoop(void);

	/* Destroy any existing displayable objects */
	virtual void DestroyOldObjects();

	/* Create array to store objects */
	void CreateObjectArray(int iNumberObjects);


	/* Store an object into an array element */
	void StoreObjectInArray(int iIndex, DisplayableObject* pObject);


public:
	//
	// Utility functions: (call these)
	//

	/*
	Is the key with the specified code currently being pressed?
	*/
	bool IsKeyPressed(int iKeyCode) const;

	/* Last clicked X coordinate of mouse - button down */
	int GetMouseXClickedDown() const { return m_iMouseXClickedDown; }
	/* Last clicked Y coordinate of mouse - button down */
	int GetMouseYClickedDown() const { return m_iMouseYClickedDown; }

	/* Last clicked X coordinate of mouse - button up */
	int GetMouseXClickedUp() const { return m_iMouseXClickedUp; }
	/* Last clicked Y coordinate of mouse - button up */
	int GetMouseYClickedUp() const { return m_iMouseYClickedUp; }

	/* Current X coordinate of mouse */
	int GetCurrentMouseX() const { return m_iCurrentMouseX; }
	/* Current Y coordinate of mouse */
	int GetCurrentMouseY() const { return m_iCurrentMouseY; }

	/* Current states of buttons */
	int GetCurrentButtonStates() const { return m_iCurrentButtonStates; }

	/*
	Ask SDL for current details about mouse position and buttons.
	Shouldn't be needed since the information is already cached and available using the functions above.
	*/
	void UpdateMouseInfo() { m_iCurrentButtonStates = SDL_GetMouseState(&m_iCurrentMouseX, &m_iCurrentMouseY); }

	/*
	Function to get the time, in terms of the number of ticks.
	Can be used to decide which animation to display, or whether it's time to do something yet
	*/
	inline int GetTime() const { return m_iTick; }

	/*
	Function to get the time, in terms of the number of ticks.
	This version allows the subclass to change the time for pausing etc.
	The GetTime() function is NOT virtual, this function is.
	This also adds on a time offset, which you can use/modify/set appropriately to handle pause, for example.
	*/
	virtual int GetModifiedTime() const { return m_iTick + m_iTimeOffset; }

	/* Return true if it is time to act, false otherwise */
	inline bool IsTimeToAct() const { return m_iTick >= m_iNextTickToActOn; }

	/* Returns true if it is now time to act (should always be the case) or false otherwise, with a 9ms sleep if the delay is longer than that. */
	inline bool IsTimeToActWithSleep() const
	{
		if (m_iTick >= m_iNextTickToActOn)
			return true;
		if (m_iNextTickToActOn - m_iTick >= 9)
			SDL_Delay(9);
		return m_iTick >= m_iNextTickToActOn;
	}

	/* Set the time offset which is added to returned ModifiedTimes */
	void SetTimeOffset(int iNewOffset) { m_iTimeOffset = iNewOffset; }
	/* Change (increase if positive value sent, decrease if -ve value used) the time offset which is added to returned ModifiedTimes */
	void IncreaseTimeOffset(int iAddedValue) { m_iTimeOffset += iAddedValue; }
	/** Retrieve the current time offset which is used by GetModifiedTime() */
	int GetTimeOffset() { return m_iTimeOffset; }

	/* Specify the next time at which to act - current time plus the delay, in ticks */
	inline void SetTimeToAct(int iDelay) { m_iNextTickToActOn = m_iTick + iDelay; }

	/*
	Gets the specified displayable object.
	You may need to dynamic_cast the resulting pointer to the correct type.
	*/
	DisplayableObject* GetDisplayableObject(int iIndex);


	/* Set the exit code - effectively ending the program! */
	void SetExitWithCode(int iCode) { m_iExitWithCode = iCode; }

	/* Indicate that either the screen or just specific regions of it must be redrawn */
	void Redraw(bool bAllScreen) { m_bNeedsRedraw = true; if (bAllScreen) m_bWholeScreenUpdate = true; }

	/* Get (possibly loading) a specific font */
	Font* GetFont(char* szFontName, int iFontSize)
	{
		return m_oFontManager.GetFont(szFontName, iFontSize);
	}

	/* Called when the drawable objects array is changed, to avoid using the array after it has changed.
	Basically, some functions may maintain a reference into it and need to know if the thing they point to was destroyed. */
	void DrawableObjectsChanged() { m_iDrawableObjectsChanged = 1; }

public:
	//
	// Overridables:
	//

	/* Override if necessary - a key was released. See also IsKeyPressed() */
	virtual void KeyUp(int iKeyCode);
	/* Override if necessary - a key was pressed. See also IsKeyPressed() */
	virtual void KeyDown(int iKeyCode);

	/* Override if necessary - Mouse event - mouse was moved */
	virtual void MouseMoved( int iX, int iY );
	/* Override if necessary - Mouse event - mouse button pressed */
	virtual void MouseDown( int iButton, int iX, int iY );
	/* Override if necessary - Mouse event - mouse button released */
	virtual void MouseUp( int iButton, int iX, int iY );

	/* Override if necessary - handle initialisation */
	virtual int GameInit(void);

	/* Override if necessary - handle cleanup - e.g. delete allocated objects */
	virtual void CleanUp(void);

	/*
	In here you need to create any movable objects that you wish to use.
	Sub-classes need to implement this function.
	*/
	virtual int InitialiseObjects();

	/*
	You probably need to override this:
	Do any setup of back buffer prior to locking the screen buffer.
	Basically do the drawing of the background in here and it'll be copied to the screen for you as needed.
	*/
	virtual void SetupBackgroundBuffer();

	/*
	You may need to override this.
	The game logic - move things and change the state if necessary.
	Must call Redraw(true/false) if anything changes that shows on the screen.
	*/
	virtual void GameAction();

	/* You probably need to override this: Draw text labels for the screen */
	virtual void UnDrawStrings();
	virtual void DrawStringsUnderneath();
	virtual void DrawStringsOnTop();
	/* You can do everything in this one if you wish, ignoring the three above. */
	virtual void DrawStrings();

	/* 
	You probably don't need to change this.
	Render the screen. 
	Base version delegates to DrawScreen() or DrawChanges() as appropriate, so you can probably override those instead. 
	*/
	virtual void GameRender();

	/* 
	You probably don't need to change this, but you may need to disable it - see demo 4.
	Redraws the background behind each of the displayable objects, in their old positions.
	*/
	virtual void UndrawObjects();

	/* 
	Draw just the changing objects. 
	You probably don't need to change this, but you may need to disable it - see demo 4.
	This just asks the objects to draw themselves.
	*/
	virtual void DrawObjects();

	/* Tell all displayable objects to update themselves. Calls DoUpdate on each displayable object. */
	virtual void UpdateAllObjects( int iCurrentTime );


	/* The notify system provides a simple way to tell all objects about something and potentially to get a return value and report on it.
	   When NotifeAllObjects...() is called it will send the specified signal number to each object, which can then respond with an integer response value.
	   The signal code is entirely defined by you - you can use whatever numbers you want.
	   What the return code is used for will depend upon which function is used. E.g. it may be ignored, or the sum, maximum or minimum values may be returned to the caller.
	 */

	/* Send a specified notification value to all displayable objects. */
	void NotifyAllObjects( int iSignalNumber );

	/* Send a specified notification value to all displayable objects and count the number which give a non-zero response. */
	int NotifyAllObjectsGetCountNonZero( int iSignalNumber );

	/* Send a specified notification value to all displayable objects and return the sum of the returned values. */
	int NotifyAllObjectsGetSum( int iSignalNumber );

	/* Send a specified notification value to all displayable objects and return the largest of the returned values. */
	int NotifyAllObjectsGetMax( int iSignalNumber );

	/* Send a specified notification value to all displayable objects and return the smallest of the returned values. */
	int NotifyAllObjectsGetMin( int iSignalNumber );

	//
	// Functions to access the drawing surface
	//

	/* Get a pointer to the SDL surface for the screen */
	SDL_Surface * GetSDLSurface() { return m_pActualScreen; }

	// Get a pointer to the foreground surface
	SDL_Surface * GetForeground() { return m_pActualScreen; }
	// Get a pointer to the background surface
	SDL_Surface * GetBackground() { return m_pBackgroundSurface; }


	//
	// Drawing functions
	//

public:

	/* Screen width */
	int GetScreenWidth() const { return m_iScreenWidth; }

	/* Screen height */
	int GetScreenHeight() const { return m_iScreenHeight; }

	/* Method to fill the entire background with the same colour */
	virtual void FillBackground(unsigned int uiPixelColour);

	/* Copy all of the background buffer onto the screen */
	void CopyAllBackgroundBuffer();

	/* Copy the background pixels in the specified region onto the screen */
	void CopyBackgroundPixels( int iX, int iY, int iWidth, int iHeight );

	/* 
	Get a colour code for a specific index.
	Prevents having to reinvent colours, you can just use a number.
	*/
	unsigned int GetColour(int iColourIndex) const;

	/* Method to set a pixel colour on the screen, in-lined, but checks the supplied parameters */
	inline void SafeSetScreenPixel( int iX, int iY, unsigned int uiColour )
	{
		SafeSetPixel( iX, iY, uiColour, m_pActualScreen );
	}

	/* Method to set a pixel colour on the screen, in-lined, but checks the supplied parameters */
	inline void SafeSetBackgroundPixel( int iX, int iY, unsigned int uiColour )
	{
		SafeSetPixel( iX, iY, uiColour, m_pBackgroundSurface );
	}

	/* Set a pixel on a specific surface */
	inline void SafeSetPixel( int iX, int iY, unsigned int uiColour, SDL_Surface* pTarget )
	{
		if ( pTarget == NULL )
			pTarget = m_pActualScreen;

		// Validate that the position is within the screen
		if ( (iX < 0) || (iX >= GetIntsPerScreenRow( pTarget ) ) || (iY < 0) || (iY >= m_iScreenHeight) )
			return;

		// Set the value of the pixel
		((unsigned int *)pTarget->pixels)[iX + iY * GetIntsPerScreenRow( pTarget ) ] = uiColour;
	}

	/* Method to get a pixel colour on the screen, in-lined, but checks the supplied parameters.
	Added by JAA in response to request in labs 2013/2014 by someone doing collision detection against a background. */
	inline unsigned int SafeGetScreenPixel(int iX, int iY) const
	{
		SafeGetPixel(iX, iY, m_pActualScreen);
	}

	/* Method to set a pixel colour on the screen, in-lined, but checks the supplied parameters
	Added by JAA in response to request in labs 2013 / 2014 by someone doing collision detection against a background. */
	inline unsigned int SafeGetBackgroundPixel(int iX, int iY) const
	{
		SafeGetPixel(iX, iY, m_pBackgroundSurface);
	}

	/* Set a pixel on a specific surface
	Added by JAA in response to request in labs 2013/2014 by someone doing collision detection against a background. */
	inline unsigned int SafeGetPixel(int iX, int iY, SDL_Surface* pTarget) const
	{
		if (pTarget == NULL)
			pTarget = m_pActualScreen;

		// Validate that the position is within the screen
		if ( (iX < 0) || (iX >= GetIntsPerScreenRow( pTarget )) || (iY < 0) || (iY >= m_iScreenHeight) )
			return 0;

		// Get the value of the pixel
		return ((unsigned int *)pTarget->pixels)[iX + iY * GetIntsPerScreenRow( pTarget )];
	}

	/* Method to set a pixel colour on the screen, fast and in-lined. */
	inline void SetScreenPixel( int iX, int iY, unsigned int uiColour )
	{
		assert( iX >= 0 );
		assert( iX < m_iScreenWidth);
		assert( iY >= 0 );
		assert( iY < m_iScreenHeight );
		SetPixel( iX, iY, uiColour, m_pActualScreen );
	}

	/* Method to set a pixel colour in the background buffer */
	inline void SetBackgroundPixel( int iX, int iY, unsigned int uiColour )
	{
		assert( iX >= 0 );
		assert( iX < m_iScreenWidth);
		assert( iY >= 0 );
		assert( iY < m_iScreenHeight );
		SetPixel( iX, iY, uiColour, m_pBackgroundSurface );
	}

	// Set a pixel on a specific surface
	inline void SetPixel( int iX, int iY, unsigned int uiColour, SDL_Surface* pTarget )
	{
		assert( iX >= 0 );
		assert( iX < GetIntsPerScreenRow( pTarget ) );
		assert( iY >= 0 );
		assert( iY < m_iScreenHeight );
		// Set the value of the pixel
		((unsigned int *)pTarget->pixels)[iX + iY * GetIntsPerScreenRow( pTarget )] = uiColour;
	}

	// Draw a line on the foreground
	void DrawScreenLine(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
	{
		DrawLine( iX1, iY1, iX2, iY2, uiColour, m_pActualScreen );
	}

	// Draw a line on the background
	void DrawBackgroundLine(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
	{
		DrawLine( iX1, iY1, iX2, iY2, uiColour, m_pBackgroundSurface );
	}

	// Draw a line
	void DrawLine(double fX1, double fY1, double fX2, double fY2, unsigned int uiColour, SDL_Surface* pTarget = NULL);

	// Draw a thick line on the foreground
	void DrawScreenThickLine(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour, int iThickness)
	{
		DrawThickLine( iX1, iY1, iX2, iY2, uiColour, iThickness, m_pActualScreen );
	}

	// Draw a thick line on the background
	void DrawBackgroundThickLine(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour, int iThickness)
	{
		DrawThickLine( iX1, iY1, iX2, iY2, uiColour, iThickness, m_pBackgroundSurface );
	}

	// Draw a thick line
	void DrawThickLine(double fX1, double fY1, double fX2, double fY2, unsigned int uiColour, int iThickness, SDL_Surface* pTarget = NULL);

	// Draw a region which has vertical sides on the foreground - one side may be a point, making a triangle
	void DrawScreenVerticalSidedRegion(
		double fX1, double fX2,
		double fY1a, double fY2a,
		double fY1b, double fY2b, 
		unsigned int uiColour )
	{
		DrawVerticalSidedRegion( fX1, fX2, fY1a, fY2a, fY1b, fY2b, uiColour, m_pActualScreen );
	}

	// Draw a region which has vertical sides on the background - one side may be a point, making a triangle
	void DrawBackgroundVerticalSidedRegion(
		double fX1, double fX2,
		double fY1a, double fY2a,
		double fY1b, double fY2b, 
		unsigned int uiColour )
	{
		DrawVerticalSidedRegion( fX1, fX2, fY1a, fY2a, fY1b, fY2b, uiColour, m_pBackgroundSurface );
	}

	// Draw a region which has vertical sides - one side may be a point, making a triangle
	void DrawVerticalSidedRegion(
		double fX1, double fX2,
		double fY1a, double fY2a,
		double fY1b, double fY2b, 
		unsigned int uiColour, SDL_Surface* pTarget = NULL );

	// Draw a triangle on the foreground where all three points are specified.
	void DrawScreenTriangle(
		int iX1, int iY1,
		int iX2, int iY2,
		int iX3, int iY3, 
		unsigned int uiColour )
	{
		DrawTriangle( iX1, iY1, iX2, iY2, iX3, iY3, uiColour, m_pActualScreen );
	}

	// Draw a triangle on the background where all three points are specified.
	void DrawBackgroundTriangle(
		int iX1, int iY1,
		int iX2, int iY2,
		int iX3, int iY3,
		unsigned int uiColour )
	{
		DrawTriangle( iX1, iY1, iX2, iY2, iX3, iY3, uiColour, m_pBackgroundSurface );
	}


	// Draw a triangle where all three points are specified.
	void DrawTriangle(
			double fX1, double fY1,
			double fX2, double fY2,
			double fX3, double fY3, 
			unsigned int uiColour, 
			SDL_Surface* pTarget = NULL );

	// Draw a rectangle on the foreground
	void DrawScreenRectangle(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
	{
		DrawRectangle( iX1, iY1, iX2, iY2, uiColour, m_pActualScreen );
	}

	// Draw a rectangle on the background
	void DrawBackgroundRectangle(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
	{
		DrawRectangle( iX1, iY1, iX2, iY2, uiColour, m_pBackgroundSurface );
	}

	// Draw a rectangle
	void DrawRectangle(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour, SDL_Surface* pTarget = NULL);

	// Draw an oval on the foreground
	void DrawScreenOval(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
	{
		DrawOval( iX1, iY1, iX2, iY2, uiColour, m_pActualScreen );
	}

	// Draw an oval on the background
	void DrawBackgroundOval(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
	{
		DrawOval( iX1, iY1, iX2, iY2, uiColour, m_pBackgroundSurface );
	}

	// Draw an oval 
	void DrawOval(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour, SDL_Surface* pTarget = NULL);

	// Draw a hollow oval - i.e. just the edge
	void DrawHollowOval(	int iX1, int iY1, int iX2, int iY2, 
							int iX3, int iY3, int iX4, int iY4, 
							unsigned int uiColour, SDL_Surface* pTarget);

	// Draw a polygon on the screen
	void DrawScreenPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4, unsigned int uiColour )
	{
		double pXArray[4] = { fX1, fX2, fX3, fX4 };
		double pYArray[4] = { fY1, fY2, fY3, fY4 };
		DrawPolygon( 4, pXArray, pYArray, uiColour, m_pActualScreen );
	}

	// Draw a polygon on the screen
	void DrawScreenPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, unsigned int uiColour )
	{
		double pXArray[5] = { fX1, fX2, fX3, fX4, fX5 };
		double pYArray[5] = { fY1, fY2, fY3, fY4, fY5 };
		DrawPolygon( 5, pXArray, pYArray, uiColour, m_pActualScreen );
	}

	// Draw a polygon on the screen
	void DrawScreenPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6, unsigned int uiColour )
	{
		double pXArray[6] = { fX1, fX2, fX3, fX4, fX5, fX6 };
		double pYArray[6] = { fY1, fY2, fY3, fY4, fY5, fY6 };
		DrawPolygon( 6, pXArray, pYArray, uiColour, m_pActualScreen );
	}

	// Draw a polygon on the screen
	void DrawScreenPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6,
		double fX7, double fY7, unsigned int uiColour )
	{
		double pXArray[7] = { fX1, fX2, fX3, fX4, fX5, fX6, fX7 };
		double pYArray[7] = { fY1, fY2, fY3, fY4, fY5, fY6, fY7 };
		DrawPolygon( 7, pXArray, pYArray, uiColour, m_pActualScreen );
	}

	// Draw a polygon on the screen
	void DrawScreenPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6,
		double fX7, double fY7, double fX8, double fY8, unsigned int uiColour )
	{
		double pXArray[8] = { fX1, fX2, fX3, fX4, fX5, fX6, fX7, fX8 };
		double pYArray[8] = { fY1, fY2, fY3, fY4, fY5, fY6, fY7, fY8 };
		DrawPolygon( 8, pXArray, pYArray, uiColour, m_pActualScreen );
	}

	// Draw a polygon on the screen
	void DrawScreenPolygon( int iPoints, double* pXArray, double* pYArray, unsigned int uiColour )
	{
		DrawPolygon( iPoints, pXArray, pYArray, uiColour, m_pActualScreen );
	}

	// Draw a polygon on the background
	void DrawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4, unsigned int uiColour )
	{
		double pXArray[4] = { fX1, fX2, fX3, fX4 };
		double pYArray[4] = { fY1, fY2, fY3, fY4 };
		DrawPolygon( 4, pXArray, pYArray, uiColour, m_pBackgroundSurface );
	}

	// Draw a polygon on the background
	void DrawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, unsigned int uiColour )
	{
		double pXArray[5] = { fX1, fX2, fX3, fX4, fX5 };
		double pYArray[5] = { fY1, fY2, fY3, fY4, fY5 };
		DrawPolygon( 5, pXArray, pYArray, uiColour, m_pBackgroundSurface );
	}

	// Draw a polygon on the background
	void DrawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6, unsigned int uiColour )
	{
		double pXArray[6] = { fX1, fX2, fX3, fX4, fX5, fX6 };
		double pYArray[6] = { fY1, fY2, fY3, fY4, fY5, fY6 };
		DrawPolygon( 6, pXArray, pYArray, uiColour, m_pBackgroundSurface );
	}

	// Draw a polygon on the background
	void DrawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6,
		double fX7, double fY7, unsigned int uiColour )
	{
		double pXArray[7] = { fX1, fX2, fX3, fX4, fX5, fX6, fX7 };
		double pYArray[7] = { fY1, fY2, fY3, fY4, fY5, fY6, fY7 };
		DrawPolygon( 7, pXArray, pYArray, uiColour, m_pBackgroundSurface );
	}

	// Draw a polygon on the background
	void DrawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6,
		double fX7, double fY7, double fX8, double fY8, unsigned int uiColour )
	{
		double pXArray[8] = { fX1, fX2, fX3, fX4, fX5, fX6, fX7, fX8 };
		double pYArray[8] = { fY1, fY2, fY3, fY4, fY5, fY6, fY7, fY8 };
		DrawPolygon( 8, pXArray, pYArray, uiColour, m_pBackgroundSurface );
	}

	// Draw a polygon on the background
	void DrawBackgroundPolygon( int iPoints, double* pXArray, double* pYArray, unsigned int uiColour )
	{
		DrawPolygon( iPoints, pXArray, pYArray, uiColour, m_pBackgroundSurface );
	}

	// Draw a polygon on the supplied surface
	void DrawPolygon( int iPoints, double* pXArray, double* pYArray, unsigned int uiColour, SDL_Surface* pTarget);

	/* Draw a string to the background */
	void DrawBackgroundString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont = NULL )
	{
		DrawString( iX, iY, pText, uiColour, pFont, m_pBackgroundSurface );
	}

	/* Draw a string to the foreground */
	void DrawScreenString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont = NULL )
	{
		DrawString( iX, iY, pText, uiColour, pFont, m_pActualScreen );
	}

	/* Draw a string to the supplied surface */
	void DrawString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont, SDL_Surface* pTarget );

	void DrawScreenShortenedArrow( int iX1,int iY1,int iX2,int iY2,int iShortedStart,int iShortenedEnd,unsigned int uiColour,int iThickness,int iHeadSize )
	{
		DrawShortenedArrow( iX1,iY1,iX2,iY2,iShortedStart,iShortenedEnd,uiColour,iThickness,iHeadSize,m_pActualScreen );
	}

	void DrawBackgroundShortenedArrow( int iX1,int iY1,int iX2,int iY2,int iShortedStart,int iShortenedEnd,unsigned int uiColour,int iThickness,int iHeadSize )
	{
		DrawShortenedArrow( iX1,iY1,iX2,iY2,iShortedStart,iShortenedEnd,uiColour,iThickness,iHeadSize,m_pBackgroundSurface );
	}

	void DrawShortenedArrow( int iX1,int iY1,int iX2,int iY2,int iShortedStart,int iShortenedEnd,unsigned int uiColour,int iThickness,int iHeadSize,SDL_Surface* pTarget = NULL );

	void DrawScreenShortenedLine( int iX1,int iY1,int iX2,int iY2,int iShortedStart,int iShortenedEnd,unsigned int uiColour,int iThickness )
	{
		DrawShortenedLine( iX1,iY1,iX2,iY2,iShortedStart,iShortenedEnd,uiColour,iThickness,m_pActualScreen );
	}

	void DrawBackgroundShortenedLine( int iX1,int iY1,int iX2,int iY2,int iShortedStart,int iShortenedEnd,unsigned int uiColour,int iThickness )
	{
		DrawShortenedLine( iX1,iY1,iX2,iY2,iShortedStart,iShortenedEnd,uiColour,iThickness,m_pBackgroundSurface );
	}

	void DrawShortenedLine( int iX1,int iY1,int iX2,int iY2,
		int iShortenedStart,int iShortenedEnd,
		unsigned int uiColour,int iThickness,SDL_Surface* pTarget );

	static int SymbolToKeycode(int iSym)
	{
		if (iSym <= SDL12_SDLK_NORMAL)
			return iSym;

		switch (iSym)
		{
		case SDLK_UP: return SDL12_SDLK_NORMAL + 1;
		case SDLK_DOWN: return SDL12_SDLK_NORMAL + 2;
		case SDLK_LEFT: return SDL12_SDLK_NORMAL + 3;
		case SDLK_RIGHT: return SDL12_SDLK_NORMAL + 4;
		case SDLK_F1: return SDL12_SDLK_NORMAL + 5;
		case SDLK_F2: return SDL12_SDLK_NORMAL + 6;
		case SDLK_F3: return SDL12_SDLK_NORMAL + 7;
		case SDLK_F4: return SDL12_SDLK_NORMAL + 8;
		case SDLK_F5: return SDL12_SDLK_NORMAL + 9;
		case SDLK_F6: return SDL12_SDLK_NORMAL + 10;
		case SDLK_F7: return SDL12_SDLK_NORMAL + 11;
		case SDLK_F8: return SDL12_SDLK_NORMAL + 12;
		case SDLK_F9: return SDL12_SDLK_NORMAL + 13;
		case SDLK_F10: return SDL12_SDLK_NORMAL + 14;
		case SDLK_F11: return SDL12_SDLK_NORMAL + 15;
		case SDLK_F12: return SDL12_SDLK_NORMAL + 16;
		case SDLK_PRINTSCREEN: return SDL12_SDLK_NORMAL + 17;
		case SDLK_SCROLLLOCK: return SDL12_SDLK_NORMAL + 18;
		case SDLK_PAUSE: return SDL12_SDLK_NORMAL + 19;
		case SDLK_INSERT: return SDL12_SDLK_NORMAL + 20;
		case SDLK_HOME: return SDL12_SDLK_NORMAL + 21;
		case SDLK_PAGEUP: return SDL12_SDLK_NORMAL + 22;
		case SDLK_DELETE: return SDL12_SDLK_NORMAL + 23;
		case SDLK_END: return SDL12_SDLK_NORMAL + 24;
		case SDLK_PAGEDOWN: return SDL12_SDLK_NORMAL + 25;
		case SDLK_RETURN: return SDL12_SDLK_NORMAL + 26;
		case SDLK_BACKSPACE: return SDL12_SDLK_NORMAL + 27;
		case SDLK_ESCAPE: return SDL12_SDLK_NORMAL + 28;
		case SDLK_TAB: return SDL12_SDLK_NORMAL + 29;
		case SDLK_SPACE: return SDL12_SDLK_NORMAL + 30;
		case SDLK_CAPSLOCK: return SDL12_SDLK_NORMAL + 31;
		case SDLK_NUMLOCKCLEAR: return SDL12_SDLK_NORMAL + 32;
		case SDLK_KP_DIVIDE: return SDL12_SDLK_NORMAL + 33;
		case SDLK_KP_MULTIPLY: return SDL12_SDLK_NORMAL + 34;
		case SDLK_KP_MINUS: return SDL12_SDLK_NORMAL + 35;
		case SDLK_KP_PLUS: return SDL12_SDLK_NORMAL + 36;
		case SDLK_KP_ENTER: return SDL12_SDLK_NORMAL + 37;
		}

		return 0; // Unknown Symbol
	}

protected:

	/*
	If set to other than -1 the program will exit with this exit code.
	*/
	int m_iExitWithCode;

	/* Array of key status information saying what is currently pressed. See IsKeyPressed(). */
	bool *m_pKeyStatus;

	/* Screen width, in pixels */
	int m_iScreenWidth;

	/* Screen height, in pixels */
	int m_iScreenHeight;

	/* Screen surface, the SDL structure. */
	SDL_Surface *m_pActualScreen;

	/* Background surface */
	SDL_Surface *m_pBackgroundSurface;

	/* Window - used by SDL2 */
	SDL_Window *m_pSDL2Window;
	SDL_Renderer *m_pSDL2Renderer;
	SDL_Texture *m_pSDL2Texture;

	/* Number of ints per row of the screen. 
	Cached variable was deleted 2013/2014 in response to some problems a student had in labs with using their own different width textures,
	since it always used the actual screen surface.
	int m_iIntsPerScreenRow; 
	Was set to :
	m_iIntsPerScreenRow = m_pActualScreen->pitch / sizeof(unsigned int);
	*/
	int GetIntsPerScreenRow(SDL_Surface* pSurface) const 
	{
		return pSurface->pitch / sizeof(unsigned int);
	}
	/* version without a parameter, since I can't default to a member variable value */
	int GetIntsPerScreenRow() const
	{
		return m_pActualScreen->pitch / sizeof(unsigned int);
	}

	/* Current tick/time */
	int m_iTick;

	/* A time offset which can be used to modify the 'modifiedtime' returned to caller. */
	int m_iTimeOffset;

	/* true if screen needs a redraw, false if nothing has changed */
	bool m_bNeedsRedraw;

	/* True if whole screen should be updated on the next draw, false if only the moving objects should. */
	bool m_bWholeScreenUpdate;

	/* The next tick to act on - to prevent continuous updates */
	int m_iNextTickToActOn;

	/* Last clicked X coordinate of mouse - button down */
	int m_iMouseXClickedDown;
	/* Last clicked Y coordinate of mouse - button down */
	int m_iMouseYClickedDown;

	/* Last clicked X coordinate of mouse - button up */
	int m_iMouseXClickedUp;
	/* Last clicked Y coordinate of mouse - button up */
	int m_iMouseYClickedUp;

	/* Current X coordinate of mouse */
	int m_iCurrentMouseX;
	/* Current Y coordinate of mouse */
	int m_iCurrentMouseY;
	/* Current states of buttons */
	int m_iCurrentButtonStates;

private:
	// Set to 1 if drawable objects changed so any update should abort
	int m_iDrawableObjectsChanged;

	/* Array of displayable objects - expect it to be modified by sub-class(es) */
	DisplayableObject** m_ppDisplayableObjects;

	/* Manager object for the fonts - maintains a cache of fonts to prevent multiple loading of fonts. */
	FontManager m_oFontManager;
	
	/* Main display font for items. Often used if a font of NULL is specified. */
	Font* g_pMainFont;

	/* Flag - set when inside draw and surface would be locked */
	bool m_bInsideDraw;
};

#endif
