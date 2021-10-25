#define FREEGLUT_STATIC
#include <math.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

// Define color
# define white 1.0, 1.0, 1.0
# define red 1.0, 0.0, 0.0
# define black 0.0, 0.0, 0.0
# define grey 0.5, 0.5, 0.5
# define green 0.0, 1.0, 0.0
# define blue 0.0, 0.0, 1.0
# define purple 1.0, 0.0, 1.0
# define darkGreen 0.0, 0.2, 0.0
# define brown 200.0 / 255.0, 100.0 / 255.0, 30.0 / 255.0
# define yellow 1.0, 0.8, 0.0
# define doorColor 0.6, 0.6, 0.6

int winWidth = 900; // the window width
int winHeight = 600; // the window height
int time = 50; // Declare refresh interval in ms
int car1MoveZ = 0; // Set the car1's move distance (the relative position of Z-axis)
int car2MoveZ = 0; // Set the car2's move distance (the relative position of Z-axis)
float viewX = -150;
float viewY = 400;
float viewZ = -1000;
double pi = 3.1415926;

double cuboid[8][3]; // Set a matrix to record cuboid's vertexes

static GLuint concrete; // Initialize the concrete texture object
static GLuint brick; // Initialize the brick texture object
static GLuint soil;  // Initialize the soil texture object
static GLuint grass; // Initialize the grass texture object
static GLint imageWidth;
static GLint imageHeight;
static GLint pixelLength;
static GLubyte* pixelData;

GLfloat rotateSpeed = 0.0; // Set the speed of rotate
GLfloat rotateAngle = 0.0; // Set the angle of rotate
GLfloat sunAngle = 0.0; // Set the parameter of sun's moving
GLfloat angleX = 0.0;
GLfloat angleY = 0.0;
GLfloat oldX;
GLfloat oldY;

// setCuboid() and drawCuboid() aim to draw a cuboid when giving start point and lenth
// Bind a cuboid's eight vertexes
void setCuboid(double x1, double y1, double z1, double x2, double y2, double z2) {
	cuboid[0][0] = x1;
	cuboid[0][1] = y1;
	cuboid[0][2] = z1;

	cuboid[1][0] = x1;
	cuboid[1][1] = y1;
	cuboid[1][2] = z1 + z2;

	cuboid[2][0] = x1 + x2;
	cuboid[2][1] = y1;
	cuboid[2][2] = z1 + z2;

	cuboid[3][0] = x1 + x2;
	cuboid[3][1] = y1;
	cuboid[3][2] = z1;

	// bind the other four vertexes
	for (int m = 0; m < 4; m++) {
		for (int n = 0; n < 3; n++) {
			if (n == 1) {
				cuboid[4 + m][n] = cuboid[m][n] + y2;
			}
			else {
				cuboid[4 + m][n] = cuboid[m][n];
			}
		}
	}
}

