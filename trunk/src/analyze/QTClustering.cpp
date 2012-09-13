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
//! \file  QTClustering.cpp

#include "analyze/QTClustering.h"

#include "sdsslib/spectra.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/helpers.h"
#include "sdsslib/spectrahelpers.h"
#include "sdsslib/HTMLExport.h"

#include <set>
#include <assert.h>


QTClustering::Parameters::Parameters( float _errorPerSpectrum, float _errorPerSample, size_t _minCandidateCount )
:errorPerSpectrum(_errorPerSpectrum)
,errorPerSample(_errorPerSample)
,minCandidateCount(_minCandidateCount)
{
}


QTClustering::QTClustering( SpectraVFS *_pSourceVFS, const Parameters &_params )
:m_pSourceVFS(_pSourceVFS)
,m_numSpectra(_pSourceVFS->getNumSpectra())
,m_params(_params)
,m_cachedCluster(~0)
,m_logFile("analyze_log.txt")
{
}

QTClustering::~QTClustering()
{
}

size_t QTClustering::getNumberOfClusters() const
{
	return m_ClusterMap.size() + 1;
}


size_t QTClustering::getNumberOfSpectraForCluster( size_t _clusterIndex ) const
{
	if ( _clusterIndex == 0 )
	{
		return m_Unclassified.size();
	}
	if ( _clusterIndex >= getNumberOfClusters() )
	{
		assert(0);
		return 0;
	}

	return m_ClusterMap[_clusterIndex-1].size();
}


size_t QTClustering::getSpectra( size_t _clusterIndex, size_t _index )
{
	if ( _clusterIndex == 0 )
	{
	}
	if ( _clusterIndex >= getNumberOfClusters() )
	{
		assert(0);
		return ~0;
	}

	rebuildIndexCache(_clusterIndex);
	return m_indexCache[_index];

}


void QTClustering::reset()
{
	m_ClusterMap.clear();
	m_Unclassified.clear();
}

void QTClustering::rebuildIndexCache( size_t _clusterIndex )
{
	if ( _clusterIndex != m_cachedCluster && _clusterIndex < getNumberOfClusters() )
	{
		size_t nSpectraCount = getNumberOfSpectraForCluster(_clusterIndex);
		m_indexCache.resize(nSpectraCount);
		size_t c = 0;

		// for unclassified spectra
		if ( _clusterIndex==0 )
		{
			std::set<unsigned int>::iterator it = m_Unclassified.begin();
			std::set<unsigned int>::iterator itEnd = m_Unclassified.end();
			while ( it != itEnd )
			{
				size_t index = *it;
				m_indexCache[c] = static_cast<unsigned int>(index);
				it++;
				c++;
			}
		} 
		else
		{
			Candidates &currentCluster = m_ClusterMap[_clusterIndex-1];

			Candidates::iterator itC = currentCluster.begin();
			Candidates::iterator itCEnd = currentCluster.end();
			while ( itC != itCEnd )
			{
				size_t index = (*itC).second;
				m_indexCache[c] = static_cast<unsigned int>(index);
				itC++;
				c++;
			}
		}
	}

}

void QTClustering::process()
{
	reset();
	// for each spectrum we store a cluster map
	Candidates *pClusterMap = new Candidates[m_numSpectra];

	// fill search set with all spectra indices
	std::set<unsigned int> searchSet;
	for ( unsigned int i=0;i<m_numSpectra;i++ )
	{
		searchSet.insert( i );
	}

	int bestCluster; // index to best cluster with most elements
	do {
		bestCluster=-1; // index to best cluster with most elements
		size_t bestClusterSize=0; // number of elements in best cluster 

		std::set<unsigned int>::iterator itA = searchSet.begin();
		std::set<unsigned int>::iterator itEnd = searchSet.end();

		std::set<unsigned int> removeSet; // elements we want to remove from our search set

		while ( itA != itEnd )
		{
			size_t a = *itA;
			// build candidate cluster for each spectrum
			Candidates &currentCluster = pClusterMap[a];
			currentCluster.clear();

			Spectra *sa = m_pSourceVFS->beginRead( a );

			sa->normalize();

			std::set<unsigned int>::iterator itB = searchSet.begin();

			while ( itB != itEnd )
			{
				size_t b = *itB;
				Spectra *sb = m_pSourceVFS->beginRead( b );
				sb->normalize();

				float error = sa->compareAdvanced(*sb, 0.1f );
				if ( error < m_params.errorPerSpectrum )
				{
					currentCluster.insert( std::make_pair(error, static_cast<unsigned int>(b)) );
				}
				m_pSourceVFS->endRead( b );
				itB++;
			}

			if ( currentCluster.size() < m_params.minCandidateCount )
			{
				// remove candidate cluster with less elements than allowed threshold
				Candidates::iterator itC = currentCluster.begin();
				Candidates::iterator itCEnd = currentCluster.end();
				while ( itC != itCEnd )
				{
					unsigned int index = (*itC).second;
					m_Unclassified.insert( index );
					removeSet.insert( index );
					itC++;
				}
			}
			else if ( currentCluster.size() > bestClusterSize ) // implicit: && currentCluster.size() >= m_params.minCandidateCount
			{
				// determine best candidate cluster
				bestCluster = static_cast<int>(a);
				bestClusterSize = currentCluster.size();
			}
			m_pSourceVFS->endRead( a );
			itA++;
		}

		if ( bestCluster >= 0 )
		{
			Candidates &currentCluster = pClusterMap[bestCluster];
			m_ClusterMap.push_back( currentCluster );

			Candidates::iterator itC = currentCluster.begin();
			Candidates::iterator itCEnd = currentCluster.end();
			while ( itC != itCEnd )
			{
				removeSet.insert( (*itC).second );
				itC++;
			}
		}

		std::set<unsigned int>::iterator itR = removeSet.begin();
		std::set<unsigned int>::iterator itREnd = removeSet.end();
		while(itR != itREnd ) 
		{
			searchSet.erase( *itR );
			itR++;
		}

	} while( bestCluster >= 0 );

	// put the remaining elements in the unclassified group
	std::set<unsigned int>::iterator itA = searchSet.begin();
	std::set<unsigned int>::iterator itAEnd = searchSet.end();
	while ( itA != itAEnd )
	{
		m_Unclassified.insert( *itA );
		itA++;
	}


	delete[] pClusterMap;
}


