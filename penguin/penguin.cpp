/***********************************************************
             CSC418/2504, Fall 2009
  
                 penguin.cpp
                 
       Simple demo program using OpenGL and the glut/glui 
       libraries

  
    Instructions:
        Please read the assignment page to determine 
        exactly what needs to be implemented.  Then read 
        over this file and become acquainted with its 
        design.

        Add source code where it appears appropriate. In
        particular, see lines marked 'TODO'.

        You should not need to change the overall structure
        of the program. However it should be clear what
        your changes do, and you should use sufficient comments
        to explain your code.  While the point of the assignment
        is to draw and animate the character, you will
        also be marked based on your design.

***********************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef _WIN32
#include <unistd.h>
#else
void usleep(unsigned int nanosec)
{
    Sleep(nanosec / 1000);
}
#endif


// *************** GLOBAL VARIABLES *************************


const float PI = 3.14159;

// --------------- USER INTERFACE VARIABLES -----------------

// Window settings
int windowID;               // Glut window ID (for display)
GLUI *glui;                 // Glui window (for controls)
int Win[2];                 // window (x,y) size


// ---------------- ANIMATION VARIABLES ---------------------

// Animation settings
int animate_mode = 0;       // 0 = no anim, 1 = animate
int animation_frame = 0;      // Specify current frame of animation

// Joint parameters
const float ARM_JOINT_MIN = -45.0f;
const float ARM_JOINT_MAX =  45.0f;
float ARM_JOINT_ROT = 0.0f;


int NUM_CIRCLE_SIDES = 100;

//////////////////////////////////////////////////////
// TODO: Add additional joint parameters here
//////////////////////////////////////////////////////

const float LEFT_LEG_HIP_JOINT_MIN = -35.0f; // Joint that connects body and top half of left leg
const float LEFT_LEG_HIP_JOINT_MAX = 15.0f;
float LEFT_LEG_HIP_JOINT_ROT = -15.0f;

// Params for joint that connects left leg segments
const float LEFT_FOOT_JOINT_MIN = -15.0f; 
const float LEFT_FOOT_JOINT_MAX = 15.0f;
float LEFT_FOOT_JOINT_ROT = 0.0f;

const float RIGHT_LEG_HIP_JOINT_MIN = -25.0f; // Joint that connects body and top half of right leg
const float RIGHT_LEG_HIP_JOINT_MAX = 25.0f;
float RIGHT_LEG_HIP_JOINT_ROT = 15.0f;

// Params for joint that connects right leg segments
const float RIGHT_FOOT_JOINT_MIN = -5.0f; 
const float RIGHT_FOOT_JOINT_MAX = 15.0f;
float RIGHT_FOOT_JOINT_ROT = 0.0f;

// Neck joint
const float HEAD_JOINT_MIN = -5.0f;
const float HEAD_JOINT_MAX = 35.0f;
float HEAD_JOINT_ROT = 0.0f;

const float BEAK_TRANSLATE_MAX = 1.0f;
const float BEAK_TRANSLATE_MIN = 0.0f;
float BEAK_TRANSLATE = 0.0f;

float PENGUIN_TRANSLATE_X = 0.0f;
float PENGUIN_TRANSLATE_Y = 0.0f;

float PENGUIN_TRANSLATE_X_MAX = 1.0f;
float PENGUIN_TRANSLATE_Y_MAX = 1.0f;

// ***********  FUNCTION HEADER DECLARATIONS ****************


// Initialization functions
void initGlut(char* winName);
void initGlui();
void initGl();


// Callbacks for handling events in glut
void myReshape(int w, int h);
void animate();
void display(void);

// Callback for handling events in glui
void GLUI_Control(int id);


// Functions to help draw the object
void drawSquare(float size);
void drawCircle(float radius);
void drawJoint(float radius);
void drawJoint(float radius, bool isWhite);
void drawHead(float width, float height);
void drawBody(float width, float height);
void drawArm(float width, float height);
void drawFoot(float size);
void drawBeakTop(float width, float height);

// Return the current system clock (in seconds)
double getTime();
// Helper function to calculate joint angle
double getJointRotAngle(float max_angle, float min_angle, float rot_speed);


// ******************** FUNCTIONS ************************



// main() function
// Initializes the user interface (and any user variables)
// then hands over control to the event handler, which calls 
// display() whenever the GL window needs to be redrawn.
int main(int argc, char** argv)
{

    // Process program arguments
    if(argc != 3) {
        printf("Usage: demo [width] [height]\n");
        printf("Using 300x200 window by default...\n");
        Win[0] = 300;
        Win[1] = 200;
    } else {
        Win[0] = atoi(argv[1]);
        Win[1] = atoi(argv[2]);
    }


    // Initialize glut, glui, and opengl
    glutInit(&argc, argv);
    initGlut(argv[0]);
    initGlui();
    initGl();

    // Invoke the standard GLUT main event loop
    glutMainLoop();

    return 0;         // never reached
}


// Initialize glut and create a window with the specified caption 
void initGlut(char* winName)
{
    // Set video mode: double-buffered, color, depth-buffered
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create window
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(Win[0],Win[1]);
    windowID = glutCreateWindow(winName);

    // Setup callback functions to handle events
    glutReshapeFunc(myReshape); // Call myReshape whenever window resized
    glutDisplayFunc(display);   // Call display whenever new frame needed 
}


// Quit button handler.  Called when the "quit" button is pressed.
void quitButton(int)
{
  exit(0);
}

// Animate button handler.  Called when the "animate" checkbox is pressed.
void animateButton(int)
{
  // synchronize variables that GLUT uses
  glui->sync_live();

  animation_frame = 0;
  if(animate_mode == 1) {
    // start animation
    GLUI_Master.set_glutIdleFunc(animate);
  } else {
    // stop animation
    GLUI_Master.set_glutIdleFunc(NULL);
  }
}

// Initialize GLUI and the user interface
void initGlui()
{
    GLUI_Master.set_glutIdleFunc(NULL);

    // Create GLUI window
    glui = GLUI_Master.create_glui("Glui Window", 0, Win[0]+10, 0);

    // Create a control to specify the rotation of the joint
    GLUI_Spinner *joint_spinner
        = glui->add_spinner("Arm Joint", GLUI_SPINNER_FLOAT, &ARM_JOINT_ROT);
    joint_spinner->set_speed(0.1);
    joint_spinner->set_float_limits(ARM_JOINT_MIN, ARM_JOINT_MAX, GLUI_LIMIT_CLAMP);

    ///////////////////////////////////////////////////////////
    // TODO: 
    //   Add controls for additional joints here
    ///////////////////////////////////////////////////////////

	joint_spinner = glui->add_spinner("Left Leg Hip Joint", GLUI_SPINNER_FLOAT, &LEFT_LEG_HIP_JOINT_ROT);
	joint_spinner->set_speed(0.6);
	joint_spinner->set_float_limits(LEFT_LEG_HIP_JOINT_MIN, LEFT_LEG_HIP_JOINT_MAX, GLUI_LIMIT_CLAMP);

	joint_spinner = glui->add_spinner("Left Leg Knee Joint", GLUI_SPINNER_FLOAT, &LEFT_FOOT_JOINT_ROT);
	joint_spinner->set_speed(0.6);
	joint_spinner->set_float_limits(LEFT_FOOT_JOINT_MIN, LEFT_FOOT_JOINT_MAX, GLUI_LIMIT_CLAMP);

    joint_spinner = glui->add_spinner("Right Leg Hip Joint", GLUI_SPINNER_FLOAT, &RIGHT_LEG_HIP_JOINT_ROT);
    joint_spinner->set_speed(0.6);
    joint_spinner->set_float_limits(RIGHT_LEG_HIP_JOINT_MIN, RIGHT_LEG_HIP_JOINT_MAX, GLUI_LIMIT_CLAMP);

    joint_spinner = glui->add_spinner("Right Leg Knee Joint", GLUI_SPINNER_FLOAT, &RIGHT_FOOT_JOINT_ROT);
    joint_spinner->set_speed(0.6);
    joint_spinner->set_float_limits(RIGHT_FOOT_JOINT_MIN, RIGHT_FOOT_JOINT_MAX, GLUI_LIMIT_CLAMP);

    joint_spinner = glui->add_spinner("Head Joint", GLUI_SPINNER_FLOAT, &HEAD_JOINT_ROT);
    joint_spinner->set_speed(0.6);
    joint_spinner->set_float_limits(HEAD_JOINT_MIN, HEAD_JOINT_MAX, GLUI_LIMIT_CLAMP);

    joint_spinner = glui->add_spinner("Beak", GLUI_SPINNER_FLOAT, &BEAK_TRANSLATE);
    joint_spinner->set_speed(0.8);
    joint_spinner->set_float_limits(BEAK_TRANSLATE_MIN, BEAK_TRANSLATE_MAX, GLUI_LIMIT_CLAMP);

    glui->add_separator();

    joint_spinner = glui->add_spinner("Translate X", GLUI_SPINNER_FLOAT, &PENGUIN_TRANSLATE_X);
    joint_spinner->set_speed(0.8);
    joint_spinner->set_float_limits(-1.0, 1.0, GLUI_LIMIT_CLAMP);

    joint_spinner = glui->add_spinner("Translate Y", GLUI_SPINNER_FLOAT, &PENGUIN_TRANSLATE_Y);
    joint_spinner->set_speed(0.8);
    joint_spinner->set_float_limits(-1.0, 1.0, GLUI_LIMIT_CLAMP);

    // Add button to specify animation mode 
    glui->add_separator();
    glui->add_checkbox("Animate", &animate_mode, 0, animateButton);

    // Add "Quit" button
    glui->add_separator();
    glui->add_button("Quit", 0, quitButton);

    // Set the main window to be the "active" window
    glui->set_main_gfx_window(windowID);
}


// Performs most of the OpenGL intialization
void initGl(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.7f,0.7f,0.9f,1.0f);
}


double getJointRotAngle(float max_angle, float min_angle, float rot_speed, float phase_shift){

    double animation_time = (sin(animation_frame * rot_speed + phase_shift) + 1.0)/ 2.0; // normalize to 0 < y < 1
    float angle = animation_time * min_angle + (1 - animation_time) * max_angle;        
    return angle;
}

// Callback idle function for animating the scene
void animate()
{
    // Update geometry
    const double ARM_JOINT_ROT_speed = 0.1;
    double ARM_JOINT_ROT_t = (sin(animation_frame*ARM_JOINT_ROT_speed) + 1.0) / 2.0;
    ARM_JOINT_ROT = ARM_JOINT_ROT_t * ARM_JOINT_MIN + (1 - ARM_JOINT_ROT_t) * ARM_JOINT_MAX;
    
    ///////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function animate the character's joints
    //   Note: Nothing should be drawn in this function!  OpenGL drawing
    //   should only happen in the display() callback.
    ///////////////////////////////////////////////////////////

    // Animate left leg at hip joint
    const double LEFT_LEG_HIP_JOINT_ROT_speed = 0.1;
    LEFT_LEG_HIP_JOINT_ROT = getJointRotAngle(LEFT_LEG_HIP_JOINT_MAX, LEFT_LEG_HIP_JOINT_MIN, LEFT_LEG_HIP_JOINT_ROT_speed, 0.0);

    // Animate left foot
    const double LEFT_FOOT_JOINT_ROT_speed = 0.2;
    LEFT_FOOT_JOINT_ROT = getJointRotAngle(LEFT_FOOT_JOINT_MAX, LEFT_FOOT_JOINT_MIN, LEFT_FOOT_JOINT_ROT_speed, 0.0);

    const double HEAD_JOINT_ROT_speed = 0.2;
    HEAD_JOINT_ROT = getJointRotAngle(HEAD_JOINT_MAX, HEAD_JOINT_MIN, HEAD_JOINT_ROT_speed, 0.0);

    const double BEAK_TRANSLATE_speed = 0.3;
    BEAK_TRANSLATE = getJointRotAngle(BEAK_TRANSLATE_MIN, BEAK_TRANSLATE_MAX, BEAK_TRANSLATE_speed, 0.0);

    // animate right leg
    const double RIGHT_LEG_HIP_JOINT_ROT_speed = 0.1;
    RIGHT_LEG_HIP_JOINT_ROT = getJointRotAngle(RIGHT_LEG_HIP_JOINT_MAX, RIGHT_LEG_HIP_JOINT_MIN, RIGHT_LEG_HIP_JOINT_ROT_speed, M_PI);

    // animate right foot
    const double RIGHT_FOOT_JOINT_ROT_speed = 0.1;
    RIGHT_FOOT_JOINT_ROT = getJointRotAngle(RIGHT_FOOT_JOINT_MAX, RIGHT_FOOT_JOINT_MIN, RIGHT_FOOT_JOINT_ROT_speed, M_PI);

    // Update user interface
    glui->sync_live();

    // Tell glut window to update itself.  This will cause the display()
    // callback to be called, which renders the object (once you've written
    // the callback).
    glutSetWindow(windowID);
    glutPostRedisplay();

    // increment the frame number.
    animation_frame++;

    // Wait 50 ms between frames (20 frames per second)
    usleep(50000);
}


// Handles the window being resized by updating the viewport
// and projection matrices
void myReshape(int w, int h)
{
    // Setup projection matrix for new window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w/2, w/2, -h/2, h/2);

    // Ensure that the penguin stays on the screen
    PENGUIN_TRANSLATE_X_MAX = w/2;
    PENGUIN_TRANSLATE_Y_MAX = h/2;

    // Update OpenGL viewport and internal variables
    glViewport(0,0, w,h);
    Win[0] = w;
    Win[1] = h;
}



// display callback
//
// This gets called by the event handler to draw
// the scene, so this is where you need to build
// your scene -- make your changes and additions here.
// All rendering happens in this function.  For Assignment 1,
// updates to geometry should happen in the "animate" function.
void display(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.7f,0.7f,0.9f,1.0f);

    // OK, now clear the screen with the background colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

    // Setup the model-view transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    ///////////////////////////////////////////////////////////
    // TODO:
    //   Modify this function draw the scene
    //   This should include function calls to pieces that
    //   apply the appropriate transformation matrice and
    //   render the individual body parts.
    ///////////////////////////////////////////////////////////

    // Draw our hinged object
    const float BODY_WIDTH = 40.0f;
    const float BODY_LENGTH = 60.0f;
    const float ARM_LENGTH = 25.0f;
    const float ARM_WIDTH = 10.0f;
    const float JOINT_RADIUS = 1.5f;
    const float JOINT_PADDING = 1.0f;
    const float HEAD_WIDTH = 25.0f;
    const float HEAD_HEIGHT = 20.0f;
    const float LEG_WIDTH = 5.0f;
    const float LEG_LENGTH = 20.0f;
    const float FOOT_WIDTH = 6.0f;
    const float FOOT_LENGTH = 15.0f;
    const float BEAK_WIDTH_BOTTOM = 1.0f;
    const float BEAK_WIDTH_TOP = 4.0f;
    const float BEAK_LENGTH = 15.0f;

	// Make everything bigger so that can see more easily
	float zoom = 2.0f;
    glTranslatef(PENGUIN_TRANSLATE_X * PENGUIN_TRANSLATE_X_MAX, PENGUIN_TRANSLATE_Y * PENGUIN_TRANSLATE_Y_MAX, 0.0);
	glScalef(zoom, zoom, 1.0);

    // Push the current transformation matrix on the stack
    glPushMatrix();

        // Draw the 'body'
        glPushMatrix();
            // Scale square to size of body
            glScalef(BODY_WIDTH, BODY_LENGTH, 1.0);

            // Draw body polygon
            drawBody(1.0, 1.0);
        glPopMatrix();

	    // Draw the 'head joint'        
        glPushMatrix();
        	glTranslatef(0.0, +BODY_LENGTH*10/20 - JOINT_RADIUS, 0.0);
	        glTranslatef(0.0, -JOINT_PADDING, 0.0);        	
	        
		    // Draw joint between head and body
		    drawJoint(JOINT_RADIUS);        	
        	           	
            // Draw head
        	glPushMatrix();      
                glRotatef(HEAD_JOINT_ROT, 0.0, 0.0, 1.0);

                // Draw main head outline
                glPushMatrix();
    		    	glScalef(HEAD_WIDTH, HEAD_HEIGHT, 1.0);
                    glTranslatef(0.0, 0.4, 0.0); // Translate to the pivot point of the head
    				drawHead(1.0, 1.0);
                glPopMatrix();

                // Draw eyes
                glPushMatrix();
                    glTranslatef(-HEAD_WIDTH * 0.2f, HEAD_HEIGHT * 0.5, 0.0);

                    glPushMatrix();
                        glTranslatef(HEAD_WIDTH * 0.02f, 0.0, 0.0); // Offcenter eye
                        glScalef(2.0f, 2.0f, 1.0f); // Draw outer eye
                        drawJoint(1.0, false);
                    glPopMatrix();

                    // Draw inner eye
                    glColor3f(0.0, 0.0, 0.0);
                    drawCircle(1.0);

                glPopMatrix();


                glPushMatrix();
                    glTranslatef(-HEAD_WIDTH*0.45, HEAD_HEIGHT * 0.15, 0.0);

                    // Draw beak bottom
                    glPushMatrix();
                        // Animate the bottom beak by moving it up to reach the top beak                    
                        glTranslatef(0.0f, HEAD_HEIGHT * 0.1 * BEAK_TRANSLATE, 0.0);
                        glScalef(BEAK_LENGTH, BEAK_WIDTH_BOTTOM, 1.0);
                        glTranslatef(-0.4, 0.0, 0.0);
                        // Draw beak yellow
                        glColor3f(0.8, 0.8, 0.0); // Yellow
                        drawSquare(1.0);
                    glPopMatrix();

                    glTranslatef(0, HEAD_HEIGHT * 0.25, 0.0);
                    // Draw beak top
                    glPushMatrix();
                        glScalef(BEAK_LENGTH, BEAK_WIDTH_TOP, 1.0);
                        glTranslatef(-0.4, 0.0, 0.0);
                        drawBeakTop(1.0, 1.0);
                    glPopMatrix();
                glPopMatrix();



		    glPopMatrix();
        glPopMatrix();
                      
        // Draw the 'arm'
   
        glPushMatrix();
        	// Move the arm to the joint hinge
		    glTranslatef(0.0, BODY_LENGTH/5, 0.0);		    
	    	glTranslatef(0.0, -JOINT_PADDING, 0.0);
	    	
			drawJoint(JOINT_RADIUS);		    
		    
		    // Rotate along the hinge
		    glRotatef(ARM_JOINT_ROT, 0.0, 0.0, 1.0);

			// Align pivot point with joint
		    glTranslatef(0.0, JOINT_RADIUS, 0.0);
		    
		    // Scale the size of the arm
		    glScalef(ARM_WIDTH, ARM_LENGTH, 1.0);

		    // Move to center location of arm, under previous rotation
		    glTranslatef(0.0, -0.45, 0.0);

		    // Draw the arm
		    drawArm(1.0, 1.0);
		    
        glPopMatrix();
        
		// Draw left leg
		
		glPushMatrix();
			glTranslatef(-BODY_WIDTH/6 , -BODY_LENGTH * 0.4, 0.0);

			// Joint between body and top part of left leg
			drawJoint(JOINT_RADIUS);		    
	
			glRotatef(LEFT_LEG_HIP_JOINT_ROT, 0.0, 0.0, 10);
			// Move to edge of leg
			glPushMatrix();
				// Scale size of leg top part
				glScalef(LEG_WIDTH, LEG_LENGTH, 1.0);
				glTranslatef(0.0, -0.4, 0.0);						
                glColor3f(0.8, 0.8, 0.0); // Yellow
				drawSquare(1.0);
			glPopMatrix();
			
			// Move to bottom edge of leg
			glTranslatef(0.0, -LEG_LENGTH * 0.8f, 0.0);
			drawJoint(JOINT_RADIUS);
			
			glPushMatrix();
				// Scale size of leg bottom part
				glRotatef(LEFT_FOOT_JOINT_ROT, 0.0, 0.0, 1.0);
				glScalef(FOOT_LENGTH, FOOT_WIDTH, 1.0);
				glTranslatef(-0.4, 0.0, 0.0);
                
				drawFoot(1.0);				
			glPopMatrix();
		glPopMatrix();

        // Draw right leg
        glPushMatrix();
            glTranslatef(BODY_WIDTH/5 , -BODY_LENGTH * 0.4, 0.0);

            // Joint between body and top part of left leg
            drawJoint(JOINT_RADIUS);            
    
            glRotatef(RIGHT_LEG_HIP_JOINT_ROT, 0.0, 0.0, 10);
            // Move to edge of leg
            glPushMatrix();
                // Scale size of leg top part
                glScalef(LEG_WIDTH, LEG_LENGTH, 1.0);
                glTranslatef(0.0, -0.4, 0.0);                       
                glColor3f(0.8, 0.8, 0.0); // Yellow
                drawSquare(1.0);
            glPopMatrix();
            
            // Move to bottom edge of leg
            glTranslatef(0.0, -LEG_LENGTH * 0.8f, 0.0);
            drawJoint(JOINT_RADIUS);
            
            glPushMatrix();
                // Scale size of leg bottom part
                glRotatef(RIGHT_FOOT_JOINT_ROT, 0.0, 0.0, 1.0);
                glScalef(FOOT_LENGTH, FOOT_WIDTH, 1.0);
                glTranslatef(-0.4, 0.0, 0.0);
                
                drawFoot(1.0);              
            glPopMatrix();
        glPopMatrix();

    // Retrieve the previous state of the transformation stack
    glPopMatrix();


    // Execute any GL functions that are in the queue just to be safe
    glFlush();

    // Now, show the frame buffer that we just drew into.
    // (this prevents flickering).
    glutSwapBuffers();
}

// Draw a square of the specified size, centered at the current location
void drawSquare(float width)
{
    // Draw the square
    glBegin(GL_LINE_LOOP);
    glVertex2d(-width/2, -width/2);
    glVertex2d(width/2, -width/2);
    glVertex2d(width/2, width/2);
    glVertex2d(-width/2, width/2);
    glEnd();
}

// Draw a circle of specified radius centered at current location, this circle is filled in
void drawCircle(float radius)
{
	glBegin(GL_POLYGON);
	// Use trig to calculate the values of each corner. Divide circle into n sides
	for(int i = 0; i < NUM_CIRCLE_SIDES; i++){
		float xPos = radius * cos(2*M_PI * i / NUM_CIRCLE_SIDES);
		float yPos = radius * sin(2*M_PI * i / NUM_CIRCLE_SIDES);
		glVertex2d(xPos, yPos);
	}

	glEnd();
}

// Draws a circle that is not filled in white
void drawJoint(float radius){
    drawJoint(radius, true);
}

// Draws a circle that is not filled in. If isWhite is set, the circle will be white
// otherwise it will be whatever color was chosen on the stack
void drawJoint(float radius, bool isWhite){
	glBegin(GL_LINE_LOOP); // Use GL_LINE_LOOP instead of GL_LINES to make the circle continuous
    if(isWhite){
        glColor3f(1.0, 1.0, 1.0);
    }
	for(int i = 0; i < NUM_CIRCLE_SIDES; i++){
		float xPos = radius * cos(2*M_PI * i / NUM_CIRCLE_SIDES);
		float yPos = radius * sin(2*M_PI * i / NUM_CIRCLE_SIDES);
		glVertex2d(xPos, yPos);
	}

	glEnd();
}

// Draws the head shape
void drawHead(float width, float height)
{
    glColor3f(0.0, 0.0, 0.0); // black
	glBegin(GL_LINE_LOOP);
    glVertex2d(-width*3/8, height*1/4);
    glVertex2d(-width/8, height/2);
    glVertex2d(width*3/8, height * 1/4);
    glVertex2d(width/2, -height/2);
    glVertex2d(-width/2, -height/2);
    glEnd();
}

// Draws body shape
void drawBody(float width, float height){
    glColor3f(0.0, 0.0, 0.0); // black
	glBegin(GL_LINE_LOOP);
	// Start with the bottom trapezoid like part
	glVertex2d(-width/2, -height/3);
	glVertex2d(-width/6, -height/2);
	glVertex2d(width/6, -height/2);
	glVertex2d(width/2, -height/3);
	// Draw upper half of body
	glVertex2d(width*3/10, height/6);
	glVertex2d(width*1/5, height/2);
	glVertex2d(-width*1/5, height/2);
	glEnd();
}

// Draws arm which is like an inverted trapezoid
void drawArm(float width, float height){
    glColor3f(0.0, 0.0, 0.0); // black
	glBegin(GL_LINE_LOOP);
	glVertex2d(-width/2, height/2);// Start at top left
	glVertex2d(width/2, height * 9/20); // top right
	glVertex2d(width*1/5, -height/2);
	glVertex2d(-width * 2/5, -height/2);
	glEnd();
}

// Draws a yellow square that is sheared for the foot
void drawFoot(float width){
    glPushMatrix();
        glColor3f(0.8, 0.8, 0.0); // Yellow
        glScalef(1.0, 1/1.8, 1.0);
        glRotatef(10, 0.0, 0.0, 1.0);
        glScalef(1.0, 1.8, 1.0);
        glRotatef(-10, 0.0, 0.0, 1.0);
        drawSquare(1.0);
    glPopMatrix();
}

void drawBeakTop(float width, float height){
    // Draw beak yellow
    glColor3f(0.8, 0.8, 0.0); // Yellow
    glBegin(GL_LINE_LOOP);
        // Beak is a quadrilateral
        glVertex2d(width/2, -height/2);// Start at bottom right
        glVertex2d(-width/2, -height/2); // Bottom left
        glVertex2d(-width/2, -height/10); // Move up
        glVertex2d(width/2, height/2); // Top right
    glEnd();
}




