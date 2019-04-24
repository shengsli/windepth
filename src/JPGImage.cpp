#include "header.h"

#include <SDL_image.h>

#include "JPGImage.h"

// These set global cropping and rotation when images are loaded.
int ImageData::g_iGlobalCropLeft = 0;
int ImageData::g_iGlobalCropRight = 0;
int ImageData::g_iGlobalCropTop = 0;
int ImageData::g_iGlobalCropBottom = 0;
int ImageData::g_iGlobalRotation = 0;

#ifdef WIN32
	static void *jpeglib, *pnglib;
#endif

void InitImage()
{
#ifdef WIN32
	// PERF_IMPROVEMENT: Avoid SDL_Image to load/unload shared libs on every image
	// This also seems to fix a double load of the same file.
	jpeglib = SDL_LoadObject("jpeg.dll");	
	pnglib = SDL_LoadObject("libpng12-0.dll");
#endif
}

void DeinitImage()
{
#ifdef WIN32
	SDL_UnloadObject(pnglib);
	SDL_UnloadObject(jpeglib);
#endif
}

// Function to work out the X coordinate to retrieve a point from given a rotation and cropping
inline int GetXForRotationAndCrop( int iImageWidth, int iX, int iY, int iRotation, int iCropLeft, int iCropTop )
{
	switch( (iRotation+4)%4 )
	{
	default: return iX+iCropLeft;
	case 1: return iY + iCropTop;
	case 2: return iImageWidth - 1 - iX - iCropLeft;
	case 3: return iImageWidth - 1 - iY - iCropTop;
	}
}

// Function to work out the Y coordinate to retrieve a point from given a rotation and cropping
inline int GetYForRotationAndCrop( int iImageHeight, int iX, int iY, int iRotation, int iCropLeft, int iCropTop )
{
	switch( (iRotation+4)%4 )
	{
	default: return iY+iCropTop;
	case 1: return iImageHeight - 1 - iCropLeft - iX;
	case 2: return iImageHeight - 1 - iY - iCropTop;
	case 3: return iX + iCropLeft;
	}
}


bool ImageData::LoadImage( const char* szFileName )
{
	char buf[128];

	// Free any current image data
	if ( m_aiPixels != NULL )
	{
		delete[] m_aiPixels;
		m_aiPixels = NULL;
	}

	SDL_Surface* imagesurface = IMG_Load( szFileName );
	if ( imagesurface != NULL )
	{
		{ // Make a 32 bit version
			SDL_Surface* newSurface = SDL_CreateRGBSurface (
						SDL_SWSURFACE, imagesurface->w, imagesurface->h, 32, 
						0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 );
			SDL_BlitSurface( imagesurface, NULL, newSurface, NULL );

			SDL_FreeSurface( imagesurface );
			imagesurface = newSurface;
		}

		if ( (g_iGlobalRotation % 2) == 1 )
		{
			m_iWidth = imagesurface->h - g_iGlobalCropTop - g_iGlobalCropBottom;
			m_iHeight = imagesurface->w - g_iGlobalCropLeft - g_iGlobalCropRight;
		}
		else
		{
			m_iWidth = imagesurface->w - g_iGlobalCropLeft - g_iGlobalCropRight;
			m_iHeight = imagesurface->h - g_iGlobalCropTop - g_iGlobalCropBottom;
		}

		// Allocate memory
		m_aiPixels = new unsigned int[ m_iWidth * m_iHeight ];

		int iIntsPerScreenRow = imagesurface->pitch/sizeof(unsigned int);

		for ( int iY = 0 ; iY < m_iHeight ; iY++ )
		{
			for ( int iX = 0 ; iX < m_iWidth ; iX++ )
			{
				// Calculate which pixel to pull from the image
				m_aiPixels[ iY * m_iWidth + iX ] 
					= (((unsigned int *)imagesurface->pixels)[GetXForRotationAndCrop( imagesurface->w,iX,iY,g_iGlobalRotation,g_iGlobalCropLeft,g_iGlobalCropTop )
					+ GetYForRotationAndCrop( imagesurface->h,iX,iY,g_iGlobalRotation,g_iGlobalCropLeft,g_iGlobalCropTop ) * iIntsPerScreenRow]);
			}
		}
		SDL_FreeSurface(imagesurface);
		return true;
	}
	return false;
}


