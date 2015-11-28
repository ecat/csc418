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
	
    void setTextureGrayScale(std::string _texturePath){
        isColourTexture = false;
        setTexture(_texturePath);
    };

    // Doing this overwrites the ambient colour properties of the object
    void setTextureColour(std::string _texturePath){
        isColourTexture = true;        
        setTexture(_texturePath);
    };

    virtual void setTextureColour(std::string _texturePath, int face) = 0;

    virtual ~SceneObject() {}

    // Variables for managing textures
    std::string texturePath;
    bool isColourTexture;    
    long unsigned int width;
    long int height;

    unsigned char* _rbuffer;
    unsigned char* _gbuffer;
    unsigned char* _bbuffer;    

protected:
    void setTexture(std::string _texturePath){
        texturePath = _texturePath;   
        //bmp_read_test(texturePath.c_str());
        bmp_read(texturePath.c_str(), &width, &height, &_rbuffer, &_gbuffer, &_bbuffer);
    };

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

        // There is a strange bug with reading in a bmp. It seems like the width wraps
        // around (this is evident if you read a texture and immediately write to it)
        // widths and heights with multiples of four does not fix it
        // As such, crop roughly 5% of the width
        j = (j + 0.05 * width) * (width-1)/(1.05 * width);

    	double r = _rbuffer[i * width + j]/255.;
    	double g = _gbuffer[i * width + j]/255.;
    	double b = _bbuffer[i * width + j]/255.;
        if(isColourTexture){
            return Colour(g, b, r);
        }else{
            return Colour(r, g, b);
        }
    };

    void setTextureColour(std::string _texturePath, int face){
        std::cerr << "This function is for unit cube only " << std::endl;
    }
};


class UnitCube : public SceneObject {
public:
    bool intersect( Ray3D& ray, const Matrix4x4& worldToModel,
            const Matrix4x4& modelToWorld );

    UnitCube(){
        _crbuffer = new unsigned char *[6];
        _cbbuffer = new unsigned char *[6];            
        _cgbuffer = new unsigned char *[6];  
        for(int i = 0 ; i < 6; i++){
            faceSet[i] = false;
            flipHorizontal[i] = 1;
            flipVertical[i] = 1;
        }          
    }

    void setTextureColour(std::string _texturePath, int _face){
        ctexturePath[_face] = _texturePath;   
        bmp_read(ctexturePath[_face].c_str(), &cwidth[_face], &cheight[_face], &_crbuffer[_face], &_cgbuffer[_face], &_cbbuffer[_face]);
        faceSet[_face] = true;    
        isColourTexture = true;
    }

    void flipTexHorizontal(int face){
        flipHorizontal[face] *= -1;
    }

    void flipTexVertical(int face){
        flipVertical[face] *= -1;
    }

    Colour getTextureValue(double dx, double dy){
        std::cerr << "Use getTextureValue and specify a face" << std::endl;
        return Colour(0., 0., 0.);
    };

    // Accesses the texture memory and returns the colour associated with that location
    Colour getTextureValue(double dx, double dy, int face){
        int h = cheight[face];
        int w = cwidth[face];

        int i = (flipHorizontal[face] * dx + 0.5) * h;
        int j = (flipVertical[face] * dy + 0.5) * w;

        // There is a strange bug with reading in a bmp. It seems like the width wraps
        // around (this is evident if you read a texture and immediately write to it)
        // widths and heights with multiples of four does not fix it
        // As such, crop roughly 5% of the width
        j = (j + 0.1 * w) * (w-1)/(1.1 * w);

        double r = _crbuffer[face][i * w + j]/255.;
        double g = _cgbuffer[face][i * w + j]/255.;
        double b = _cbbuffer[face][i * w + j]/255.;

        if(isColourTexture){
            return Colour(g, b, r);
        }else{
            return Colour(r, g, b);
        }
    };

    bool faceSet[6];
    long unsigned int cwidth[6];
    long int cheight[6];
    int flipHorizontal[6];
    int flipVertical[6];
    std::string ctexturePath[6];
    unsigned char** _crbuffer = nullptr;
    unsigned char** _cgbuffer = nullptr;
    unsigned char** _cbbuffer = nullptr;
};


class UnitSphere : public SceneObject {
public:
	bool intersect( Ray3D& ray, const Matrix4x4& worldToModel,
			const Matrix4x4& modelToWorld );


    // Accesses the texture memory and returns the colour associated with that location
    Colour getTextureValue(double u, double v){
        // u, v is scaled between 0 and 1

        int i = (u) * width;
        int j = (v) * height;    

        double r = _rbuffer[j * width + i]/255.;
        double g = _gbuffer[j * width + i]/255.;
        double b = _bbuffer[j * width + i]/255.;
        if(isColourTexture){
            // For some weird reason, the bgr  channels are flipped when reading from texture
            // colour. Verified this by loading a red and got purely green
            return Colour(g, b, r);
        }else{
            return Colour(r, g, b);
        }
    };

    void setTextureColour(std::string _texturePath, int face){
        std::cerr << "This function is for unit cube only " << std::endl;
    }
};

