/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		   light source classes

***********************************************************/

#include "util.h"

// Base class for a light source.  You could define different types
// of lights here, but point light is sufficient for most scenes you
// might want to render.  Different light sources shade the ray 
// differently.
class LightSource {
public:
    using Ptr = std::shared_ptr<LightSource>;
	virtual void shade( Ray3D& ) = 0;
	virtual Point3D get_position() const = 0; 
    virtual ~LightSource() {}
	double area; // Area for calculating soft shadows
};

// A point light is defined by its position in world space and its
// colour.
class PointLight : public LightSource {
public:
	PointLight( Point3D pos, Colour col ) : _pos(pos), _col_ambient(col), 
	_col_diffuse(col), _col_specular(col) {
		area = 0.5;
	}
	PointLight( Point3D pos, Colour ambient, Colour diffuse, Colour specular ) 
	: _pos(pos), _col_ambient(ambient), _col_diffuse(diffuse), 
	_col_specular(specular) {
		area = 0.5;
	}
	void shade( Ray3D& ray );
	Point3D get_position() const { return _pos; }
	
private:
	Point3D _pos;
	Colour _col_ambient;
	Colour _col_diffuse; 
	Colour _col_specular; 
};

// Directional light source
class SpotLight : public LightSource {
public:
	SpotLight( Point3D pos, Vector3D dir, Colour col, double dropoff) : _pos(pos), _dir(dir), _col_ambient(col), 
	_col_diffuse(col), _col_specular(col), _dropoff(dropoff) {
		_dir.normalize();
		area = 0.5;
	}
	SpotLight( Point3D pos, Vector3D dir, Colour ambient, Colour diffuse, Colour specular , double dropoff) 
	: _pos(pos), _dir(dir), _col_ambient(ambient), _col_diffuse(diffuse), 
	_col_specular(specular), _dropoff(dropoff) {
		_dir.normalize();
		area = 0.5;		
	}
	void shade( Ray3D& ray );
	Point3D get_position() const { return _pos; }
	Vector3D get_dir() const {return _dir;}

private:
	Point3D _pos;
	Vector3D _dir;
	Colour _col_ambient;
	Colour _col_diffuse; 
	Colour _col_specular; 
	double _dropoff;
};