// Create an image by shrinking another one by a factor of three
void ImageData::ShrinkFrom( const ImageData* pSource, int iReductionMultiplier )
{
	// Free any current image data
	if ( m_aiPixels != NULL )
	{
		delete[] m_aiPixels;
		m_aiPixels = NULL;
	}

	m_iWidth = (pSource->m_iWidth)/iReductionMultiplier;
	m_iHeight = (pSource->m_iHeight)/iReductionMultiplier;
	m_aiPixels = new unsigned int[ m_iWidth * m_iHeight ];

	unsigned int iR, iG, iB, iA;
	unsigned int pixel;

	int iBrightnessFactor = (iReductionMultiplier * iReductionMultiplier);

	for ( int x = 0 ; x < m_iWidth ; x++ )
		for ( int y = 0 ; y < m_iHeight ; y++ )
		{
			iR = iG = iB = iA = 0;
			for ( int i = 0 ; i < iReductionMultiplier ; i++ )
				for ( int j = 0 ; j < iReductionMultiplier ; j++ )
				{
					pixel = pSource->m_aiPixels[ (y*iReductionMultiplier+j) * (pSource->m_iWidth) + (x*iReductionMultiplier+i) ];
					iA += ( 0xFF & ( pixel >> 24 ) );
					iR += ( 0xFF & ( pixel >> 16 ) );
					iG += ( 0xFF & ( pixel >> 8 ) );
					iB += ( 0xFF & pixel );
				}
			m_aiPixels[ y * m_iWidth + x ]  = //(((iA)&0xFF)<<24) +
					(((iR/iBrightnessFactor)&0xFF)<<16) 
					+ (((iG/iBrightnessFactor)&0xFF)<<8) 
					+ (((iB/iBrightnessFactor)&0xFF));
		}
}

// Create an image by shrinking another one by a factor of three
void ImageData::ResizeFrom( const ImageData* pSource, int iNewWidth, int iNewHeight, bool bJustSampleOnePixel )
{
	// Free any current image data
	if ( m_aiPixels != NULL )
	{
		delete[] m_aiPixels;
		m_aiPixels = NULL;
	}

	int iOldWidth = pSource->m_iWidth;
	int iOldHeight = pSource->m_iHeight;

	m_iWidth = iNewWidth;
	m_iHeight = iNewHeight;
	m_aiPixels = new unsigned int[ m_iWidth * m_iHeight ];

	double dR, dG, dB;
	unsigned int pixel;
	double dSampleX, dSampleY;
	int iSampleX, iSampleY;
	double dX[2], dY[2];

	for ( int x = 0 ; x < m_iWidth ; x++ )
		for ( int y = 0 ; y < m_iHeight ; y++ )
		{
			dSampleX = ((double)x * (double)iOldWidth) / (double)iNewWidth + 0.001;
			dSampleY = ((double)y * (double)iOldHeight) / (double)iNewHeight + 0.001;
			iSampleX = (int)dSampleX;
			iSampleY = (int)dSampleY;

			dX[1] = dSampleX - iSampleX;
			dX[0] = 1.0 - dX[1];
			dY[1] = dSampleY - iSampleY;
			dY[0] = 1.0 - dY[1];

			if ( bJustSampleOnePixel )
			{ // Just sample one of the pixels - not a good thing!
				dX[0] = dY[0] = 1.0;
				dX[1] = dY[1] = 0.0;
			}

			dR = dG = dB = 0.0;
			for ( int i = 0 ; i < 2 ; i++ )
				for ( int j = 0 ; j < 2 ; j++ )
				{
					if ( ((iSampleX+i) < iOldWidth) && ((iSampleY+j) < iOldHeight) )
					{
						pixel = pSource->m_aiPixels[ (iSampleY+j) * iOldWidth + (iSampleX+i) ];
						dR += ( ( 0xFF & ( pixel >> 16 ) ) * dX[i] * dY[j] );
						dG += ( ( 0xFF & ( pixel >> 8 ) ) * dX[i] * dY[j] );
						dB += ( ( 0xFF & pixel ) * dX[i] * dY[j] );
					}
				}
			m_aiPixels[ y * m_iWidth + x ] = 
					  (( ((int)(dR+0.5)) & 0xFF)<<16) 
					+ (( ((int)(dG+0.5)) & 0xFF)<<8) 
					+ (( ((int)(dB+0.5)) & 0xFF));
		}
}

