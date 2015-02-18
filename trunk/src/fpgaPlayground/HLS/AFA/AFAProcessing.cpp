#include <float.h>
#include <math.h>
#include <stddef.h>

#include "include/AFASpectra.h"
#include "include/AFAProcessing.h"
#include "include/AFADefines.h"
#include "include/AFATypes.h"
#include "include/AFARandom.h"

AFAProcessing::AFAProcessing(
    volatile void *netSrc,	    // array with all spectra
    volatile void *netDst,      // "sofmnet.bin"
    uint32_t numSpectra,        // number of spectra stored in array
    bool bContinueComputation )
    : m_pSourceVFS(( volatile AFASpectra * ) netSrc )
    , m_pNet(( volatile AFASpectra * ) netDst )
    , m_numSpectra( numSpectra )
    , m_params( AFAParameters::defaultParameters )
{
	volatile AFASpectra *spectraArray  = ( volatile AFASpectra * ) netSrc;
	volatile AFASpectra *spectraArray2 = ( volatile AFASpectra * ) netDst;

    reset( m_params );

    AFASpectra::setOperationRange( true );

    calcFluxAndNormalizeInputDS();
    calcMinMaxInputDS();
#if 1

    if ( !bContinueComputation )
    {
#if 1
        //
        // start new computation 
        //
        m_gridSize = static_cast<size_t>(ceilf(sqrtf((float)m_numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
        m_gridSizeSqr = m_gridSize*m_gridSize;

        // generate random filled cluster and load it. (AFA changes: do not load it, generate it in memory only)
//        SpectraVFS::write( m_gridSize, m_Min, m_Max*0.01f, sstrSOFMFileName );
        spectraArray2->write( spectraArray2, m_gridSize, m_Min, m_Max*0.01f );

//        m_pNet = new SpectraVFS( sstrSOFMFileName, false );

        // fill network with random spectra, this improves the convergence times.
        // it does not matter if some spectra are inserted multiple times or other may missing since this is just for initialization purposes.
        AFARnd r(m_params.randomSeed);
        for ( size_t i=0;i<m_gridSizeSqr;i++ )
        {
            size_t xp = i % m_gridSize;
            size_t yp = i / m_gridSize;
            const size_t inset = 0;
            if ( xp >= inset && xp <= (m_gridSize-inset) && yp >= inset && yp <= (m_gridSize-inset) )
            {
                AFASpectra *a = &m_pNet[ i ];
                //Spectra *a = m_pNet->beginWrite( i );

                size_t spectraIndex = r.randomInt( m_numSpectra - 1 );
                AFASpectra *b = &m_pSourceVFS[ spectraIndex ];
                a->set( *b );
                //m_pSourceVFS->endRead( spectraIndex );
                //m_pNet->endWrite( i );
            }
        }
#endif
    }
    else
    {
#if 0
        //
        // continue old computation 
        //

        m_pNet = new SpectraVFS( sstrSOFMFileName, false );

        m_gridSizeSqr = m_pNet->getNumSpectra();
        m_gridSize = sqrtf(m_gridSizeSqr);

        if ( m_gridSizeSqr == 0 )
        {
            // error
            assert(0);
            exit(0);
        }
#endif
    }
#endif
}

AFAProcessing::~AFAProcessing()
{
}

void
AFAProcessing::calcFluxAndNormalizeInputDS()
{
    m_flux = 0.0f;
    for ( size_t i=0;i<m_numSpectra;i++ )
    {
        AFASpectra *a = &m_pSourceVFS[ i ];
        a->normalizeByFlux();
        m_flux = AFAMAX( a->m_flux, m_flux );
    }
}

void
AFAProcessing::reset( const AFAParameters &_params )
{
    // input from outside
    m_params = _params;
    m_Random.initRandom( m_params.randomSeed );

    m_currentStep = 0;
}

void
AFAProcessing::calcMinMax(
    AFASpectra *_vfs,
    float &_outMin,
    float &_outMax )
{
    _outMin = FLT_MAX;
    _outMax = -FLT_MAX;

    // calc min/max
    for ( size_t i=0;i<m_numSpectra;i++ )
    {
        AFASpectra *a = &m_pSourceVFS[ i ];

        if ( _outMin > a->m_Min ) 
        {
            _outMin = a->m_Min;
        }
        if ( _outMax < a->m_Max) 
        {
            _outMax = a->m_Max;
        }
    }
    // for sdds: -560 .. ~20.000 Angström
}


void
AFAProcessing::calcMinMaxInputDS()
{
    calcMinMax( m_pSourceVFS, m_Min, m_Max );
}
