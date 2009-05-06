#include <Windows.h>

#include "sdsslib/spectra.h"
#include "sdsslib/spectraVFS.h"
#include "sdsslib/Timer.h"
#include "sdsslib/Helpers.h"
#include "sdsslib/defines.h"

typedef char _TCHAR;

int main(int argc, _TCHAR* argv[])
{
	// this must run on x64 architectures too!

	// write a test dump, needed only to be done once.
	// these are 10000 artificial ones, a sine function plus noise.
	SpectraVFS::write(10000, 1.f, "spectra.bin");

	// load the dump, read only
	SpectraVFS vfs("spectra.bin", true);

	// create another single spectrum
	ISSE_ALIGN Spectra a;
	a.set(2, 0.1); // initialize with test data

	std::ofstream logFile("playgroung_log.txt");

	Timer t;

	// one-to many comparison, the classic use case for us...
	size_t numSpectra( vfs.getNumSpectra() );
	float errAccum = 0.0f;
	for (size_t i=0;i<numSpectra;i++)
	{
		Spectra *sp = vfs.beginRead(i);

		float errEuclidean = a.compare(*sp); //< getting this to run on a GPU would be fine in the first case
		//float errPeakDetectionPlusContinuum = a.compareSuperAdvanced(*sp, 0.1, true); //< this one is too complex for the first shot

		vfs.endRead(i);
	}

	double seconds = t.getElapsedSecs();

	Helpers::print( std::string("CPU comparison time: ")+Helpers::numberToString( seconds ), &logFile );

	

	return 0;
}

