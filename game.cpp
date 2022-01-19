//============================================================================
// Name        : Brick Slayer Game
// Author      : Zaid Siddiqui
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Brick Slayer...
//============================================================================

#ifndef BSLAYER_CPP_
#define BSLAYER_CPP_
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt
#include <unistd.h>
using namespace std;

//defining some MACROS

#define MAX(A,B) ((A) > (B) ? (A):(B)) // finds max of two numbers
#define MIN(A,B) ((A) < (B) ? (A):(B)) // find min of two numbers
#define ABS(A) ((A) < (0) ? -(A):(A))  // find ABS of a given number

// define some constants
// constant FPS (Frame per second) is  used to specify
// number of frames per second in your game.. if FPS is small 
// your objects will move slowly and if large your objects
// will move faster, see the Timer function below for further help

#define FPS 80 // frame per seconds

// define another constant to hold ASCII for Escape key.
#define KEY_ESC 27 // A

// define some colors as 2D arrays, we have five pre-defined colors here..
// you can add many more if you wish..

float colors[5][3] = { { 1 / 255.0, 164 / 255.0, 164 / 255.0 }, { 215 / 255.0, 0
		/ 255.0, 96 / 255.0 }, { 208 / 255.0, 209 / 255.0, 2 / 255.0 }, { 0
		/ 255.0, 161 / 255.0, 203 / 255.0 }, { 50 / 255.0, 116 / 255.0, 44
		/ 255.0 } };

// defining some utility functions...
/*
 * This function converts an input angle from degree to radians */
float Deg2rad(float degree) {
	return (degree / 180.0) * M_PI;
}
// seed the random numbers generator by current time (see the documentation of srand for further help)...
void InitRandomizer() {
	srand((unsigned int) time(0)); // time(0) returns number of seconds elapsed since January 1, 1970.
}
//This function returns a random value within the specified range of [rmin, rmax] ...
long GetRandInRange(const long &rmin, const long &rmax) {
	long range = rmax - rmin; // find the range
	long value = (rand() % (long) range) + rmin; // translate the generated number ...
//	cout << value << endl << flush;
	return value;
}

/*To draw a triangle we need three vertices with each vertex having 2-coordinates [x, y] and a color for the triangle.
 * This function takes 4 arguments first three arguments (3 vertices + 1 color) to
 * draw the triangle with the given color.
 * */
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
		float color[]) {
	glColor3fv(color); // Set the triangle colour
	// ask library to draw triangle at given position...

	glBegin(GL_TRIANGLES);
	/*Draw triangle using given three vertices...*/
	glVertex4i(x1, y1, 0, 1);
	glVertex4i(x2, y2, 0, 1);
	glVertex4i(x3, y3, 0, 1);
	glEnd();
}
/*
 * Write two functions DrawRectangle and DrawSphere
 * */
 
int X1 = 360, Y1 = 10, X2 = 470, Y2 = 20, circleX = 400, circleY = 20, c = 0;
bool check = 1,check2 = 0,check3 = 0,check4 = 0;

int blocks[1000][5];
int block_num = 0;

int ball_direction = 1;

bool CheckOverlapping( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) {
	if (x4 < x1) {
		// Second rectangle must lie at left side of the first
		return false;
	} else if (x3 > x2) {
		// Second rectangle must lie at the right side of the first
		return false;
	} else if (y3 > y2) {
		// Second rectangle must lie above the first
		return false;
	} else if (y4 < y1) {
		// Second rectangle must lie below the first 
		return false;
	}
	return true;
}

int DetermineSideOfOverlapping( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) {
	if (y1 == y4) {
		return 2;
	} else if (y2 == y3) {
		return 0;
	} else if (x4 == x1) {
		return 4;
	} else if (x3 == x2) {
		return 1;
	}
	return -1;
}

void DrawSingleRectangle( int x1, int y1, int x2, int y2, float color[] ) {
	DrawTriangle( x1, y1, x1, y2, x2, y1, color );
	DrawTriangle( x2, y2, x1, y2, x2, y1, color );
}

void InitializeBlocksCoordinates( int x = 60, int y = 140, int height = 20, int width = 60 ) {
	
	int backup_x = x;
	
	for ( int row = 3; row < 25; row++ ) {
		for ( int col = 0; col < 10; col++ ) {
			blocks[ block_num ][ 0 ] = x;
			blocks[ block_num ][ 1 ] = y;
			blocks[ block_num ][ 2 ] = x + width;
			blocks[ block_num ][ 3 ] = y + height;
			blocks[ block_num ][ 4 ] = 1;
			block_num++;
			
			x += width + 1;
		}
		
		x = backup_x;
		y += height + 1;
	}
}

void DrawBlocks() {
	int color_index = 1;
	for ( int i = 0; i < block_num; i++ ) {
		if ( blocks[ i ][ 4 ] == 1 ) {
			DrawSingleRectangle( blocks[ i ][ 0 ], blocks[ i ][ 1 ], blocks[ i ][ 2 ], blocks[ i ][ 3 ], colors[ color_index ] );
		}
		color_index = color_index % 2 + 1;
	}
}

void DrawBar() {
	DrawTriangle(X1, Y1, X1, Y2, X2, Y1, colors[3]);
	DrawTriangle(X2, Y2, X1, Y2, X2, Y1, colors[3]);
}

