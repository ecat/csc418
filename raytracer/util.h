/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
            CSC418, SPRING 2005

        utility functions and structures
        (based on code from CGL, University of Waterloo), 
        modify this file as you see fit.

***********************************************************/

#ifndef _UTIL_
#define _UTIL_

#include <iostream>
#include <cmath>
#include <array>
#include <memory>

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

template <typename T, int D>
class Tuple: public std::array<T,D> {
    public:
        using Base = std::array<T,D>;
//        using Base::Base;
        template <typename... Args>
        Tuple(Args... args) : Base({ static_cast<T>(args)... }) {
                static_assert(sizeof...(args) == D,"wrong number of arguments");
            }
        Tuple(){}
};

template <typename T>
using Triplet = Tuple<T,3>;

class Index3: public Triplet<int> {
    public:
        using Base = Triplet<int>;
        template <typename... Args>
            Index3(Args... args): Base(args...) {}
        Index3();
};

class Point3D: public Triplet<double> {
    public:
        using Base = Triplet<double>;
        template <typename... Args>
            Point3D(Args... args): Base(args...) {}
        Point3D();
};

class Vector3D: public Triplet<double> {
    public:
        using Base = Triplet<double>;
        template <typename... Args>
            Vector3D(Args... args): Base(args...) {}
        Vector3D();

    double length() const; 
    double normalize();
    double dot(const Vector3D& other) const; 
    Vector3D cross(const Vector3D& other) const; 
};

// standard operators on points and vectors
Vector3D operator *(double s, const Vector3D& v); 
Vector3D operator +(const Vector3D& u, const Vector3D& v); 
Point3D operator +(const Point3D& u, const Vector3D& v); 
Vector3D operator -(const Point3D& u, const Point3D& v); 
Vector3D operator -(const Vector3D& u, const Vector3D& v); 
Vector3D operator -(const Vector3D& u); 
Point3D operator -(const Point3D& u, const Vector3D& v); 
Vector3D cross(const Vector3D& u, const Vector3D& v); 
std::ostream& operator <<(std::ostream& o, const Point3D& p); 
std::ostream& operator <<(std::ostream& o, const Vector3D& v); 

class Vector4D {
public:
    Vector4D(); 
    Vector4D(double w, double x, double y, double z); 
    Vector4D(const Vector4D& other); 

    Vector4D& operator =(const Vector4D& other); 
    double& operator[](int i);  
    double operator[](int i) const;  

private:
    double m_data[4];
};

class Matrix4x4 {
public:
  Matrix4x4(); 
  Matrix4x4(const Matrix4x4& other); 
  Matrix4x4& operator=(const Matrix4x4& other); 

  Vector4D getRow(int row) const; 
  double *getRow(int row); 
  Vector4D getColumn(int col) const; 

  Vector4D operator[](int row) const; 
  double *operator[](int row); 

  Matrix4x4 transpose() const; 
        
private:
  double m_data[16];
};

Matrix4x4 operator *(const Matrix4x4& M, const Matrix4x4& N); 
Vector3D operator *(const Matrix4x4& M, const Vector3D& v); 
Point3D operator *(const Matrix4x4& M, const Point3D& p);
// Multiply n by the transpose of M, useful for transforming normals.  
// Recall that normals should be transformed by the inverse transpose 
// of the matrix.  
Vector3D transNorm(const Matrix4x4& M, const Vector3D& n); 
std::ostream& operator <<(std::ostream& os, const Matrix4x4& M); 

class Colour {
public:
    Colour(); 
    Colour(double r, double g, double b); 
    Colour(const Colour& other); 

    Colour& operator =(const Colour& other); 
    Colour operator *(const Colour& other); 
    double& operator[](int i);  
    double operator[](int i) const; 
    
    void clamp();   

private:
    double m_data[3];
};

Colour operator *(double s, const Colour& c); 
Colour operator +(const Colour& u, const Colour& v); 
std::ostream& operator <<(std::ostream& o, const Colour& c); 

struct Material {
    using Ptr = std::shared_ptr<Material>;
    Material( Colour ambient, Colour diffuse, Colour specular, double exp ) :
        ambient(ambient), diffuse(diffuse), specular(specular), 
        specular_exp(exp) {
            n = 1.0;
            transparent = false;
            transparency = 0.0;
        }

    Material( Colour ambient, Colour diffuse, Colour specular, double exp, double _n ) :
        ambient(ambient), diffuse(diffuse), specular(specular), 
        specular_exp(exp) {
            n = _n;
            transparent = true;
            transparency = 1.0;
        }

    // Ambient components for Phong shading.
    Colour ambient; 
    // Diffuse components for Phong shading.
    Colour diffuse;
    // Specular components for Phong shading.
    Colour specular;
    // Specular expoent.
    double specular_exp;
    // Refractive index of material
    double n;    
    bool transparent;
    // % of object that is transparent, 1.0 is perfectly transparent
    double transparency;
};

struct Intersection {
    // Location of intersection.
    Point3D point;
    // Normal at the intersection.
    Vector3D normal;
    // Material at the intersection.
    Material::Ptr mat;
    // Position of the intersection point on your ray.
    // (i.e. point = ray.origin + t_value * ray.dir)
    // This is used when you need to intersect multiply objects and
    // only want to keep the nearest intersection.
    double t_value;     
    // Set to true when no intersection has occured.
    bool none;

    // Check whether the intersection intersects an object with texture
    bool hasTexture;
    // Does that intersection have colour or is it grayscale
    bool hasColourTexture; 
    // Texture value at the intersection     
    Colour texValue;
};

// Ray structure. 
struct Ray3D {
    Ray3D() {
        intersection.none = true; 
        num_reflections = 0;     
        refractive_index = 1.0;   
    }
    Ray3D( Point3D p, Vector3D v ) : origin(p), dir(v) {
        intersection.none = true;
        num_reflections = 0;
        refractive_index = 1.0;        
    }

    Ray3D( Point3D p, Vector3D v, int r) : origin(p), dir(v) {
        intersection.none = true;
        num_reflections = r;   
        refractive_index = 1.0;        
    }
    // Origin and direction of the ray.
    Point3D origin;
    Vector3D dir;
    // Intersection status, should be computed by the intersection
    // function.
    Intersection intersection;
    // Current colour of the ray, should be computed by the shading
    // function.
    Colour col;
    // Number of times this ray has been reflected or refracted
    int num_reflections;
    // Refractive index of the medium that the ray's origin is in
    double refractive_index;
};
#endif





