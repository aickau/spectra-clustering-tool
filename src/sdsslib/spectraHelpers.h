//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 1/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  spectraHelpers.h
//! \brief 

#ifndef _SPECTRAHELPERS_H
#define _SPECTRAHELPERS_H


#include <string>
#include <set>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif 

#ifdef __linux
#define HDC int
#endif 

class Spectra;
class SpectraVFS;

// Some spectra helper functions which do very specific tasks for drawing, presentation, etc.
// Additional, low-level helper functions can be found in SpectraBaseHelpers.
namespace SpectraHelpers
{
	/** @name DRAWING
	*/
	//@{
	static int s_FontID = -1;
	static int s_largeFontID = -1;

	// init system (for rendering of diagrams etc)
	void init( HDC _hDC );

	// get GL default font ID
	int getDefaultFontID();

	// Render one or more diagrams to disk.
	// _values pointer to float array, should be _valueCount*_numDiagrams in size
	// _valueCount number of values per diagram
	// _numDiagrams number of diagrams
	void renderDiagramToDisk( float *_values, size_t _valueCount, size_t _numDiagrams, size_t _strideInBytes, size_t _offsetInBytes, 
		size_t _width, size_t _height, const std::string &sstrFilename );

	// save a floating point map as RGB png image
	// _pMap map
	// _sstrFileName path+filename without extension
	void saveIntensityMap( float *_pMap, size_t _sizeX, size_t _sizeY, const std::string &_sstrFileName );

	// Draw Spectrum (to framebuffer)
	// _smooth smoothing of spectra [0..10] (0=no smoothing, n=number of smooth iterations)
	void drawSpectra(Spectra &_spectra, 
		bool _showInfo,
		bool _showSpectraLines, 
		size_t _xp=0, size_t _yp=0, 
		size_t _width=400, size_t _height=300, 
		float _yscale=1.f,
		float _xScale=1.f,
		int _smooth=2);

	// Render spectrum to disk.
	// _spectra = spectrum to render
	// _sstrFilename = filename including extension (.png, gif, jpg) 
	// height and width in pixels
	// yscale
	// redness = [0..2] (0=black, 1=red, 2=yellow)
	void renderSpectraIconToDisk( Spectra &_spectra, const std::string &_sstrFilename, size_t _width, size_t _height, float _redness );


	// Calculate unified distance matrix (Ultsch 1989) of the SOM for the current calculation step. 
	// sstrFilenName path + filename (no extension)
	// _bUseLogScale use logarithmic scaling for matrix display
	// _bShowEmpty show intensities for empty cells (no input data in there), otherwise empty cells are filled with black pixels
	// _bNormalize normalize map for comparison
	// _showRanges write many u matrices with a red rectangle marking the actual range
	// _planSize plan size for rectangles (only used if _showRanges true)
	void calcUMatrix( SpectraVFS &_network, const std::string &_sstrFilenName, bool _bUseLogScale, bool _bShowEmpty, bool _bNormalize, bool _showRanges, size_t _planSize);

	// Calculate difference map of the SOM codebook vectors and the input data assigned to a neuron  for the current calculation step. 
	// sstrFilenName path + filename (no extension)
	// _bUseLogScale use logarithmic scaling for matrix display
	// _bNormalize normalize map for comparison
	// bOutputAsTextFile if true, we write the difference map as text file too (then no log scale).
	// _planSize plan size for rectangles (only used if _showRanges true)
	void calcDifferenceMap( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilenName, bool _bUseLogScale, bool _bNormalize, bool _bOutputAsTextFile, bool _showRanges, size_t _planSize );

	// Calculate z map of th input data assigned to a neuron  for the current calculation step. 
	// sstrFilenName path + filename (no extension)
	// _bUseLogScale use logarithmic scaling for matrix display
	// _planSize plan size for rectangles (only used if _showRanges true)
	void calcZMap( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilenName, bool _bUseLogScale, bool _showRanges, size_t _planSize );

	// Write spectrum class (i.e. star, galaxy, QSO, ..) with the following color codes to map:
	//  star = blue
	//  galaxy = red
	//  QSO = orange
	//  unknown = white
	// sstrFilenName path + filename (no extension)
	void writeSpectraClass( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilename );

	void writeSpectraGalaxyAndQSOSubclass( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilename );
	void writeSpectraStarSubclass( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrFilename );


	//@}



	/** @name IO
	*/
	//@{

	// load spectra from dump and combine all spectra into one.
	// _sstrDumpFilename path + filename to spectra dump.
	// _sstrFilename output image graph of resulting spectra
	void combineSpectra( std::string &_sstrDumpFilename, const std::string &_sstrFilename );

	// write spectrum image and sdss link into HTML table entry
	void writeTableEntry( const Spectra &_spectrum, float _error, std::string &_sstrOutTable );

	// \param _sstrSelectionListFilename path+filename to a text file with FITS file names.
	// \param _outFITSFilenameSet set where FITS file names are inserted, file names are only inserted, existing items will not get deleted.
	// \return true on successful load, otherwise false
	bool readSelectionList( const std::string &_sstrSelectionListFilename, std::set<std::string> &_outFITSFilenameSet );

	//  write float values from array to disk (as text)
	void writeFloatList( float *_pArray, size_t _size, const std::string &_sstrFilename );

	// write dotted red rectangles into a float rgb map (for u matrix and difference map rendering)
	void writeMapWithSubpageMarkers( const std::string &_sstrFilenName, float *_pRGBMap, size_t _gridSize, size_t _planSize );

	// Writes mapindex, xpos, ypos, specObjID, aspect internal spectraIndex, plate ID, MJD, fiber ID, z, objtype into cvs table
	// \param pLogFile optional pointer to log file.
	void writeSpectraInfoToTable( SpectraVFS &_sourceSpectra, SpectraVFS &_network, const std::string &_sstrTableFilename, std::ofstream *pLogFile=NULL );

	//@}



	/** @name CALCULATIONS
	*/
	//@{

	// calculate vector field of two different maps
	// Booth maps must have the same dimension and must use the index to the source spectra
	// \param _map1 SOM map1
	// \param _map2 SOM map2
	// \param [out] position, velocity and distance field (index 0,1 initial position, 2,3 velocity vector, 4 distance)
	// \return true on success, otherwise false
	bool calcVectorField( SpectraVFS &_map1, SpectraVFS &_map2, std::vector<float>& _outVectorField );

	// intensity [0..1]
	// _pRGB pointer to location where out rgb triple is written to.
	// if red false -> map intensity to black->blue->cyan->white color gradient
	// otherwise       map intensity to black->red->yellow->white color gradient
	void intensityToRGB(float _intensity, float *_pRGB, bool _bRed=false );

	// intensity [0..1]
	// _pRGB pointer to location where out rgb triple is written to.
	void intesityToRGBGradient( float _intensity, float *_pRGB, int gradientType = 0 );

	//@}



};


#endif