// Draw the binded cuboid
void drawCuboid() {
	// Draw a cuboid's bottom
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(cuboid[0][0], cuboid[0][1], cuboid[0][2]);
	glVertex3f(cuboid[1][0], cuboid[1][1], cuboid[1][2]);
	glVertex3f(cuboid[2][0], cuboid[2][1], cuboid[2][2]);
	glVertex3f(cuboid[3][0], cuboid[3][1], cuboid[3][2]);
	glEnd();

	// Draw a cuboid's top
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(cuboid[4][0], cuboid[4][1], cuboid[4][2]);
	glVertex3f(cuboid[5][0], cuboid[5][1], cuboid[5][2]);
	glVertex3f(cuboid[6][0], cuboid[6][1], cuboid[6][2]);
	glVertex3f(cuboid[7][0], cuboid[7][1], cuboid[7][2]);
	glEnd();

	// Draw a cuboid' left side
	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(cuboid[0][0], cuboid[0][1], cuboid[0][2]);
	glVertex3f(cuboid[1][0], cuboid[1][1], cuboid[1][2]);
	glVertex3f(cuboid[5][0], cuboid[5][1], cuboid[5][2]);
	glVertex3f(cuboid[4][0], cuboid[4][1], cuboid[4][2]);
	glEnd();

	// Draw a cuboid's right side
	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(cuboid[2][0], cuboid[2][1], cuboid[2][2]);
	glVertex3f(cuboid[3][0], cuboid[3][1], cuboid[3][2]);
	glVertex3f(cuboid[7][0], cuboid[7][1], cuboid[7][2]);
	glVertex3f(cuboid[6][0], cuboid[6][1], cuboid[6][2]);
	glEnd();

	// Draw a cublid's front side
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(cuboid[5][0], cuboid[5][1], cuboid[5][2]);
	glVertex3f(cuboid[6][0], cuboid[6][1], cuboid[6][2]);
	glVertex3f(cuboid[2][0], cuboid[2][1], cuboid[2][2]);
	glVertex3f(cuboid[1][0], cuboid[1][1], cuboid[1][2]);
	glEnd();

	// Draw a cublid's back side
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(cuboid[0][0], cuboid[0][1], cuboid[0][2]);
	glVertex3f(cuboid[3][0], cuboid[3][1], cuboid[3][2]);
	glVertex3f(cuboid[7][0], cuboid[7][1], cuboid[7][2]);
	glVertex3f(cuboid[4][0], cuboid[4][1], cuboid[4][2]);
	glEnd();
}

// Draw and decorrate the foundation
void ground() {
	// Draw a ground
	glPushMatrix();
		glScalef(600.0, 8.0, 900.0);
		glColor3f(green);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-525, 0, 0);
		glScalef(150.0, 8.0, 900.0);
		glColor3f(green);
		glutSolidCube(1);
	glPopMatrix();

	// Draw a road
	glPushMatrix();
		glTranslatef(-375, 0, 0);
		glScalef(150.0, 8.0, 900.0);
		glColor3f(grey);
		glutSolidCube(1);
	glPopMatrix();

	// Draw a zebra crossing
	for (int i = 0; i < 9; i++) {
		glPushMatrix();
			glTranslatef(-375, 5, -400 + 100 * i);
			glScalef(10.0, 5.0, 50.0);
			glColor3f(grey);
			glutSolidCube(1);
		glPopMatrix();
	}

}

