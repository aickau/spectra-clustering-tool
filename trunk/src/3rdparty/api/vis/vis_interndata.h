#ifndef _VIS_INTERNDATA_H
#define _VIS_INTERNDATA_H

#include "vis_interface.h"
#include "vis_interface_opt.h"
#include "vis_interndatahelper.h"


/** internal data representation for all 3d meshes. this class is responsible for:
 * - data initializing
 * - normal vector calculations
 * - dataset integrity checking
 * - color buffer updates
 * - element selection
 */
class CVisDataIntern: public CVisDataInterface		
{
public:
	CVisDataIntern();
	CVisDataIntern(const CVisDataIntern&);
	CVisDataIntern(CVisGlobalOptions *global_opt, CVisDataInterface *ds, CVisDataOptions opt);
	~CVisDataIntern();

	/** has dataset points. this should always return true.
	 */
	bool						HasPoints();

	/** has dataset lines.
	 */
	bool						HasLines();

	/** has dataset triangles. 
	 */
	bool						HasTriangles();

	
	/** overloaded from CVisDataInterface
	 */
	CString						GetDsName()			{ return m_DataSetName; }

	/** get unique dataset id.						
	 */
	TVIS_OBJECTID				GetDsID()			{ return m_ID; }

	/** get temporaray color pointer, this may return null.
	 */
	const TVIS_COLOR4B			*GetColorPoints()		{ return m_BufferColorPoints.m_pColors; }
	
	/** get temporaray color pointer, this may return null.
	 */
	const TVIS_COLOR4B			*GetColorLines()		{ return m_BufferColorLines.m_pColors; }

	/** get temporaray color pointer, this may return null.
	 */
	const TVIS_COLOR4B			*GetColorTriangles()	{ return m_BufferColorTriangles.m_pColors; }

	/** number of colors for lines. this can be num_points or num_lines.
	 */
	int							GetColorLinesCount()	{ return m_BufferColorLines.m_NumColors; }

	/** number of colors for lines. this can be num_points or num_triangles.
	 */
	int							GetColorTrianglesCount(){ return m_BufferColorTriangles.m_NumColors; }

	/** get temporaray pointsize pointer, this may return null.
	 */
	unsigned __int8				*GetPointSize()			{ return m_BufferPointSize.m_pSize; }

	/** get temporaray linewidth pointer, this may return null.
	 */
	unsigned __int8				*GetLineWidth()			{ return m_BufferLineWidth.m_pSize; }


	/** get single color for points. use this function instead of using directly color
	 * information from CVisGlobalOptions or CVisDataOptions.
  	 */
	TVIS_COLOR4B				GetSingleColorPoints();

	/** get single color for lines. use this function instead of using directly color
	 * information from CVisGlobalOptions or CVisDataOptions.
  	 */
	TVIS_COLOR4B				GetSingleColorLines();

	/** get single color for triangles. use this function instead of using directly color
	 * information from CVisGlobalOptions or CVisDataOptions.
  	 */
	TVIS_COLOR4B				GetSingleColorTriangles();

	/** get single point size. use this function instead of using directly size
	 * information from CVisGlobalOptions or CVisDataOptions.
  	 */
	int							GetSinglePointSize();
	TVIS_PS						GetPointStyle();

	/** get single line width. use this function instead of using directly size
	 * information from CVisGlobalOptions or CVisDataOptions.
  	 */
	int							GetSingleLineWidth();
	TVIS_LS						GetLineStyle();

	void						SetFlagDataHide(bool datahide);

	/** return data options pointer
  	 */
	CVisDataOptions				*GetDataOptions()	{ return &m_DataOptions; }

	/** set data options pointer
  	 */
	void						SetDataOptions(CVisDataOptions opt) {m_DataOptions = opt; }

	/** updates color buffers with color values depending on color code mode.
 	 */
	void						SetColorCode();

	/** updates size buffers with pointsize/linewidth values depending on PointSizeMode/LineWidthMode.
 	 */
	void						SetSizeBuffer();

	/** flip surface normals if possible. this only applies to polygonal meshes
	*/
	void						FlipSurfaceNormals();

	/** get minimal and maximal extensions from object.
	 */
	void						GetMinMax(TVIS_POINT4D &pmin, TVIS_POINT4D &pmax);

	/** get minimal and maximal extensions from datset.
	 */
	void						GetMinMax(TVIS_DATASETPARAMRANGE &range);

	/** get center position of objects bounding box.
	 * @return center position.
	 */
	TVIS_POINT4D				GetCenter();

	/** destroy complete dataset.
  	 */
	void						Destroy();	
	
	/** get number of selected points in dataset.
	 */
	int							GetSelectedPointCount()		{ return m_SelectionNumPoints; };

	/** get number of selected lines in dataset.
	 */
	int							GetSelectedLineCount()		{ return m_SelectionNumLines; };

	/** get number of selected triangles in dataset.
	 */
	int							GetSelectedTriangleCount()	{ return m_SelectionNumTriangles; };

	/** get pointer to selection buffer for points. 
	 * every element is marked as selected (true) or not selected (false).
	 */
	TVIS_BOOL					*GetSelectedPoints()		{ return m_pSelectionPoints; }

