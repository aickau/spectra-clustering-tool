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
 
	// cluster count, which should be at least 2 (one for unclassifieds, one cluster)
	size_t getNumberOfClusters() const;

	// _clusterIndex = 0 contains unclassified spectra
	size_t getNumberOfSpectraForCluster( size_t _clusterIndex ) const;
	
	// returns index to a spectrum of the given cluster
	// within the cluster spectra with lower index come first
	// _clusterIndex 0..number of cluster
	// _spectraIndex 0..number of spectra of the cluster
	size_t getSpectra( size_t _clusterIndex, size_t _index );

	// process clustering
	void process();

	// export to HTML.
	// _sstrFilename absolute or relative filename without (!) extension.
	void exportToHTML( const std::string &_sstrFilename );


private:

	void reset();
	void rebuildIndexCache( size_t _clusterIndex );

	Parameters					m_params;
	SpectraVFS					*m_pSourceVFS;
	size_t						m_numSpectra;

	// errors->spectrum 
	typedef std::multimap<float,unsigned int> Candidates;

	std::vector<Candidates>		m_ClusterMap;					// all clustered spectra go here
	std::set<unsigned int>		m_Unclassified;					// all unclassified spectra.

	std::vector<unsigned int>	m_indexCache;					// cache for faster access by index.
	size_t						m_cachedCluster;				// index to cluster that is cached.

	std::ofstream				m_logFile;
};

#endif 