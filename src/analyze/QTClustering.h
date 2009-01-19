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
//! \file  QTClustering.h


#ifndef _QTCLUSTERING_H
#define _QTCLUSTERING_H

#include <fstream>
#include <map>
#include <set>
#include <vector>


class SpectraVFS;


// uses the quality treshold clustering algorithm to cluster our spectral data.
// see http://en.wikipedia.org/wiki/Data_clustering for details.
// ... or see: Heyer, L.J., Kruglyak, S. and Yooseph, S., 
// Exploring Expression Data: Identification and Analysis of Coexpressed Genes, Genome Research 199 9:1106-1115
// for a detailed algorithm description.
// interesting would be a matching the results of this method into the kohonen map (e.g. coloring the results of this into the map).
class QTClustering
{
public:

	// QT clustering parameters
	class Parameters
	{
	public:
		Parameters( float _errorPerSpectrum, float _errorPerSample, size_t _minCandidateCount );
		float errorPerSpectrum;									// error over the whole spectrum
		float errorPerSample;									// error per sample
		size_t minCandidateCount;								// minimum number of candidate spectra per cluster
	};

	// _pSourceVFS is your input data which you want to cluster
	QTClustering( SpectraVFS *_pSourceVFS, const Parameters &_params );
	~QTClustering();

	// process clustering
	void process();

	// export to HTML.
	// _sstrFilename absolute or relative filename without (!) extension.
	void exportToHTML( const std::string &_sstrFilename );


private:
	Parameters					m_params;
	SpectraVFS					*m_pSourceVFS;
	size_t						m_numSpectra;

	// errors->spectrum 
	typedef std::multimap<float,unsigned int> Candidates;

	std::vector<Candidates>		m_ClusterMap;					// all clustered spectra go here
	std::set<unsigned int>		m_Unclassified;					// all unclassified spectra.

	std::ofstream				m_logFile;
};

#endif 