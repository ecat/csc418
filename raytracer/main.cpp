/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

        Implementations the main function which 
        specifies the scene to be rendered.	

***********************************************************/
#include "raytracer.h"
#include <ctime>
#include <cstdlib>        
int main(int argc, char* argv[])
{	
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	//int width = 1920; 
	//int height = 1080; 
	int width = 480; 
	int height = 360; 

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	// Seed random  number generator for stochastic raytracing
	srand (static_cast <unsigned> (time(0)));
	const clock_t begin_time = std::clock();

	// Camera parameters.
	Point3D eye(0., 0., 1.);
	Vector3D view(0., 0., -1.);
	//Point3D eye(5., 5., 0.1);
	//Vector3D view(-1., -1., 0.);
	Vector3D up(0., 1., 0.);
	double fov = 120;

	// Defines a material for shading.
    Material::Ptr gold = std::make_shared<Material>( Colour(0.3, 0.3, 0.3), Colour(0.75164, 0.60648, 0.22648), 
			Colour(0.628281, 0.555802, 0.366065), 
			51.2 );
    Material::Ptr jade = std::make_shared<Material>( Colour(0, 0, 0), Colour(0.54, 0.89, 0.63), 
			Colour(0.316228, 0.316228, 0.316228), 
			12.8 );
    // Highly specular material
    Material::Ptr mirror = std::make_shared<Material>( Colour(0.05, 0.05, 0.05), Colour(0.0, 0.0, 0.0), 
			Colour(0.9, 0.9, 0.9), 
			100.8 );
    // Transparent material
    Material::Ptr glass = std::make_shared<Material> ( Colour(0.0, 0.0, 0.0), Colour(0.0, 0.0, 0.0),
    		Colour(0.0, 0.0, 0.0),
    		100.8,
    		1.5);
    Material::Ptr blue = std::make_shared<Material> ( Colour(0.0, 0.0, 0.2), Colour(0.0, 0.0, 0.4),
    		Colour(0.3, 0.3, 0.3), 68);
    Material::Ptr red = std::make_shared<Material>  ( Colour(0.2, 0.0, 0.0), Colour(0.4, 0.0, 0.0),
    		Colour(0.3, 0.3, 0.3), 68);
    Material::Ptr green = std::make_shared<Material>  ( Colour(0.0, 0.2, 0.0), Colour(0.54, 0.89, 0.63),
    		Colour(0.0, 0.0, 0.0), 68);
    
	// Defines a point light source.
	raytracer.addLightSource( std::make_shared<PointLight>(Point3D(0., 0., 5.), 
				Colour(0.9, 0.9, 0.9) ) );

	// Add a unit square into the scene with material mat.
    SceneDagNode::Ptr sphere = raytracer.addObject( std::make_shared<UnitSphere>(), glass );
    SceneDagNode::Ptr sphere2 = raytracer.addObject( std::make_shared<UnitSphere>(), blue );
    sphere2->obj->setTextureColour("earth2.bmp");    
	SceneDagNode::Ptr sphere3 = raytracer.addObject( std::make_shared<UnitSphere>(), red );    
    SceneDagNode::Ptr leftPlane = raytracer.addObject( std::make_shared<UnitSquare>(), jade );
    leftPlane->obj->setTextureGrayScale("jade_tex.bmp");    
    SceneDagNode::Ptr backPlane = raytracer.addObject( std::make_shared<UnitSquare>(), jade );    
	backPlane->obj->setTextureGrayScale("jade_tex.bmp");    
    SceneDagNode::Ptr rightPlane = raytracer.addObject( std::make_shared<UnitSquare>(), jade );
    rightPlane->obj->setTextureGrayScale("jade_tex.bmp");    	
    SceneDagNode::Ptr bottomPlane = raytracer.addObject( std::make_shared<UnitSquare>(), jade );
    bottomPlane->obj->setTextureGrayScale("jade_tex.bmp");      
    /*SceneDagNode::Ptr earthSphere = raytracer.addObject( std::make_shared<UnitSphere>(), blue);    
    earthSphere->obj->setTextureColour("earth2.bmp");
*/

	
	// Apply some transformations to the unit square.
	double factor1[3] = { 2.0, 2.0, 2.0 };
	double factor2[3] = { 4.0, 4.0, 4.0 };
	double factor3[3] = { 20.0, 20.0, 1.0};
	double factor4[3] = { 10.0, 20., 20.};
	double factor5[3] = { 20.0, 1.0, 20.0};
	double earthFactor[3] = {7.5, 7.5, 7.5};

	raytracer.translate(sphere, Vector3D(0., 0., -3.));	
	//raytracer.rotate(sphere, 'x', -45); 
	//raytracer.rotate(sphere, 'z', 45); 
	raytracer.scale(sphere, Point3D(0., 0., 0.), factor1);

	raytracer.translate(sphere2, Vector3D(0., -2., -10));
	raytracer.rotate(sphere2, 'x', 90);
	raytracer.rotate(sphere2, 'y', 90);
	raytracer.rotate(sphere2, 'z', 90);
	raytracer.scale(sphere2, Point3D(0., 0., 0.), factor2);

	raytracer.translate(sphere3, Vector3D(0., 2., -10));
	raytracer.scale(sphere3, Point3D(0., 0., 0.), factor2);	

	raytracer.translate(backPlane, Vector3D(0, 0, -16));
	raytracer.scale(backPlane, Point3D(0, 0,  0), factor3);

	raytracer.translate(leftPlane, Vector3D(-10, 0., -6));	
	raytracer.scale(leftPlane, Point3D(0., 0., 0.), factor4);
	raytracer.rotate(leftPlane, 'y', 90); 

	raytracer.translate(rightPlane, Vector3D(10, 0., -6));	
	raytracer.scale(rightPlane, Point3D(0., 0., 0.), factor4);
	raytracer.rotate(rightPlane, 'y', -90); 	

	raytracer.translate(bottomPlane, Vector3D(0, -10, -6));	
	raytracer.scale(bottomPlane, Point3D(0., 0., 0.), factor5);
	raytracer.rotate(bottomPlane, 'x', -90); 	
/*
	raytracer.translate(earthSphere, Vector3D(0., 0., -8));
	raytracer.rotate(earthSphere, 'y',60); // Change this angle to rotate globe
	// the following rotations make the earth sphere appear right side up
	raytracer.rotate(earthSphere, 'x', 90);
	raytracer.rotate(earthSphere, 'y', 90);
	raytracer.rotate(earthSphere, 'z', 90);
	raytracer.scale(earthSphere, Point3D(0., 0., 0.), earthFactor);
*/

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	raytracer.render(width, height, eye, view, up, fov, "view1.bmp");
	
	// Render it from a different point of view.
	Point3D eye2(6., 0., 3.);
	Vector3D view2(-6., -0., -4.);
	raytracer.render(width, height, eye2, view2, up, fov, "view2.bmp");
	
	std::cout << "Elapsed time: " << float(std::clock() - begin_time)/CLOCKS_PER_SEC << std::endl;

	return 0;
}

