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

	if(fabs(x_check) < 0.5 && fabs(y_check) < 0.5){
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

			if(width > 0 && height > 0){
				ray.intersection.texValue = getTextureValue(x_check, y_check);
				ray.intersection.hasTexture = true;				
				ray.intersection.hasColourTexture = false;				
			}else{
				ray.intersection.hasTexture = false;
			}

				
			return true;
		}
	}

	return false;
}

bool UnitCube::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// Convert ray to object space
	Ray3D r_objectSpace(worldToModel * ray.origin, worldToModel * ray.dir);
	r_objectSpace.dir.normalize(); // Need to normalize this vector so that projection works

	// Determine intersection of unit cube with ray 
	// Find x projections
	double t1 = (-0.5 - r_objectSpace.origin[0])/r_objectSpace.dir[0];
	double t2 = (0.5 - r_objectSpace.origin[0])/r_objectSpace.dir[0];	
	// Find y projections
	double t3 = (0.5 - r_objectSpace.origin[1])/r_objectSpace.dir[1];	
	double t4 = (-0.5 - r_objectSpace.origin[1])/r_objectSpace.dir[1];	
	// Find z projections
	double t5 = (0.5 - r_objectSpace.origin[2])/r_objectSpace.dir[2];	
	double t6 = (-0.5 - r_objectSpace.origin[2])/r_objectSpace.dir[2];	

	double tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	double tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));


	if(tmin < 0){
		return false;
	}

	//std::cout << "tmax " << tmax << " tmin " << tmin << " int " << ray.intersection.t_value << std::endl;
	// ray doesn't intersect
	if(!ray.intersection.none){
		if(tmin > tmax || tmin > ray.intersection.t_value){
			return false;
		}
	}
	
	Point3D localIntersection = r_objectSpace.origin + tmin * r_objectSpace.dir;

	for(int i = 0 ; i < 3; i++){
		if(localIntersection[i] < -0.5 || localIntersection[i] > 0.5){
			return false;
		}
	}

	Vector3D localNormal;

	ray.intersection.t_value = tmin;
	// Bring the points and normals back to world space
	ray.intersection.point = modelToWorld * localIntersection;

	double EPS = 0.001;
	if(localIntersection[0] < -0.5 + EPS){
		localNormal = Vector3D(-1, 0,  0);
	}else if(localIntersection[0] > 0.5 - EPS){
		localNormal = Vector3D(1, 0,  0);
	}else if(localIntersection[1] < -0.5 + EPS){
		localNormal = Vector3D(0, -1, 0);
	}else if(localIntersection[1] > 0.5 - EPS){
		localNormal = Vector3D(0, 1, 0);
	}else if(localIntersection[2] > 0.5 - EPS){
		localNormal = Vector3D(0, 0, 1);
	}else{
		localNormal = Vector3D(0, 0, -1);
	}

	// Bringing normals back to world space requires special math
	ray.intersection.normal = worldToModel.transpose() * localNormal;
	ray.intersection.normal.normalize();

	ray.intersection.none = false;
	ray.intersection.hasTexture = false;


	return true;
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

	// Convert ray to object space
	Ray3D r_objectSpace(worldToModel * ray.origin, worldToModel * ray.dir);
	r_objectSpace.dir.normalize(); // Need to normalize this vector so that projection works

	// Find vector qc pointing from origin of ray to center of sphere, c is center of circle, q is ray origin
	Point3D q = r_objectSpace.origin;
	Point3D c = Point3D(0., 0., 0.);
	Vector3D qc = c - q;
	// Radius of circle is 0.5
	double d = 0.5;

	// Project vector R onto ray direction to get length
	double projectedLength = qc.dot(r_objectSpace.dir);
	// Find length of vector of triangle formed by this projected length, center of circle, and ray origin
	double k_squared = pow(d, 2) - (pow(qc.length(), 2) - pow(projectedLength, 2));

	// Compare length to unit circle radius
	if(k_squared > 0){

		// Calculate intersections
		Point3D intersection_1 = q + (qc.dot(r_objectSpace.dir) + sqrt(k_squared)) * r_objectSpace.dir;
		Point3D intersection_2 = q + (qc.dot(r_objectSpace.dir) - sqrt(k_squared)) * r_objectSpace.dir;

		// Choose intersection that has the non negative t value that is smaller
		// this works since ray.dir is unit length
		double t_1 = (intersection_1 - q).dot(r_objectSpace.dir);
		double t_2 = (intersection_2 - q).dot(r_objectSpace.dir);

		// Both intersections lie behind the origin
		if(t_1 <= 0 && t_2 <= 0){
			return false;
		}

		// There is an intersection with circle
		if(ray.intersection.none || t_1 < ray.intersection.t_value || t_2 < ray.intersection.t_value){

			// Determine mapping for texture coordinates for sphere
			double u, v;

			// Bring the points and normals back to world space
			// Intersection 2 will always be closer to origin so it sufficies to check
			// that it is not behind the ray origin (t_1 > t_2) is always true
			if(t_2 < 0 || t_1 < t_2){
				ray.intersection.t_value = t_1;
				ray.intersection.point = modelToWorld * intersection_1;
				ray.intersection.normal = worldToModel.transpose() * (intersection_1 - c);

				u = 0.5 + atan2(intersection_1[2], intersection_1[0])/ (2 * M_PI);
				v = 0.5 - asin(intersection_1[1]/d)/ M_PI;
			}else {
				ray.intersection.t_value = t_2;
				ray.intersection.point = modelToWorld * intersection_2;
				ray.intersection.normal = worldToModel.transpose() * (intersection_2 - c);

				u = 0.5 + atan2(intersection_2[2], intersection_2[0])/ (2 * M_PI);
				v = 0.5 - asin(intersection_2[1]/d)/ M_PI;
			}

			ray.intersection.normal.normalize();

			ray.intersection.none = false;

			if(width > 0 && height > 0){
				ray.intersection.hasTexture = true;
				ray.intersection.texValue = getTextureValue(u, v);
				ray.intersection.hasColourTexture = isColourTexture;
			}else{
				ray.intersection.hasTexture = false;	
			}

			return true;
		}


	}
	return false;
}