	/** get pointer to selection buffer for lines. 
	 * every element is marked as selected (true) or not selected (false).
	 */
	TVIS_BOOL					*GetSelectedLines()			{ return m_pSelectionLines; }

	/** get pointer to selection buffer for triangles. 
	 * every element is marked as selected (true) or not selected (false).
	 */
	TVIS_BOOL					*GetSelectedTriangles()		{ return m_pSelectionTriangles; }

	int							GetHighlightPoints()		{ return m_HLPointIndex; }
	int							GetHighlightLines()			{ return m_HLLineIndex; }
	int							GetHighlightTriangles()		{ return m_HLTriangleIndex; }

	void						SetHighlightPoints(int index);
	void						SetHighlightLines(int index);
	void						SetHighlightTriangles(int index);

	/** select all elements. 
	 */
	void						SelectAll();

	/** invert selection of all elements. 
	 */
	void						SelectInvert();

	/** deselect all elements. 
	 */
	void						SelectNone();

	/** fill selection buffer. this function allocates memory and fills the buffer with indices whose elements are selected.
	 * @param points std::vector<unsigned int> a vector which will contain all indices to selected points.
	 * this buffer can be empty or not empty, it any data is getting overwritten by this function.
	 * @param points std::vector<unsigned int> a vector which will contain all indices to selected lines.
	 * this buffer can be empty or not empty, it any data is getting overwritten by this function.
	 * @param points std::vector<unsigned int> a vector which will contain all indices to selected triangles.
	 * this buffer can be empty or not empty, it any data is getting overwritten by this function.
	 */
	void						FillSelectionBuffers(TVIS_VECTORUINT &points, TVIS_VECTORUINT &lines,  TVIS_VECTORUINT &triangles);

	/** get element information
	 * @return true if an element was hit, otherwise false
	 */
	bool						GetElementInfo(TVIS_POINT4D &position, TVIS_MATRIX4D &rotation, TVIS_CIRCLE2D &epsilon_circle, TVIS_POINT4D &outposition, int &outindex);

	/** single point (de)selection. set selection or deselection state of a given element index. 
	 * @param index point index.
	 * @param state true for selection, false for deselection
	 */
	void						DoSelectionPoints(int index, bool state);

	/** single line (de)selection. set selection or deselection state of a given element index. 
	 * @param index line index.
	 * @param state true for selection, false for deselection
	 */
	void						DoSelectionLines(int index, bool state);
	
	/** single triangle (de)selection. set selection or deselection state of a given element index. 
	 * @param index triangle index.
	 * @param state true for selection, false for deselection
	 */
	void						DoSelectionTriangles(int index, bool state);

	/** rectangular (de)selection of elements
  	 */
	void						DoSelection(TVIS_POINT4D &position, TVIS_MATRIX4D &rotation, TVIS_RECT2D &sel_rect, bool selection_state);

	/** circular (de)selection of elements
  	 */
	void						DoSelection(TVIS_POINT4D &position, TVIS_MATRIX4D &rotation, TVIS_CIRCLE2D &sel_circle, bool selection_state);


private:
	void						CalcMinMax();
	void						CalcMinMax(TVIS_RANGE &Range, double *pSource, int count);
	void						CalcMinMax(TVIS_RANGE &Range, unsigned int *pSource, int count);

	/** initalize / generates geometry.
	 * @return true on success otherwise false.
	 */
	bool						GenerateGeometry();

	/** check index boundaries of dataset.
	 * @return false if an error was found otherwise true.
	 */
	bool						ValidateData();

	virtual void				ReleaseBuffer(){};		// we need to override that

	bool						Select(TVIS_BOOL *buffer, int count, TVIS_BOOL state);
	bool						SelectInvert(TVIS_BOOL *buffer, int count);

	void						FillIndexBuffer(TVIS_VECTORUINT &destination, TVIS_BOOL *sourcebitfield, int destinationsize, int sourcesize);
	
	
	CVisGlobalOptions			*m_pGlobalOptions;		// pointer to global options
	static TVIS_OBJECTID		m_IDCount;				// object id counter
														
	CVisDataInterface			*m_pDS;					// dataset
	CVisDataOptions				m_DataOptions;			// dataset options
														
	CString						m_DataSetName;			// dataset name
	TVIS_OBJECTID				m_ID;					// unique dataset id

	TVIS_DATASETPARAMRANGE		m_ParamRange;			// min / max values of all additional dataset buffers.
		
	CVisDataColorBuffer			m_BufferColorPoints;	// temporary color data points
	CVisDataColorBuffer			m_BufferColorLines;		// temporary color data lines 
	CVisDataColorBuffer			m_BufferColorTriangles;	// temporary color data trianglesles;

	CVisDataSizeBuffer			m_BufferPointSize;
	CVisDataSizeBuffer			m_BufferLineWidth;

	bool						m_FlippedSurfaceNormals;
	bool						m_HaveFaceNormals;

	// elemtnt higlight indecies
	int							m_HLPointIndex;
	int							m_HLLineIndex;
	int							m_HLTriangleIndex;

	// selection
	int							m_SelectionNumPoints;
	int							m_SelectionNumLines;
	int							m_SelectionNumTriangles;
	TVIS_BOOL					*m_pSelectionPoints;
	TVIS_BOOL					*m_pSelectionLines;
	TVIS_BOOL					*m_pSelectionTriangles;
};

#endif







