#include "XMLExport.h"

#include <sstream>
#include <assert.h>


static 
void appendTabToString( unsigned int n, std::string &sstrOut )
{
	for (unsigned int i=0; i < n; i++ )
	{
		sstrOut += "\t";
	}
}


void XMLExport::xmlElementBegin( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut )
{
	sstrOut += "\n";
	appendTabToString( nLevel, sstrOut );
	sstrOut += "<";
	sstrOut += sstrTagName;
}


void XMLExport::xmlElementEndBegin( std::string &sstrOut )
{
	sstrOut += ">";
}

void XMLExport::xmlElementEnd( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut )
{
	sstrOut += "\n";
	appendTabToString( nLevel, sstrOut );
	sstrOut += "</";
	sstrOut += sstrTagName;
	sstrOut += ">";
}



void XMLExport::xmlSingleElementBegin( const std::string &sstrTagName, unsigned int nLevel, std::string &sstrOut )
{
	xmlElementBegin( sstrTagName, nLevel, sstrOut );
}


void XMLExport::xmlSingleElementEnd( std::string &sstrOut )
{
	sstrOut += "/>";
}



void XMLExport::xmlAddAttribute( const std::string &sstrAttributeName, float value, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();
	strStream << " " << sstrAttributeName << "=\"" << value << "\"";
	sstrOut += strStream.str();
}


void XMLExport::xmlAddAttribute( const std::string &sstrAttributeName, int value, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();
	strStream << " " << sstrAttributeName << "=\"" << value << "\"";
	sstrOut += strStream.str();

}


void XMLExport::xmlAddAttribute( const std::string &sstrAttributeName, unsigned int value, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();
	strStream << " " << sstrAttributeName << "=\"" << value << "\"";
	sstrOut += strStream.str();
}


void XMLExport::xmlAddAttribute( const std::string &sstrAttributeName, const std::string &sstrValue, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();
	strStream << " " << sstrAttributeName << "=\"" << sstrValue << "\"";
	sstrOut += strStream.str();
}



void XMLExport::xmlAddAttributeArray( const std::string *psstrAttributeNames, const float *pValues, size_t nValues, std::string &sstrOut )
{
	std::stringstream strStream;
	strStream.clear();

	assert( psstrAttributeNames != NULL );
	assert( pValues != NULL );

	for ( size_t i=0; i < nValues;i++ )
	{
		strStream << " " << psstrAttributeNames[i] << "=\"" << pValues[i] << "\"";
	}
	sstrOut += strStream.str();
}


