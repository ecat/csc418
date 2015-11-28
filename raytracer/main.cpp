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
	std::time_t start;
	time(&start);

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
    // Tinted material
    Material::Ptr tintedglass = std::make_shared<Material> ( Colour(0.5, 0.2, 0.2), Colour(0.0, 0.0, 0.0),
    		Colour(0.0, 0.0, 0.0),
    		100.8,
    		1.5);
    tintedglass->transparency = 0.8;

    Material::Ptr blue = std::make_shared<Material> ( Colour(0.0, 0.0, 0.2), Colour(0.0, 0.0, 0.4),
    		Colour(0.25, 0.25, 0.25), 68);
    Material::Ptr red = std::make_shared<Material>  ( Colour(0.2, 0.0, 0.0), Colour(0.4, 0.0, 0.0),
    		Colour(0.25, 0.25, 0.25), 68);
    Material::Ptr green = std::make_shared<Material>  ( Colour(0.0, 0.2, 0.0), Colour(0.54, 0.89, 0.63),
    		Colour(0.0, 0.0, 0.0), 68);
    Material::Ptr yellow = std::make_shared<Material>  ( Colour(0.8, 0.8, 0.5), Colour(0.0, 0.4, 0.4),
    		Colour(0.25, 0.25, 0.25), 68);    
    Material::Ptr white = std::make_shared<Material> (Colour (0.9, 0.9, 0.9), Colour(0,0,0),
    		Colour(0,0,0), 68);
    

	// All planets scene

	eye = Point3D(-3, 0, -3);
	Point3D viewPoint(5, 0, -8);
	up = Vector3D(0, 0, 1);
	view = viewPoint - eye;
	//eye = Point3D(0, 0, 5);
    //fov = 110;

    Material::Ptr wood = std::make_shared<Material> (Colour (0.9, 0.9, 0.9), Colour(0,0,0),
		Colour(0.05 ,0.05 ,0.05), 68);
    Material::Ptr pool_green = std::make_shared<Material>  ( Colour(0.2, 0.5, 0.0), Colour(0., 0., 0.),
    		Colour(0.0, 0.0, 0.0), 68);
   	Material::Ptr planet = std::make_shared<Material> ( Colour(0.15, 0.15, 0.2), Colour(0.0, 0.0, 0.4),
    		Colour(0.25, 0.25, 0.25), 68);

	raytracer.addLightSource( std::make_shared<PointLight>(Point3D(0.0, 8.0, 5.0), 
					Colour(0.1, 0.1, 0.1) ) );	
	SceneDagNode::Ptr earthSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    earthSphere->obj->setTextureColour("earth2.bmp");
	SceneDagNode::Ptr venusSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    venusSphere->obj->setTextureColour("venus.bmp");
	SceneDagNode::Ptr mercurySphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    mercurySphere->obj->setTextureColour("mercury.bmp");    
	SceneDagNode::Ptr marsSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    marsSphere->obj->setTextureColour("mars.bmp");        
	SceneDagNode::Ptr jupiterSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    jupiterSphere->obj->setTextureColour("jupiter.bmp");   
	SceneDagNode::Ptr saturnSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    saturnSphere->obj->setTextureColour("saturn.bmp"); 
	SceneDagNode::Ptr uranusSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    uranusSphere->obj->setTextureColour("uranus.bmp"); 
	SceneDagNode::Ptr neptuneSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    neptuneSphere->obj->setTextureColour("neptune.bmp"); 
	SceneDagNode::Ptr plutoSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    plutoSphere->obj->setTextureColour("pluto.bmp"); 
	SceneDagNode::Ptr sunSphere = raytracer.addObject( std::make_shared<UnitSphere>(), planet);    
    sunSphere->obj->setTextureColour("sun.bmp"); 

	SceneDagNode::Ptr bottomBorder = raytracer.addObject( std::make_shared<UnitCube>(), wood);    
	SceneDagNode::Ptr topBorder = raytracer.addObject( std::make_shared<UnitCube>(), wood);    
	SceneDagNode::Ptr rightBorder = raytracer.addObject( std::make_shared<UnitCube>(), wood);    
	SceneDagNode::Ptr leftBorder = raytracer.addObject( std::make_shared<UnitCube>(), wood);    

	for(int i = 0 ; i < 6; i++){
		bottomBorder->obj->setTextureColour("wood1.bmp", i);
		topBorder->obj->setTextureColour("wood1.bmp", i);		
		rightBorder->obj->setTextureColour("wood1.bmp", i);
		leftBorder->obj->setTextureColour("wood1.bmp", i);
	}

	double horizontalBorderFactor[3] = {40.0, 2.0, 1.0};
	double verticalBorderFactor[3] = {2.0, 40.0, 1.0};
	double factor1[3] = { 2.0, 2.0, 0.3 };
	double factor3[3] = { 40.0, 40.0, 1.0};
	double factor5[3] = { 20.0, 1.0, 20.0};    
	double earthFactor[3] = {2.5, 2.5, 2.5};

	Point3D focusPoint(0.5, 0.5, -9);
	Point3D focusPoint2(0.5, 0.5, -9);	
	Point3D lightOrigin(-20.0, 5.0, 5.0);
	Point3D lightOrigin3(-20.0, -5.0, 5.0);
	double rotationAngle = -10; // Have triangle poitn in another direction
	raytracer.addLightSource( std::make_shared<SpotLight>(lightOrigin,  focusPoint - lightOrigin,
					Colour(0.55, 0.55, 0.55), 13 ) );	
	raytracer.addLightSource( std::make_shared<SpotLight>(lightOrigin3,  focusPoint2 - lightOrigin3,
					Colour(0.55, 0.55, 0.55), 13 ) );			

    SceneDagNode::Ptr backPlane = raytracer.addObject( std::make_shared<UnitSquare>(), pool_green );    
	backPlane->obj->setTextureGrayScale("felt.bmp"); 

	raytracer.translate(bottomBorder, Vector3D(0, -20, -9));
	raytracer.scale(bottomBorder, Point3D(0, 0,  0), horizontalBorderFactor);

	raytracer.translate(topBorder, Vector3D(0, 20, -9));
	raytracer.scale(topBorder, Point3D(0, 0,  0), horizontalBorderFactor);
	
	raytracer.translate(rightBorder, Vector3D(20, 0, -9));
	raytracer.scale(rightBorder, Point3D(0, 0,  0), verticalBorderFactor);

	raytracer.translate(leftBorder, Vector3D(-20, 0, -9));
	raytracer.scale(leftBorder, Point3D(0, 0,  0), verticalBorderFactor);

	raytracer.translate(backPlane, Vector3D(0, 0, -10));
	raytracer.scale(backPlane, Point3D(0, 0,  0), factor3);

	raytracer.rotate(mercurySphere, 'z', -rotationAngle);
	raytracer.translate(mercurySphere, Vector3D(0., 0., -8));
	raytracer.scale(mercurySphere, Point3D(0., 0., 0.), earthFactor);

	raytracer.rotate(venusSphere, 'z', -rotationAngle);
	raytracer.translate(venusSphere, Vector3D(2.5, 0., -8));
	raytracer.scale(venusSphere, Point3D(0., 0., 0.), earthFactor);

	raytracer.rotate(earthSphere, 'z', -rotationAngle);
	raytracer.translate(earthSphere, Vector3D(5., 0., -8));
	raytracer.rotate(earthSphere, 'y',60); // Change this angle to rotate globe
	// the following rotations make the earth sphere appear right side up
	raytracer.rotate(earthSphere, 'x', 90);
	raytracer.rotate(earthSphere, 'y', 90);
	raytracer.rotate(earthSphere, 'z', 90);
	raytracer.scale(earthSphere, Point3D(0., 0., 0.), earthFactor);
	
	raytracer.rotate(marsSphere, 'z', -rotationAngle);
	raytracer.translate(marsSphere, Vector3D(7.5, 0, -8));
	raytracer.scale(marsSphere, Point3D(0., 0., 0.), earthFactor);		
	
	raytracer.rotate(jupiterSphere, 'z', -rotationAngle);
	raytracer.translate(jupiterSphere, Vector3D(1.25, 2.5, -8));
	raytracer.scale(jupiterSphere, Point3D(0., 0., 0.), earthFactor);	
	
	raytracer.rotate(saturnSphere, 'z', -rotationAngle);
	raytracer.translate(saturnSphere, Vector3D(3.75, 2.5, -8));
	raytracer.scale(saturnSphere, Point3D(0., 0., 0.), earthFactor);	

	raytracer.rotate(uranusSphere, 'z', -rotationAngle);
	raytracer.translate(uranusSphere, Vector3D(6.25, 2.5, -8));
	raytracer.scale(uranusSphere, Point3D(0., 0., 0.), earthFactor);	

	raytracer.rotate(neptuneSphere, 'z', -rotationAngle);
	raytracer.translate(neptuneSphere, Vector3D(2.5, 5., -8));
	raytracer.scale(neptuneSphere, Point3D(0., 0., 0.), earthFactor);	

	raytracer.rotate(plutoSphere, 'z', -rotationAngle);
	raytracer.translate(plutoSphere, Vector3D(5., 5., -8));
	raytracer.scale(plutoSphere, Point3D(0., 0., 0.), earthFactor);	

	raytracer.rotate(sunSphere, 'z', -rotationAngle);
	raytracer.translate(sunSphere, Vector3D(3.75, 7.5, -8));
	raytracer.scale(sunSphere, Point3D(0., 0., 0.), earthFactor);	

	raytracer.render(width, height, eye, view, up, fov, "view1.bmp");
	