void QTClustering::exportToHTML( const std::string &_sstrFilename )
{
	std::string sstrLog;
	sstrLog = "Exporting results to ";
	sstrLog += _sstrFilename;
	sstrLog +="\n";

	Helpers::print( sstrLog, &m_logFile );

	std::string sstrTemp;
	std::string sstrHTMLDoc;
	std::string sstrHTMLMainDoc;
	std::string sstrHTMLDocTemplate = HTMLExport::loadHTMLTemplate();
	std::string sstrTable;

	sstrHTMLMainDoc = sstrHTMLDoc = sstrHTMLDocTemplate;

	std::string sstrInfo;

	sstrInfo += std::string("minCandidateCount ")+Helpers::numberToString( m_params.minCandidateCount )+std::string("<br>\n");
	sstrInfo += std::string("errorPerSpectrum ")+Helpers::numberToString( m_params.errorPerSpectrum )+std::string("<br>\n");
	sstrInfo += std::string("errorPerSample ")+Helpers::numberToString( m_params.errorPerSample )+std::string("<br>\n");
	sstrInfo += std::string("spectrum size in bytes ")+Helpers::numberToString( sizeof(Spectra) )+std::string("<br>\n");

	Helpers::insertString( HTMLExport::HTML_TOKEN_INFO, sstrInfo, sstrHTMLMainDoc );

	sstrTable += "<tr>\n";
	sstrTable += "<br><h2>unclassified:</h2>\n";
	sstrTable += "</tr>\n";
	sstrTable += "<tr>\n";
	sstrTable += "<table cellspacing=0 cellpadding=0 border=0>\n";
	sstrTable += "<tr>\n";
	// insert link
	std::set<unsigned int>::iterator it = m_Unclassified.begin();
	std::set<unsigned int>::iterator itEnd = m_Unclassified.end();
	while ( it != itEnd )
	{
		size_t index = *it;
		Spectra *sp = m_pSourceVFS->beginRead( index );
		sstrTable += "<td><a href=\"";
		sstrTable += sp->getURL();
		sstrTable += "\" target=\"_blank\">";
		sstrTable += "<img src=\"";
		sstrTable += Spectra::plateToString(sp->getPlate()) + std::string("/") + sp->getFileName();
		sstrTable += ".png\"></td>\n";

		m_pSourceVFS->endRead( index );
		it++;
	}
	sstrTable += "</tr>\n";
	sstrTable += "</table>\n";
	sstrTable += "</tr>\n";

	for ( size_t i=0;i<m_ClusterMap.size();i++)
	{
		sstrTable += "<tr>\n";
		sstrTable += "<br><h2>cluster "+Helpers::numberToString(i)+":</h2>\n";
		sstrTable += "</tr>\n";
		sstrTable += "<tr>\n";
		sstrTable += "<table cellspacing=0 cellpadding=0 border=0>\n";
		sstrTable += "<tr>\n";
		Candidates &currentCluster = m_ClusterMap[i];
		
		Candidates::iterator itC = currentCluster.begin();
		Candidates::iterator itCEnd = currentCluster.end();
		while ( itC != itCEnd )
		{
			size_t index = (*itC).second;
			Spectra *sp = m_pSourceVFS->beginRead( index );
			sstrTable += "<td><a href=\"";
			sstrTable += sp->getURL();
			sstrTable += "\" target=\"_blank\">";
			sstrTable += "<img src=\"";
			sstrTable += Spectra::plateToString(sp->getPlate()) + std::string("/") + sp->getFileName();
			sstrTable += ".png\"></td>\n";
			m_pSourceVFS->endRead( index );
			itC++;
		}
		sstrTable += "</tr>\n";
		sstrTable += "</table>\n";
		sstrTable += "</tr>\n";
	}
	
	if (!Helpers::insertString( HTMLExport::HTML_TOKEN_TEMPLATE, sstrTable, sstrHTMLMainDoc ) )
	{
		Helpers::print( std::string("export failed. Wrong template.html ?!?\n"), &m_logFile );
		return;
	}

	std::ofstream fon(std::string(_sstrFilename+".html").c_str());
	fon<<sstrHTMLMainDoc;

}