// Draw a house and its decorations
void house() {
	// Draw house's walls
	// Draw the back wall
	glPushMatrix();
		glColor3f(white);
		setCuboid(50, 0, -200, -5, 150, 300);
		drawCuboid();

		// Draw the two sides of the house
		glColor3f(white);
		setCuboid(50, 0, -200, -200, 150, 5);
		drawCuboid();
		glColor3f(white);
		setCuboid(50, 0, 95, -200, 150, 5);
		drawCuboid();

		// Draw the front wall
		glColor3f(white);
		glTranslatef(-150, 0, -200);
		setCuboid(0, 0, 0, 5, 50, 200);
		drawCuboid();
		setCuboid(0, 0, 250, 5, 100, 50);
		drawCuboid();

		glTranslatef(0, 50, 0);
		setCuboid(0, 0, 0, 5, 50, 50);
		drawCuboid();
		setCuboid(0, 0, 100, 5, 50, 25);
		drawCuboid();
		setCuboid(0, 0, 175, 5, 50, 25);
		drawCuboid();

		glTranslatef(0, 50, 0);
		setCuboid(0, 0, 0, 5, 50, 300);
		drawCuboid();
	glPopMatrix();

	// Draw house's roof
	for (int i = 0; i < 8; i++) {
		glPushMatrix();
			glTranslatef(-50, 150 + 7 * i, -50);
			glScalef(225.0 - 25 * i, 5.0, 350.0 - 25 * i);
			glColor3f(red);
			glutSolidCube(1);
		glPopMatrix();
	}

	// Draw a door
	glPushMatrix();
		glTranslatef(-150, 0, 0);
		glColor3f(doorColor);
		setCuboid(-1, 0, 0, 5, 100, 50);
		drawCuboid();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-150, 50, 6);
		glColor3f(black);
		setCuboid(0, 0, 0, -2, 10, 2);
		drawCuboid();
	glPopMatrix();

	// Draw some windows
	glPushMatrix();
		glTranslatef(-150, 50, -75);
		glColor4f(blue, 0.6);
		setCuboid(0, 0, 0, 5, 50, 50);
		drawCuboid();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-150, 50, -150);
		glColor4f(blue, 0.6);
		setCuboid(0, 0, 0, 5, 50, 50);
		drawCuboid();
	glPopMatrix();

	// Draw house's fence
	glPushMatrix();
		glTranslatef(100, 0, -250);
		glColor3f(white);
		setCuboid(0, 0, 0, -10, 25, 400);
		drawCuboid();
		setCuboid(0, 70, 0, -10, -15, 400);
		drawCuboid();

		setCuboid(0, 0, 0, -10, 70, 10);
		drawCuboid();

		setCuboid(0, 0, 0, -400, 25, 10);
		drawCuboid();
		setCuboid(0, 70, 0, -400, -15, 10);
		drawCuboid();

		setCuboid(-400, 0, 0, 10, 70, 10);
		drawCuboid();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-300, 0, 150);
		glColor3f(white);
		setCuboid(0, 0, 0, 400, 25, -10);
		drawCuboid();
		setCuboid(0, 70, 0, 400, -15, -10);
		drawCuboid();

		setCuboid(0, 0, 0, 10, 70, -10);
		drawCuboid();
		setCuboid(400, 0, 0, -10, 70, -10);
		drawCuboid();

		setCuboid(0, 0, 0, 10, 25, -80);
		drawCuboid();
		setCuboid(0, 70, 0, 10, -15, -80);
		drawCuboid();

		setCuboid(0, 0, -80, 10, 70, -10);
		drawCuboid();

		setCuboid(0, 0, -400, 10, 25, 220);
		drawCuboid();
		setCuboid(0, 70, -400, 10, -15, 220);
		drawCuboid();

		setCuboid(0, 0, -180, 10, 70, -10);
		drawCuboid();
	glPopMatrix();

	// Draw railings
	glPushMatrix();
		glTranslatef(100, 25, -250);
		glColor3f(black);
		for (int i = 0; i < 26; i++) {
			setCuboid(0, 0, 10 + 15 * i, -5, 30, 5);
			drawCuboid();
		}
		for (int i = 0; i < 26; i++) {
			setCuboid(-10 - 15 * i, 0, 0, -5, 30, 5);
			drawCuboid();
		}
		for (int i = 0; i < 26; i++) {
			setCuboid(-10 - 15 * i, 0, 400, -5, 30, -5);
			drawCuboid();
		}
		glTranslatef(-400, 0, 0);
		for (int i = 0; i < 14; i++) {
			setCuboid(0, 0, 10 + 15 * i, 5, 30, 5);
			drawCuboid();
		}
		glTranslatef(0, 0, 400);
		for (int i = 0; i < 5; i++) {
			setCuboid(0, 0, -15 - 15 * i, 5, 30, 5);
			drawCuboid();
		}
	glPopMatrix();
}