void DrawSphere(int x1, int y1, int r, float color[]) {
	int ar[100][2];
	int temp, temp2;
	int j = 0, Xc, Yc;
	
	for (int i = 0; i <= 360; i += 7, j++){
		//temp = static_cast<double>(r) * static_cast<double>(cos(i));
		//temp2 = static_cast<double>(r) * static_cast<double>(sin(i));
		
		temp = r * cos( i );
		temp2 = r * sin( i );
		
		//temp = round(temp);
		//temp2 = round(temp2);
		
		Xc = x1 + temp;
		Yc = y1 + temp2;
		ar[j][0] = Xc;
		ar[j][1] = Yc;
	}
	
	for (int i = 0; i < j - 1; i++) {
		DrawTriangle(x1, y1, ar[i][0], ar[i][1], ar[i + 1][0], ar[i + 1][1], color);
	}
	
}


int CheckBallCollisionWithBlock(int &side) {
	for (int i = 0 ; i < block_num; i++) {
		if (blocks[ i ][ 4 ] == 1 && CheckOverlapping( blocks[ i ][ 0 ], blocks[ i ][ 1 ], blocks[ i ][ 2 ], blocks[ i ][ 3 ], 
			circleX - 7, circleY - 7 , circleX + 7, circleY + 7)) {
				
			side = DetermineSideOfOverlapping( blocks[ i ][ 0 ], blocks[ i ][ 1 ], blocks[ i ][ 2 ], blocks[ i ][ 3 ], 
			circleX - 7, circleY - 7 , circleX + 7, circleY + 7 );
			
			return i;
		}
	}
	return -1;
}
/*
 * Main Canvas drawing function.
 * */
void Display()/**/{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0/*Red Component*/, 0/*Green Component*/,
			0.07/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//write your drawing commands here or call your drawing functions...

	// Drawing set of three triangles at different points of a circle using simple trignometry...
	/*for (int midx = 150; midx < 800; midx += 250) {
	
		int midy = 350;
		float theta = Deg2rad(-10);
		DrawTriangle(midx, midy, midx + 100 * cos(theta),
				midy + 100 * sin(-theta), midx + 100 * cos(theta),
				midy + 100 * sin(theta), colors[1]);

		float theta2 = Deg2rad(100);
		theta = Deg2rad(80);
		DrawTriangle(midx, midy, midx + 100 * cos(theta),
				midy + 100 * sin(theta), midx + 100 * cos(theta2),
				midy + 100 * sin(theta2), colors[2]);
		theta = Deg2rad(170);
		theta2 = Deg2rad(190);
		DrawTriangle(midx, midy, midx + 100 * cos(theta),
				midy + 100 * sin(theta), midx + 100 * cos(theta2),
				midy + 100 * sin(theta2), colors[3]);
	
	}*/
	
	DrawBlocks();
	DrawBar();
	
	DrawSphere(circleX, circleY, 9, colors[1]);
	
	
//	DrawTriangle(300, 300, 400, 400, 300, 500, colors[1]);
//	DrawTriangle(100, 100, 400, 400, 300, 500, colors[1]);
//	DrawTriangle(300, 300, 400, 400, 300, 500, colors[1]);
	//
	glutSwapBuffers(); // do not modify this line..
}

void moveBall() {
	int side = -1;
	int colliding_block = CheckBallCollisionWithBlock( side );
	
	if (colliding_block != -1) {
		blocks[ colliding_block ][ 4 ] = 0;

		if (side == 0) {
			if (ball_direction == 3) {
				ball_direction = 2;
			} else if (ball_direction == 4) {
				ball_direction = 1;
			}
		} else if (side == 1) {
			if (ball_direction == 2) {
				ball_direction = 1;
			} else if (ball_direction == 3) {
				ball_direction = 4;
			}
		} else if (side == 2) {
			if (ball_direction == 1) {
				ball_direction = 4;
			} else if (ball_direction == 2) {
				ball_direction = 3;
			}
		} else if ( side == 3 ) {
			if (ball_direction == 4) {
				ball_direction = 3;
			} else if (ball_direction == 1) {
				ball_direction = 2;
			}
		}  
	}
	
	if (ball_direction == 1) {
		circleX++;
		circleY++;

		if (circleX >= 791) {
			ball_direction = 2;
		} else if ( circleY >= 591 ) {
			ball_direction = 4;
		}
	} else if (ball_direction == 2) {
		circleX--;
		circleY++;
		
		if (circleX <= 0) {
			ball_direction = 1;
		} else if (circleY >= 591) {
			ball_direction = 3;
		}
	} else if ( ball_direction == 3 ) {
		circleX--;
		circleY--;
		
		if ( circleY - 9 <= Y2 && circleX - 9 >= X1 && circleX + 9 <= X2 ) {
			ball_direction = 2;
		} else if (circleX <= 0) {
			ball_direction = 4;
		} else if (circleY <= 0) {// bottom 
			exit(0);
		}
	} else if ( ball_direction == 4 ) {
		circleX++;
		circleY--;
		
		if ( circleY - 9 <= Y2 && circleX - 9 >= X1 && circleX + 9 <= X2 ) {
			ball_direction = 1;
		} else if (circleX >= 791) {
			ball_direction = 3;
		} else if (circleY <= 0) {// Bottom
			exit(0);
		}
	}

	glutPostRedisplay();
}


/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		if (X1 != 0) {
			X1 -= 10;
			X2 -= 10;
			glutPostRedisplay();
		}
	} else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
		if (X1 != 690) {
			X1 += 10;
			X2 += 10;
			glutPostRedisplay();
		}
	} else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {

	} else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {

	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/
	/*
	 glutPostRedisplay();
	 */
}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
 * This function is called after every 1000.0/FPS milliseconds 
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here
	moveBall();
	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(1000.0 / FPS, Timer, 0);
}

/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {
	
	int width = 800, height = 600; // i have set my window size to be 800 x 600
	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	
	InitializeBlocksCoordinates();
	

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Brick Slayer"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	glutDisplayFunc(Display); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0 / FPS, Timer, 0);

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* BSLAYER_CPP_ */
