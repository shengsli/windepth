#pragma once




class ImageData
{
public:
	ImageData()
		: m_iWidth(0), m_iHeight(0), m_aiPixels(NULL)
	{}

	~ImageData()
	{
		if ( m_aiPixels != NULL )
			delete[] m_aiPixels;
	}

	// Ask for the wisth of the image
	int GetWidth() const { return m_iWidth; }

	// Ask for the height of the image
	int GetHeight() const { return m_iHeight; }

	// Load the image with the specified filename
	bool LoadImage( const char* szFileName );

	// Has an image been loaded correctly
	bool IsLoaded() const { return m_aiPixels != NULL; }

	// Draw the image to the specified surface
	void RenderImage( SDL_Surface* pTarget, 
		int iXSource, int iYSource, 
		int iXTarget, int iYTarget, 
		int iWidth, int iHeight ) const;

	// Draw the image to the specified surface and assume that the bottom right pixel is the transparency colour.
	void RenderImageWithMask( SDL_Surface* pTarget, 
		int iXSource, int iYSource, 
		int iXTarget, int iYTarget, 
		int iWidth, int iHeight ) const;

	// Create a shrunk copy of another image
	void ShrinkFrom( const ImageData* pSource, int iReductionMultiplier );

	// Create a re-sized copy of another image
	void ResizeFrom( const ImageData* pSource, int iNewWidth, int iNewHeight, bool bJustSampleOnePixel );

	// Set some cropping for the image loading, e.g. to remove a border from an image
	// Used ONLY when loading images.
	static void SetGlobalImageLoadCropping( int iLeft,int iRight,int iTop,int iBottom )
	{
		g_iGlobalCropLeft = iLeft;
		g_iGlobalCropRight = iRight;
		g_iGlobalCropTop = iTop;
		g_iGlobalCropBottom = iBottom;
	}

	// Set a rotation for the image - used only when loading the images 
	static void SetGlobalImageLoadRotation( int iRotation ) { g_iGlobalRotation = iRotation; }

	/* Draw an image to the screen, with a few other options:
	If you set the iAlternatePixelColour to other than -1 then every alternate pixel will be coloured in this colour.
	If you set the iEveryThirdPixelColour to other than -1 then every third pixel will be coloured in this colour.
	If you set the iEveryFourthPixelColour to other than -1 then every fourth pixel will be coloured in this colour.

	If you set iMergeColour to other than -1 then any remaining pixel colours will be averaged with this one - e.g. to make it more grey?

	If you set iBrightnessPercentage to other than 100 you can make the image darker or brighter (reduces or increases each of the RGB values). Note that each will be saturated at 255 though so going too bright can appear to change the colour.

	Setting iRotate90Degrees to 1 will rotate 90 degrees clockwise, to 2 will rotate 180 degrees and to 3 will rotate 270 degrees. Only values 0-3 have distinct meanings.
	*/
	void FlexibleRenderImageWithMask(
		SDL_Surface* pTarget, // Target surface to render to
		int iXSource, int iYSource, // Coordinates in source to start draw from
		int iXTarget, int iYTarget, // Coordinates in dest to start draw to
		int iWidth, int iHeight, // Size of the area to copy - in the SOURCE not destination - since it may be swapped in destination.
		int iRotate90Degrees = 0, // Number of 90 degree rotations to apply
		int iTransparencyPixelX = -1, int iTransparencyPixelY = -1, // Position of the transparency pixel
		int iBrightnessPercentage = 100, // Percentage brightness to use
		int iAlternatePixelColour = -1, // An alternate pixel colour - set to -1 to not use otherwise you will get a crosshatch pattern
		int iThirdPixelColour = -1, // Pixel colour for each third pixel - set to -1 to not use
		int iFourthPixelColour = -1, // Pixel colour for each fourth pixel - set to -1 to not use
		int iMergeColour = -1 // Merge colour - set to -1 to not use, otherwise this will average with current pixels
		) const;

	// Free the data for the image - you should never need to do this
	void FreeData()
	{
		if ( m_aiPixels != NULL )
			delete[] m_aiPixels;
		m_iWidth = 0;
		m_iHeight = 0;
		m_aiPixels = NULL;
	}

protected:
	int m_iWidth;
	int m_iHeight;
	unsigned int* m_aiPixels;

	static int g_iGlobalRotation;
	static int g_iGlobalCropLeft;
	static int g_iGlobalCropRight;
	static int g_iGlobalCropTop;
	static int g_iGlobalCropBottom;
};

/* Removed as replaced by ImageData
// Thin wrapper class around an SDL surface, which will maintain an image on the surface.
class ImageSurface
{
public:
	ImageSurface()
		: m_pSurface(NULL)
	{}

	~ImageSurface()
	{
		if ( m_pSurface != NULL )
			SDL_FreeSurface( m_pSurface );
	}

	int GetWidth() const { return m_pSurface->w; }
	int GetHeight() const { return m_pSurface->h; }

	bool LoadImage( const char* szFileName )
	{
		m_pSurface = IMG_Load( szFileName );
		return m_pSurface != NULL;
	}

	void RenderImage( SDL_Surface* pTarget, 
		int iXSource, int iYSource, 
		int iXTarget, int iYTarget, 
		int iWidth, int iHeight ) const
	{
		SDL_Rect rectS = { (Sint16)iXSource,(Sint16)iYSource,(Uint16)iWidth,(Uint16)iHeight };
		SDL_Rect rectD = { (Sint16)iXTarget,(Sint16)iYTarget,(Uint16)iWidth,(Uint16)iHeight };
		SDL_BlitSurface( m_pSurface,&rectS,pTarget,&rectD );
	}

protected:
	SDL_Surface* m_pSurface;
};
*/
