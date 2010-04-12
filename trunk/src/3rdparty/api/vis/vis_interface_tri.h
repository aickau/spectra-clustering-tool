#ifndef _VIS_INTERFACE_TRI_H
#define _VIS_INTERFACE_TRI_H

#include "vis_interface.h"
//#include "../vis_external/triangulation.h"  



/// interface for TR::Triangulation
class CVisTriDataInterface : public CVisDataInterface
{
public:
	VIS_DLLEXPORT  CVisTriDataInterface();
	VIS_DLLEXPORT  virtual ~CVisTriDataInterface();
	
	/** get dataset name. this must not be an unique name.
	 * @return dataset name.
	 */
	VIS_DLLEXPORT  CString				GetDsName();
	
	VIS_DLLEXPORT  void					Init(float *points, int *faces, int numpoints, int numfaces);

	VIS_DLLEXPORT  void					ReleaseBuffer();

private:
	void					Destroy();

};


#endif