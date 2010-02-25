#ifndef _VIS_INTERFACE_H
#define _VIS_INTERFACE_H

#include <afxmt.h>			
#include "vis_dllexport.h"
#include "vis_types.h"



/** dataset interface class.
 */
class CVisDataInterface
{
	public:
		CVisDataInterface() :
			pt_points( 0 ),
			pt_lines( 0 )			,
			pt_triangles( 0 )		,
			pt_trianglenormals( 0 )	,
			pt_pointparam1( 0 )		,
			pt_pointparam2( 0 )		,	
			pt_lineparam1( 0 )		,
			pt_lineparam2( 0 )		,
			pt_triangleparam1( 0 )	,
			pt_triangleparam2( 0 )	,

			num_points(0)			,
			num_lines(0)			,
			num_triangles(0)
		{};
		
		~CVisDataInterface() {};

		virtual CString							GetDsName() {ASSERT(false); return "";};
		
		virtual void							Lock() { critical_section.Lock(); };				
		virtual void							UnLock() { critical_section.Unlock(); };			
										
		virtual int								GetPointCount() {return num_points;};	
		virtual int								GetLineCount() {return num_lines;};	
		virtual int								GetTriangleCount() {return num_triangles;};
													
		virtual const TVIS_POINT4D				*GetPoints() { return pt_points;};	
		virtual const TVIS_LINES				*GetLines() {return pt_lines;};		
		virtual const TVIS_TRIANGLES			*GetTriangles() {return pt_triangles; };	
		virtual const TVIS_POINT4D				*GetTriangleNormals() {return pt_trianglenormals;};	
		virtual const TVIS_DATAPARAM1			*GetPointParam1() { return pt_pointparam1; };	
		virtual const TVIS_DATAPARAM2			*GetPointParam2() { return pt_pointparam2; };	
		virtual const TVIS_DATAPARAM1			*GetLineParam1() { return pt_lineparam1;};	
		virtual const TVIS_DATAPARAM2			*GetLineParam2() { return pt_lineparam2;};	
		virtual const TVIS_DATAPARAM1			*GetTriangleParam1() { return pt_triangleparam1; };	
		virtual const TVIS_DATAPARAM2			*GetTriangleParam2() { return pt_triangleparam2; };	

		virtual void							ReleaseBuffer() = 0;

										
	protected:	
												
		CCriticalSection						critical_section;
												
		TVIS_POINT4D							*pt_points;
		TVIS_LINES								*pt_lines;
		TVIS_TRIANGLES							*pt_triangles;
		TVIS_POINT4D							*pt_trianglenormals;
		TVIS_DATAPARAM1							*pt_pointparam1;
		TVIS_DATAPARAM2							*pt_pointparam2;
		TVIS_DATAPARAM1							*pt_lineparam1;
		TVIS_DATAPARAM2							*pt_lineparam2;
		TVIS_DATAPARAM1							*pt_triangleparam1;
		TVIS_DATAPARAM2							*pt_triangleparam2;
												
		unsigned int							num_points;
		unsigned int							num_lines;
		unsigned int							num_triangles;
};


#endif