/*	// White back wall with a textured cube
	raytracer.addLightSource( std::make_shared<PointLight>(Point3D(0.0, 0.0, 5.0), 
					Colour(0.9, 0.9, 0.9) ) );	
	double factor1[3] = { 2.0, 2.0, 0.3 };
	double factor3[3] = { 20.0, 20.0, 1.0};
	double factor5[3] = { 20.0, 1.0, 20.0};
    SceneDagNode::Ptr backPlane = raytracer.addObject( std::make_shared<UnitSquare>(), white);    
	raytracer.translate(backPlane, Vector3D(0, 0, -16));
	raytracer.scale(backPlane, Point3D(0, 0,  0), factor3);

    SceneDagNode::Ptr cube = raytracer.addObject( std::make_shared<UnitCube>(), tintedglass );    
	raytracer.translate(cube, Vector3D(0., 0., -3.));
	raytracer.scale(cube, Point3D(0., 0., 0.), factor1);
	cube->obj->setTextureColour("earth2.bmp", 0);
*/
/*
	// Jade and glass and two spheres

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
	
	// Apply some transformations to the unit square.
	double factor1[3] = { 2.0, 2.0, 2.0 };
	double factor2[3] = { 4.0, 4.0, 4.0 };
	double factor3[3] = { 20.0, 20.0, 1.0};
	double factor4[3] = { 10.0, 20., 20.};
	double factor5[3] = { 20.0, 1.0, 20.0};

	raytracer.translate(sphere, Vector3D(0., 0., -3.));	
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
*/
/*
	// Trio of spheres with semi transparent glass and textured cube
	fov = 105;

    Material::Ptr wood = std::make_shared<Material> (Colour (0.9, 0.9, 0.9), Colour(0,0,0),
		Colour(0.05 ,0.05 ,0.05), 68);
    Material::Ptr tintedglass2 = std::make_shared<Material> ( Colour(0.5, 0.5, 0.2), Colour(0.0, 0.0, 0.0),
    		Colour(0.0, 0.0, 0.0),
    		100.8,
    		1.01);
    tintedglass2->transparency = 0.3;
	raytracer.addLightSource( std::make_shared<PointLight>(Point3D(0.0, 8.0, 5.0), 
					Colour(0.1, 0.1, 0.05) ) );
	Point3D lightOrigin(-10.0, 5.0, 5.0);
	Point3D lightOrigin2(-27.2, 9.80, -11.7);
	Point3D lightOrigin3(20, 5.0, -10);
	Point3D focusPoint(-12, -7, -9);
	Point3D focusPoint2(-1.5, -7.9, -11.9);
	raytracer.addLightSource( std::make_shared<SpotLight>(lightOrigin,  focusPoint - lightOrigin,
					Colour(0.6, 0.6, 0.6), 10 ) );	
	raytracer.addLightSource( std::make_shared<SpotLight>(lightOrigin2,  focusPoint - lightOrigin2,
					Colour(0.45, 0.45, 0.45), 14 ) );		
	raytracer.addLightSource( std::make_shared<SpotLight>(lightOrigin3,  focusPoint2 - lightOrigin3,
					Colour(0.25, 0.25, 0.25), 18 ) );		

    SceneDagNode::Ptr sphere = raytracer.addObject( std::make_shared<UnitSphere>(), yellow );
    SceneDagNode::Ptr sphere2 = raytracer.addObject( std::make_shared<UnitSphere>(), blue );
    sphere2->obj->setTextureColour("earth2.bmp");    
	SceneDagNode::Ptr sphere3 = raytracer.addObject( std::make_shared<UnitSphere>(), red );    
	SceneDagNode::Ptr sphere4 = raytracer.addObject( std::make_shared<UnitSphere>(), blue );  
	SceneDagNode::Ptr cube = raytracer.addObject( std::make_shared<UnitCube>(), tintedglass2 ); 
    cube->obj->setTextureColour("Starry_Night.bmp", 0);	// front face
    cube->obj->setTextureColour("Starry_Night.bmp", 5);    // back face
    (std::static_pointer_cast<UnitCube>(cube->obj))->flipTexHorizontal(5);
    SceneDagNode::Ptr cube2 = raytracer.addObject( std::make_shared<UnitCube>(), white); 
    //cube2->obj->setTextureColour("Starry_Night.bmp", 1); // Left face
	cube2->obj->setTextureColour("bob_ross.bmp", 4); // Top face
	cube2->obj->setTextureColour("three_musicians.bmp", 2); //Right face
	cube2->obj->setTextureColour("rocky.bmp", 0);
     	
    SceneDagNode::Ptr leftPlane = raytracer.addObject( std::make_shared<UnitSquare>(), wood );
    SceneDagNode::Ptr backPlane = raytracer.addObject( std::make_shared<UnitSquare>(), white ); 
    SceneDagNode::Ptr frontPlane = raytracer.addObject( std::make_shared<UnitSquare>(), white );     
    SceneDagNode::Ptr rightPlane = raytracer.addObject( std::make_shared<UnitSquare>(), wood );
    SceneDagNode::Ptr bottomPlane = raytracer.addObject( std::make_shared<UnitSquare>(), wood );
    SceneDagNode::Ptr topPlane  = raytracer.addObject( std::make_shared<UnitSquare>(), wood );    
	double factor1[3] = { 5., 4.0, 0.03 };
	double factor2[3] = { 5.0, 5.0, 5.0 };
	double factor3[3] = { 80.0, 20.0, 1.0};
	double factor4[3] = { 1.0, 20., 80.};
	double factor5[3] = { 80.0, 1.0, 80.0};
	double factor6[3] = { 4.0, 4.0, 4.0 };

	raytracer.translate(cube2, Vector3D(-1.5, -7.9, -11.9));
	raytracer.rotate(cube2, 'y', -30);
	raytracer.scale(cube2, Point3D(0., 0., 0.), factor6);

	raytracer.translate(cube, Vector3D(-5.5, -8., -3.9));
	raytracer.rotate(cube, 'y', 10); 
	raytracer.scale(cube, Point3D(0., 0., 0.), factor1);

	raytracer.translate(sphere, Vector3D(-11., -8., -4.5));	
	raytracer.scale(sphere, Point3D(0., 0., 0.), factor2);

	raytracer.translate(sphere4, Vector3D(-12., -5., -8.));	
	raytracer.scale(sphere4, Point3D(0., 0., 0.), factor2);	

	raytracer.translate(sphere2, Vector3D(-13., -8., -9.));
	raytracer.rotate(sphere2, 'x', 90);
	raytracer.rotate(sphere2, 'y', 90);
	raytracer.rotate(sphere2, 'z', 90);
	raytracer.scale(sphere2, Point3D(0., 0., 0.), factor2);

	raytracer.translate(sphere3, Vector3D(-9., -8., -9.));
	raytracer.scale(sphere3, Point3D(0., 0., 0.), factor2);	

	raytracer.translate(backPlane, Vector3D(0, 0, -20));
	raytracer.scale(backPlane, Point3D(0, 0,  0), factor3);
	backPlane->obj->setTextureColour("bricks3.bmp");

	raytracer.translate(frontPlane, Vector3D(0, 0, 5.2));
	raytracer.scale(frontPlane, Point3D(0, 0,  0), factor3);
	raytracer.rotate(frontPlane, 'y', 180);
	frontPlane->obj->setTextureColour("wood1.bmp");

	raytracer.translate(leftPlane, Vector3D(-40, 0., 0));	
	raytracer.scale(leftPlane, Point3D(0., 0., 0.), factor4);
	raytracer.rotate(leftPlane, 'y', 90); 
	leftPlane->obj->setTextureColour("wood1.bmp");

	raytracer.translate(rightPlane, Vector3D(40, 0., 0));	
	raytracer.scale(rightPlane, Point3D(0., 0., 0.), factor4);
	raytracer.rotate(rightPlane, 'y', -90); 	
	rightPlane->obj->setTextureColour("wood1.bmp");

	raytracer.translate(bottomPlane, Vector3D(0, -10, 0));	
	raytracer.scale(bottomPlane, Point3D(0., 0., 0.), factor5);
	raytracer.rotate(bottomPlane, 'x', -90); 	
	bottomPlane->obj->setTextureColour("wood1.bmp");

	raytracer.translate(topPlane, Vector3D(0, 10, 0));	
	raytracer.scale(topPlane, Point3D(0., 0., 0.), factor5);
	raytracer.rotate(topPlane, 'x', 90); 	
	topPlane->obj->setTextureColour("wood1.bmp");	
	
	// Render it from a different point of view.
	Point3D eye2(-1.3, -5.3, -3.2);
	Vector3D view2(-4, -3, -4.);
	raytracer.render(width, height, eye2, view2, up, fov, "semiglass.bmp");
*/
/*
	// Single earth sphere
	raytracer.addLightSource( std::make_shared<PointLight>(Point3D(0.0, 8.0, 5.0), 
					Colour(0.5, 0.5, 0.5) ) );	
	SceneDagNode::Ptr earthSphere = raytracer.addObject( std::make_shared<UnitSphere>(), blue);    
    earthSphere->obj->setTextureColour("earth2.bmp");
	double earthFactor[3] = {7.5, 7.5, 7.5};

	raytracer.translate(earthSphere, Vector3D(0., 0., -8));
	raytracer.rotate(earthSphere, 'y',60); // Change this angle to rotate globe
	// the following rotations make the earth sphere appear right side up
	raytracer.rotate(earthSphere, 'x', 90);
	raytracer.rotate(earthSphere, 'y', 90);
	raytracer.rotate(earthSphere, 'z', 90);
	raytracer.scale(earthSphere, Point3D(0., 0., 0.), earthFactor);


	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	raytracer.render(width, height, eye, view, up, fov, "view1.bmp");
	
	// Render it from a different point of view.
	Point3D eye2(2., 0., 1.);
	Vector3D view2(-6., -0., -4.);
	raytracer.render(width, height, eye2, view2, up, fov, "view2.bmp");
*/
	
	std::time_t end;
	time(&end);
	std::cout << "Elapsed time: " << difftime(end, start) << std::endl;

	return 0;
}

