/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"
#include <iostream>

void PointLight::shade( Ray3D& ray ) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.  

	// Vector from point on surface to light source
	Vector3D L = _pos - ray.intersection.point;
	L.normalize();

	// Vector of the reflected ray
	Vector3D R = 2 * ray.intersection.normal.dot(L) * ray.intersection.normal - L;
	R.normalize();

	// Vector from point of intersection to viewer
	Vector3D V = ray.origin - ray.intersection.point;
	V.normalize();

	// Take product of material color and light source color to calculalte phong shading model
	if(ray.intersection.hasTexture){

		if(ray.intersection.hasColourTexture){
			// Blend colour with colour texture and ignore ambient material properties
			ray.col = ray.intersection.texValue * _col_ambient/*
					+ std::max(0.0, ray.intersection.normal.dot(L)) * ray.intersection.mat->diffuse * _col_diffuse
					+ std::max(0.0, std::pow(V.dot(R), ray.intersection.mat->specular_exp)) * ray.intersection.mat->specular * _col_specular*/
					;			
		}else{
			// Blend colour with a black and white texture
			ray.col = ray.intersection.mat->ambient * _col_ambient
					+ std::max(0.0, ray.intersection.normal.dot(L)) * ray.intersection.mat->diffuse * _col_diffuse
					+ std::max(0.0, std::pow(V.dot(R), ray.intersection.mat->specular_exp)) * ray.intersection.mat->specular * _col_specular
					;		

			ray.col[0] *= ray.intersection.texValue[0];
			ray.col[1] *= ray.intersection.texValue[1];		
			ray.col[2] *= ray.intersection.texValue[2];						
		}
	}else{
		// Normal shading, take the colour properties of object
		ray.col = ray.intersection.mat->ambient * _col_ambient
				+ std::max(0.0, ray.intersection.normal.dot(L)) * ray.intersection.mat->diffuse * _col_diffuse
				+ std::max(0.0, std::pow(V.dot(R), ray.intersection.mat->specular_exp)) * ray.intersection.mat->specular * _col_specular
				;				
	}


	// Clamp value of color so that there is no overflow
	ray.col.clamp();
}

void SpotLight::shade( Ray3D& ray ) {
	// Same as point source, except there is a gain pattern for this light

	// Vector from point on surface to light source
	Vector3D L = _pos - ray.intersection.point;
	L.normalize();

	// Vector of the reflected ray
	Vector3D R = 2 * ray.intersection.normal.dot(L) * ray.intersection.normal - L;
	R.normalize();

	// Vector from point of intersection to viewer
	Vector3D V = ray.origin - ray.intersection.point;
	V.normalize();

	// Take product of material color and light source color to calculalte phong shading model
	if(ray.intersection.hasTexture){

		if(ray.intersection.hasColourTexture){
			// Blend colour with colour texture and ignore ambient material properties
			ray.col = ray.intersection.texValue * _col_ambient/*
					+ std::max(0.0, ray.intersection.normal.dot(L)) * ray.intersection.mat->diffuse * _col_diffuse
					+ std::max(0.0, std::pow(V.dot(R), ray.intersection.mat->specular_exp)) * ray.intersection.mat->specular * _col_specular*/
					;			
		}else{
			// Blend colour with a black and white texture
			ray.col = ray.intersection.mat->ambient * _col_ambient
					+ std::max(0.0, ray.intersection.normal.dot(L)) * ray.intersection.mat->diffuse * _col_diffuse
					+ std::max(0.0, std::pow(V.dot(R), ray.intersection.mat->specular_exp)) * ray.intersection.mat->specular * _col_specular
					;		

			ray.col[0] *= ray.intersection.texValue[0];
			ray.col[1] *= ray.intersection.texValue[1];		
			ray.col[2] *= ray.intersection.texValue[2];						
		}
	}else{
		// Normal shading, take the colour properties of object
		ray.col = ray.intersection.mat->ambient * _col_ambient
				+ std::max(0.0, ray.intersection.normal.dot(L)) * ray.intersection.mat->diffuse * _col_diffuse
				+ std::max(0.0, std::pow(V.dot(R), ray.intersection.mat->specular_exp)) * ray.intersection.mat->specular * _col_specular
				;				
	}

	// Find angle between the point on surface to the light source, don't light up things behind
	double gain = pow(std::max(-L.dot(_dir), 0.), _dropoff);

	ray.col = gain * ray.col;

	// Clamp value of color so that there is no overflow
	ray.col.clamp();
}
