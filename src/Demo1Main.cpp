#include "header.h"

#include "Demo1Main.h"

Demo1Main::Demo1Main()
: BaseEngine( 50 )
{
}

Demo1Main::~Demo1Main()
{
}

/* Fill the background - should be overridden to actually draw the correct thing. */
void Demo1Main::SetupBackgroundBuffer()
{
	// Put any special code to call different render functions for different states here
	FillBackground( 0xff00ff );
}
