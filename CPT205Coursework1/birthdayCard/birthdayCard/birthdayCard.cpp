#define GLEW_STATIC
#define FREEGLUT_STATIC

#include <math.h>
#include <GL\glew.h>
#include <GL/freeglut.h>
#include <iostream>

double PI = 3.1415926;
bool candleFlag = true; // Set the candleFlag to control the ingite or extinguish of th candles
bool timeFlag1 = true; // Set a boolean parameter to control the direction of the swinging balloons
bool eyeFlag = true; // Set a boolean parameter to identify whether the cat opens or closes eyes
float bp = 0.9; // Set a balloon parameter which could control the size

int ba = 16; // Set a balloon parameter which could control the balloon angle
int time1 = 40; // Declare refresh interval of balloons in ms
int time2 = 40; // Declare refresh interval of clock in ms
int lp = 0; //To control the light
int candleFlame = 0; // Set the type of the candle flame
int cp = 0; // Set a clock pointer angle parameter to control clock pointers and cat's eyes

void display();
void keyboardInput(unsigned char, int, int);
void whenInMainloop();
void onTimer1(int);
void onTimer2(int);
void drawCake();
void drawCat();
void balloon1();
void balloon2();
void balloon3();
void drawClock();
void setLight();
void mouseInput(int, int, int, int);

// Create onTimer1 function to record the time of balloons swinging
void onTimer1(int Value) {
	if (timeFlag1 == true) {
		// Increase rotate angle
		ba++;
	}
	else if (timeFlag1 == false) {
		// Decrease rotate angle
		ba--;
	}
	if (ba > 15) {
		timeFlag1 = false;
		candleFlame = 0;
	}
	else if (ba < -15) {
		timeFlag1 = true;
		candleFlame = 1;
	}
	glutTimerFunc(time1, onTimer1, 1);
}

// Create onTimer2 function to record time of clock and cat's eyes
void onTimer2(int Value) {
	// Increase time
	cp += 3;
	if (cp % 180 < 20) {
		// Choose a certain time to make cat's eyes closed
		eyeFlag = false;
	}
	else if (cp % 180 >= 20){
		// Choose most of the time to make cat's eyes open
		eyeFlag = true;
	}
	glutTimerFunc(time2, onTimer2, 1);

}

// Create keyBoardInput function to adjust candles, balloons and clock
void keyboardInput(unsigned char key, int x, int y) {

	if (key == 'Q' || key == 'q') {
		exit(0);
	}
	else if (key == 'R' || key == 'r') {
		if (candleFlag == true) {
			candleFlag = false;
		}
		else {
			candleFlag = true;
		}
	}
	else if (key == 'B' || key == 'b') {
		if (bp < 1) {
			bp = bp + 0.1;
		}
	}
	else if (key == 'S' || key == 's') {
		if (bp > 0.2) {
			bp = bp - 0.1;
		}
	}
	else if (key == 'C' || key == 'c') {
		time2 -= 10;
		if (time2 < 20) {
			time2 = 80;
		}
	}

}

// Create moustInput to adjust the light
void mouseInput(int button, int state, int x, int y) { 

	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && lp == 3) {
		lp = 0;
	}	
	else if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		lp++;
	}
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON && lp == 0) {
		lp = 3;
	}
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
		lp--;
	}
	
}

// Idle callback function
void whenInMainloop() { 
	// Force OpenGL to redraw the current window
	glutPostRedisplay();
}

void printBitmapString(void* font, const char* s){
	if (s && strlen(s)) {
		while (*s) {
			glutBitmapCharacter(font, *s);
			s++;
		}
	}
}

int TextOut(float x, float y, const char* cstr){
	glRasterPos2f(x, y);
	printBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, cstr);
	return 1;
}

void display() {

	glClearColor(1.0, 0.3, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Set the Dimensions
	glutInitWindowSize(800, 500);
	glutInitWindowPosition(50, 50);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, 800, 0, 500);
	
	drawCake();
	drawCat();
	drawClock();
	
	glColor3f(0, 0, 1);
	TextOut(100, 400, "Happy Birthday!");

	glPushMatrix();
	balloon1();
	balloon2();
	balloon3();
	glPopMatrix();

	setLight();
	
	glutSwapBuffers();
}

