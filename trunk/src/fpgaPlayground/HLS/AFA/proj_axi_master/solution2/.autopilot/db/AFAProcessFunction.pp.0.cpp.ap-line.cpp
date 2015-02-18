#pragma line 1 "proj_axi_master/src/AFAProcessFunction.cpp"
#pragma line 1 "proj_axi_master/src/AFAProcessFunction.cpp" 1
#pragma line 1 "<built-in>" 1
#pragma line 1 "<built-in>" 3
#pragma line 152 "<built-in>" 3
#pragma line 1 "<command line>" 1
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/common/technology/autopilot\\etc/autopilot_ssdm_op.h" 1
/* autopilot_ssdm_op.h*/
/*
#-  (c) Copyright 2011-2014 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************
#pragma empty_line
 *
 * $Id$
 */
#pragma line 145 "C:/Xilinx/Vivado_HLS/2014.4/common/technology/autopilot\\etc/autopilot_ssdm_op.h"
/*#define AP_SPEC_ATTR __attribute__ ((pure))*/
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
extern "C" {
#pragma empty_line
    /****** SSDM Intrinsics: OPERATIONS ***/
    // Interface operations
#pragma empty_line
    //typedef unsigned int __attribute__ ((bitwidth(1))) _uint1_;
    typedef bool _uint1_;
#pragma empty_line
    void _ssdm_op_IfRead(...) __attribute__ ((nothrow));
    void _ssdm_op_IfWrite(...) __attribute__ ((nothrow));
    _uint1_ _ssdm_op_IfNbRead(...) __attribute__ ((nothrow));
    _uint1_ _ssdm_op_IfNbWrite(...) __attribute__ ((nothrow));
    _uint1_ _ssdm_op_IfCanRead(...) __attribute__ ((nothrow));
    _uint1_ _ssdm_op_IfCanWrite(...) __attribute__ ((nothrow));
#pragma empty_line
    // Stream Intrinsics
    void _ssdm_StreamRead(...) __attribute__ ((nothrow));
    void _ssdm_StreamWrite(...) __attribute__ ((nothrow));
    _uint1_ _ssdm_StreamNbRead(...) __attribute__ ((nothrow));
    _uint1_ _ssdm_StreamNbWrite(...) __attribute__ ((nothrow));
    _uint1_ _ssdm_StreamCanRead(...) __attribute__ ((nothrow));
    _uint1_ _ssdm_StreamCanWrite(...) __attribute__ ((nothrow));
#pragma empty_line
    // Misc
    void _ssdm_op_MemShiftRead(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_Wait(...) __attribute__ ((nothrow));
    void _ssdm_op_Poll(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_Return(...) __attribute__ ((nothrow));
#pragma empty_line
    /* SSDM Intrinsics: SPECIFICATIONS */
    void _ssdm_op_SpecSynModule(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecTopModule(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecProcessDecl(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecProcessDef(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecPort(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecConnection(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecChannel(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecSensitive(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecModuleInst(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecPortMap(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecReset(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecPlatform(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecClockDomain(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecPowerDomain(...) __attribute__ ((nothrow));
#pragma empty_line
    int _ssdm_op_SpecRegionBegin(...) __attribute__ ((nothrow));
    int _ssdm_op_SpecRegionEnd(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecLoopName(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecLoopTripCount(...) __attribute__ ((nothrow));
#pragma empty_line
    int _ssdm_op_SpecStateBegin(...) __attribute__ ((nothrow));
    int _ssdm_op_SpecStateEnd(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecInterface(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecPipeline(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecDataflowPipeline(...) __attribute__ ((nothrow));
#pragma empty_line
#pragma empty_line
    void _ssdm_op_SpecLatency(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecParallel(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecProtocol(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecOccurrence(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecResource(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecResourceLimit(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecCHCore(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecFUCore(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecIFCore(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecIPCore(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecKeepValue(...) __attribute__ ((nothrow));
    void _ssdm_op_SpecMemCore(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecExt(...) __attribute__ ((nothrow));
    /*void* _ssdm_op_SpecProcess(...) SSDM_SPEC_ATTR;
    void* _ssdm_op_SpecEdge(...) SSDM_SPEC_ATTR; */
#pragma empty_line
    /* Presynthesis directive functions */
    void _ssdm_SpecArrayDimSize(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_RegionBegin(...) __attribute__ ((nothrow));
    void _ssdm_RegionEnd(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_Unroll(...) __attribute__ ((nothrow));
    void _ssdm_UnrollRegion(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_InlineAll(...) __attribute__ ((nothrow));
    void _ssdm_InlineLoop(...) __attribute__ ((nothrow));
    void _ssdm_Inline(...) __attribute__ ((nothrow));
    void _ssdm_InlineSelf(...) __attribute__ ((nothrow));
    void _ssdm_InlineRegion(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_SpecArrayMap(...) __attribute__ ((nothrow));
    void _ssdm_SpecArrayPartition(...) __attribute__ ((nothrow));
    void _ssdm_SpecArrayReshape(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_SpecStream(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_SpecExpr(...) __attribute__ ((nothrow));
    void _ssdm_SpecExprBalance(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_SpecDependence(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_SpecLoopMerge(...) __attribute__ ((nothrow));
    void _ssdm_SpecLoopFlatten(...) __attribute__ ((nothrow));
    void _ssdm_SpecLoopRewind(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_SpecFuncInstantiation(...) __attribute__ ((nothrow));
    void _ssdm_SpecFuncBuffer(...) __attribute__ ((nothrow));
    void _ssdm_SpecFuncExtract(...) __attribute__ ((nothrow));
    void _ssdm_SpecConstant(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_DataPack(...) __attribute__ ((nothrow));
    void _ssdm_SpecDataPack(...) __attribute__ ((nothrow));
#pragma empty_line
    void _ssdm_op_SpecBitsMap(...) __attribute__ ((nothrow));
#pragma empty_line
}
#pragma line 401 "C:/Xilinx/Vivado_HLS/2014.4/common/technology/autopilot\\etc/autopilot_ssdm_op.h"
/*#define _ssdm_op_WaitUntil(X) while (!(X)) _ssdm_op_Wait(1);
#define _ssdm_op_Delayed(X) X */
#pragma line 9 "<command line>" 2
#pragma line 1 "<built-in>" 2
#pragma line 1 "proj_axi_master/src/AFAProcessFunction.cpp" 2
#pragma empty_line
#pragma line 1 "proj_axi_master/src/../../include/AFATypes.h" 1
#pragma empty_line
#pragma empty_line
#pragma empty_line
typedef signed char sint8_t;
typedef unsigned char uint8_t;
typedef signed short sint16_t;
typedef unsigned short uint16_t;
typedef signed int sint32_t;
typedef unsigned int uint32_t;
typedef signed long long sint64_t;
typedef unsigned long long uint64_t;
#pragma empty_line
//typedef long unsigned int   size_t;
#pragma empty_line
typedef float float32_t;
typedef double float64_t;
#pragma line 3 "proj_axi_master/src/AFAProcessFunction.cpp" 2
#pragma line 1 "proj_axi_master/src/../../include/AFADefines.h" 1
#pragma empty_line
#pragma empty_line
#pragma empty_line
//const float FLT_MAX = 3.40282347e+38f;
#pragma line 4 "proj_axi_master/src/AFAProcessFunction.cpp" 2
#pragma line 1 "proj_axi_master/src/../../include/AFAProcessFunction.h" 1
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
typedef struct
{
 float32_t f;
 uint8_t i;
 bool b;
} AFAProcessFunctionParameter_t;
#pragma empty_line
void
AFAProcessFunction(
    volatile uint32_t *p,
    volatile void *srcNet,
    volatile void *dstNet );
#pragma line 5 "proj_axi_master/src/AFAProcessFunction.cpp" 2
#pragma line 1 "proj_axi_master/src/../../include/AFASpectra.h" 1
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/include\\stddef.h" 1 3 4
/*===---- stddef.h - Basic type definitions --------------------------------===
 *
 * Copyright (c) 2008 Eli Friedman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *===-----------------------------------------------------------------------===
 */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
typedef __typeof__(((int*)0)-((int*)0)) ptrdiff_t;
#pragma empty_line
#pragma empty_line
#pragma empty_line
typedef __typeof__(sizeof(int)) size_t;
#pragma line 56 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/include\\stddef.h" 3 4
/* Some C libraries expect to see a wint_t here. Others (notably MinGW) will use
__WINT_TYPE__ directly; accommodate both by requiring __need_wint_t */
#pragma line 5 "proj_axi_master/src/../../include/AFASpectra.h" 2
#pragma empty_line
#pragma empty_line
#pragma line 1 "proj_axi_master/src/../../include/AFASpectra.h" 1
#pragma line 8 "proj_axi_master/src/../../include/AFASpectra.h" 2
#pragma empty_line
#pragma empty_line
#pragma empty_line
// Can hold only one spectrum so the name is a bit misleading here.
//
// warning:
// changing order and / or size of this structure will invalidate all dumped files !
// 
class AFASpectra
{
public:
    static const int numSamplesSDSS = 3900; // number of samples in SDSS spectra
    static const int waveLenStartSDSS = 3800; // wavelength coverage (in Angström) for SDSS spectra (EDR..DR8)
    static const int waveLenEndSDSS = 9200;
#pragma empty_line
    static const int numSamplesBOSS = 4700; // number of samples in BOSS spectra
    static const int waveLenStartBOSS = 3650; // wavelength coverage (in Angström) for BOSS spectra (DR9 and upcoming)
    static const int waveLenEndBOSS = 10400;
#pragma empty_line
    static const int reductionFactor = 8; // reduce number of pixel by a factor of 8 (use only values of 2^n)
    static const int numSamples = numSamplesBOSS/reductionFactor; // number of samples in reduced spectra
    static const int numSpectraLines = 44; // number of stored emission and absorption lines
    static const float waveBeginDst; // spectrum start in destination system 
    static const float waveEndDst; // spectrum end in destination system 
#pragma empty_line
    static int pixelStart; // spectrum pixel range for compare and adaption processes [0..<pixelEnd], must be multiples of four.
    static int pixelEnd; // spectrum pixel range for compare and adaption processes [0..numSamples]
#pragma empty_line
#pragma empty_line
    /** @name TYPES
    */
    //@{
#pragma empty_line
#pragma empty_line
    enum SpectraVersion
    {
        SP_VERSION_INVALID=0, //< invalid version or spectra
        SP_ARTIFICIAL, //< artificial spectra, not loaded from any source.
        SP_CSV, //< from comma seperated values
        SP_VERSION_DR7, //<	Spectra DR1..DR7
        SP_VERSION_DR8, //< DR8, DR9
        SP_VERSION_BOSS, //< BOSS spectra from DR9/DR10, new spectrograph, different wavelenght range
        SP_VERSION_APOGEE, //< Infrared Apogee spectrom from SDSS III
        SP_LIGHTCURVE_SDSS, //< Lightcurve with associated SDSS info (plate id, MJD fiber id)
        SP_LIGHTCURVE_RADEC, //< Lightcurve with associated ra/dec
        SP_LIGHTCURVE_PLAIN, //< Lightcurve without any associated information
        SP_COUNT //< must be the last entry here.
    };
#pragma empty_line
    // was called objecttype in SDSS II
    // is now called source type in SDSS III
    enum SpectraType
    {
        SPT_NOT_SET = 0,
        SPT_UNKNOWN = 0x000000001,
        SPT_STAR = 0x000000002,
        SPT_GALAXY = 0x000000004,
        SPT_QSO = 0x000000008,
        SPT_HIZ_QSO = 0x000000010, //< high redshift QSO, z>2.3, Ly-alpha finding code is triggered  
        SPT_SKY = 0x000000020, //< blank sky
        SPT_STAR_LATE = 0x000000040, //< star dominated by molecular bands, Type M or later           
        SPT_GAL_EM = 0x000000080, //< emission line galaxy -- not set by the code       
        // the following types are SDSS III only
        SPT_QA = 0x000000100, //<  Quality assurance (assigned to more than one tile)
        SPT_STAR_PN = 0x000000200,
        SPT_STAR_CARBON = 0x000000400,
        SPT_STAR_BROWN_DWARF = 0x000000800,
        SPT_STAR_SUB_DWARF = 0x000001000,
        SPT_STAR_CATY_VAR = 0x000002000,
        SPT_STAR_RED_DWARF = 0x000004000,
        SPT_STAR_WHITE_DWARF = 0x000008000,
        SPT_STAR_BHB = 0x000010000,
        SPT_ROSAT_A = 0x000020000,
        SPT_ROSAT_B = 0x000040000,
        SPT_ROSAT_C = 0x000080000,
        SPT_ROSAT_D = 0x000100000,
        SPT_SPECTROPHOTO_STD = 0x000200000,
        SPT_HOT_STD = 0x000400000,
        SPT_SERENDIPITY_BLUE = 0x000800000,
        SPT_SERENDIPITY_FIRST = 0x001000000,
        SPT_SERENDIPITY_RED = 0x002000000,
        SPT_SERENDIPITY_DISTANT = 0x004000000,
        SPT_SERENDIPITY_MANUAL = 0x008000000,
        SPT_REDDEN_STD = 0x010000000,
        // the following types are from DR10:
        SPT_BLAZAR = 0x020000000,
        SPT_QSO_BAL = 0x040000000,
        SPT_EXOTIC = 0x080000000 //< pheletora of different new source types from DR10
    };
#pragma empty_line
#pragma empty_line
    enum SpectraMask
    {
        SP_MASK_OK = 0x000,
        SP_MASK_NOPLUG = 0x001, //  Fiber not listed in plugmap file                     
        SP_MASK_BADTRACE = 0x002, //  Bad trace from routine TRACE320CRUDE                 
        SP_MASK_BADFLAT = 0x004, //  Low counts in fiberflat                              
        SP_MASK_BADARC = 0x008, //  Bad arc solution                                     
        SP_MASK_MANYBADCOL = 0x010, //  More than 10% pixels are bad columns                 
        SP_MASK_MANYREJECT = 0x020, //  More than 10% pixels are rejected in extraction      
        SP_MASK_LARGESHIFT = 0x040, //  Large spatial shift between flat and object position 
        SP_MASK_NEARBADPIX = 0x10000, //  Bad pixel within 3 pixels of trace                   
        SP_MASK_LOWFLAT = 0x20000, //  Flat field less than 0.5                             
        SP_MASK_FULLREJECT = 0x40000, //  Pixel fully rejected in extraction                   
        SP_MASK_PARTIALREJ = 0x80000, //  Some pixels rejected in extraction                   
        SP_MASK_SCATLIGHT = 0x100000, //  Scattered light significant                          
        SP_MASK_CROSSTALK = 0x200000, //  Cross-talk significant                               
        SP_MASK_NOSKY = 0x400000, //  Sky level unknown at this wavelength                 
        SP_MASK_BRIGHTSKY = 0x800000, //  Sky level > flux + 10*(flux error)                   
        SP_MASK_NODATA = 0x1000000, //  No data available in combine B-spline                
        SP_MASK_COMBINEREJ = 0x2000000, //  Rejected in combine B-spline                         
        SP_MASK_BADFLUXFACTOR= 0x4000000, //  Low flux-calibration or flux-correction factor       
        SP_MASK_BADSKYCHI = 0x8000000, //  Chi^2 > 4 in sky residuals at this wavelength        
        SP_MASK_REDMONSTER = 0x10000000, //  Contiguous region of bad chi^2 in sky residuals      
        SP_MASK_EMLINE = 0x40000000 //  Emission line detected here                         
    };
#pragma empty_line
    enum SpectraNormalization
    {
        SN_NONE,
        SN_AMPLITUDE,
        SN_FLUX
    };
#pragma empty_line
    enum SpectraZStatus
    {
        SP_ZSTATUS_NOT_MEASURED = 0, // Redshift not yet measured.
        SP_ZSTATUS_FAILED = 1, // Redshift measurement failed.
        SP_ZSTATUS_INCONSISTENT = 2, // Redshift cross-correlation and emz both high-confidence but inconsistent.
        SP_ZSTATUS_XCORR_EMLINE = 3, // Redshift determined from cross-correlation and emz are consistent.
        SP_ZSTATUS_XCORR_HIC = 4, // Redshift determined from x-corr with high confidence.
        SP_ZSTATUS_XCORR_LOC = 5, // Redshift determined from cross-correlation with low confidence.
        SP_ZSTATUS_EMLINE_XCORR = 6, // Redshift from emz plus consistent xcorr redshift measurement.
        SP_ZSTATUS_EMLINE_HIC = 7, // Redshift determined from em-lines with high confidence.
        SP_ZSTATUS_EMLINE_LOC = 8, // Redshift determined from em-lines with low confidence.
        SP_ZSTATUS_MANUAL_HIC = 9, // Redshift determined "by hand" with high confidence.
        SP_ZSTATUS_MANUAL_LOC = 10, // Redshift determined "by hand" with low confidence.
        SP_ZSTATUS_4000BREAK = 11, // x-corr redshift determined when EW(4000Ã… break) > 0.95.
        SP_ZSTATUS_ABLINE_CAII = 12 // Redshift determined from average of CaII triplet fits.
    };
#pragma empty_line
    enum SpectraZWarning
    {
        SP_ZWARNING_OK = 0x00000000, //	No warnings.
        SP_ZWARNING_NO_SPEC = 0x00000001, //	No spectrum.
        SP_ZWARNING_NO_BLUE = 0x00000004, //	No blue side spectrum.
        SP_ZWARNING_NO_RED = 0x00000008, //	No red side spectrum.
        SP_ZWARNING_NOT_GAL = 0x00000010, //	Classification does not match galaxy target.
        SP_ZWARNING_NOT_QSO = 0x00000020, //	Classification does not match quasar target.
        SP_ZWARNING_NOT_STAR = 0x00000040, //	Classification does not match star target.
        SP_ZWARNING_GAL_COEF = 0x00000080, //	Galaxy PCA coefficients far off locus (placeholder).
        SP_ZWARNING_EMAB_INC = 0x00000100, //	Emission and absorption z's inconsistent.
        SP_ZWARNING_AB_INC = 0x00000200, //	Absorption redshifts inconsistent.
        SP_ZWARNING_EM_INC = 0x00000400, //	Emission redshifts inconsistent.
        SP_ZWARNING_HIZ = 0x00000800, //	Redshift is high.
        SP_ZWARNING_LOC = 0x00001000, //	Confidence is low.
        SP_ZWARNING_LOW_SNG = 0x00002000, //	Signal to noise is low in g.
        SP_ZWARNING_LOW_SNR = 0x00004000, //	Signal to noise is low in r .
        SP_ZWARNING_LOW_SNI = 0x00008000, //	Signal to noise is low in i.
        SP_ZWARNING_4000BREAK = 0x00010000, //	EW(4000A break) > 0.95.
        SP_ZWARNING_NOT_MAPPED = 0x00020000, //	Fiber was not mapped
        SP_ZWARNING_MANUAL_MAPPED = 0x00040000, //	Fiber mapping was set manually
        SP_ZWARNING_LOADER_MAPPED = 0x00080000 //	Single unmapped fiber re-mapped by DB loader
    };
#pragma empty_line
#pragma empty_line
    // emission and absorption lines info
    struct SpectraLines
    {
        float wave; // in Angström
        float waveMin; // in Angström
        float waveMax; // in Angström
        float height; // in 10^(-17) erg/cm/s^2/Ang
    };
#pragma empty_line
    //@}
#pragma empty_line
    /** @name CONSTRUCTORS / DESTRUCTORS
    */
    //@{
#pragma empty_line
    AFASpectra();
    AFASpectra( const AFASpectra &_source );
    ~AFASpectra();
#pragma empty_line
    //@}
#pragma empty_line
    /** @name ACCESSORS
    */
    //@{
#pragma empty_line
    // returns assembled filename
//	std::string getFileName() const;
#pragma empty_line
    // get modified Julian date of observation.
    // MJD starts at November 7.th 1858, see http://en.wikipedia.org/wiki/Julian_date
//	int getMJD() const;
#pragma empty_line
    // fiber ID for SDSS
    // 1..640 for SDSS I & II
    // 1..1000 for BOSS spectra
    // 1..300 for APOGEE spectra
    // note that on re-observations fibers may change
    // see http://spectro.princeton.edu/#plots
    //     https://www.sdss3.org/dr9/spectro/spectro_basics.php
//	int getFiber() const;
#pragma empty_line
    // plate id 
//	int getPlate() const;
#pragma empty_line
    // center point right ascension in J2000 decimal degrees
    // only for light curve import ATM.
//	double getRa() const;
#pragma empty_line
    // center point declination in J2000 decimal degrees
    // only for light curve import ATM.
//	double getDec() const;
#pragma empty_line
    // get SDSS URL to spectrum overview page
//	std::string getURL() const;
#pragma empty_line
    // get URL to SDSS image of spectrum
//	std::string getImgURL() const;
#pragma empty_line
#pragma empty_line
    // returns true if marked as empty spectrum (specObjID is set to zero).
//	bool isEmpty() const;
#pragma empty_line
    // returns true if the number of bad pixels in spectrum are over a threshold of 5%
//	bool hasBadPixels() const;
#pragma empty_line
    // return true if spectrum does not contain any awkward numbers, e.g. NANs, infinite numbers.
//	bool checkConsistency() const;
#pragma empty_line
    // compare spectra and return accumulated quadratic error of all compared samples (euclidean style).
//	float compare(const Spectra &_spectra) const;
#pragma empty_line
    //@}
#pragma empty_line
    /** @name MODIFIERS
    */
    //@{
#pragma empty_line
    // clear data/reset spectrum
 void clear();
#pragma empty_line
    void set(const AFASpectra &_spectra);
#pragma empty_line
    // set experimental test spectra
    // type: 0=sin, 1=cos, 2=linear, 3=linear inverse, 4=squared
//	void set( size_t _type, float _noize );
#pragma empty_line
    // set rectangular impulse (for testing purposes)
    // _phase 0..1, _width 0..1
//	void setRect( float _width=0.1, float _phase=0.5, float _amplitude=1.f );
#pragma empty_line
    // set sine curve with a given frequency, phase and amplitude.
//	void setSine( float _frequency = 1.f, float _phase = 0.f, float _amplitude=1.f, float _noize=0.f );
#pragma empty_line
    // fill spectrum with noise
 void randomize( float _minrange, float _maxrange);
#pragma empty_line
    // add signals from other spectra
//	void add(const Spectra &_spectra);
#pragma empty_line
    // add constant value to amplitudes
//	void add(float _value);
#pragma empty_line
    // subtract signals from other spectra
//	void subtract(const Spectra &_spectra);
#pragma empty_line
    // multiply signals with other spectra
//	void multiply( const Spectra &_spectra);
#pragma empty_line
    // scale signals
//	void multiply(float _multiplier);
#pragma empty_line
    // Load spectrum from comma separated values, we assume ~3900 samples.
    // spectrum should look like this:
    //
    //
    //	Wavelength(A),Flux,Error,Mask
    //	3830.01081226449, -1.97607004642487, 0, 83886080
    //	3830.01081226449, -1.97607004642487, 0, 83886080
    //  .. omitting next ~3900 lines.
    //	3833.54000759044, 1.56192994117737, 1.66190469264984, 0
    //
    // _logStream optional log stream for detailed logging output in case loading fails.
//	bool loadFromCSV(const std::string &_filename, std::ofstream *_logStream=NULL );
#pragma empty_line
    // Load spectrum or light curve from XML.
    // Here we do not read the number of samples directly, instead we read sparse data. 
    // A variable number of x,y value pairs are read and interpolated to the number of samples.
    //
    // Structure of the XML:
    //
    // 	<?xml version="1.0" encoding="UTF-8"?>
    // 	<LIGHTCURVE>
    // 		<type value="SDSS">										<!-- can be any type string, used to distinguish different datasets-->
    // 		<plate value="4096"/>									<!-- SDSS plate ID -->
    // 		<mjd value="55896"/>									<!-- SDSS modified Julian date of observation. -->
    // 		<fiberId value="4"/>									<!-- SDSS fiber ID 1..640 for SDSS, 1..1000 for BOSS -->
    // 		<ra value="0.01"/>										<!-- right ascension, J2000 -->
    // 		<dec value="0.056"/>									<!-- declination, J2000 -->
    // 		<z value="5.0"/>										<!-- red shift -->
    // 		<DATA xMin="0.5" xMax="4.0" yMin="-2.0" yMax="0.0" >
    // 		0.5,-1.2
    // 		0.55,-1.3
    // 		...
    // 		</DATA>
    // 	</LIGHTCURVE>
    //
    // _logStream optional log stream for detailed logging output in case loading fails.
//	bool loadFromXML(const std::string &_filename, std::ofstream *_logStream=NULL);
#pragma empty_line
    // automatically checks the version
    // _logStream optional log stream for detailed logging output in case loading fails.
//	bool loadFromFITS(const std::string &_filename, std::ofstream *_logStream=NULL);
#pragma empty_line
    // load from SDSS .fit file up to DR7
    // e.g. spSpec-51630-0266-633.fit
    // FITS file description see http://www.sdss.org/DR6/dm/flatFiles/spSpec.html
    // general info here: http://www.sdss.org/DR6/dm/flatFiles/FILES.html
//	bool loadFromFITS_SDSS(const std::string &_filename, std::ofstream *_logStream=NULL);
#pragma empty_line
    // load SDSS DR8 spectra
//	bool loadFromFITS_DR8(const std::string &_filename, std::ofstream *_logStream=NULL);
#pragma empty_line
    // BOSS Spectra from DR9/DR10. e.g. dr9spec-3588-55184-0511.fits
//	bool loadFromFITS_BOSS(const std::string &_filename, std::ofstream *_logStream=NULL);
#pragma empty_line
    // save to ASCII CSV
//	bool saveToCSV(const std::string &_filename);
#pragma empty_line
    // calculate extrema
    void calcMinMax();
#pragma empty_line
    // calculates the surface of the spectrum
    void calculateFlux();
#pragma empty_line
    // normalize to range -1..1
//	void normalize();
#pragma empty_line
    // normalize by flux
    void normalizeByFlux();
#pragma empty_line
    // adapt spectrum towards another spectrum by a given factor
    // _spectra spectrum to adapt to
    // _adaptionRate [0..1]
//	void adapt( const Spectra &_spectra, float _adaptionRate );
#pragma empty_line
    //@}
#pragma empty_line
#pragma empty_line
#pragma empty_line
    /** @name MEMBERS
    */
    //@{
#pragma empty_line
#pragma empty_line
    float m_Amplitude[numSamples]; // amplitude in 10^(-17) erg/cm/s^2/Ang
    float m_Min;
    float m_Max;
    sint32_t m_Index; // index to source spectrum [0..num src spectra-1], -1 = no src spectrum
    sint16_t m_SamplesRead;
    sint64_t m_SpecObjID; // spectra object identifier, encodes plate id, fiber id & MJD for SDSS spectra. Light curves with no SDSS association may use a simple hash)
    SpectraType m_Type;
    SpectraVersion m_version;
    double m_Z;
#pragma empty_line
#pragma empty_line
#pragma empty_line
    float m_flux;
    char m_status; // 0=ok, 1=bad pixels
    char pad[3]; // for padding to multiple of 16 byte boundaries
#pragma empty_line
#pragma empty_line
    //@}
#pragma empty_line
    /** @name HELPER FUNCTIONS
    */
    //@{
    void write( AFASpectra *spectraArray, size_t _gridSize, float _minPeak, float _maxPeak );
#pragma line 426 "proj_axi_master/src/../../include/AFASpectra.h"
    // if we use BOSS and SDSS spectra combined calculate offset for SDSS spectra.
    // BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
    static int getSDSSSpectraOffsetStart();
    static int getSDSSSpectraOffsetEnd();
#pragma empty_line
    // set to true if we should use the entire BOSS wavelength range during clusering or false if only default SDSS wavelength range should be used.
    static void setOperationRange( bool _BOSSWavelengthRange );
#pragma empty_line
    //@}
#pragma empty_line
};
#pragma line 6 "proj_axi_master/src/AFAProcessFunction.cpp" 2
#pragma line 1 "proj_axi_master/src/../../include/AFAProcessing.h" 1
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "proj_axi_master/src/../../include/AFANetworkSettings.h" 1
//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 7/19/2009
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  SOFMNetworkSettings.h
//! \brief Settings defines
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/include\\stddef.h" 1 3 4
/*===---- stddef.h - Basic type definitions --------------------------------===
 *
 * Copyright (c) 2008 Eli Friedman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *===-----------------------------------------------------------------------===
 */
#pragma line 56 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/include\\stddef.h" 3 4
/* Some C libraries expect to see a wint_t here. Others (notably MinGW) will use
__WINT_TYPE__ directly; accommodate both by requiring __need_wint_t */
#pragma line 24 "proj_axi_master/src/../../include/AFANetworkSettings.h" 2
#pragma empty_line
#pragma empty_line
class AFAParameters
{
public:
    typedef enum
    {
        AFANET_SETTINGS_SEARCHMODE_localfast,
        AFANET_SETTINGS_SEARCHMODE_local,
        AFANET_SETTINGS_SEARCHMODE_global,
#pragma empty_line
        AFANET_SETTINGS_NORMALIZATION_amplitude,
        AFANET_SETTINGS_NORMALIZATION_flux,
        AFANET_SETTINGS_NORMALIZATION_none,
#pragma empty_line
        AFANET_SETTINGS_WAVELENGTHRANGE_sdss,
        AFANET_SETTINGS_WAVELENGTHRANGE_boss
    } AFANET_SETTINGS;
#pragma empty_line
public:
    AFAParameters( size_t _numSteps, size_t _randomSeed, float _lRateBegin, float _lRateEnd, float _radiusBegin, float _radiusEnd);
#pragma empty_line
    size_t numSteps; // number of learn steps. reasonable: 10..500
    size_t randomSeed; // zero is not allowed
    float lRateBegin; // learn rate begin 0.0 .. 1.0
    float lRateEnd; // learn rate end (0.0 .. 1.0)
    float radiusBegin; // radius begin (0.0 .. grid size)
    float radiusEnd; // radius end (0.0 .. grid size)
    bool exportSubPage; // if true add sub pages to HTML output
    bool waitForUser; // if true wait for user input after each calculation step
    bool fullExport; // if true create a detailed export for each spectrum after final calculation step (online useful for small maps)
    int imageBoderSize; // to draw colored frames around the image icons, to distinguish between visited and not visited images
    int iconSize; // icon width/height in pixels for all spectra icons
//    std::string sstrSearchMode;									// search mode to control quality vs. computation time
    AFANET_SETTINGS sstrSearchMode; // search mode to control quality vs. computation time
    // SOFMNET_SETTINGS_SEARCHMODE_global     < uses global search for BMUs, best quality, runtime is O(n²)
    // SOFMNET_SETTINGS_SEARCHMODE_local      < uses local search for BMUs if possible.
    // SOFMNET_SETTINGS_SEARCHMODE_localfast  < uses very fast local search for BMUs if possible, runtime is O(n) 
    AFASpectra::SpectraNormalization normaliziationType; // type of normalization of spectra
#pragma empty_line
    bool useBOSSWavelengthRange; // process spectra in an extended wavelength window of 3650..10400 Angstrom 
    // instead of the 3800..92000 SDSS default window.
#pragma empty_line
    static AFAParameters defaultParameters;
};
#pragma line 5 "proj_axi_master/src/../../include/AFAProcessing.h" 2
#pragma empty_line
#pragma line 1 "proj_axi_master/src/../../include/AFARandom.h" 1
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
//! \file  random.h
//! \brief random number genration
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
// random number generation class
class AFARnd
{
public:
#pragma empty_line
 // random seed, zero is not allowed.
 AFARnd( unsigned long _seed = 4357 );
#pragma empty_line
 // reset random number generation to a given seed
 void initRandom( unsigned long _seed = 4357 );
#pragma empty_line
 // return random number [0..1]
 float randomFloat();
#pragma empty_line
 // return random number [0..1]
 double randomDouble();
#pragma empty_line
 // return random number with logarithmic scale [0..1], ex = erwartungswert 
 double randomDouleLog( float _ex );
#pragma empty_line
 // return random number [0..0xffffffff]
 unsigned int randomInt();
#pragma empty_line
 // return random number [0.._range]
 unsigned int randomInt( unsigned int _range );
#pragma empty_line
#pragma empty_line
private:
#pragma empty_line
 static const int N=624;
#pragma empty_line
 unsigned long m_mt[N]; // the array for the state vector 
 int m_mti;
};
#pragma line 6 "proj_axi_master/src/../../include/AFAProcessing.h" 2
#pragma empty_line
#pragma empty_line
class AFAProcessing
{
public:
    AFAProcessing(
        AFASpectra *spectraArray,
        AFASpectra *spectraArray2, // "sofmnet.bin"
        uint32_t numSpectra,
        bool continueProcessing = false );
    ~AFAProcessing();
#pragma empty_line
private:
#pragma empty_line
    // resets the network
    void reset( const AFAParameters &_params );
#pragma empty_line
    // one learning step
    // returns true if learning is finished and maximum number of learning steps are reached.
    bool process();
#pragma empty_line
    // grid size in cells of the map
    size_t m_gridSize;
#pragma empty_line
    // squared grid size, number of neurons
    size_t m_gridSizeSqr;
#pragma empty_line
    // current learning step
    size_t m_currentStep;
#pragma empty_line
    AFAParameters m_params;
#pragma empty_line
    // maximum amplitude (before normalization)
    float m_Min;
#pragma empty_line
    // minimum amplitude (before normalization)
    float m_Max;
#pragma empty_line
    // maximum flux of input spectra
    float m_flux;
#pragma empty_line
private:
    struct BestMatch
    {
        void reset();
        size_t index; //< index in the map
        float error; //< euclidean distance
    };
#pragma empty_line
    // calculate min/max values for a given SpectraVFS
    void calcMinMax( AFASpectra *_vfs, float &_outMin, float &_outMax );
#pragma empty_line
    // calculate min/max values for the input data set.
    void calcMinMaxInputDS();
#pragma empty_line
    // calculate flux values for the input data set and normalize
    void calcFluxAndNormalizeInputDS();
#pragma empty_line
    // search for best matching spectrum/neuron in the network
    // this version will perform a brute-force full search in the entire network
    // _src source spectra
    // returns best spectra in the network
//    BestMatch searchBestMatchComplete( const Spectra &_src );
#pragma empty_line
    // search for best matching spectrum/neuron in the network using only a local window ( s_searchRadius )
    // _src source spectra
    // returns best spectra in the network
//    BestMatch searchBestMatchLocal( const Spectra &_src, const int _searchRadius );
#pragma empty_line
    // adapt network for a given neuron/spectrum
    // _spectrum source spectrum to adapt
    // _bestMatchIndex index into network where it best matches
    // _adaptionThreshold adaption threshold so we do not need to go through the whole network
    // _sigmaSqr sigma squared
    // _lRate current learning rate for the given processing step
//    void adaptNetwork( const Spectra &_spectrum, size_t _bestMatchIndex, float _adaptionThreshold, float _sigmaSqr, float _lRate );
#pragma empty_line
    // calculate index from cell positions
    size_t getIndex( size_t _cellX, size_t _cellY );
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
    // code book spectra
    AFASpectra *m_pNet;
#pragma empty_line
    // training data
    AFASpectra *m_pSourceVFS;
#pragma empty_line
    // number of source spectra
    const size_t m_numSpectra;
#pragma empty_line
    // random number generator
    AFARnd m_Random; //<<<< initialized twice!
};
#pragma line 7 "proj_axi_master/src/AFAProcessFunction.cpp" 2
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include/_mingw_mac.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma line 18 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include/_mingw_mac.h" 3
/* mingw.org's version macros: these make gcc to define
   MINGW32_SUPPORTS_MT_EH and to use the _CRT_MT global
   and the __mingwthr_key_dtor() function from the MinGW
   CRT in its private gthr-win32.h header. */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
/* MS does not prefix symbols by underscores for 64-bit.  */
#pragma empty_line
/* As we have to support older gcc version, which are using underscores
   as symbol prefix for x64, we have to check here for the user label
   prefix defined by gcc.  */
#pragma line 62 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include/_mingw_mac.h" 3
/* Use alias for msvcr80 export of get/set_output_format.  */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
/* Set VC specific compiler target macros.  */
#pragma line 10 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 2 3
#pragma empty_line
#pragma empty_line
/* C/C++ specific language defines.  */
#pragma line 32 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
/* Note the extern. This is needed to work around GCC's
limitations in handling dllimport attribute.  */
#pragma line 147 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
/* Attribute `nonnull' was valid as of gcc 3.3.  We don't use GCC's
   variadiac macro facility, because variadic macros cause syntax
   errors with  --traditional-cpp.  */
#pragma line 225 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
/*  High byte is the major version, low byte is the minor. */
#pragma line 247 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
/*typedef int __int128 __attribute__ ((__mode__ (TI)));*/
#pragma line 277 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\vadefs.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma line 674 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include/sdks/_mingw_directx.h" 1 3
#pragma line 674 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 2 3
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include/sdks/_mingw_ddk.h" 1 3
#pragma line 675 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 2 3
#pragma line 13 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\vadefs.h" 2 3
#pragma empty_line
#pragma empty_line
#pragma pack(push,_CRT_PACKING)
#pragma empty_line
#pragma empty_line
extern "C" {
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  typedef __builtin_va_list __gnuc_va_list;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  typedef __gnuc_va_list va_list;
#pragma line 46 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\vadefs.h" 3
/* Use GCC builtins */
#pragma line 99 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\vadefs.h" 3
}
#pragma empty_line
#pragma empty_line
#pragma pack(pop)
#pragma line 277 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 2 3
#pragma empty_line
#pragma empty_line
#pragma pack(push,_CRT_PACKING)
#pragma line 316 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
/* We have to define _DLL for gcc based mingw version. This define is set
   by VC, when DLL-based runtime is used. So, gcc based runtime just have
   DLL-base runtime, therefore this define has to be set.
   As our headers are possibly used by windows compiler having a static
   C-runtime, we make this definition gnu compiler specific here.  */
#pragma line 370 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
__extension__ typedef unsigned long long size_t;
#pragma line 380 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
__extension__ typedef long long ssize_t;
#pragma line 392 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
__extension__ typedef long long intptr_t;
#pragma line 405 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
__extension__ typedef unsigned long long uintptr_t;
#pragma line 418 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
__extension__ typedef long long ptrdiff_t;
#pragma line 436 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
#pragma line 456 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
typedef int errno_t;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
typedef long __time32_t;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
__extension__ typedef long long __time64_t;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
typedef __time64_t time_t;
#pragma line 518 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
/* _dowildcard is an int that controls the globbing of the command line.
 * The MinGW32 (mingw.org) runtime calls it _CRT_glob, so we are adding
 * a compatibility definition here:  you can use either of _CRT_glob or
 * _dowildcard .
 * If _dowildcard is non-zero, the command line will be globbed:  *.*
 * will be expanded to be all files in the startup directory.
 * In the mingw-w64 library a _dowildcard variable is defined as being
 * 0, therefore command line globbing is DISABLED by default. To turn it
 * on and to leave wildcard command line processing MS's globbing code,
 * include a line in one of your source modules defining _dowildcard and
 * setting it to -1, like so:
 * int _dowildcard = -1;
 */
#pragma line 605 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 3
/* MSVC-isms: */
#pragma empty_line
struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;
#pragma empty_line
typedef struct localeinfo_struct {
  pthreadlocinfo locinfo;
  pthreadmbcinfo mbcinfo;
} _locale_tstruct,*_locale_t;
#pragma empty_line
#pragma empty_line
#pragma empty_line
typedef struct tagLC_ID {
  unsigned short wLanguage;
  unsigned short wCountry;
  unsigned short wCodePage;
} LC_ID,*LPLC_ID;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
typedef struct threadlocaleinfostruct {
  int refcount;
  unsigned int lc_codepage;
  unsigned int lc_collate_cp;
  unsigned long lc_handle[6];
  LC_ID lc_id[6];
  struct {
    char *locale;
    wchar_t *wlocale;
    int *refcount;
    int *wrefcount;
  } lc_category[6];
  int lc_clike;
  int mb_cur_max;
  int *lconv_intl_refcount;
  int *lconv_num_refcount;
  int *lconv_mon_refcount;
  struct lconv *lconv;
  int *ctype1_refcount;
  unsigned short *ctype1;
  const unsigned short *pctype;
  const unsigned char *pclmap;
  const unsigned char *pcumap;
  struct __lc_time_data *lc_time_curr;
} threadlocinfo;
#pragma empty_line
#pragma empty_line
#pragma empty_line
extern "C" {
#pragma empty_line
#pragma empty_line
/* mingw-w64 specific functions: */
const char *__mingw_get_crt_info (void);
#pragma empty_line
#pragma empty_line
}
#pragma empty_line
#pragma empty_line
#pragma pack(pop)
#pragma line 9 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 2 3
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw_print_push.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma empty_line
/* Undefine __mingw_<printf> macros.  */
#pragma line 11 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 2 3
#pragma empty_line
#pragma empty_line
#pragma pack(push,_CRT_PACKING)
#pragma empty_line
#pragma empty_line
extern "C" {
#pragma line 26 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  struct _iobuf {
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;
  };
  typedef struct _iobuf FILE;
#pragma line 84 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  typedef long _off_t;
#pragma empty_line
  typedef long off_t;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  __extension__ typedef long long _off64_t;
#pragma empty_line
  __extension__ typedef long long off64_t;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  __attribute__ ((__dllimport__)) FILE * __iob_func(void);
#pragma line 120 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  __extension__ typedef long long fpos_t;
#pragma line 157 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  __attribute__ ((__dllimport__)) int _filbuf(FILE *_File);
  __attribute__ ((__dllimport__)) int _flsbuf(int _Ch,FILE *_File);
#pragma empty_line
#pragma empty_line
#pragma empty_line
  __attribute__ ((__dllimport__)) FILE * _fsopen(const char *_Filename,const char *_Mode,int _ShFlag);
#pragma empty_line
  void clearerr(FILE *_File);
  int fclose(FILE *_File);
  __attribute__ ((__dllimport__)) int _fcloseall(void);
#pragma empty_line
#pragma empty_line
#pragma empty_line
  __attribute__ ((__dllimport__)) FILE * _fdopen(int _FileHandle,const char *_Mode);
#pragma empty_line
  int feof(FILE *_File);
  int ferror(FILE *_File);
  int fflush(FILE *_File);
  int fgetc(FILE *_File);
  __attribute__ ((__dllimport__)) int _fgetchar(void);
  int fgetpos(FILE * __restrict__ _File ,fpos_t * __restrict__ _Pos);
  char * fgets(char * __restrict__ _Buf,int _MaxCount,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int _fileno(FILE *_File);
#pragma empty_line
#pragma empty_line
#pragma empty_line
  __attribute__ ((__dllimport__)) char * _tempnam(const char *_DirName,const char *_FilePrefix);
  __attribute__ ((__dllimport__)) int _flushall(void);
  FILE * fopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode) ;
  FILE *fopen64(const char * __restrict__ filename,const char * __restrict__ mode);
  int fprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,...);
  int fputc(int _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) int _fputchar(int _Ch);
  int fputs(const char * __restrict__ _Str,FILE * __restrict__ _File);
  size_t fread(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  FILE * freopen(const char * __restrict__ _Filename,const char * __restrict__ _Mode,FILE * __restrict__ _File) ;
  int fscanf(FILE * __restrict__ _File,const char * __restrict__ _Format,...) ;
  int _fscanf_l(FILE * __restrict__ _File,const char * __restrict__ _Format,_locale_t locale,...) ;
  int fsetpos(FILE *_File,const fpos_t *_Pos);
  int fseek(FILE *_File,long _Offset,int _Origin);
  int fseeko64(FILE* stream, _off64_t offset, int whence);
  long ftell(FILE *_File);
  _off64_t ftello64(FILE * stream);
  __extension__ int _fseeki64(FILE *_File,long long _Offset,int _Origin);
  __extension__ long long _ftelli64(FILE *_File);
  size_t fwrite(const void * __restrict__ _Str,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  int getc(FILE *_File);
  int getchar(void);
  __attribute__ ((__dllimport__)) int _getmaxstdio(void);
  char * gets(char *_Buffer) ;
  int _getw(FILE *_File);
#pragma empty_line
#pragma empty_line
  void perror(const char *_ErrMsg);
#pragma empty_line
  __attribute__ ((__dllimport__)) int _pclose(FILE *_File);
  __attribute__ ((__dllimport__)) FILE * _popen(const char *_Command,const char *_Mode);
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  int printf(const char * __restrict__ _Format,...);
  int putc(int _Ch,FILE *_File);
  int putchar(int _Ch);
  int puts(const char *_Str);
  __attribute__ ((__dllimport__)) int _putw(int _Word,FILE *_File);
#pragma empty_line
#pragma empty_line
  int remove(const char *_Filename);
  int rename(const char *_OldFilename,const char *_NewFilename);
  __attribute__ ((__dllimport__)) int _unlink(const char *_Filename);
#pragma empty_line
  int unlink(const char *_Filename) ;
#pragma empty_line
#pragma empty_line
  void rewind(FILE *_File);
  __attribute__ ((__dllimport__)) int _rmtmp(void);
  int scanf(const char * __restrict__ _Format,...) ;
  int _scanf_l(const char * __restrict__ format,_locale_t locale,... ) ;
  void setbuf(FILE * __restrict__ _File,char * __restrict__ _Buffer) ;
  __attribute__ ((__dllimport__)) int _setmaxstdio(int _Max);
  __attribute__ ((__dllimport__)) unsigned int _set_output_format(unsigned int _Format);
  __attribute__ ((__dllimport__)) unsigned int _get_output_format(void);
  unsigned int __mingw_set_output_format(unsigned int _Format);
  unsigned int __mingw_get_output_format(void);
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  int setvbuf(FILE * __restrict__ _File,char * __restrict__ _Buf,int _Mode,size_t _Size);
  __attribute__ ((__dllimport__)) int _scprintf(const char * __restrict__ _Format,...);
  int sscanf(const char * __restrict__ _Src,const char * __restrict__ _Format,...) ;
  int _sscanf_l(const char * __restrict__ buffer,const char * __restrict__ format,_locale_t locale,...) ;
  __attribute__ ((__dllimport__)) int _snscanf(const char * __restrict__ _Src,size_t _MaxCount,const char * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int _snscanf_l(const char * __restrict__ input,size_t length,const char * __restrict__ format,_locale_t locale,...) ;
  FILE * tmpfile(void) ;
  char * tmpnam(char *_Buffer);
  int ungetc(int _Ch,FILE *_File);
  int vfprintf(FILE * __restrict__ _File,const char * __restrict__ _Format,va_list _ArgList);
  int vprintf(const char * __restrict__ _Format,va_list _ArgList);
#pragma empty_line
  /* Make sure macros are not defined.  */
  extern
    __attribute__((__format__ (gnu_printf, 3, 0))) __attribute__ ((__nonnull__ (3)))
    int __mingw_vsnprintf(char * __restrict__ _DstBuf,size_t _MaxCount,const char * __restrict__ _Format,
      va_list _ArgList);
  extern
    __attribute__((__format__ (gnu_printf, 3, 4))) __attribute__ ((__nonnull__ (3)))
    int __mingw_snprintf(char * __restrict__ s, size_t n, const char * __restrict__ format, ...);
  extern
    __attribute__((__format__ (gnu_printf, 1, 2))) __attribute__ ((__nonnull__ (1)))
    int __mingw_printf(const char * __restrict__ , ... ) __attribute__ ((__nothrow__));
  extern
    __attribute__((__format__ (gnu_printf, 1, 0))) __attribute__ ((__nonnull__ (1)))
    int __mingw_vprintf (const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
  extern
    __attribute__((__format__ (gnu_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
    int __mingw_fprintf (FILE * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
  extern
    __attribute__((__format__ (gnu_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
    int __mingw_vfprintf (FILE * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
  extern
    __attribute__((__format__ (gnu_printf, 2, 3))) __attribute__ ((__nonnull__ (2)))
    int __mingw_sprintf (char * __restrict__ , const char * __restrict__ , ...) __attribute__ ((__nothrow__));
  extern
    __attribute__((__format__ (gnu_printf, 2, 0))) __attribute__ ((__nonnull__ (2)))
    int __mingw_vsprintf (char * __restrict__ , const char * __restrict__ , va_list) __attribute__ ((__nothrow__));
#pragma empty_line
  __attribute__ ((__dllimport__)) int _snprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int _snprintf_l(char * __restrict__ buffer,size_t count,const char * __restrict__ format,_locale_t locale,...) ;
  __attribute__ ((__dllimport__)) int _vsnprintf(char * __restrict__ _Dest,size_t _Count,const char * __restrict__ _Format,va_list _Args) ;
  __attribute__ ((__dllimport__)) int _vsnprintf_l(char * __restrict__ buffer,size_t count,const char * __restrict__ format,_locale_t locale,va_list argptr) ;
  int sprintf(char * __restrict__ _Dest,const char * __restrict__ _Format,...) ;
  int _sprintf_l(char * __restrict__ buffer,const char * __restrict__ format,_locale_t locale,...) ;
  int vsprintf(char * __restrict__ _Dest,const char * __restrict__ _Format,va_list _Args) ;
#pragma empty_line
/* this is here to deal with software defining
 * vsnprintf as _vsnprintf, eg. libxml2.  */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  int vsnprintf(char * __restrict__ _DstBuf,size_t _MaxCount,const char * __restrict__ _Format,va_list _ArgList) ;
#pragma empty_line
  int snprintf(char * __restrict__ s, size_t n, const char * __restrict__ format, ...);
#pragma line 312 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  int vscanf(const char * __restrict__ Format, va_list argp);
  int vfscanf (FILE * __restrict__ fp, const char * __restrict__ Format,va_list argp);
  int vsscanf (const char * __restrict__ _Str,const char * __restrict__ Format,va_list argp);
#pragma empty_line
  __attribute__ ((__dllimport__)) int _vscprintf(const char * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _set_printf_count_output(int _Value);
  __attribute__ ((__dllimport__)) int _get_printf_count_output(void);
#pragma line 330 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  __attribute__ ((__dllimport__)) FILE * _wfsopen(const wchar_t *_Filename,const wchar_t *_Mode,int _ShFlag);
#pragma empty_line
#pragma empty_line
  wint_t fgetwc(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t _fgetwchar(void);
  wint_t fputwc(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t _fputwchar(wchar_t _Ch);
  wint_t getwc(FILE *_File);
  wint_t getwchar(void);
  wint_t putwc(wchar_t _Ch,FILE *_File);
  wint_t putwchar(wchar_t _Ch);
  wint_t ungetwc(wint_t _Ch,FILE *_File);
  wchar_t * fgetws(wchar_t * __restrict__ _Dst,int _SizeInWords,FILE * __restrict__ _File);
  int fputws(const wchar_t * __restrict__ _Str,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) wchar_t * _getws(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) int _putws(const wchar_t *_Str);
  int fwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
  int wprintf(const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int _scwprintf(const wchar_t * __restrict__ _Format,...);
  int vfwprintf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
  int vwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int swprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ , ...) ;
  __attribute__ ((__dllimport__)) int _swprintf_l(wchar_t * __restrict__ buffer,size_t count,const wchar_t * __restrict__ format,_locale_t locale,... ) ;
  __attribute__ ((__dllimport__)) int vswprintf(wchar_t * __restrict__ , const wchar_t * __restrict__ ,va_list) ;
  __attribute__ ((__dllimport__)) int _swprintf_c(wchar_t * __restrict__ _DstBuf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int _vswprintf_c(wchar_t * __restrict__ _DstBuf,size_t _SizeInWords,const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _snwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int _vsnwprintf(wchar_t * __restrict__ _Dest,size_t _Count,const wchar_t * __restrict__ _Format,va_list _Args) ;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  int snwprintf (wchar_t * __restrict__ s, size_t n, const wchar_t * __restrict__ format, ...);
  int vsnwprintf (wchar_t * __restrict__ , size_t, const wchar_t * __restrict__ , va_list);
#pragma line 373 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  int vwscanf (const wchar_t * __restrict__ , va_list);
  int vfwscanf (FILE * __restrict__ ,const wchar_t * __restrict__ ,va_list);
  int vswscanf (const wchar_t * __restrict__ ,const wchar_t * __restrict__ ,va_list);
#pragma empty_line
  __attribute__ ((__dllimport__)) int _fwprintf_p(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int _wprintf_p(const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int _vfwprintf_p(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _vwprintf_p(const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _swprintf_p(wchar_t * __restrict__ _DstBuf,size_t _MaxCount,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int _vswprintf_p(wchar_t * __restrict__ _DstBuf,size_t _MaxCount,const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _scwprintf_p(const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int _vscwprintf_p(const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _wprintf_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _wprintf_p_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _vwprintf_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _vwprintf_p_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _fwprintf_l(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _fwprintf_p_l(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _vfwprintf_l(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _vfwprintf_p_l(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _swprintf_c_l(wchar_t * __restrict__ _DstBuf,size_t _MaxCount,const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _swprintf_p_l(wchar_t * __restrict__ _DstBuf,size_t _MaxCount,const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _vswprintf_c_l(wchar_t * __restrict__ _DstBuf,size_t _MaxCount,const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _vswprintf_p_l(wchar_t * __restrict__ _DstBuf,size_t _MaxCount,const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _scwprintf_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _scwprintf_p_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _vscwprintf_p_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _snwprintf_l(wchar_t * __restrict__ _DstBuf,size_t _MaxCount,const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  __attribute__ ((__dllimport__)) int _vsnwprintf_l(wchar_t * __restrict__ _DstBuf,size_t _MaxCount,const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList) ;
  __attribute__ ((__dllimport__)) int _swprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int _vswprintf(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,va_list _Args);
  __attribute__ ((__dllimport__)) int __swprintf_l(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,_locale_t _Plocinfo,...) ;
  __attribute__ ((__dllimport__)) int _vswprintf_l(wchar_t * __restrict__ buffer,size_t count,const wchar_t * __restrict__ format,_locale_t locale,va_list argptr) ;
  __attribute__ ((__dllimport__)) int __vswprintf_l(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Format,_locale_t _Plocinfo,va_list _Args) ;
#pragma line 417 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  __attribute__ ((__dllimport__)) wchar_t * _wtempnam(const wchar_t *_Directory,const wchar_t *_FilePrefix);
  __attribute__ ((__dllimport__)) int _vscwprintf(const wchar_t * __restrict__ _Format,va_list _ArgList);
  __attribute__ ((__dllimport__)) int _vscwprintf_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,va_list _ArgList);
  int fwscanf(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int _fwscanf_l(FILE * __restrict__ _File,const wchar_t * __restrict__ _Format,_locale_t _Locale,...) ;
  int swscanf(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int _swscanf_l(const wchar_t * __restrict__ _Src,const wchar_t * __restrict__ _Format,_locale_t _Locale,...) ;
  __attribute__ ((__dllimport__)) int _snwscanf(const wchar_t * __restrict__ _Src,size_t _MaxCount,const wchar_t * __restrict__ _Format,...);
  __attribute__ ((__dllimport__)) int _snwscanf_l(const wchar_t * __restrict__ _Src,size_t _MaxCount,const wchar_t * __restrict__ _Format,_locale_t _Locale,...);
  int wscanf(const wchar_t * __restrict__ _Format,...) ;
  __attribute__ ((__dllimport__)) int _wscanf_l(const wchar_t * __restrict__ _Format,_locale_t _Locale,...) ;
  __attribute__ ((__dllimport__)) FILE * _wfdopen(int _FileHandle ,const wchar_t *_Mode);
  __attribute__ ((__dllimport__)) FILE * _wfopen(const wchar_t * __restrict__ _Filename,const wchar_t *__restrict__ _Mode) ;
  __attribute__ ((__dllimport__)) FILE * _wfreopen(const wchar_t * __restrict__ _Filename,const wchar_t * __restrict__ _Mode,FILE * __restrict__ _OldFile) ;
#pragma empty_line
#pragma empty_line
#pragma empty_line
  __attribute__ ((__dllimport__)) void _wperror(const wchar_t *_ErrMsg);
#pragma empty_line
  __attribute__ ((__dllimport__)) FILE * _wpopen(const wchar_t *_Command,const wchar_t *_Mode);
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  __attribute__ ((__dllimport__)) int _wremove(const wchar_t *_Filename);
  __attribute__ ((__dllimport__)) wchar_t * _wtmpnam(wchar_t *_Buffer);
  __attribute__ ((__dllimport__)) wint_t _fgetwc_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) wint_t _fputwc_nolock(wchar_t _Ch,FILE *_File);
  __attribute__ ((__dllimport__)) wint_t _ungetwc_nolock(wint_t _Ch,FILE *_File);
#pragma line 475 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 3
  __attribute__ ((__dllimport__)) void _lock_file(FILE *_File);
  __attribute__ ((__dllimport__)) void _unlock_file(FILE *_File);
  __attribute__ ((__dllimport__)) int _fclose_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) int _fflush_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) size_t _fread_nolock(void * __restrict__ _DstBuf,size_t _ElementSize,size_t _Count,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int _fseek_nolock(FILE *_File,long _Offset,int _Origin);
  __attribute__ ((__dllimport__)) long _ftell_nolock(FILE *_File);
  __extension__ __attribute__ ((__dllimport__)) int _fseeki64_nolock(FILE *_File,long long _Offset,int _Origin);
  __extension__ __attribute__ ((__dllimport__)) long long _ftelli64_nolock(FILE *_File);
  __attribute__ ((__dllimport__)) size_t _fwrite_nolock(const void * __restrict__ _DstBuf,size_t _Size,size_t _Count,FILE * __restrict__ _File);
  __attribute__ ((__dllimport__)) int _ungetc_nolock(int _Ch,FILE *_File);
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  char * tempnam(const char *_Directory,const char *_FilePrefix) ;
  int fcloseall(void) ;
  FILE * fdopen(int _FileHandle,const char *_Format) ;
  int fgetchar(void) ;
  int fileno(FILE *_File) ;
  int flushall(void) ;
  int fputchar(int _Ch) ;
  int getw(FILE *_File) ;
  int putw(int _Ch,FILE *_File) ;
  int rmtmp(void) ;
#pragma empty_line
#pragma empty_line
#pragma empty_line
}
#pragma empty_line
#pragma empty_line
#pragma pack(pop)
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\sec_api/stdio_s.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma line 9 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\sec_api/stdio_s.h" 2 3
#pragma line 509 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 2 3
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw_print_pop.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma empty_line
/* Define __mingw_<printf> macros.  */
#pragma line 511 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\stdio.h" 2 3
#pragma line 9 "proj_axi_master/src/AFAProcessFunction.cpp" 2
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\string.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\_mingw.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma line 9 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\string.h" 2 3
#pragma empty_line
#pragma empty_line
#pragma empty_line
extern "C" {
#pragma line 36 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\string.h" 3
  __attribute__ ((__dllimport__)) void * _memccpy(void *_Dst,const void *_Src,int _Val,size_t _MaxCount);
                void * memchr(const void *_Buf ,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int _memicmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  __attribute__ ((__dllimport__)) int _memicmp_l(const void *_Buf1,const void *_Buf2,size_t _Size,_locale_t _Locale);
  int memcmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  void * memcpy(void * __restrict__ _Dst,const void * __restrict__ _Src,size_t _Size) ;
  void * memset(void *_Dst,int _Val,size_t _Size);
#pragma empty_line
  void * memccpy(void *_Dst,const void *_Src,int _Val,size_t _Size) ;
  int memicmp(const void *_Buf1,const void *_Buf2,size_t _Size) ;
#pragma empty_line
#pragma empty_line
  char * _strset(char *_Str,int _Val) ;
  char * _strset_l(char *_Str,int _Val,_locale_t _Locale) ;
  char * strcpy(char * __restrict__ _Dest,const char * __restrict__ _Source);
  char * strcat(char * __restrict__ _Dest,const char * __restrict__ _Source);
  int strcmp(const char *_Str1,const char *_Str2);
  size_t strlen(const char *_Str);
  size_t strnlen(const char *_Str,size_t _MaxCount);
  void * memmove(void *_Dst,const void *_Src,size_t _Size) ;
  __attribute__ ((__dllimport__)) char * _strdup(const char *_Src);
                char * strchr(const char *_Str,int _Val);
  __attribute__ ((__dllimport__)) int _stricmp(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int _strcmpi(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int _stricmp_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  int strcoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int _strcoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int _stricoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int _stricoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int _strncoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int _strncoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int _strnicoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int _strnicoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  size_t strcspn(const char *_Str,const char *_Control);
  __attribute__ ((__dllimport__)) char * _strerror(const char *_ErrMsg) ;
  char * strerror(int) ;
  __attribute__ ((__dllimport__)) char * _strlwr(char *_String) ;
  char *strlwr_l(char *_String,_locale_t _Locale) ;
  char * strncat(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  int strncmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int _strnicmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int _strnicmp_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  char *strncpy(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  __attribute__ ((__dllimport__)) char * _strnset(char *_Str,int _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) char * _strnset_l(char *str,int c,size_t count,_locale_t _Locale) ;
                char * strpbrk(const char *_Str,const char *_Control);
                char * strrchr(const char *_Str,int _Ch);
  __attribute__ ((__dllimport__)) char * _strrev(char *_Str);
  size_t strspn(const char *_Str,const char *_Control);
                char * strstr(const char *_Str,const char *_SubStr);
  char * strtok(char * __restrict__ _Str,const char * __restrict__ _Delim) ;
  __attribute__ ((__dllimport__)) char * _strupr(char *_String) ;
  __attribute__ ((__dllimport__)) char *_strupr_l(char *_String,_locale_t _Locale) ;
  size_t strxfrm(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t _strxfrm_l(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);
#pragma empty_line
#pragma empty_line
  char * strdup(const char *_Src) ;
  int strcmpi(const char *_Str1,const char *_Str2) ;
  int stricmp(const char *_Str1,const char *_Str2) ;
  char * strlwr(char *_Str) ;
  int strnicmp(const char *_Str1,const char *_Str,size_t _MaxCount) ;
  int strncasecmp (const char *, const char *, size_t);
  int strcasecmp (const char *, const char *);
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  char * strnset(char *_Str,int _Val,size_t _MaxCount) ;
  char * strrev(char *_Str) ;
  char * strset(char *_Str,int _Val) ;
  char * strupr(char *_Str) ;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
  __attribute__ ((__dllimport__)) wchar_t * _wcsdup(const wchar_t *_Str);
  wchar_t * wcscat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
                wchar_t * wcschr(const wchar_t *_Str,wchar_t _Ch);
  int wcscmp(const wchar_t *_Str1,const wchar_t *_Str2);
  wchar_t * wcscpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  size_t wcscspn(const wchar_t *_Str,const wchar_t *_Control);
  size_t wcslen(const wchar_t *_Str);
  size_t wcsnlen(const wchar_t *_Src,size_t _MaxCount);
  wchar_t *wcsncat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  int wcsncmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  wchar_t *wcsncpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  wchar_t * _wcsncpy_l(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count,_locale_t _Locale) ;
                wchar_t * wcspbrk(const wchar_t *_Str,const wchar_t *_Control);
                wchar_t * wcsrchr(const wchar_t *_Str,wchar_t _Ch);
  size_t wcsspn(const wchar_t *_Str,const wchar_t *_Control);
                wchar_t * wcsstr(const wchar_t *_Str,const wchar_t *_SubStr);
  wchar_t * wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim) ;
  __attribute__ ((__dllimport__)) wchar_t * _wcserror(int _ErrNum) ;
  __attribute__ ((__dllimport__)) wchar_t * __wcserror(const wchar_t *_Str) ;
  __attribute__ ((__dllimport__)) int _wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int _wcsicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int _wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int _wcsnicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t * _wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) wchar_t * _wcsrev(wchar_t *_Str);
  __attribute__ ((__dllimport__)) wchar_t * _wcsset(wchar_t *_Str,wchar_t _Val) ;
  __attribute__ ((__dllimport__)) wchar_t * _wcslwr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcslwr_l(wchar_t *_String,_locale_t _Locale) ;
  __attribute__ ((__dllimport__)) wchar_t * _wcsupr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcsupr_l(wchar_t *_String,_locale_t _Locale) ;
  size_t wcsxfrm(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t _wcsxfrm_l(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);
  int wcscoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int _wcscoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int _wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int _wcsicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int _wcsncoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int _wcsncoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int _wcsnicoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int _wcsnicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
#pragma empty_line
#pragma empty_line
  wchar_t * wcsdup(const wchar_t *_Str) ;
#pragma empty_line
  int wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2) ;
  int wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount) ;
  wchar_t * wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  wchar_t * wcsrev(wchar_t *_Str) ;
  wchar_t * wcsset(wchar_t *_Str,wchar_t _Val) ;
  wchar_t * wcslwr(wchar_t *_Str) ;
  wchar_t * wcsupr(wchar_t *_Str) ;
  int wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2) ;
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
}
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\sec_api/string_s.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma empty_line
#pragma line 1 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\string.h" 1 3
/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#pragma line 9 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\sec_api/string_s.h" 2 3
#pragma line 175 "C:/Xilinx/Vivado_HLS/2014.4/win64/tools/clang/bin/../lib/clang/3.1/../../../x86_64-w64-mingw32/include\\string.h" 2 3
#pragma line 10 "proj_axi_master/src/AFAProcessFunction.cpp" 2
#pragma empty_line
void
AFAProcessFunction(
    volatile uint32_t *p,
    volatile void *srcNet,
    volatile void *dstNet )
{
#pragma HLS INTERFACE m_axi port=p depth=64
#pragma HLS INTERFACE m_axi port=srcNet depth=256
#pragma HLS INTERFACE m_axi port=dstNet depth=256
#pragma empty_line
 int i;
  int buff[50];
  uint32_t AFAProcessFunctionParamBlock[ 256 / 4 ];
  AFAProcessFunctionParameter_t *paramBlock;
#pragma empty_line
  memcpy(( void * ) &AFAProcessFunctionParamBlock, ( const void * ) p, 64 * sizeof( uint32_t ));
  memcpy(( void * ) p, ( const void * ) &AFAProcessFunctionParamBlock, 64 * sizeof( uint32_t ));
}
