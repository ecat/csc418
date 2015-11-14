/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	// Convert ray to object space
	Ray3D r_objectSpace(worldToModel * ray.origin, worldToModel * ray.dir);

	// Determine intersection of unit square with ray 
	double t = - (r_objectSpace.origin[2])/(r_objectSpace.dir[2]);

	// Intersection is behind eye
	if(t <= 0)
		return false;

	// Check that x and y values lie within unit square
	double x_check = r_objectSpace.origin[0] + t * r_objectSpace.dir[0];
	double y_check = r_objectSpace.origin[1] + t * r_objectSpace.dir[1];

	if(abs(x_check) < 1.0 && abs(y_check) < 1.0){
		// Intersection lies within unit square

		// Check that there was no previous intersection and that the intersection is nearest
		// the eye
		if(ray.intersection.none || t < ray.intersection.t_value){
			// New intersection is good so update the intersection values
			ray.intersection.t_value = t;

			// Bring the points and normals back to world space
			ray.intersection.point = modelToWorld * Point3D(x_check, y_check, 0);

			// Bringing normals back to world space requires special math
			ray.intersection.normal = worldToModel.transpose() * Vector3D(0, 0, 1);
			ray.intersection.normal.normalize();

			ray.intersection.none = false;

			return true;
		}
	}

	return false;
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
	
	return false;
}