void ImageData::RenderImage( SDL_Surface* pTarget, 
							int iXSource, int iYSource, 
							int iXTarget, int iYTarget, 
							int iWidth, int iHeight ) const
{
	if ( iXTarget < 0 )
	{ // Then trim
		iWidth += iXTarget;
		iXSource -= iXTarget;
		iXTarget = 0;
	}

	if ( (iXTarget+iWidth) >= pTarget->w )
	{ // Then trim
		iWidth = pTarget->w - iXTarget;
	}

	if ( iYTarget < 0 )
	{ // Then trim
		iHeight += iYTarget; // Decrease height
		iYSource -= iYTarget; // Increase start point
		iYTarget = 0;
	}

	if ( (iYTarget+iHeight) >= pTarget->h )
	{ // Then trim
		iHeight = pTarget->h - iYTarget;
	}

	int iXS, iYS = iYSource, iXT, iYT = iYTarget;
	int iIntsPerScreenRow = pTarget->pitch/sizeof(unsigned int);

	for ( int iYOffset = 0 ; iYOffset < iHeight ; iYOffset++ )
	{
		iXS = iXSource; 
		iXT = iXTarget;
		for ( int iXOffset = 0 ; iXOffset < iWidth ; iXOffset++ )
		{
			((unsigned int *)pTarget->pixels)[iXT + iYT * iIntsPerScreenRow] = m_aiPixels[ iYS * m_iWidth + iXS ];
			iXS++;
			iXT++;
		}
		iYS++; 
		iYT++;
	}
}

void ImageData::RenderImageWithMask( SDL_Surface* pTarget, 
							int iXSource, int iYSource, 
							int iXTarget, int iYTarget, 
							int iWidth, int iHeight ) const
{
	if ( iXTarget < 0 )
	{ // Then trim
		iWidth += iXTarget;
		iXSource -= iXTarget;
		iXTarget = 0;
	}

	if ( (iXTarget+iWidth) >= pTarget->w )
	{ // Then trim
		iWidth = pTarget->w - iXTarget;
	}

	if ( iYTarget < 0 )
	{ // Then trim
		iHeight += iYTarget; // Decrease height
		iYSource -= iYTarget; // Increase start point
		iYTarget = 0;
	}

	if ( (iYTarget+iHeight) >= pTarget->h )
	{ // Then trim
		iHeight = pTarget->h - iYTarget;
	}

	int iMask = m_aiPixels[ m_iHeight * m_iWidth -1 ];

	int iXS, iYS = iYSource, iXT, iYT = iYTarget;
	int iIntsPerScreenRow = pTarget->pitch/sizeof(unsigned int);

	for ( int iYOffset = 0 ; iYOffset < iHeight ; iYOffset++ )
	{
		iXS = iXSource; 
		iXT = iXTarget;
		for ( int iXOffset = 0 ; iXOffset < iWidth ; iXOffset++ )
		{
			if ( m_aiPixels[ iYS * m_iWidth + iXS ] != iMask )
				((unsigned int *)pTarget->pixels)[iXT + iYT * iIntsPerScreenRow] = m_aiPixels[ iYS * m_iWidth + iXS ];
			iXS++;
			iXT++;
		}
		iYS++; 
		iYT++;
	}
}

