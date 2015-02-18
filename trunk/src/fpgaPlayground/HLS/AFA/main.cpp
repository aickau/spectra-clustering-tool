#if 1
#include "include/AFATypes.h"
#include "include/AFAProcessFunction.h"
#include "include/AFASpectra.h"
#include "include/AFAProcessing.h"

#include "include/AFAProcessFunction.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <Memory.h>
#include <math.h>

// debug and test code
// ===================
uint32_t
ReadSpectraFileRAW(
    const char *filename,
    uint32_t *spectraCount,
    AFASpectra **spectraData )
{
    FILE *f;
    uint32_t rv = 0xffffffff;
    long size = 0;

    if ( spectraData )
    {
        f = fopen( filename, "r+b" );
        if ( f )
        {
            fseek( f, 0, SEEK_END );

            const size_t fileSize = ( size_t ) ftell( f );
            const size_t fileCount = fileSize / sizeof( AFASpectra );
            const size_t remainder = fileSize % sizeof( AFASpectra );

            if ( 0 == fileSize )
            {
                // file illegal
                rv = 3;
            }
            else
            {
                if  ( fileSize < sizeof( AFASpectra ) || remainder != 0 )
                {
                    // wrong size
                    rv = 4;
                }
                else
                {
                    *spectraData = ( AFASpectra * ) malloc( fileSize );
                    if ( *spectraData )
                    {
                        // clear memory
                        memset(( void * )*spectraData, 0x00, fileSize );
                        //
                        *spectraCount = ( unsigned long ) fileCount;
                        fseek( f, 0, SEEK_SET );
                        size_t n = fread(( void * )( *spectraData ), fileSize, 1, f );
                        if ( 1 == n )
                        {
                            // everything is fine
                            rv = 0;
                        }
                        else
                        {
                            // read failed
                            rv = 6;
                        }
                    }
                    else
                    {
                        // no mem avail
                        rv = 5;
                    }
                }
            }
            fclose( f );
        }
        else
        {
            // cannot open
            rv = 2;
        }
    }
    else
    {
        // illegal pointer
        rv = 1;
    }
    return rv;
}

// This is the calling and csimulation start program.
// It calls the hardware function and compares results with the software calculation
// =================================================================================
int
main(int argc, char* argv[])
{
	int numArgs = 0;
	while ( numArgs < argc )
	{
	    printf("argv[%d]=%s\n", numArgs, argv[ numArgs ]);
	    numArgs++;
	}
    uint32_t numSpectra;
    AFASpectra *spectraData;
    uint32_t rv = ReadSpectraFileRAW(
        "../../../../allSpectra.bin",
        &numSpectra,
        &spectraData );

    uint32_t spectraSize = sizeof( AFASpectra );
    printf( "AFASpectraSize=%d\n", spectraSize );

    size_t gridSize = static_cast<size_t>(ceilf(sqrtf((float)numSpectra+1))*1.1f); // gives a factor of 1.1^2=1.21 in total
    size_t gridSizeSqr = gridSize*gridSize;

    AFASpectra *net = new AFASpectra[ gridSizeSqr ];


    // here the calculation is done the first time (on hardware; or software if we're in simulation)
    // =============================================================================================
    AFAProcessFunctionParameter_t paramBlock;

    paramBlock.numSpectra = numSpectra;
    paramBlock.bContinueComputation = false;

    AFAProcessFunction(
        ( uint32_t * )&paramBlock,	//< shitty casting needed because of burst transfers in AXI4 bus
        spectraData,
        net );

    // here the calculation is done a second time to compare with hardware results
    // ===========================================================================
    AFAProcessing AFAProc(
        spectraData,
        net,
        numSpectra,
        false );

    // compare results
    // ===============
    // todo: compare and set rv to zero if everything is ok

    // clean up
    free( spectraData );
    delete [] net;
    return ( int ) rv;
}


#else
/*******************************************************************************
Vendor: Xilinx 
Associated Filename: example.cpp
Purpose: AXI master example for Vivado HLS
Revision History: February 13, 2012 - initial release
                                                
*******************************************************************************
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


This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#include <stdio.h>

typedef unsigned long
    uint32_t;

typedef struct
{
	bool flag;
	uint32_t i1;
	uint32_t i2;
} param_t;

void example(
    volatile char *p,
    volatile int *a);

int main()
{
  int i;
  int A[50];
  int B[50];
  char paramBuf[ 100 ];
  param_t *p = ( param_t * )paramBuf;

  p->flag = true;
  p->i1 = 81726354;
  p->i2 = 100;

  printf("HLS AXI-Stream no side-channel data example\n");
  //Put data into A
  for(i=0; i < 50; i++){
    A[i] = i;
  }

  //Call the hardware function
  example( paramBuf, A );

  //Run a software version of the hardware function to validate results
  for(i=0; i < 50; i++)
  {
#if 1
    B[i] = p->flag ? i + p->i1 : i + p->i2;
#else
    B[i] = ( unsigned long )( RecursiveTestTB( B[ i ]));
#endif
  }

  //Compare results
  for(i=0; i < 50; i++){
    if(B[i] != A[i]){
      printf("i = %d A = %d B= %d\n",i,A[i],B[i]);
      printf("ERROR HW and SW results mismatch\n");
      return 1;
    }
  }
  printf("Success HW and SW results match\n");
  return 0;
}

  

#endif
