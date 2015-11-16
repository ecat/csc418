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
	int width = 320; 
	int height = 240; 

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	// Seed random  number generator for stochastic raytracing
	srand (static_cast <unsigned> (time(0)));

	// Camera parameters.
	Point3D eye(0., 0., 1.);
	Vector3D view(0., 0., -1.);
	//Point3D eye(5., 5., 0.1);
	//Vector3D view(-1., -1., 0.);
	Vector3D up(0., 1., 0.);
	double fov = 90;

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
    		Colour(0.00, 0.00, 0.00),
    		100.8,
    		1.5);

	// Defines a point light source.
	raytracer.addLightSource( std::make_shared<PointLight>(Point3D(0., 0., 5.), 
				Colour(0.9, 0.9, 0.9) ) );

	// Add a unit square into the scene with material mat.
    SceneDagNode::Ptr sphere = raytracer.addObject( std::make_shared<UnitSphere>(), gold );
    //SceneDagNode::Ptr plane = raytracer.addObject( std::make_shared<UnitSquare>(), mirror );
    //SceneDagNode::Ptr plane2 = raytracer.addObject( std::make_shared<UnitSquare>(), mirror );
    //SceneDagNode::Ptr plane3 = raytracer.addObject( std::make_shared<UnitSquare>(), mirror );
    //SceneDagNode::Ptr plane4 = raytracer.addObject( std::make_shared<UnitSquare>(), jade );
    //SceneDagNode::Ptr glassPieceFwd = raytracer.addObject( std::make_shared<UnitSquare>(), glass );
    //SceneDagNode::Ptr glassPieceBkwd = raytracer.addObject( std::make_shared<UnitSquare>(), glass );    
  	SceneDagNode::Ptr jadePiece = raytracer.addObject( std::make_shared<UnitSquare>(), jade);

	
	// Apply some transformations to the unit square.
	double factor1[3] = { 2.0, 2.0, 2.0 };
	double factor2[3] = { 6.0, 6.0, 6.0 };
	raytracer.translate(sphere, Vector3D(0., 0., -6.));	
	raytracer.rotate(sphere, 'x', -45); 
	raytracer.rotate(sphere, 'z', 45); 
	raytracer.scale(sphere, Point3D(0., 0., 0.), factor1);

	raytracer.translate(jadePiece, Vector3D(0, 0, -12));
	raytracer.scale(jadePiece, Point3D(0, 0,  0), factor2);
	/*
	raytracer.translate(glassPieceFwd, Vector3D(0., -1., -3.));	
	raytracer.scale(glassPieceFwd, Point3D(0., 0., 0.), factor1);

	raytracer.translate(glassPieceBkwd, Vector3D(0., -1., -3.26));	
	raytracer.rotate(glassPieceBkwd, 'y', 180);	
	raytracer.scale(glassPieceBkwd, Point3D(0., 0., 0.), factor1);	

	raytracer.translate(plane, Vector3D(-5., 0., -7.));	
	raytracer.rotate(plane, 'y', 80); 
	raytracer.scale(plane, Point3D(0., 0., 0.), factor2);

	raytracer.translate(plane2, Vector3D(5, 0, -7));
	raytracer.rotate(plane2, 'y', -80); 
	raytracer.scale(plane2, Point3D(0., 0., 0.), factor2);	

	raytracer.translate(plane3, Vector3D(0, 6, -7));
	raytracer.rotate(plane3, 'x', 90); 
	raytracer.scale(plane3, Point3D(0., 0., 0.), factor2);	

	raytracer.translate(plane4, Vector3D(0, -6, -7));
	raytracer.rotate(plane4, 'x', -90); 
	raytracer.scale(plane4, Point3D(0., 0., 0.), factor2);	
	*/

	//raytracer.translate(backPlane, Vector3D(0, 0, -12));
	//raytracer.scale(backPlane, Point3D(0., 0., 0.), factor2);	

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	raytracer.render(width, height, eye, view, up, fov, "view1.bmp");
	
	// Render it from a different point of view.
	Point3D eye2(4., 2., 1.);
	Vector3D view2(-4., -2., -6.);
	raytracer.render(width, height, eye2, view2, up, fov, "view2.bmp");
	
	return 0;
}

