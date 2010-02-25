/*********************************************************************************

 file:			vis_helper.h
 author:		aick in der au
 description:	math helper routines:
				intersection tests, 2d/3d conversion

**********************************************************************************/


#ifndef _VIS_HELPER_H
#define _VIS_HELPER_H

#define PI		3.141592654
#define RAD2DEG 57.29577951
#define DEG2RAD (1.0/57.29577951)

#include "vis_types.h"

/// returns true if point lies inside or at it's border of axis aligned bounding box
bool			visIsPointInAABB2D(const POINT &point, const RECT &rect);
/// returns true if point lies inside or at it's border of axis aligned bounding box
bool			visIsPointInAABB2D(const TVIS_POINT2D &point, const TVIS_RECT2D &rect);

/** returns true if line lies inside or at it's border of axis aligned bounding box
*/
bool			visIsLineInAABB2D(const TVIS_POINT2D &p1, const TVIS_POINT2D &p2, const TVIS_RECT2D &rect);

/** returns true if triangle lies completly inside (including border) of axis aligned bounding box.
 - p1, p2, p3	triangle points
 - rect			axis aligned bounding box
*/
bool			visIsTriangleInAABB2D(const TVIS_POINT2D &p1, const TVIS_POINT2D &p2, const TVIS_POINT2D &p3, const TVIS_RECT2D &rect);
/// returns true if point lies inside or at it's border of circle
bool			visIsPointInCircle2D(TVIS_POINT2D &point, TVIS_CIRCLE2D &circle);
/// returns true if line lies completly inside or at it's border of circle
bool			visIsLineInCircle2D(TVIS_POINT2D &p1, TVIS_POINT2D &p2, TVIS_CIRCLE2D &circle);
/// returns true if triangle lies completly inside or at it's border of circle
bool			visIsTriangleInCircle2D(TVIS_POINT2D &p1, TVIS_POINT2D &p2, TVIS_POINT2D &p3, TVIS_CIRCLE2D &circle);

/// returns true if axis aligned bounding box (aabb) lies completly inside or at it's border of other aabb
bool			visIsAABBInAABB2D(const TVIS_RECT2D &rect, const TVIS_POINT2D &bounding_rect);

/** intersection with two lines.
 - ip		returned intersection point
 - l1p1		first point of line 1
 - l1p2		second point of line 1
 - l2p1		first point of line 2
 - l2p2		second point of line 2
*/
bool			visIntersectLines2D(TVIS_POINT2D &ip, TVIS_POINT2D l1p1, TVIS_POINT2D l1p2, TVIS_POINT2D l2p1, TVIS_POINT2D l2p2);
/// intersection between two aabb's, no contact points, planes are calculated.
bool			visIntersectAABB2D(TVIS_RECT2D &r0, TVIS_RECT2D &r1);
/// intersection between two oriented bounding boxes (obb's), not contact points, planes are calculated.
bool			visIntersectOBB2D(TVIS_BOX2D &b0, TVIS_BOX2D &b1);
/// intersection between a line and a circle, no contact points, planes are calculated.
bool			visIntersectCircleLine2D(TVIS_POINT2D &p1, TVIS_POINT2D &p2, TVIS_CIRCLE2D &circle);
/// intersection between a circle and an aabb, no contact points, planes are calculated.
bool			visIntersectCircleAABB2D(TVIS_RECT2D &rect, TVIS_CIRCLE2D &circle);
/// intersection between a circle and an oriented bounding box (obb), no contact points, planes are calculated.
bool			visIntersectCircleOBB2D(TVIS_BOX2D &box, TVIS_CIRCLE2D &circle);

/// intersection between a ray in 3d and an axis aligned bounding box (aabb) in 3d. no contact points, planes are calculated.
bool			visIntersectRayAABB3D(const TVIS_POINT4D &ray_pos, const TVIS_POINT4D &ray_dir, TVIS_POINT4D box_min, TVIS_POINT4D box_max);

/** intersection between a ray and a sphere.
 */
bool			visIntersectRaySphere3D(TVIS_POINT4D &ray_pos, TVIS_POINT4D &ray_dir, const TVIS_POINT4D &sphere_pos, double radius);

/** intersection between a ray and a triangle.
 */
bool			visIntersectRayTriangle3D(const TVIS_POINT4D &ray_pos, const TVIS_POINT4D &ray_dir, TVIS_POINT4D &v0, TVIS_POINT4D &v1, TVIS_POINT4D &v2, TVIS_POINT4D &cp);

				
		
// 3d -> 2d conversion routines

TVIS_POINT2D	visGetPointFromPoint2D(const TVIS_POINT4D &point, const TVIS_POINT4D &position, TVIS_MATRIX4D &rotation);
TVIS_RECT2D		visGetAABBFromBox2D(const TVIS_POINT4D &box_min, const TVIS_POINT4D &box_max, TVIS_MATRIX4D &rotation);
TVIS_RECT2D		visGetAABBFromBox2D(const TVIS_POINT4D &box_min, const TVIS_POINT4D &box_max, const TVIS_POINT4D &position, TVIS_MATRIX4D &rotation);
void			visGetOBBFromBox2D(TVIS_BOX2D *b, const TVIS_POINT4D &box_min, const TVIS_POINT4D &box_max, TVIS_MATRIX4D &rotation);
void			visGetOBBFromBox2D(TVIS_BOX2D *b, const TVIS_POINT4D &box_min, const TVIS_POINT4D &box_max, const TVIS_POINT4D &position, TVIS_MATRIX4D &rotation);

#endif

/*
void		visEuler2Quat(TVIS_POINT4D angle, TVIS_POINT4D &quat);
void		visQuat2AngleAxis(TVIS_POINT4D &quat, TVIS_POINT4D &outaxis, double &outang);
void		visQuatMul(TVIS_POINT4D &q1, TVIS_POINT4D &q2, TVIS_POINT4D &qres);
void		visQuatRotate(TVIS_POINT4D &pos, TVIS_POINT4D &ang, TVIS_POINT4D &outpos);
*/