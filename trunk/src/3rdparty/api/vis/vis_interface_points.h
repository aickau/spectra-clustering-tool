#ifndef _VIS_INTERFACE_POINTS_H
#define _VIS_INTERFACE_POINTS_H

#include "vis_interface.h"


/** interface for points - only for testing purposes at the moment. 
*/
class CVisPointDataInterface : public CVisDataInterface
{
public:
	VIS_DLLEXPORT  CVisPointDataInterface();
	VIS_DLLEXPORT  virtual ~CVisPointDataInterface();
	
	/** get dataset name. this must not be an unique name.
	 * @return dataset name.
	 */
	VIS_DLLEXPORT  CString					GetDsName();
	
	/// convert point buffer -> points
	VIS_DLLEXPORT  void					Init(double *points, int num_points);	
	
	VIS_DLLEXPORT void					ReleaseBuffer();

private:
	void					Destroy();

};


#endif