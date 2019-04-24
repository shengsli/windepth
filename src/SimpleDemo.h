#ifndef MYPROJECTMAIN_H
#define MYPROJECTMAIN_H

#include "BaseEngine.h"

/* This is a relatively simple demo but it illustrates a few things:
1) You can put source code anywhere - check out the contents of the .cpp file!
2) It illustrates the more complex way to draw static text, so that some appears on top of and some underneath the moving objects.
3) The object itself looks for key presses and alters its speed according to the pressing of the arrow keys or space
*/
class SimpleDemo : public BaseEngine
{
public:

	/**
	Constructor
	*/
	SimpleDemo()
	: BaseEngine( 6 )
	{}

	// Do any setup of back buffer prior to locking the screen buffer
	// Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
	virtual void SetupBackgroundBuffer();

	// Create any moving objects
	int InitialiseObjects();

	/** Draw any strings */
	void UnDrawStrings();
	void DrawStringsUnderneath();
	void DrawStringsOnTop();

	/**
	The game logic - move things and change the state if necessary.
	Must call Redraw(true/false) if anything changes that shows on the screen.
	*/
	virtual void GameAction();

	// Handle pressing of a mouse button
	void MouseDown( int iButton, int iX, int iY );

	// Handle pressing of a key
	virtual void KeyDown(int iKeyCode);
};

#endif
