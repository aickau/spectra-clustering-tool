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
//! \file  spectraMapper.h


#include "sdsslib/spectra.h"
#include "sdsslib/defines.h"
#include "sdsslib/spectraVFS.h"


 // draw multiple spectra into one diagram from a mask selection

class SpectraMapper
{
public:
	SpectraMapper();
	virtual ~SpectraMapper();

	void draw( int _width, int _height, bool _toRestFrame, int _selection, bool _writeDataToPNG );

private:
	const size_t m_gridSize;
	const size_t m_gridSizeSqr;
	std::vector<int> m_mappedSpectra;
	SSE_ALIGN Spectra m_spAvg;
	SSE_ALIGN Spectra m_spMax;
	SSE_ALIGN Spectra m_spMin;
	SpectraVFS *m_pSourceVFS;
	size_t m_numSourceSpecra;
	int m_numSpectraToDraw;
	int m_currentIndex;
};