// Draw some trees
void trees() {
	// Draw a tree
	glPushMatrix();
		glColor3f(brown);
		glTranslatef(180, 6, -250);
		setCuboid(0, 0, 0, 20, 100, 20);
		drawCuboid();
		glColor3f(darkGreen);
		glTranslatef(0, 90, 10);
		glutSolidSphere(30, 20, 20);
		glTranslatef(20, 0, 0);
		glutSolidSphere(30, 20, 20);
		glTranslatef(-10, 0, -10);
		glutSolidSphere(30, 20, 20);
		glTranslatef(0, 0, 20);
		glutSolidSphere(30, 20, 20);
		glTranslatef(0, 10, -10);
		glutSolidSphere(30, 20, 20);
	glPopMatrix();

	// Draw another tree
	glPushMatrix();
		glColor3f(brown);
		glTranslatef(-80, 6, -370);
		setCuboid(0, 0, 0, 20, 100, 20);
		drawCuboid();
		glColor3f(darkGreen);
		glTranslatef(0, 90, 10);
		glutSolidSphere(30, 20, 20);
		glTranslatef(20, 0, 0);
		glutSolidSphere(30, 20, 20);
		glTranslatef(-10, 0, -10);
		glutSolidSphere(30, 20, 20);
		glTranslatef(0, 0, 20);
		glutSolidSphere(30, 20, 20);
		glTranslatef(0, 10, -10);
		glutSolidSphere(30, 20, 20);
	glPopMatrix();

	// Draw another tree
	glPushMatrix();
		glColor3f(brown);
		glTranslatef(0, 6, -350);
		setCuboid(0, 0, 0, 20, 100, 20);
		drawCuboid();
		glColor3f(darkGreen);
		glTranslatef(0, 90, 10);
		glutSolidSphere(30, 20, 20);
		glTranslatef(20, 0, 0);
		glutSolidSphere(30, 20, 20);
		glTranslatef(-10, 0, -10);
		glutSolidSphere(30, 20, 20);
		glTranslatef(0, 0, 20);
		glutSolidSphere(30, 20, 20);
		glTranslatef(0, 10, -10);
		glutSolidSphere(30, 20, 20);
	glPopMatrix();

	// Draw another tree
	glPushMatrix();
		glColor3f(brown);
		glTranslatef(100, 6, -350);
		setCuboid(0, 0, 0, 15, 70, 15);
		drawCuboid();
		glColor3f(darkGreen);
		glTranslatef(-2, 60, 10);
		glutSolidSphere(20, 20, 20);
		glTranslatef(20, 0, 0);
		glutSolidSphere(20, 20, 20);
		glTranslatef(-10, 0, -10);
		glutSolidSphere(20, 20, 20);
		glTranslatef(0, 0, 20);
		glutSolidSphere(20, 20, 20);
		glTranslatef(0, 10, -10);
		glutSolidSphere(20, 20, 20);
	glPopMatrix();

}

// Draw a sun which moves as time goes by
void sun() {
	glPushMatrix();
		glColor3f(yellow);
		glTranslatef(-150, 0, 0);
		glTranslatef(-500 * cos(pi * sunAngle / 180), 400 * sin(pi * sunAngle / 180), 0);
		glutSolidSphere(40, 20, 20);
	glPopMatrix();
}

// Draw the car1 which moves as time goes by
void car1() {
	glPushMatrix();
		// Draw a car's boddy
		glTranslatef(-395, 15, -450);
		glTranslatef(0, 0, car1MoveZ);

		glColor3f(0.3, 0.3, 0.8);
		setCuboid(0, 0, 0, -40, 20, 80);
		drawCuboid();

		glColor3f(0.5, 0.5, 0.8);
		setCuboid(0, 20, 10, -40, 20, 45);
		drawCuboid();
		// Draw a car's four wheels
		glTranslatef(1, 0, 10);
		glColor3f(purple);
		glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex3f(0, 10 * cos(2 * pi * i / 360), 10 * sin(2 * pi * i / 360));
		}
		glEnd();

		glTranslatef(0, 0, 60);
		glColor3f(purple);
		glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex3f(0, 10 * cos(2 * pi * i / 360), 10 * sin(2 * pi * i / 360));
		}
		glEnd();

		glTranslatef(-42, 0, 0);
		glColor3f(purple);
		glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex3f(0, 10 * cos(2 * pi * i / 360), 10 * sin(2 * pi * i / 360));
		}
		glEnd();

		glTranslatef(0, 0, -60);
		glColor3f(purple);
		glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex3f(0, 10 * cos(2 * pi * i / 360), 10 * sin(2 * pi * i / 360));
		}
		glEnd();

	glPopMatrix();
}

