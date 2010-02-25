#ifndef _VIS_GLHELPER_H
#define _VIS_GLHELPER_H


#include "vis_types.h"
#include "vis_typesintern.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "glext.h"

/**
\brief			GL helper class for drawing simple primitives and managing state changes. always try to use this class instead of opengl calls

\attention		always use vis_glEnable/vis_glDisable and NOT glEnable/glDisable
				always use vis_glEnableArray/vis_glDisableArray and NOT glEnableArray/glDisableArray
				otherwise the state management will fail.
				exception: use glEnable/glDisable instead of vis_glEnable/vis_glDisable when 
				working with glPushAttrib(x)/glPopAttrib(x) x = {GL_ENABLE_BIT, GL_DISABLE_BIT}

*/
class CVisGLHelper
{
public:
	CVisGLHelper();
	~CVisGLHelper();

	/** Init OpenGL. 
	 * @param hDC	Windows Device Context
	 * @param bpp	bits per pixel and should be 32
	 * @return HGLRC GL devicecontext if init was sucessful, otherwise this value is NULL.
	 * Make sure to call this only once in a instance. 
	 * This function sets some gl states which are necessary in our application. 
	 * Do not change these states, which are set here, within the application. 
	 * If states are changed for a short period of time, make sure to set them back.
	 */
	HGLRC vis_glInit(HDC hDC, int bpp);

	/** deinit OpenGL.
	 */
	void vis_glKill(HDC hDC, HGLRC hGLRC);

	/** bind all gl calls to a gl device context.
	 * @param hDC Windows Device Context.
	 * @param hGLRC an valid GL Device Context.
	 */
	void vis_glSetDeviceContext(HDC hDC, HGLRC hGLRC);

	/** get current gl device context. 
	 * @return if no HGLRC is bound this returns NULL, otherwise the current GL Device Context.
	 */
	HGLRC vis_glGetDeviceContext();

	/** Get last gl error. 
	 * @return returns true if an error occured, otherwise false.
	 */
	bool vis_glGetError();

	/** Get last gl error, returns an error message in CString::error_message
	 * @param error_message an short error message is stored there.
	 * @return returns true if an error occured, otherwise false.
	 */
	bool vis_glGetError(CString &error_message);

	/** register a gl extension. this can only be called within or after vis_glInit(). not used at he moment.
	 - extension_string		: pointer to a valid extension name
	 - return value			: returns TRUE if the specified extension is available, otherwise FALSE.
	 */
//	bool vis_glIsExtensionAvailable(const char *extension_name);

	/** register a gl extension procedure. this can only be called within or after vis_glInit(). not used at he moment.
	 - extension_string		: pointer to a valid extension procedure name
	 - return value			: returns NULL if extensionproc is not available, otherwise the pointer to procedure.
	 */
//	void *vis_glRegisterExtensionProc(const char *extension_procname);

	/** register a 2d texture. wrong values are ignored or an assertion occurs. not used at he moment.
	 - texid		: texture id, any positive number
	 - size			: any 2^n texture, maximum 1024
	 - format		: one of the following: GL_RGB, GL_RGBA, GL_ALPHA, GL_LUMINANCE, GL_LUMINANCE_ALPHA
	 - mipmap		: set to true if you want auto-generated mipmaps
	 - texmagfilter : texture filtering: GL_NEAREST (nearest texel) or GL_LINEAR (bilinear)
	 - pt			: pointer to texture data
	*/
//	void vis_glRegisterTexture2D(int texid, int size, int format, bool mipmap, GLenum texmagfilter, unsigned __int8 *pt);

	/** bind texture. not used at he moment.
	 - texid		: texture id, this must be registered one with vis_glRegisterTexture2D().
	 - warp			: warp mode. if set to true texture is repeated, otherwise edge texels get repeated.
	*/
//	void vis_glBindTexture(int texid, bool warp);

	/// Get maximum available point size, which can be passed to vis_glPointSize(..)
	float vis_glGetMaxPointSize();
	/// Get maximum available line width, which can be passed to vis_glLineWidth(..)
	float vis_glGetMaxLineWidth();

	/// Suggested number of vertices in an glDrawElements call without loosing performance.
	int	 vis_glGetMaxElementsVertices();		
	/// Suggested number of indices in an glDrawElements call without loosing performance.
	int	 vis_glGetMaxElementsIndices();

