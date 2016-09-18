//! \verbatim
//! ###########################################################################
//! # ASPECT: A spectra clustering tool for exploration of large spectral surveys - Stage I
//! #
//! # Copyright (c) 2009 Aick in der Au
//! # All rights reserved.
//! ###########################################################################
//!
//!      created by : Aick in der Au <aick.inderau@gmail.com>
//!      created on : 17/09/2016
//! additional docs : none
//!  responsibility : 1. Aick in der Au
//!                   2. 
//! \endverbatim
//!
//! \file  afaconnector.h
//! \brief 

#ifndef _AFACONNECTOR_H
#define _AFACONNECTOR_H

#include <string>

class SpectraVFS;

// Read & Write Data from / to ASPECT-FPGA-Accelerator (AFA)
class AfaConnector
{
public:

	//! maximum of size supported for reading / writing to the device
	//! Currently 1 GB supported. 2 GB is the maximum over the io ports..
	static const int maxBufferSize = 1024*1024*1024;

	AfaConnector(const std::string path="/dev/afapci0");
	virtual ~AfaConnector();

	//! \return true if ASPECT-FPGA-Accelerator device could be found at path /dev/afapci0 , otherwise false
	bool isAFADeviceAvailable() const;

	//! \return error msg in case device could not be opened
	std::string getErrorMsg() const;

	//! return true if writing was successful.
	bool writeSpectra( SpectraVFS &spectra );


private:

	void openConnection();
	void closeConnection();

	//! read data from AFA 
	//! Currently read speed is only ~16 MB/s 
	//! Currently we support a maximum size of 1 GB of data to read
	//! \return true if the buffer is read
	bool readData( void *buffer, unsigned int size );

	//! Write data to AFA
	//! Currently write speed is only ~16 MB/s 
	//! Currently we support a maximum size of 1 GB of data to write
	//! \return true if complete buffer could be written
	bool writeData( void *buffer, unsigned int size );


	//! path to AFA device
	std::string m_sstrPath;

	// handle, -1 = no connection
	int m_deviceHandle;

	std::string m_sstrErrorMsg;
};


#endif // _AFACONNECTOR_H
