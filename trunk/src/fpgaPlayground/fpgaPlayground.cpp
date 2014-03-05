
#include "sdsslib/spectraVFS.h"
#include "FPGASOFMNetwork.h"


void main(int argc, char* argv[])
{

	// write 10.000 sine test spectra. 
	SpectraVFS::write( 10000, 0.0f, "allSpectra.bin" );

	SpectraVFS src( "allSpectra.bin", false );
	const int numSpectra = src.getNumSpectra();

	if ( numSpectra > 0 )
	{
		FPGASOFMNetwork net( &src, false );

		do 
		{
			printf("calculating step %i\n", net.m_currentStep );
		} while ( net.process() == false );
	}
}