	/** sets an orthogonal projection
	 - client_height	: height of the client window, used for coordinate mapping
	 - px, py			: x and y position in pixels of the gl window
	 - sx, sy			: width and height in  pixels of the gl window
	 - projection_plane : projection rectangle
	 - znear, zfar		: near and far clipping plane
	*/
	void vis_glSetProjection(int client_height, int px, int py, int sx, int sy, TVIS_RECT2D &projection_plane, double z_near, double z_far);

	/** sets the scissor window (the rectangle where all drawing is performed). 
	 This is useful when not all portions of the window should be drawn. 
	 Scissoring is only performed when the scissor state is enabled. Use vis_glSetScissorState() to set scissor state. 
	 - px, py			: x and y position in pixels of the gl window
	 - sx, sy			: width and height in  pixels of the gl window
	*/
	void vis_glSetScissor(int client_height, int px, int py, int sx, int sy);

	/// Enables/Disables scissoring. 
	void vis_glSetScissorState(bool state);

	/** Enable a gl state. cap can be any gl enumerator (GL_DEPTH_TEST, GL_LIGHTING, ..). 
	 Do not mix vis_glEnable and glEnable commands !
	*/
	void vis_glEnable(GLenum cap);

	/** Disable a gl state. cap can be any gl enumerator (GL_DEPTH_TEST, GL_LIGHTING, ..). 
	 Do not mix vis_glDisable and glDisable commands ! 
	*/
	void vis_glDisable(GLenum cap);

	/// Rotate, the w component is the angle in degrees. x,y,z is the axis. 
	void vis_glRotate(TVIS_POINT4D &axisangle);

	/// Translate relative by position.
	void vis_glTranslate(TVIS_POINT4D &position);

	/** Scale.
	 Make sure that vis_glEnable(GL_NORMALIZE) is called before scaling objects.
	 Otherwise lighting information is corrupted. (on init, this is already enabled)
	*/
	void vis_glScale(TVIS_POINT4D &factor);

	/// Wraps glPushMatrix(), in debug mode, additional stack checkings are included.
	void vis_glPushMatrix();
	/// Wraps glPopMatrix(), in debug mode, additional stack checkings are included.
	void vis_glPopMatrix();

	/** Wraps glPushAttrib(), in debug mode, additional stack checkings are included.
	 */
	void vis_glPushAttrib(GLbitfield mask);

	/** Wraps glPopAttrib(), in debug mode, additional stack checkings are included.
	 */
	void vis_glPopAttrib();

	/// Wraps glLoadIdentity()
	void vis_glLoadIdentity();

	/// Get current modelview matrix
	void vis_glGetModelviewMatrix(TVIS_MATRIX4D &matrix);

	/// Multiplies matrix with current matrix on gl stack
	void vis_glMultMatrix(TVIS_MATRIX4D &matrix);

	/// Enables Vertex, Normal, Color Arrays. 
	void vis_glEnableArray(bool vertex, bool normal, bool color);

	/// Disables Vertex, Normal, Color Arrays. 
	void vis_glDisableArray(bool vertex, bool normal, bool color);

	/// Clears frame- and z-buffer.
	void vis_glClear(TVIS_COLOR4B &color);

	/// Set color. This has no effect if lighting is enabled. Automatically disables blending.
	void vis_glSetColor(TVIS_COLOR4B &col);
	/// Set color and transparency. alpha = 0 full transparent, alpha = 255 = full opaque. Blending will be enabled till to the next call of SetColor()
	void vis_glSetColor(TVIS_COLOR4B &col, unsigned __int8 alpha);
	/// Set color. When drawing illuminated (GL_LIGHTING is enabled) objectes, set lighting flag to true.
	void vis_glSetLightingIndependentColor(TVIS_COLOR4B &col, bool lighting);

	/// set blending mode. vis_glSetBlendingMode and glBlendFunc commands ! Otherwise the state management will fail.
	void vis_glSetBlendingMode(GLenum sfactor, GLenum dfactor);

	void vis_glPointSize(float size);

	void vis_glLineParam(float width, TVIS_LS style);

	void vis_glPolygonMode(GLenum mode);

	/** draws a 3D point cloud.
	 - points			: pointer to an array with points. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 - width			: pointer to an array with point size values or zero for fixed point size.
	 - color			: pointer to an array with color values or zero for no color. data must be aligned to 4 bytes (r1,g1,b1,free1).
	 - num_points		: number of points.
	 - style			: point style (quadratic blocks, points or crosses)
	 - pixel_ratio		: pixel size in logical units
	*/
	void vis_glDraw3DPoints(double *points, unsigned __int8 *width, unsigned __int8 *color, int num_points, TVIS_PS style, double pixel_ratio);


