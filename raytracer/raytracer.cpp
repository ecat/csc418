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
#include <boost/thread.hpp>

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

void Raytracer::computeShading( int thread_id, Ray3D& ray ) {
    LightListNode::Ptr curLight = _lightSource;
    // For keeping track of multiple light sources
    Colour accumulatedColour;
    for (;;) {
        if (curLight == nullptr) break;
        // Each lightSource provides its own shading function.

        // Implement shadows here if needed.
        // Calculate ray between light source and shading intersection point
        if(ENABLE_SOFT_SHADOWS){
        	// Use this to calculate soft shadows and simulate an area source
			Vector3D pointLightingRayDirection = curLight->light->get_position() - ray.intersection.point;        

			// Find plane that light resides in
			Vector3D v1 = cross(pointLightingRayDirection, Vector3D(1, 0, 1));
			v1.normalize();
			Vector3D v2 = cross(v1, pointLightingRayDirection);
			v2.normalize();
 
        	double areaLightRadius = curLight->light->area;
        	double rayPower = 0;
        	Colour transparencyColour;	// Colour added from light passing through semitransparent objects
        	for(int i = 0 ; i < NUM_SOFT_SHADOW_SOURCES; i++){
				// Calculate a dr and dtheta uniformly distributed to simulate circle
	    		double dr = static_cast <double> (raytracerRand(thread_id)) / static_cast<double>(RAND_MAX);
	    		double dtheta = static_cast <double> (raytracerRand(thread_id)) / static_cast<double>(RAND_MAX);	

	    		// - PI < dtheta < PI
	    		// 0 < dr < R
	    		dr = dr * areaLightRadius;
	    		dtheta = (dtheta * 2 * M_PI) - M_PI;

        		Point3D lightOrigin = curLight->light->get_position() + dr * (cos(dtheta) * v1 + sin(dtheta) * v2);
        		Vector3D areaLightingRayDirection = ray.intersection.point - lightOrigin;
		        Ray3D lightingRay(lightOrigin, areaLightingRayDirection);
		        lightingRay.dir.normalize();
		        traverseScene(_root, lightingRay);

		        if(lightingRay.intersection.none){
		        	// light ray didn't intersect anything
		        	rayPower += 1.0/(double)(NUM_SOFT_SHADOW_SOURCES);		        	
		        }else{
			        Vector3D delta = lightingRay.intersection.point - ray.intersection.point;

		        	if(delta.length() < EPSILON){
		        		// Since lighting ray starts at the light and points towards surface point, we check that we didn't
		        		// intersect with the surface

			        	// light ray didn't intersect anything
			        	rayPower += 1.0/(double)(NUM_SOFT_SHADOW_SOURCES);
		        	}else if(lightingRay.intersection.mat->transparent){
			        	if(lightingRay.intersection.mat->transparency == 1.0){
			        		// Let glass have a little bit of shadow and approximate fresnel equations assuming ray passed through glass and air
			        		// Reflectance is given for perpendicular ((n1 - n2)/(n1+n2))^2
			        		// We want transmittance so take 1 - reflectance        	
			        		rayPower += (1.0 - pow((lightingRay.intersection.mat->n - 1.0)/(lightingRay.intersection.mat->n + 1.0), 2))/(double)(NUM_SOFT_SHADOW_SOURCES);
						}else{
							rayPower += lightingRay.intersection.mat->transparency/(double)(NUM_SOFT_SHADOW_SOURCES);
							
							if(lightingRay.intersection.hasColourTexture){
								transparencyColour = transparencyColour + 
								(1.0 - lightingRay.intersection.mat->transparency)/(double)(NUM_SOFT_SHADOW_SOURCES) 
								* lightingRay.intersection.texValue;
							}
						}
		        	}else{
		        		// light ray intersected some other object, don't add to raypower
		        	}
	        	}
        	}

	        curLight->light->shade(ray);
	        ray.col = rayPower * ray.col + 0.5 * rayPower * transparencyColour;
	        ray.col.clamp();


        }else{
        	// Shadows for point source
			Vector3D lightingRayDirection = curLight->light->get_position() - ray.intersection.point;        
	        Ray3D lightingRay(ray.intersection.point + EPSILON * ray.intersection.normal, lightingRayDirection);
	        lightingRay.dir.normalize();
	        traverseScene(_root, lightingRay);

	        if(lightingRay.intersection.none || lightingRay.intersection.mat->transparent){
		        curLight->light->shade(ray);
	        }
    	}

	    accumulatedColour = ray.col + accumulatedColour;

        curLight = curLight->next;
    }

	ray.col = accumulatedColour;
    ray.col.clamp();
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

Colour Raytracer::shadeRay(int thread_id,  Ray3D& ray ) {
    Colour col(0.0, 0.0, 0.0); 
    traverseScene(_root, ray); 

    // Don't bother shading if the ray didn't hit 
    // anything.
    if (!ray.intersection.none) {
        computeShading(thread_id, ray); 
        col = ray.col;  
    }

    // You'll want to call shadeRay recursively (with a different ray, 
    // of course) here to implement reflection/refraction effects.  

    // Check if there was an intersection and that we have not exceeded the max number of reflections
    if (!ray.intersection.none && ray.num_reflections < MAX_NUM_REFLECTIONS){

    	bool approachingFromBack = false;

    	if(ray.dir.dot(-ray.intersection.normal) < 0){
    		approachingFromBack = true;
    	}

	    // Do refraction effect
	    if (ray.intersection.mat->transparent){

			double n_1 = ray.refractive_index;
			double n_2 = ray.intersection.mat->n; // refractive index of material ray is entering
	    	double theta_incident = acos(ray.dir.dot(-ray.intersection.normal));
	    	
	    	// If dotproduct is negative, then we are approaching from back of material
	    	// We only support air to glass and glass to air interfaces
	    	if(ray.dir.dot(-ray.intersection.normal) < 0){
	    		n_1 = n_2;
	    		n_2 = 1.0;
	    		theta_incident = acos(ray.dir.dot(ray.intersection.normal));
	    	}

	    	double total_reflectance = 1.0;
	    	double total_transmittance = 0.0;
	    	double critical_angle = asin(n_2/n_1);

	    	// Not total internal reflection
	    	if(theta_incident < critical_angle || n_2 > n_1){
	    		double theta_transmitted = asin(sin(theta_incident) * n_1/n_2);    	

		    	// Create new ray for refracted vector
		    	Vector3D refractedRayDirection;

		    	if(approachingFromBack){
		    		refractedRayDirection = n_1/n_2 * ray.dir + 
			    		(n_1/n_2 * cos(theta_incident) - 
			    		sqrt(1 - pow(sin(theta_transmitted), 2))) * (-ray.intersection.normal);	
		    	}else{
			    	refractedRayDirection = n_1/n_2 * ray.dir + 
			    		(n_1/n_2 * cos(theta_incident) - 
			    		sqrt(1 - pow(sin(theta_transmitted), 2))) * ray.intersection.normal;	
		    	} 

		    	// Start the ray a little bit away from the surface to remove artifacts
		    	// note that it is subtraction here because we are crossing material interface
		    	Ray3D refractedRay(ray.intersection.point, refractedRayDirection, ray.num_reflections + 1);

		    	if(approachingFromBack){
		    		refractedRay.origin = refractedRay.origin + EPSILON * ray.intersection.normal;
		    	}else{
		    		refractedRay.origin = refractedRay.origin - EPSILON * ray.intersection.normal;
		    	}

		    	refractedRay.refractive_index = n_2;    	
		    	refractedRay.dir.normalize();

		    	Colour refractedCol = shadeRay(thread_id, refractedRay);
		    	double transparency = ray.intersection.mat->transparency;

		    	// Calculate Fresnel equations
		    	double reflectance_perp = (n_1 * cos(theta_incident) - n_2 * cos(theta_transmitted))/
		    								(n_1 * cos(theta_incident) + n_2 * cos(theta_transmitted));
				reflectance_perp = pow(reflectance_perp, 2);

				double reflectance_para = (n_2 * cos(theta_incident) - n_1 * cos(theta_transmitted))/
											(n_2 * cos(theta_incident) + n_1 * cos(theta_transmitted));
				reflectance_para = pow(reflectance_para, 2);	    	

				// Calculate total reflectance
				total_reflectance = (reflectance_para + reflectance_perp) / 2;
				total_transmittance = 1.0 - total_reflectance;	
				
		    	col[0] = total_reflectance * col[0] + (1.0 - transparency) * col[0] + transparency * total_transmittance * refractedCol[0];
		    	col[1] = total_reflectance * col[1] + (1.0 - transparency) * col[1] + transparency * total_transmittance * refractedCol[1];
		    	col[2] = total_reflectance * col[2] + (1.0 - transparency) * col[2] + transparency * total_transmittance * refractedCol[2];	    	
	    	}
	    }else{

	    	// Do reflection effect
	    	// Create a new ray with a new origin and direction
	    	// The reflected ray direction is calculated using Snell's law
	    	Vector3D reflectedRayDirection = ray.dir - 2 * (ray.dir.dot(ray.intersection.normal)) * ray.intersection.normal;

	    	// Start the ray a little bit away from the surface to remove artifacts
	    	// This calculation is reversed from the refraction calculation
	    	Ray3D reflectedRay(ray.intersection.point, reflectedRayDirection, ray.num_reflections + 1);	

	    	if(approachingFromBack){
	    		reflectedRay.origin = reflectedRay.origin - EPSILON * ray.intersection.normal;
	    	}else{
	    		reflectedRay.origin = reflectedRay.origin + EPSILON * ray.intersection.normal;
	    	}

	    	reflectedRay.refractive_index = ray.refractive_index;
	    	reflectedRay.dir.normalize();

	    	Colour reflectedCol = shadeRay(thread_id, reflectedRay);

	    	// We don't care if the reflected ray went into the background 
	    	if(!reflectedRay.intersection.none || reflectedRay.intersection.none){
		    	// Blend the reflected colors according to the specular reflection component
		    	// If the specular component is zero, then take the local illuminated color

		    	//TODO add damping factor for reflected colour
		    	double r_scale = ray.intersection.mat->specular[0];
		    	double g_scale = ray.intersection.mat->specular[1];	
		    	double b_scale = ray.intersection.mat->specular[2];    	
		    	col[0] += (r_scale) * reflectedCol[0];
		    	col[1] += (g_scale) * reflectedCol[1];
		    	col[2] += (b_scale) * reflectedCol[2];    	
		   		
	    	}
	    }


    }


    col.clamp();
    return col; 
}	

void Raytracer::render( int width, int height, Point3D eye, Vector3D view, 
        Vector3D up, double fov, std::string fileName, Point3D focusPoint ) {
    computeTransforms(_root);
    _scrWidth = width;
    _scrHeight = height;
    view.normalize();
    double factor = (double(height)/2)/tan(fov*M_PI/360.0);

    initPixelBuffer();

	// Unfortunately, multithreading doesn't provide many gains
	// suspicion is due to rand which is blocking, had to switch to rand_r instead
	if(ENABLE_MULTI_THREAD){
	    boost::thread_group group;
	    int numThreads = 8;
	    _seeds = new unsigned int[numThreads];

	    for(int i = 0 ; i < numThreads; i++){
	    	_seeds[i] = clock();
	    	group.add_thread(new boost::thread(&Raytracer::segment, this, i,
	    	i * height/numThreads, (i+1) * height/numThreads, factor, eye, view, up, focusPoint));
	    }
	    group.join_all();
	}else{
		_seeds = new unsigned int[1];
		_seeds[0] = clock();
	    segment(0, 0, _scrHeight, factor, eye, view, up, focusPoint);
    }

	flushPixelBuffer(fileName);
}

void Raytracer::segment(int thread_id, int row_start, int row_end, double factor, Point3D eye, Vector3D view, Vector3D up,
	Point3D focusPoint){

    Matrix4x4 viewToWorld;
    double apertureRadius = 0.3; // Higher aperture radius gives more blur
	for(int k = 0 ; k < NUM_DEPTH_OF_FIELD_SAMPLES; k++){

		if(ENABLE_DEPTH_OF_FIELD || NUM_DEPTH_OF_FIELD_SAMPLES > 1){
			// Find plane that camera aperture resides in
			Vector3D v1 = cross(view, Vector3D(1, 0, 1));
			v1.normalize();
			Vector3D v2 = cross(v1, view);
			v2.normalize();

			double dr = static_cast <double> (raytracerRand(thread_id)) / static_cast<double>(RAND_MAX);
			double dtheta = static_cast <double> (raytracerRand(thread_id)) / static_cast<double>(RAND_MAX);	
			dr = dr * apertureRadius;
			dtheta = (dtheta * 2 * M_PI) - M_PI;
			
			Point3D newEye = eye + (cos(dtheta) * dr * v1) + (sin(dtheta) * dr * v2);
			// Move eye in an area perpendicular to the viewing axis to do depth of field
			Vector3D focusView = focusPoint - newEye;
			focusView.normalize();

			viewToWorld = initInvViewMatrix(newEye, focusView, up);
		}else{
			viewToWorld = initInvViewMatrix(eye, view, up);
		}
	  	// Construct multiple rays for each pixel.
	    for (int i = row_start; i < row_end; i++) {
	        for (int j = 0; j < _scrWidth; j++) {

	        	renderHelper(thread_id, factor, viewToWorld, _scrWidth, _scrHeight, i, j);
			}
			std::cout << "thread_id: " << thread_id << " " << ((double) i - row_start)/(row_end - row_start) * 100<< std::endl;
		}
	}
}

void Raytracer::renderHelper(int thread_id, double factor, Matrix4x4 viewToWorld, int width, int height, int i, int j){

    // Sets up ray origin and direction in view space, 
    // image plane is at z = -1.
    Point3D origin(0., 0., 0.);

    if(ENABLE_ANTI_ALIASING){
    	double _rbuffertmp = 0;
    	double _gbuffertmp = 0;
    	double _bbuffertmp = 0;
    	
        for(int u = 0; u < NUM_ANTIALIASING_SAMPLES; u++){
        		double dx = static_cast <double> (raytracerRand(thread_id)) / static_cast<double>(RAND_MAX);
        		double dy = static_cast <double> (raytracerRand(thread_id)) / static_cast<double>(RAND_MAX);	
				Point3D imagePlane;
				imagePlane[0] = (-double(width)/2 + dx + j)/factor;
				imagePlane[1] = (-double(height)/2 + dy + i)/factor;
				imagePlane[2] = -1;

				// TODO: Convert ray to world space and call 
				// shadeRay(ray) to generate pixel colour. 	
				
				Ray3D ray(viewToWorld * origin, viewToWorld * Vector3D(imagePlane[0], imagePlane[1], imagePlane[2]));
				ray.dir.normalize();

				Colour col = shadeRay(thread_id, ray); 

				// prevent buffer overflow
				_rbuffertmp += col[0];
				_gbuffertmp += col[1];
				_bbuffertmp += col[2];
        }

		_rbuffer[i*width+j] += std::min((int) (255./(NUM_ANTIALIASING_SAMPLES * NUM_DEPTH_OF_FIELD_SAMPLES) * _rbuffertmp), 255);
		_gbuffer[i*width+j] += std::min((int) (255./(NUM_ANTIALIASING_SAMPLES * NUM_DEPTH_OF_FIELD_SAMPLES) * _gbuffertmp), 255);
		_bbuffer[i*width+j] += std::min((int) (255./(NUM_ANTIALIASING_SAMPLES * NUM_DEPTH_OF_FIELD_SAMPLES) * _bbuffertmp), 255);

    }else{
        Point3D imagePlane;
		imagePlane[0] = (-double(width)/2 + 0.5 + j)/factor;
		imagePlane[1] = (-double(height)/2 + 0.5 + i)/factor;
		imagePlane[2] = -1;

		Ray3D ray(viewToWorld * origin, viewToWorld * Vector3D(imagePlane[0], imagePlane[1], imagePlane[2]));
		ray.dir.normalize();

		Colour col = shadeRay(0, ray); 

		_rbuffer[i*width+j] += int(col[0]*255);
		_gbuffer[i*width+j] += int(col[1]*255);
		_bbuffer[i*width+j] += int(col[2]*255);
	}	
}