// Draw cake and candles
void drawCake() {
	//Draw cake
	for (int m = 1; m <= 3; m++) {
		glColor3f(1, 1, 0);
		glBegin(GL_POLYGON);
			glVertex2f(25 + 25 * m, 75 * m);
			glVertex2f(25 + 25 * m, 0);
			glVertex2f(325 - 25 * m, 0);
			glVertex2f(325 - 25 * m, 75 * m);
		glEnd();
	}

	//Decorate the cake
	for (int m = 1; m <= 3; m++) {
		glColor3f(0, 0.5, 1);
		int n = 0;
		while (true) {
			glBegin(GL_POLYGON);
				for (int i = 0; i < 360; i++) {
					glVertex2f(50 + 25 * m + 50 * n + 25 * cos(PI + PI * i / 360), 75 * m + 25 * sin(PI + PI * i / 360));
				}
			glEnd();
			n++;
			if (50 + 25 * m + 50 * n > 300 - 25 * m) {
				break;
			}
		}
	}

	//Draw some candles
	for (int m = 0; m < 3; m++) {
		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
			glVertex2f(120 + 50 * m, 225);
			glVertex2f(130 + 50 * m, 225);
			glVertex2f(130 + 50 * m, 275);
			glVertex2f(120 + 50 * m, 275);
		glEnd();

		if (candleFlag == true) {
			if (candleFlame == 0) {
				glColor3f(1, 1, 0);
			}
			else if (candleFlame == 1) {
				glColor3f(1, 0.6, 0.6);
			}
			
			glBegin(GL_POLYGON);
				glVertex2f(125 + 50 * m, 275);
				glVertex2f(130 + 50 * m, 285);
				glVertex2f(125 + 50 * m, 295);
				glVertex2f(120 + 50 * m, 285);
			glEnd();
		}
	}

}

//Draw a cat
void drawCat() {
	//Draw a cat's body
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(390 + 60 * cos(PI * i / 360), 120 * sin(PI * i / 360));
		}
	glEnd();

	//Draw a cat's head
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(370 + 50 * cos(2 * PI * i / 360), 150 + 50 * sin(2 * PI * i / 360));
		}
	glEnd();

	//Draw a cat's ears
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
		glVertex2f(380 + 50 * cos(0.7 * PI), 150 + 50 * sin(0.7 * PI));
		glVertex2f(340 + 50 * cos(0.7 * PI), 160 + 50 * sin(0.7 * PI));
		glVertex2f(360 + 50 * cos(0.7 * PI), 150 + 50 * sin(1.1 * PI));
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
		glVertex2f(360 + 50 * cos(0.3 * PI), 150 + 50 * sin(0.3 * PI));
		glVertex2f(400 + 50 * cos(0.3 * PI), 160 + 50 * sin(0.3 * PI));
		glVertex2f(380 + 50 * cos(0.3 * PI), 150 + 50 * sin(-0.1 * PI));
	glEnd();

	// Draw a cat's eyes
	// Eyes open
	if (eyeFlag == true) {
		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
			for (int i = 0; i <= 360; i++) {
				glVertex2f(350 + 14 * cos(2 * PI * i / 360), 165 + 7 * sin(2 * PI * i / 360));
			}
		glEnd();

		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
			for (int i = 0; i <= 360; i++) {
				glVertex2f(390 + 14 * cos(2 * PI * i / 360), 165 + 7 * sin(2 * PI * i / 360));
			}
		glEnd();

		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
			for (int i = 0; i <= 360; i++) {
				glVertex2f(350 + 3.5 * cos(2 * PI * i / 360), 165 + 7 * sin(2 * PI * i / 360));
			}
		glEnd();

		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
			for (int i = 0; i <= 360; i++) {
				glVertex2f(390 + 3.5 * cos(2 * PI * i / 360), 165 + 7 * sin(2 * PI * i / 360));
			}
		glEnd();
	}
	// Eyes close
	else if (eyeFlag == false) {
		glColor3f(1, 1, 1);
		glLineWidth(2);
		glBegin(GL_LINES);
			glVertex2f(336, 165);
			glVertex2f(364, 165);
		glEnd();

		glBegin(GL_LINES);
			glVertex2f(376, 165);
			glVertex2f(404, 165);
		glEnd();

	}
	
	// Draw a cat's mouth
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(365 + 5 * cos(PI + PI * i / 360), 140 + 5 * sin(PI + PI * i / 360));
		}
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(375 + 5 * cos(PI + PI * i / 360), 140 + 5 * sin(PI + PI * i / 360));
		}
	glEnd();

	//Draw a cat's tail
	glColor3f(0, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(445 + 30 * cos(-0.5 * PI * i / 360), 60 + 60 * sin(-0.5 * PI * i / 360));
		}
	glEnd();

	glColor3f(0, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(505 + 30 * cos(0.5 * PI + 0.5 * PI * i / 360), 60 + 60 * sin(0.5 * PI + 0.5 * PI * i / 360));
		}
	glEnd();
	
}

