#ifndef _SPECTRAHELPERS_H
#define _SPECTRAHELPERS_H

#include <windows.h>
#include <string>

class Spectra;


class SpectraHelpers
{
public:

	static void Init( HDC _hDC );

	// Render diagram to disk.
	static void RenderDiagramToDisk( float *_values, size_t _valueCount, size_t _strideInBytes, size_t _offsetInBytes, 
		size_t _width, size_t _height, const std::string &sstrFilename );

	// save a floating point map as RGB png image
	// _pMap map
	// _sstrFileName path+filename without extension
	static void SaveIntensityMap( float *_pMap, size_t _sizeX, size_t _sizeY, const std::string &_sstrFileName );

	// Draw Spectrum (to framebuffer)
	static void DrawSpectra(Spectra &_spectra, 
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
	static void RenderSpectraIconToDisk( Spectra &_spectra, const std::string &_sstrFilename, size_t _width, size_t _height, float _yscale, float _redness );

	// load spectra from dump and combine
	// _sstrDumpFilename path + filename to spectra dump.
	// _sstrFilename output image graph of resulting spectra
	static void CombineSpectra( std::string &_sstrDumpFilename, const std::string &_sstrFilename );

	static int s_FontID;

private:

	static bool s_IsInitialized;
};


#endif