	/** draws a 3D line mesh using glDrawElements.
	 - points			: pointer to an array with points. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 - width			: pointer to an array with point size values or zero for fixed point size.
	 - color			: pointer to an array with color values or zero for no color. data must be aligned to 4 bytes (r1,g1,b1,free1).
	 - ind				: index pointer with 32bit indices. 
	 - num_points		: number of points.
	 - num_colors		: for colors per point : num_colors = num_points, for colors per triangle : num_colors = number of triangles. other values are are ignored.
	 - num_lines		: number of lines
	*/
	void vis_glDraw3DLinesImmediate(double *points, unsigned __int8 *width, unsigned __int8 *color, unsigned int *ind, int num_points, int num_colors, int num_lines);


	/** draws a 3D triangle mesh using glDrawElements.
	 - points			: pointer to an array with points. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 - normals			: pointer to an array with normals or zero for no normals. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 - color			: pointer to an array with color values or zero for no color. data must be aligned to 4 bytes (r1,g1,b1,free1).
	 - ind				: index pointer with 32bit indices. 
	 - num_points		: number of points.
	 - num_normals		: number of normals for normals per point : num_normals = num_points, for normals per triangle : num_colors = number of triangles. other values are ignored.
	 - num_colors		: for colors per point : num_colors = num_points, for colors per triangle : num_colors = number of triangles. other values are are ignored.
	 - num_triangles	: number triangles
	*/
	void vis_glDraw3DTrianglesImmediate(double *points, double *normals, unsigned __int8 *color, unsigned int *ind, int num_points, int num_normals, int num_colors, int num_triangles);

	/** draws a 3D triangle mesh using simple gl calls, e.g. glVertex(..). 
	Due to massive calling overhead, this procedure should only be used for small meshes or in conjuction gl display lists.
	 - points			: pointer to an array with points. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 - normals			: pointer to an array with normals or zero for no normals. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 - color			: pointer to an array with color values or zero for no color. data must be aligned to 4 bytes (r1,g1,b1,free1).
	 - ind				: index pointer with 32bit indices. 
	 - num_points		: number of points.
	 - num_normals		: number of normals for normals per point : num_normals = num_points, for normals per triangle : num_colors = number of triangles. other values are ignored.
	 - num_colors		: for colors per point : num_colors = num_points, for colors per triangle : num_colors = number of triangles. other values are are ignored.
	 - num_triangles	: number triangles
	*/
	void vis_glDraw3DTrianglesRetained(double *points, double *normals, unsigned __int8 *color, unsigned int *ind, int num_points, int num_normals, int num_colors, int num_triangles);

	/// draws a 3D point.
	void vis_glDraw3DPoint(TVIS_POINT4D &p);
	/// draws a 3D line.
	void vis_glDraw3DLine(TVIS_POINT4D &l1, TVIS_POINT4D &l2);
	/// draws a 3D triangle.
	void vis_glDraw3DTriangle(TVIS_POINT4D &t1, TVIS_POINT4D &t2, TVIS_POINT4D &t3);

	/** draws a 3d box.
	- min : minimum extension of to box. (one conner of the box).
	- max : maximum extension of to box. (other conner of the box).
	*/
	void vis_glDraw3DBox(TVIS_POINT4D &min, TVIS_POINT4D &max);

	/** draws a 3d box, where only the conner edges are drawn.
	- min : minimum extension of to box. (one conner of the box).
	- max : maximum extension of to box. (other conner of the box).
	- dim : length of the conner edges in x,y,z direction.
	*/
	void vis_glDraw3DBox(TVIS_POINT4D &min, TVIS_POINT4D &max, TVIS_POINT4D dim);

	/// draws a 2D point. znear specifies the depth. znear must lie in the depth range. When no translation is performed use -(znear clip+zfar clip)*0.5
	void vis_glDraw2DPoint(TVIS_POINT2D &p, double znear);
	/// draws a 2D rectangle (axis aligned). znear specifies the depth. znear must lie in the depth range. When no translation is performed use -(znear clip+zfar clip)*0.5
	void vis_glDraw2DRectangle(TVIS_RECT2D &rect, double znear);
	/// draws a 2D box (axis aligned). znear specifies the depth. znear must lie in the depth range. When no translation is performed use -(znear clip+zfar clip)*0.5
	void vis_glDraw2DBox(TVIS_BOX2D &box, double znear);
	/// draws a 2D wireframe rectangle (axis aligned). znear specifies the depth. znear must lie in the depth range. When no translation is performed use -(znear clip+zfar clip)*0.5
	void vis_glDraw2DWireRectangle(TVIS_RECT2D &rect, double znear);
	/// draws a 2D wireframe box. znear specifies the depth. znear must lie in the depth range. When no translation is performed use -(znear clip+zfar clip)*0.5
	void vis_glDraw2DWireBox(TVIS_BOX2D &box, double znear);

