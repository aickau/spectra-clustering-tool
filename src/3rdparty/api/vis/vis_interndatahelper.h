#ifndef _VIS_INTERNDATAHELPER_H
#define _VIS_INTERNDATAHELPER_H

#include "vis_typesintern.h"
#include "vis_interface.h"
#include "vis_interface_opt.h"


class CVisDataIntern;


/** helper class for managing temporary pointsize/linewidth buffers
 */
class CVisDataSizeBuffer
{
public:
	typedef enum
	{
		CT_POINTS,
		CT_LINES,
	} BUFFERTYPE;

	CVisDataSizeBuffer(BUFFERTYPE type);
	~CVisDataSizeBuffer();
	CVisDataSizeBuffer& operator=(const CVisDataSizeBuffer &d);

	void						Reset();
	void						ReleaseSizeBuffer();
	void						SetSizeBuffer(CVisGlobalOptions *pGS, CVisDataIntern *pDS, TVIS_SIZEM mode);

	unsigned __int8				*m_pSize;			// temporary size buffer
	int							m_NumSize;			// number of colors, this can be num_points, num_lines, num_triangles

private:

	void						SetSizeBufferFromInt(CVisDataIntern *pDS, TVIS_SIZEM mode);
	void						SetSizeBufferFromFloat(CVisDataIntern *pDS, TVIS_SIZEM mode);

	/** maps all values from a source buffer into the size buffer.
	 * @param pSource source buffer.
	 * @param Range min and max values between the source values. 
	 * @param Stride offset between two source values. a stride of one means the double 
	 *		  values are tightly packed in the array. 
	 */
	void						Map(double *pSource, TVIS_RANGE &Range, int Stride);

	/** maps all values from a source buffer into the size buffer.
	 * @param pSource source buffer.
	 * @param Range min and max values between the source values. 
	 * @param Stride offset between two source values. a stride of one means the double 
	 *		  values are tightly packed in the array. 
	 */
	void						Map(unsigned int *pSource, TVIS_RANGE &Range, int Stride);


	CVisGlobalOptions			*m_pGS;
	TVIS_SIZEM					m_LastSizeMode;
	BUFFERTYPE					m_Type;
};


/** helper class for managing temporary color buffers
 */
class CVisDataColorBuffer
{
public:

	typedef enum
	{
		CT_POINTS,
		CT_LINES,
		CT_TRIANGLES
	} BUFFERTYPE;

	CVisDataColorBuffer(BUFFERTYPE type);
	~CVisDataColorBuffer();
	CVisDataColorBuffer& operator=(const CVisDataColorBuffer &d);


	void						Reset();
	void						ReleaseColorBuffer();
	void						SetColorCode(CVisGlobalOptions *pGS, CVisDataIntern *pDS, TVIS_COLS mode);

	TVIS_COLOR4B				*m_pColors;				// temporary color buffer
	int							m_NumColors;			// number of colors, this can be num_points, num_lines, num_triangles


private:

	void						SetColorCodeSingle(CVisDataIntern *pDS);
	void						SetColorCodeFromAxis(CVisDataIntern *pDS, TVIS_COLS mode);
	void						SetColorCodeFromIntensityInt(CVisDataIntern *pDS);
	void						SetColorCodeFromIntensityFloat(CVisDataIntern *pDS);
	void						SetColorCodeFromColor(CVisDataIntern *pDS);

	void						FillSingleColor(TVIS_COLOR4B &single_color, TVIS_COLOR4B &selection_color, TVIS_BOOL *selection_buffer);
	void						ProcessPointsToIntensities(CVisDataIntern *pDS, double *pSource, double *pDest);

	/** maps all values from a source buffer into the color buffer.
	 * @param pDS pointer to CVisDataIntern.
	 * @param pSource source buffer.
	 * @param pSelection pointer to selection buffer, this mst be the same number of elements like pSource 
	 * @param Range min and max values between the source values. 
	 *		  max means 100% intensity which is mapped to upper color in the color table,
	 *		  min means 0% intensity which is mapped to lower color in the color table.
	 * @param Stride offset between two source values. a stride of one means the double 
	 *		  values are tightly packed in the array. 
	 */
	void						Map(CVisDataIntern *pDS, double *pSource, TVIS_BOOL *pSelection, TVIS_RANGE &Range, int Stride);

	/** maps all values from a source buffer into the color buffer.
	 * @param pDS pointer to CVisDataIntern.
	 * @param pSource source buffer.
	 * @param pSelection pointer to selection buffer, this mst be the same number of elements like pSource 
	 * @param Range min and max values between the source values. 
	 *		  max means 100% intensity which is mapped to upper color in the color table,
	 *		  min means 0% intensity which is mapped to lower color in the color table.
	 * @param Stride offset between two source values. a stride of one means the double 
	 *		  values are tightly packed in the array. 
	 */
	void						Map(CVisDataIntern *pDS, unsigned int *pSource, TVIS_BOOL *pSelection, TVIS_RANGE &Range, int Stride);

	CVisGlobalOptions			*m_pGS;
	TVIS_COLS					m_LastColorizeStyle;
	BUFFERTYPE					m_Type;
};


#endif