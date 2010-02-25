#ifndef _VIS_INTERFACE_LINES_H
#define _VIS_INTERFACE_LINES_H

#include "vis_interface.h"


/** interface for lines - only for testing purposes at the moment
*/
class CVisLineDataInterface : public CVisDataInterface
{
public:
	VIS_DLLEXPORT  CVisLineDataInterface();
	VIS_DLLEXPORT  virtual ~CVisLineDataInterface();
	
	/** get dataset name. this must not be an unique name.
	 * @return dataset name.
	 */
	VIS_DLLEXPORT  CString				GetDsName();
	
	VIS_DLLEXPORT void					Init(double *points, int num_points, int *lines, int num_lines);	
	
	VIS_DLLEXPORT  void					ReleaseBuffer();

private:
	void					Destroy();

};


#endif