// Draw the car2 which moves as time goes by
void car2() {
	glPushMatrix();
	// Draw a car's boddy
	glTranslatef(-320, 15, 380);
	glTranslatef(0, 0, -car2MoveZ);

	glColor3f(1.0, 0.3, 0.0);
	setCuboid(0, 0, 0, -40, 20, 80);
	drawCuboid();

	glColor3f(1.0, 0.4, 0.3);
	setCuboid(0, 20, 25, -40, 20, 45);
	drawCuboid();
	// Draw a car's four wheels
	glTranslatef(1, 0, 10);
	glColor3f(black);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		glVertex3f(0, 10 * cos(2 * pi * i / 360), 10 * sin(2 * pi * i / 360));
	}
	glEnd();

	glTranslatef(0, 0, 60);
	glColor3f(black);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		glVertex3f(0, 10 * cos(2 * pi * i / 360), 10 * sin(2 * pi * i / 360));
	}
	glEnd();

	glTranslatef(-42, 0, 0);
	glColor3f(purple);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		glVertex3f(0, 10 * cos(2 * pi * i / 360), 10 * sin(2 * pi * i / 360));
	}
	glEnd();

	glTranslatef(0, 0, -60);
	glColor3f(purple);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		glVertex3f(0, 10 * cos(2 * pi * i / 360), 10 * sin(2 * pi * i / 360));
	}
	glEnd();

	glPopMatrix();
}

// Read image and bind texture
GLuint setTexture(const char* fileName) {
	GLuint lastTextureID = 0, textureID = 0;
	GLubyte* pixeldata = 0;

	// Read in and open an image file
	FILE* file = NULL; // The image file should be placed in the same folder with the source code.
	fopen_s(&file, fileName, "rb"); // read in binary mode
	if (file == 0) {
		return 0;
	}
	// Get the width and height of image
	// Reposition stream position indicator
	// SEEK_SET: Beginning of file
	fseek(file, 0x0012, SEEK_SET);
	fread(&imageWidth, sizeof(imageWidth), 1, file);
	fread(&imageHeight, sizeof(imageHeight), 1, file);
	fseek(file, 54, SEEK_SET);

	// Count the length of in the image by pixel
	// Pixel data consists of three colors red, green and blue
	pixelLength = imageWidth * 3;
	while (pixelLength % 4 != 0) {
		pixelLength++;
	}
	// Pixel data length = width * height
	pixelLength *= imageHeight;
	// Malloc for the image by pixel
	pixeldata = (GLubyte*)malloc(pixelLength); // memory allocation

	// Close file
	if (pixeldata == 0) {
		fclose(file);
		return 0;
	}

	if (fread(pixeldata, pixelLength, 1, file) <= 0) {
		free(pixeldata);
		fclose(file);
		return 0;
	}

	glGenTextures(1, &textureID);
	if (textureID == 0) {
		free(pixeldata);
		fclose(file);
		return 0;
	}

	// Set texture parameters
	GLint last_texture_ID = lastTextureID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixeldata);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);
	free(pixeldata);

	return textureID;
}

