/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		classes defining primitives in the scene

***********************************************************/

#include "util.h"
#include "bmp_io.h"

// All primitives should provide a intersection function.  
// To create more primitives, inherit from SceneObject.
// Namely, you can create, Sphere, Cylinder, etc... classes
// here.
class SceneObject {
public:
	// Returns true if an intersection occured, false otherwise.
    using Ptr = std::shared_ptr<SceneObject>;
	virtual bool intersect( Ray3D&, const Matrix4x4&, const Matrix4x4& ) = 0;
	virtual Colour getTextureValue(double dx, double dy) = 0;
	void setTexture(std::string _texturePath){
        texturePath = _texturePath;
        bmp_read_test(texturePath.c_str());
        bmp_read(texturePath.c_str(), &width, &height, &_rbuffer, &_gbuffer, &_bbuffer);
    };;
    virtual ~SceneObject() {}

    // Variables for managing textures
    std::string texturePath;
    long unsigned int width;
    long int height;

    unsigned char* _rbuffer;
    unsigned char* _gbuffer;
    unsigned char* _bbuffer;    

};

// Example primitive you can create, this is a unit square on 
// the xy-plane.
class UnitSquare : public SceneObject {
public:
	bool intersect( Ray3D& ray, const Matrix4x4& worldToModel,
			const Matrix4x4& modelToWorld );

    // Accesses the texture memory and returns the colour associated with that location
    Colour getTextureValue(double dx, double dy){
    	int i = (dx + 0.5) * height;
    	int j = (dy + 0.5) * width;

    	double r = _rbuffer[i * width + j]/255.;
    	double g = _gbuffer[i * width + j]/255.;
    	double b = _bbuffer[i * width + j]/255.;
    	return Colour(r, g, b);
    };
};

class UnitSphere : public SceneObject {
public:
	bool intersect( Ray3D& ray, const Matrix4x4& worldToModel,
			const Matrix4x4& modelToWorld );


    // Accesses the texture memory and returns the colour associated with that location
    Colour getTextureValue(double dx, double dy){

    	return Colour(0., 0., 0.);
    };
};