	/// draws a circle. znear specifies the depth. znear must lie in the depth range. When no translation is performed use -(znear clip+zfar clip)*0.5
	void vis_glDraw2DWireCircle(TVIS_CIRCLE2D &c, double znear);
	/// draws a 2D line. znear specifies the depth. znear must lie in the depth range. When no translation is performed use -(znear clip+zfar clip)*0.5
	void vis_glDraw2DLine(TVIS_POINT2D &l1, TVIS_POINT2D &l2, double znear);

	/** draws a axisaligned billboard. texture coordinates are set.
	*/
	void vis_glDrawBillboard(double size);


	/** print text, similar to printf
	 */
	void vis_glPrint(TVIS_FONTID &font_id, TVIS_POINT4D &pos, const char *fmt, ...);	

	/** checks for gl errors and calls glFlush().
	*/
	void vis_glFinish();

	/** start selection. setup selection buffer, and prepares rendering in selection mode. 
	 * after rendering in selection mode make sure to call vis_glSelectionEnd(). all rendering in selection mode is done via
	 * vis_glSelectionDraw3DPoints(), vis_glSelectionDraw3DLines(), vis_glSelectionDraw3DTriangles(). 
	 * there is only one call of these functions allowed between a vis_glSelectionBegin()/vis_glSelectionEnd() pair.
	 * see ogl redbook chapter 13 for more details on this topic.
	 * @param x mouse x-position in viewport pixel coordinates.
	 * @param y mouse y-position in viewport pixel coordinates.
	 * @param hitbuffer pointer to an hitbuffer where all information of hitted elements are written.
	 * this must be a multiple of 4, since every element needs 4 int's to store information. information:
	 * - int 0 is not used.
	 * - int 1 is min z value.
	 * - int 2 is max z value.
	 * - int 3 index to element (point, line, or triangle)
	 * @param buffersize size of hitbuffer, this must be a multiple of 4.
	 * @param selectionsize size of the selection field around the mouse position in pixels.
	 */
	void vis_glSelectionBegin(int x, int y, unsigned int *hitbuffer, int buffersize, int selectionsize);

	/** end selection. 
	 * @return index to an element or -1 if no element was hit.
	 */
	int  vis_glSelectionEnd();
	
	/** draw points into selection buffer.
	 * there is only one call of this function allowed between a vis_glSelectionBegin()/vis_glSelectionEnd() pair.
	 * @param points pointer to an array with points. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 * @param num_points number of points.
	 */
	void vis_glSelectionDraw3DPoints(double *points, int num_points);

	/** draw lines into selection buffer.
	 * there is only one call of this function allowed between a vis_glSelectionBegin()/vis_glSelectionEnd() pair.
	 * @param points pointer to an array with points. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 * @param ind index pointer with 32bit indices. 
	 * @param num_points number of points.
	 * @param num_lines number of lines.
	 */
	void vis_glSelectionDraw3DLines(double *points, unsigned int *ind, int num_points, int num_lines);

	/** draw lines into selection buffer.
	 * there is only one call of this function allowed between a vis_glSelectionBegin()/vis_glSelectionEnd() pair.
	 * @param points pointer to an array with points. data must be aligned to 32 bytes (x8,y8,z8,free8).
	 * @param ind index pointer with 32bit indices. 
	 * @param num_points number of points.
	 * @param num_triangles number of triangles.
	 */
	void vis_glSelectionDraw3DTriangles(double *points, unsigned int *ind, int num_points, int num_triangles);
	
private:

	// draw real points
	void vis_glDraw3DPointsPoint(double *points, unsigned __int8 *width, unsigned __int8 *color, int num_points);
	// use lines
	void vis_glDraw3DPointsCross(double *points, unsigned __int8 *width, unsigned __int8 *color, int num_points, TVIS_PS style, double pixel_ratio);

	// ranges
	static float				m_MinLineWidth;
	static float				m_MaxLineWidth;
	static float				m_MinPointSize;
	static float				m_MaxPointSize;

	static int					m_MaxElementsVertices;
	static int					m_MaxElementsIndices;
	
	// extensions
//	CString						m_ExtensionString;

//	static PFNGLPOINTPARAMETERFEXTPROC		glPointParameterfEXT;
//	static PFNGLPOINTPARAMETERFVEXTPROC		glPointParameterfvEXT;
};
 

#endif