// Put wall texture into proper places
void wallTexture() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, brick);

	// Decorate the left wall
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(51, 0, -201);
		glTexCoord2f(0, 1); glVertex3f(51, 150, -201);
		glTexCoord2f(1, 1); glVertex3f(-151, 150, -201);
		glTexCoord2f(1, 0); glVertex3f(-151, 0, -201);
	glEnd();

	// Decorate the right wall
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(51, 0, 101);
		glTexCoord2f(0, 1); glVertex3f(51, 150, 101);
		glTexCoord2f(1, 1); glVertex3f(-151, 150, 101);
		glTexCoord2f(1, 0); glVertex3f(-151, 0, 101);
	glEnd();

	// Decorate the back wall
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(51, 0, 101);
		glTexCoord2f(0, 1); glVertex3f(51, 150, 101);
		glTexCoord2f(1, 1); glVertex3f(51, 150, -201);
		glTexCoord2f(1, 0); glVertex3f(51, 0, -201);
	glEnd();

	// Decorate the front wall
	glTranslatef(-151, 0, -201);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
		glTexCoord2f(0, 1.0 / 3.0); glVertex3f(0, 50, 0);
		glTexCoord2f(2.0 / 3.0, 1.0 / 3.0); glVertex3f(0, 50, 200);
		glTexCoord2f(2.0 / 3.0, 0); glVertex3f(0, 0, 200);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1.0 / 3.0); glVertex3f(0, 50, 0);
		glTexCoord2f(0, 2.0 / 3.0); glVertex3f(0, 100, 0);
		glTexCoord2f(1.0 / 6.0, 2.0 / 3.0); glVertex3f(0, 100, 50);
		glTexCoord2f(1.0 / 6.0, 1.0 / 3.0); glVertex3f(0, 50, 50);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(1.0 / 3.0, 1.0 / 3.0); glVertex3f(0, 50, 100);
		glTexCoord2f(1.0 / 3.0, 2.0 / 3.0); glVertex3f(0, 100, 100);
		glTexCoord2f(5.0 / 12.0, 2.0 / 3.0); glVertex3f(0, 100, 125);
		glTexCoord2f(5.0 / 12.0, 1.0 / 3.0); glVertex3f(0, 50, 125);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(7.0 / 12.0, 1.0 / 3.0); glVertex3f(0, 50, 175);
		glTexCoord2f(7.0 / 12.0, 2.0 / 3.0); glVertex3f(0, 100, 175);
		glTexCoord2f(2.0 / 3.0, 2.0 / 3.0); glVertex3f(0, 100, 200);
		glTexCoord2f(2.0 / 3.0, 1.0 / 3.0); glVertex3f(0, 50, 200);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(5.0 / 6.0, 0); glVertex3f(0, 0, 250);
		glTexCoord2f(5.0 / 6.0, 2.0 / 3.0); glVertex3f(0, 100, 250);
		glTexCoord2f(1, 2.0 / 3.0); glVertex3f(0, 100, 302);
		glTexCoord2f(1, 0); glVertex3f(0, 0, 302);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(0, 2.0 / 3.0); glVertex3f(0, 100, 0);
		glTexCoord2f(0, 1); glVertex3f(0, 150, 0);
		glTexCoord2f(1, 1); glVertex3f(0, 150, 302);
		glTexCoord2f(1, 2.0 / 3.0); glVertex3f(0, 100, 302);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

// Put sand texture and plant some vegetables
void setFarmLand() {
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, soil);
	
		// Put the sand texture
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-275, 6, -200);
			glTexCoord2f(0, 1); glVertex3f(-175, 6, -200);
			glTexCoord2f(1, 1); glVertex3f(-175, 6, -25);
			glTexCoord2f(1, 0); glVertex3f(-275, 6, -25);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		// Plant vegetables
		glTranslatef(-175, 6, -200);
		glColor3f(green);
		for (int p = 1; p < 5; p++) {
			for (int q = 1; q < 9; q++) {
				setCuboid(-20 * p, 0, 20 * q, -3, 10, -3);
				drawCuboid();
			}
		}

	glPopMatrix();
}

// Put grass texture to the ground
void setGrass() {
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, grass);
		// Put the grass texture
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(300, 5, -450);
			glTexCoord2f(0, 5); glVertex3f(300, 5, 450);
			glTexCoord2f(5, 5); glVertex3f(-300, 5, 450);
			glTexCoord2f(5, 0); glVertex3f(-300, 5, -450);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-450, 5, -450);
			glTexCoord2f(0, 5); glVertex3f(-450, 5, 450);
			glTexCoord2f(1, 5); glVertex3f(-600, 5, 450);
			glTexCoord2f(1, 0); glVertex3f(-600, 5, -450);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Put concrete texture to the road
void setConcrete() {
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, concrete);

		// Put the concrete texture
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-300, 5, -450);
			glTexCoord2f(0, 5); glVertex3f(-300, 5, 450);
			glTexCoord2f(5, 5); glVertex3f(-450, 5, 450);
			glTexCoord2f(5, 0); glVertex3f(-450, 5, -450);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Init light
void init(void){
	GLfloat sunPosition[] = { -500 * cos(pi * sunAngle / 180) - 150, 400 * sin(pi * sunAngle / 180), 0.0, 0.0 };
	GLfloat sunIntensity[] = { 1.0, 1.0, 1.0, 0.4 };
	GLfloat ambientIntensity[] = { 0.3, 0.3, 0.3, 1.0 };

	// Set up ambient light
	glEnable(GL_LIGHTING);             
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientIntensity);

	// Set up sunlight
	glEnable(GL_LIGHT0);                
	glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunIntensity);
	
	glEnable(GL_COLOR_MATERIAL);        
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	glLineWidth(5);
	glClearColor(0.0, 0.0, 0.0, 0.0); 

	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-winWidth, winWidth, -winHeight, winHeight, -winWidth, winWidth); 
	
}

