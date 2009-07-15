//! \verbatim
//! ###########################################################################
//! # SDSS sorting prototype - Stage I
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

#include <windows.h>

#include <string>
#include <map>
#include <vector>


class Spectra;
class SpectraVFS;

// some spectra helper functions which do very specific tasks for drawing, presentation, etc.
namespace SpectraHelpers
{
	// init system
	void init( HDC _hDC );

	// get GL default font ID
	int getDefaultFontID();

	// Render diagram to disk.
	void renderDiagramToDisk( float *_values, size_t _valueCount, size_t _strideInBytes, size_t _offsetInBytes, 
		size_t _width, size_t _height, const std::string &sstrFilename );

	// save a floating point map as RGB png image
	// _pMap map
	// _sstrFileName path+filename without extension
	void saveIntensityMap( float *_pMap, size_t _sizeX, size_t _sizeY, const std::string &_sstrFileName );

	// Draw Spectrum (to framebuffer)
	void drawSpectra(Spectra &_spectra, 
							bool _showInfo,
							bool _showSpectraLines, 
							size_t _xp=0, size_t _yp=0, 
							size_t _width=400, size_t _height=300, 
							float _yscale=1.f);

	// Render spectrum to disk.
	// _spectra = spectrum to render
	// _sstrFilename = filename including extension (.png, gif, jpg) 
	// height and width in pixels
	// yscale
	// redness = [0..2] (0=black, 1=red, 2=yellow)
	void renderSpectraIconToDisk( Spectra &_spectra, const std::string &_sstrFilename, size_t _width, size_t _height, float _redness );

	// load spectra from dump and combine all spectra into one.
	// _sstrDumpFilename path + filename to spectra dump.
	// _sstrFilename output image graph of resulting spectra
	void combineSpectra( std::string &_sstrDumpFilename, const std::string &_sstrFilename );

	// write spectrum image and sdss link into HTML table entry
	void writeTableEntry( const Spectra &_spectrum, float _error, std::string &_sstrOutTable );

	// \param _sstrSelectionListFilename path+filename to a text file with FITS file names.
	// \param _outFITSFilenameSet set where FITS file names are inserted, file names are only inserted, exisitng items will not get deleted.
	// \return true on successful load, otherwise false
	bool readSelectionList( const std::string &_sstrSelectionListFilename, std::map<std::string,float> &_outFITSFilenameSet );

	// calculate vector field of two different maps
	// Booth maps must have the same dimension and must use the index to the source spectra
	// \param _map1 SOM map1
	// \param _map2 SOM map2
	// \param [out] position, velocity and distance field (index 0,1 initial position, 2,3 velocity vector, 4 distance)
	// \return true on success, otherwise false
	bool calcVectorField( SpectraVFS &_map1, SpectraVFS &_map2, std::vector<float>& _outVectorField );
};


#endif
