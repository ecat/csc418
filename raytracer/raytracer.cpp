/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		Implementations of functions in raytracer.h

***********************************************************/


#include "raytracer.h"
#include "bmp_io.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

Raytracer::Raytracer() : _lightSource(nullptr) {
	_root = std::make_shared<SceneDagNode>();
}

Raytracer::~Raytracer() {
}

SceneDagNode::Ptr Raytracer::addObject( SceneDagNode::Ptr parent, 
		SceneObject::Ptr obj, Material::Ptr mat ) {
    SceneDagNode::Ptr node = std::make_shared<SceneDagNode>( obj, mat );
	node->parent = parent;
	node->next = nullptr;
	node->child = nullptr;
	
	// Add the object to the parent's child list, this means
	// whatever transformation applied to the parent will also
	// be applied to the child.
	if (parent->child == nullptr) {
		parent->child = node;
	}
	else {
		parent = parent->child;
		while (parent->next != nullptr) {
			parent = parent->next;
		}
		parent->next = node;
	}
	
	return node;
}

LightListNode::Ptr Raytracer::addLightSource( LightSource::Ptr light ) {
    LightListNode::Ptr tmp = _lightSource;
	_lightSource = std::make_shared<LightListNode>( light, tmp );
	return _lightSource;
}

void Raytracer::rotate( SceneDagNode::Ptr node, char axis, double angle ) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;
	
	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
		    node->trans = node->trans*rotation; 	
			angle = -angle;
		} 
		else {
			node->invtrans = rotation*node->invtrans; 
		}	
	}
}

void Raytracer::translate( SceneDagNode::Ptr node, Vector3D trans ) {
	Matrix4x4 translation;
	
	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	node->trans = node->trans*translation; 	
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	node->invtrans = translation*node->invtrans; 
}

void Raytracer::scale( SceneDagNode::Ptr node, Point3D origin, double factor[3] ) {
	Matrix4x4 scale;
	
	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	node->trans = node->trans*scale; 	
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	node->invtrans = scale*node->invtrans; 
}

Matrix4x4 Raytracer::initInvViewMatrix( Point3D eye, Vector3D view, 
		Vector3D up ) {
	Matrix4x4 mat; 
	Vector3D w;
	view.normalize();
	up = up - up.dot(view)*view;
	up.normalize();
	w = view.cross(up);

	mat[0][0] = w[0];
	mat[1][0] = w[1];
	mat[2][0] = w[2];
	mat[0][1] = up[0];
	mat[1][1] = up[1];
	mat[2][1] = up[2];
	mat[0][2] = -view[0];
	mat[1][2] = -view[1];
	mat[2][2] = -view[2];
	mat[0][3] = eye[0];
	mat[1][3] = eye[1];
	mat[2][3] = eye[2];

	return mat; 
}


void Raytracer::computeTransforms( SceneDagNode::Ptr node )
{
    SceneDagNode::Ptr childPtr;
    if (node->parent != nullptr)
    {
        node->modelToWorld = node->parent->modelToWorld*node->trans;
        node->worldToModel = node->invtrans*node->parent->worldToModel; 
    }
    else
    {
        node->modelToWorld = node->trans;
        node->worldToModel = node->invtrans; 
    }
    // Traverse the children.
    childPtr = node->child;
    while (childPtr != NULL) {
        computeTransforms(childPtr);
        childPtr = childPtr->next;
    }



}

void Raytracer::traverseScene( SceneDagNode::Ptr node, Ray3D& ray ) {
    SceneDagNode::Ptr childPtr;

    // Applies transformation of the current node to the global
    // transformation matrices.
    if (node->obj) {
        // Perform intersection.
        if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
            ray.intersection.mat = node->mat;
        }
    }
    // Traverse the children.
    childPtr = node->child;
    while (childPtr != nullptr) {
        traverseScene(childPtr, ray);
        childPtr = childPtr->next;
    }

}

void Raytracer::computeShading( Ray3D& ray ) {
    LightListNode::Ptr curLight = _lightSource;
    for (;;) {
        if (curLight == nullptr) break;
        // Each lightSource provides its own shading function.

        // Implement shadows here if needed.

        curLight->light->shade(ray);
        curLight = curLight->next;
    }
}

void Raytracer::initPixelBuffer() {
    int numbytes = _scrWidth * _scrHeight * sizeof(unsigned char);
    _rbuffer = new unsigned char[numbytes];
    std::fill_n(_rbuffer, numbytes,0);
    _gbuffer = new unsigned char[numbytes];
    std::fill_n(_gbuffer, numbytes,0);
    _bbuffer = new unsigned char[numbytes];
    std::fill_n(_bbuffer, numbytes,0);
}

void Raytracer::flushPixelBuffer( std::string file_name ) {
    bmp_write( file_name.c_str(), _scrWidth, _scrHeight, _rbuffer, _gbuffer, _bbuffer );
    delete _rbuffer;
    delete _gbuffer;
    delete _bbuffer;
}