// Set a timer to control car's moving and sun's moving
void onTimer(int Value) {
	car1MoveZ += 10;
	car2MoveZ += 20;
	sunAngle += 3;
	if (car1MoveZ >= 900) {
		car1MoveZ = 0;
	}
	if (car2MoveZ >= 900) {
		car2MoveZ = 0;
	}
	if (sunAngle >= 180) {
		sunAngle = 0;
	}
	glutTimerFunc(time, onTimer, 1);
}

// Idle callback function
void whenInMainloop() {
	// Force OpenGL to redraw the current window
	glutPostRedisplay();
}

// This function mixes the objects together and implements them
void display() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0.1, 5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	gluLookAt(viewX, viewY, viewZ, -150, 0, 0, 0, 1, 0);
	glRotatef(rotateAngle, 0.0, 1.0, 0.0);
	glRotatef(angleX, 0.0, 1.0, 0.0);
	glRotatef(angleY, 1.0, 0.0, 0.0);
	
	ground();
	house();
	trees();
	sun();
	car1();
	car2();
	wallTexture();
	setFarmLand();
	setGrass();
	setConcrete();

	init();
	
	glFlush();
	glutSwapBuffers();

}

// Set a keyboard to control the view field size and the speed of time
void keyboardOperation(unsigned char key, int x, int y) {
	// Narrow the perspective
	if (key == 'W' || key == 'w') {
		viewX -= 0.1 * viewX;
		viewY -= 0.1 * viewY;
		viewZ -= 0.1 * viewZ;
	}
	// Enlarge the perspective
	if (key == 'S' || key == 's') {
		viewX += 0.1 * viewX;
		viewY += 0.1 * viewY;
		viewZ += 0.1 * viewZ;
	}
	// Set the boundary of narrowing the perspective
	if (viewY >= 500) { 
		viewX = -187.5;
		viewY = 500;
		viewZ = -1250;
	}
	// Set the boundary of enlarging the perspective
	if (viewY <= 200) { 
		viewX = -75;
		viewY = 200;
		viewZ = -500;
	}
	// Decrease the refresh interval
	if (key == 'F' || key == 'f') {
		time -= 10;
	}
	// Increase the refresh interval
	if (key == 'E' || key == 'e') {
		time += 10;
	}
	// Set the boundary of refresh interval
	if (time >= 100) { 
		time = 100;
	}
	// Set the boundary of refresh interval
	if (time <= 10) { 
		time = 10;
	}
	// Quit the program
	if (key == 'Q' || key == 'q') { 
		exit(0);
	}
	glutPostRedisplay();
}

// The mouse() and motion() aims to use mouse to rotate the angle of view
void mouse(int button, int state, int x, int y) {
	if (button = GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			rotateSpeed = 0.0;
			rotateAngle = 0.0;
			oldX = x + winWidth / 2.0;
			oldY = y + winHeight / 2.0;
		}
	}
	
}

void motion(int x, int y) {
	GLint deltaX = oldX - x + winWidth / 2.0;
	GLint deltaY = oldY - y + winHeight / 2.0;
	angleX += 360 * (GLfloat)deltaX / (GLfloat)winWidth;  
	angleY += 360 * (GLfloat)deltaY / (GLfloat)winHeight;
	oldX = x + winWidth / 2.0;
	oldY = y + winHeight / 2.0;
	glutPostRedisplay();
}

// The main function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("villa");
	
	// Add texture information to the object
	brick = setTexture("brick.bmp");
	soil = setTexture("sand.bmp");
	grass = setTexture("grass.bmp");
	concrete = setTexture("concrete.bmp");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Recall the function
	glutDisplayFunc(display);
	glutIdleFunc(whenInMainloop);
	glutKeyboardFunc(keyboardOperation);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(time, onTimer, 1);
	glutMainLoop();
}