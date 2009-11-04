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
//! \file  specObjOperations.cpp
//! \brief program to generate binary dumps from a directory of FITS files.


#include <Windows.h>
#include <conio.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <math.h>
#include <float.h>
#include <assert.h>

typedef char _TCHAR;
/*
+ ./
+ /spectro
+ /spectro/1d_25
+ /spectro/1d_25/0266
+ /spectro/1d_25/0266/1d
+ /spectro/1d_25/0266/1d/spSpec-51630-0266-002.fit
+ /spectro/1d_25/0266/1d/spSpec-51630-0266-004.fit
+ /spectro/1d_25/0266/1d/spSpec-51630-0266-039.fit
- * 
*/

int _tmain(int argc, _TCHAR* argv[])
{
	std::ifstream fin("all_specObjDr4.csv");
	// skip first line
	std::string line,plateold("");
	fin >> line;
	fin >> line;
	fin >> line;

	std::string plate,mjd,fiberID;

	std::string out("");
	out += "+ ./\n";
	out += "+ /spectro\n";
	out += "+ /spectro/1d_25\n";


	size_t fcount=0;
	size_t c=0;
	while( fin ) 
	{	
		fin >> plate;
		fin >> mjd;
		fin >> fiberID;

		if ( plate.size() < 4)
			plate = std::string("0")+plate;
		if ( fiberID.size() == 1 )
			fiberID = std::string("00")+fiberID;
		if ( fiberID.size() == 2 )
			fiberID = std::string("0")+fiberID;

		if ( plate != plateold )
		{
			c++;
			if ( c%50 == 0)
			{
				out += "- *\n";

				std::string fn("spSpecAll");
				std::stringstream st;
				st << fcount;
				fn += st.str();
				fn += ".lis";

				std::ofstream fon(fn.c_str());
				fon<<out;
				fcount++;

				out="";
				out += "+ ./\n";
				out += "+ /spectro\n";
				out += "+ /spectro/1d_25\n";
			}
			out += "+ /spectro/1d_25/"+plate+"\n";
			out += "+ /spectro/1d_25/"+plate+"/1d\n";
		}
		out += "+ /spectro/1d_25/"+plate+"/1d/spSpec-"+mjd+"-"+plate+"-"+fiberID+".fit\n";


		plateold = plate;
	}

	out += "- *\n";

	std::string fn("spSpecAll");
	std::stringstream st;
	st << fcount;
	fn += st.str();
	fn += ".lis";
	std::ofstream fon(fn.c_str());
	fon<<out;


	return 0;
}