Colour Raytracer::shadeRay( Ray3D& ray ) {
    Colour col(0.0, 0.0, 0.0); 
    traverseScene(_root, ray); 

    // Don't bother shading if the ray didn't hit 
    // anything.
    if (!ray.intersection.none) {
        computeShading(ray); 
        col = ray.col;  
    }

    // You'll want to call shadeRay recursively (with a different ray, 
    // of course) here to implement reflection/refraction effects.  

    // Check if there was an intersection and that we have not exceeded the max number of reflections
    if (!ray.intersection.none && ray.num_reflections < MAX_NUM_REFLECTIONS){
    	// Create a new ray with a new origin and direction
    	// The reflected ray direction is calculated using Snell's law
    	Vector3D reflectedRayDirection = ray.dir - 2 * (ray.dir.dot(ray.intersection.normal)) * ray.intersection.normal;

    	// Start the ray a little bit away from the surface to remove artifacts, note that it is addition here
    	Ray3D reflectedRay(ray.intersection.point + 0.001 * ray.intersection.normal, reflectedRayDirection, ray.num_reflections + 1);	
    	reflectedRay.refractive_index = ray.refractive_index;
    	reflectedRay.dir.normalize();

    	Colour reflectedCol = shadeRay(reflectedRay);

    	// We don't care if the reflected ray went into the background 
    	if(!reflectedRay.intersection.none || reflectedRay.intersection.none){
	    	// Blend the reflected colors according to the specular reflection component
	    	// If the specular component is zero, then take the local illuminated color
	    	double r_scale = ray.intersection.mat->specular[0];
	    	double g_scale = ray.intersection.mat->specular[1];	
	    	double b_scale = ray.intersection.mat->specular[2];    	
	    	col[0] = (1 - r_scale) * col[0] + (r_scale) * reflectedCol[0];
	    	col[1] = (1 - g_scale) * col[1] + (g_scale) * reflectedCol[1];
	    	col[2] = (1 - b_scale) * col[2] + (b_scale) * reflectedCol[2];
    	}
    }

    // Do refraction effect
    if (!ray.intersection.none && ray.intersection.mat->transparent && ray.num_reflections < MAX_NUM_REFLECTIONS){

    	double theta_incident = ray.dir.dot(-ray.intersection.normal);
		double n_1 = ray.refractive_index;
		double n_2 = ray.intersection.mat->n; // refractive index of material ray is entering
    	
    	// Check for total internal reflection
    	if(sin(theta_incident) < n_2/n_1){
	    	// Create new ray for refracted vector
	    	Vector3D refractedRayDirection = n_1/n_2 * ray.dir + 
	    		(n_1/n_2 * cos(theta_incident) - 
	    		sqrt(1 - pow(sin(theta_incident), 2))) * ray.intersection.normal;

	    	// Start the ray a little bit away from the surface to remove artifacts
	    	// note that it is subtraction here because we are crossing material interface
	    	Ray3D refractedRay(ray.intersection.point - 0.001 * ray.intersection.normal, refractedRayDirection, ray.num_reflections + 1);
	    	refractedRay.refractive_index = n_2;    	
	    	refractedRay.dir.normalize();

	    	Colour refractedCol = shadeRay(refractedRay);

	    	std::cout << "n1 " << n_1 << " n_2 " << n_2 << std::endl;
	    	// TODO approximate fresnel equations
	    	col[0] = 0.5 * col[0] + 0.5 * refractedCol[0];
	    	col[1] = 0.5 * col[0] + 0.5 * refractedCol[1];
	    	col[2] = 0.5 * col[0] + 0.5 * refractedCol[2];	    	
    	}
    }


    return col; 
}	

void Raytracer::render( int width, int height, Point3D eye, Vector3D view, 
        Vector3D up, double fov, std::string fileName ) {
    computeTransforms(_root);
    Matrix4x4 viewToWorld;
    _scrWidth = width;
    _scrHeight = height;
    double factor = (double(height)/2)/tan(fov*M_PI/360.0);

    initPixelBuffer();
    viewToWorld = initInvViewMatrix(eye, view, up);


    bool enableMultiSampling = false; // DEBUG

    // Construct multiple rays for each pixel.
    for (int i = 0; i < _scrHeight; i++) {
        for (int j = 0; j < _scrWidth; j++) {
            // Sets up ray origin and direction in view space, 
            // image plane is at z = -1.
            Point3D origin(0., 0., 0.);

            if(enableMultiSampling){
            	int num_samples = 9;
	            for(int u = 0; u < num_samples; u++){
	            		double dx = static_cast <double> (rand()) / static_cast<double>(RAND_MAX);
	            		double dy = static_cast <double> (rand()) / static_cast<double>(RAND_MAX);	
						Point3D imagePlane;
						imagePlane[0] = (-double(width)/2 + dx + j)/factor;
						imagePlane[1] = (-double(height)/2 + dy + i)/factor;
						imagePlane[2] = -1;

						// TODO: Convert ray to world space and call 
						// shadeRay(ray) to generate pixel colour. 	
						
						Ray3D ray(viewToWorld * origin, viewToWorld * Vector3D(imagePlane[0], imagePlane[1], imagePlane[2]));
						ray.dir.normalize();

						Colour col = shadeRay(ray); 

						// prevent buffer overflow
						int _rbuffertmp = _rbuffer[i*width+j] + int(col[0]*255./ num_samples);
						int _gbuffertmp = _gbuffer[i*width+j] + int(col[1]*255./ num_samples);
						int _bbuffertmp = _bbuffer[i*width+j] + int(col[2]*255./ num_samples);            		

						_rbuffer[i*width+j] = std::min(_rbuffertmp, 255);
						_gbuffer[i*width+j] = std::min(_gbuffertmp, 255);
						_bbuffer[i*width+j] = std::min(_bbuffertmp, 255);						
	            }
            }else{
	            Point3D imagePlane;
				imagePlane[0] = (-double(width)/2 + 0.5 + j)/factor;
				imagePlane[1] = (-double(height)/2 + 0.5 + i)/factor;
				imagePlane[2] = -1;

				Ray3D ray(viewToWorld * origin, viewToWorld * Vector3D(imagePlane[0], imagePlane[1], imagePlane[2]));
				ray.dir.normalize();

				Colour col = shadeRay(ray); 

				_rbuffer[i*width+j] += int(col[0]*255);
				_gbuffer[i*width+j] += int(col[1]*255);
				_bbuffer[i*width+j] += int(col[2]*255);
			}	
		}
	}

	flushPixelBuffer(fileName);
}