/* Draw an image to the screen, with a few other options which I needed for a program I was working on so added to this file too:
If you set the iAlternatePixelColour to other than -1 then every alternate pixel will be coloured in this colour.
If you set the iEveryThirdPixelColour to other than -1 then every third pixel will be coloured in this colour.
If you set the iEveryFourthPixelColour to other than -1 then every fourth pixel will be coloured in this colour.

If you set iMergeColour to other than -1 then any remaining pixel colours will be averaged with this one - e.g. to make it more grey?

If you set iBrightnessPercentage to other than 100 you can make the image darker or brighter (reduces or increases each of the RGB values). Note that each will be saturated at 255 though so going too bright can appear to change the colour.

Setting iRotate90Degrees to 1 will rotate 90 degrees clockwise, to 2 will rotate 180 degrees and to 3 will rotate 270 degrees. Only values 0-3 have distinct meanings.
*/
void ImageData::FlexibleRenderImageWithMask(
	SDL_Surface* pTarget, // Target surface to render to
	int iXSource,int iYSource, // Coordinates in source to start draw from
	int iXTarget,int iYTarget, // Coordinates in dest to start draw to
	int iWidth,int iHeight, // Size of the area to copy - in the SOURCE not destination - since it may be swapped in destination.
	int iRotate90Degrees, // Number of 90 degree rotations to apply
	int iTransparencyPixelX,int iTransparencyPixelY, // Position of the transparency pixel
	int iBrightnessPercentage, // Percentage brightness to use
	int iAlternatePixelColour, // An alternate pixel colour - set to -1 to not use otherwise you will get a crosshatch pattern
	int iThirdPixelColour, // Pixel colour for each third pixel - set to -1 to not use
	int iFourthPixelColour, // Pixel colour for each fourth pixel - set to -1 to not use
	int iMergeColour // Merge colour - set to -1 to not use, otherwise this will average with current pixels
	) const
{
	const int XOFFSET1[] = { 1, 0,-1, 0};
	const int YOFFSET1[] = { 0, 1, 0,-1};
	const int XOFFSET2[] = { 0,-1, 0, 1};
	const int YOFFSET2[] = { 1, 0,-1, 0};

	// Get the colour of the mask to apply, if any
	int iMask = -1; // None
	if ( (iTransparencyPixelX > -1) && (iTransparencyPixelY > -1) )
		iMask = m_aiPixels[m_iHeight * iTransparencyPixelY + iTransparencyPixelX];

	int iXS,iYS = iYSource,iXT,iYT = iYTarget;
	int iIntsPerScreenRow = pTarget->pitch / sizeof( unsigned int );
	int iPixel;
	double dR,dG,dB; 
	bool fColourSet = false;

	for ( int iYOffset = 0; iYOffset < iHeight; iYOffset++ )
	{
		iXS = iXSource;
		for ( int iXOffset = 0; iXOffset < iWidth; iXOffset++ )
		{
			// Start the other side of the area
			iXT = iXTarget + iWidth / 2 - iWidth * (XOFFSET1[iRotate90Degrees % 4] + XOFFSET2[iRotate90Degrees % 4]) / 2;
			iYT = iYTarget + iHeight / 2 - iHeight * (YOFFSET1[iRotate90Degrees % 4] + YOFFSET2[iRotate90Degrees % 4]) / 2;

			iXT += iXOffset * XOFFSET1[iRotate90Degrees % 4] + iYOffset * XOFFSET2[iRotate90Degrees % 4];
			iYT += iXOffset * YOFFSET1[iRotate90Degrees % 4] + iYOffset * YOFFSET2[iRotate90Degrees % 4];

			if ( iXT < 0 ) iXT = iIntsPerScreenRow - 1; 
			if ( iXT >= iIntsPerScreenRow ) iXT = 0;
			if ( iYT < 0 ) iYT = pTarget->h - 1; 
			if ( iYT >= pTarget->h ) iYT = 0;

			iPixel = m_aiPixels[iYS * m_iWidth + iXS];
			if ( iPixel != iMask )
			{
				fColourSet = false; 

				if ( (iAlternatePixelColour != -1) && ((iXOffset + iYOffset) & 1) )
				{
					iPixel = iAlternatePixelColour;
					fColourSet = true;
				}
				
				if ( (iThirdPixelColour != -1) && ((iXOffset + iYOffset)%3 == 1) )
				{
					iPixel = iThirdPixelColour;
					fColourSet = true;
				}
				
				if ( (iFourthPixelColour != -1) && ((iXOffset + iYOffset) % 4 == 1) )
				{
					iPixel = iFourthPixelColour;
					fColourSet = true;
				}

				if ( !fColourSet) // Not an alternating pixel colour etc, so no colour set
				{
					if ( (iBrightnessPercentage != 100) || (iMergeColour != -1 ) )
					{
						// Alter brightness of RGB separately by the specified amount
						dR = ((0xFF & (iPixel >> 16)) * iBrightnessPercentage) / 100.0;
						dG = ((0xFF & (iPixel >> 8)) * iBrightnessPercentage) / 100.0;
						dB = ((0xFF & iPixel) * iBrightnessPercentage) / 100.0;

						if ( iMergeColour != -1 )
						{
							dR = (dR + (double)(iMergeColour>>16 & 0xff)) / 2.0;
							dG = (dG + (double)(iMergeColour>>8 & 0xff)) / 2.0;
							dB = (dB + (double)(iMergeColour & 0xff)) / 2.0;
						}

						// Catch saturation
						if ( dR > 255 ) dR = 255;
						if ( dG > 255 ) dG = 255;
						if ( dB > 255 ) dB = 255;

						iPixel = ((((int)(dR + 0.5)) & 0xFF) << 16)
							+ ((((int)(dG + 0.5)) & 0xFF) << 8)
							+ ((((int)(dB + 0.5)) & 0xFF));
					}
				}

				((unsigned int *)pTarget->pixels)[iXT + iYT * iIntsPerScreenRow] = iPixel;
			}
			iXS++;
		}
		iYS++;
	}
}