//Draw some Balloons
void balloon1() {
	glPushMatrix();
	glTranslatef(625, 0, 0);
	glRotatef(ba, 0, 0, 1);
	
	glColor3f(1, 1, 1);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, 175 * bp);
	glEnd();
	
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(30 * cos(2 * PI * i / 360) * bp, (225 + 50 * sin(2 * PI * i / 360)) * bp);
		}
	glEnd();
	glPopMatrix();

}

void balloon2() {
	glPushMatrix();
	glTranslatef(625, 0, 0);
	glRotatef(20 + ba, 0, 0, 1);
	glColor3f(1, 1, 1);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, 175 * bp);
	glEnd();

	glColor3f(0, 1, 1);
	glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(30 * cos(2 * PI * i / 360) * bp, (225 + 50 * sin(2 * PI * i / 360)) * bp);
		}
	glEnd();
	glPopMatrix();

}

void balloon3() {
	glPushMatrix();
	glTranslatef(625, 0, 0);
	glRotatef(-20 + ba, 0, 0, 1);
	glColor3f(1, 1, 1);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, 175 * bp);
	glEnd();

	glColor3f(1, 0, 1);
	glBegin(GL_POLYGON);
		for (int i = 0; i <= 360; i++) {
			glVertex2f(30 * cos(2 * PI * i / 360) * bp, (225 + 50 * sin(2 * PI * i / 360)) * bp);
		}
	glEnd();
	glPopMatrix();

}

// Draw a clock
void drawClock() {
	glPushMatrix();
	glTranslatef(625, 400, 0);
	glColor3f(1, 1, 1);

	// Draw the outline of the clock
	glBegin(GL_POLYGON);
		for (int i = 0; i <= 3600; i++) {
			glVertex2f(75 * cos(2 * PI * i / 3600), 75 * sin(2 * PI * i / 3600));
		}
	glEnd();

	glColor3f(0, 0, 0);
	for (int i = 0; i < 12; i++) {
		glBegin(GL_LINES);
			glVertex2f(65 * cos(2 * PI * i / 12), 65 * sin(2 * PI * i / 12));
			glVertex2f(75 * cos(2 * PI * i / 12), 75 * sin(2 * PI * i / 12));
		glEnd();
	}
	glPopMatrix();

	// Draw a minute hand
	glPushMatrix();
	glTranslatef(625, 400, 0);
	glRotatef(-cp, 0, 0, 1);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, 65);
	glEnd();
	glPopMatrix();

	// Draw an hour hand
	glPushMatrix();
	glTranslatef(625, 400, 0);
	glRotatef(-cp / 12, 0, 0, 1);
	glLineWidth(4);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, 40);
	glEnd();
	glPopMatrix();

}

// Draw a light
void setLight() {
	glPushMatrix();
	glTranslatef(400, 500, 0);
	glColor3f(1, 1, 1);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, -100);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
		glVertex2f(-20, -100);
		glVertex2f(-40, -150);
		glVertex2f(40, -150);
		glVertex2f(20, -100);
	glEnd();

	// Set alpha channel transparency
	glEnable(GL_BLEND);  // Turn on hybrid mode
	glDisable(GL_DEPTH_TEST);  // Turn off depth mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1, 1, 1, 0.25 * lp);
	glBegin(GL_POLYGON);
		glVertex2f(-40, -150);
		glVertex2f(-200, -500);
		glVertex2f(200, -500);
		glVertex2f(40, -150);
	glEnd();
	glPopMatrix();

}

// The main function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// Set the Dimensions
	glutInitWindowSize(800, 500);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Birthday Card");

	glutDisplayFunc(display);
	glutIdleFunc(whenInMainloop);

	glutTimerFunc(time1, onTimer1, 1);
	glutTimerFunc(time2, onTimer2, 1);

	glutKeyboardFunc(keyboardInput);
	glutMouseFunc(mouseInput);

	glutMainLoop();

}