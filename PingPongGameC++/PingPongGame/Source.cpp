#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <sstream>

void draw();
void update(int value);
void drawDottedLines();
void changeViewPort(int w, int h);
void drawRacket(float x, float y, float width, float height);
void keyboard();
void drawBall(float x, float y, float width, float height);
void updateBall();
void vec2_norm(float& x, float &y);
void circleBall(float gx, float gy, float gd, float radius);
using namespace std;

#define VK_W 0x57
#define VK_S 0x53
# define PI           3.14159265358979323846  /* pi */
int width = 640, height = 480;
int interval = 1000 / 60;
//1000ms
int racket_width = 20;
int racket_height = 80;
int racket_speed = 3;

// left racket position
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

// right racket position
float racket_right_x = width - racket_width-10;
float racket_right_y = 50;

// ball
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 10;
int ball_speed = 4;


void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
	// initialize opengl (via glut)
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(30, 30);
	glutCreateWindow("Ping Pong Game");
	glutReshapeFunc(changeViewPort);
	glutDisplayFunc(draw);
	glutTimerFunc(interval, update, 0);
	// start the whole thing
	glutMainLoop();
	return 0;
}
void draw() {

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);

	/*glBegin(GL_POLYGON);
	glVertex2f(2, 1);
	glVertex2f(12, 1);
	glVertex2f(14, 3);
	glVertex2f(1, 3);
	glEnd();
	drawDottedLines();*/
	drawRacket(racket_left_x, racket_left_y, racket_width, racket_height);
	drawRacket(racket_right_x, racket_right_y, racket_width, racket_height);
	drawDottedLines();
	//drawBall(ball_pos_x, ball_pos_y, ball_size, ball_size);
	float gd = 2;
	float r = 10;
	circleBall(ball_pos_x, ball_pos_y, gd, ball_size);

	glFlush();
	glutSwapBuffers();
}
void update(int value) {
		
	keyboard();
	updateBall();
		// Call update() again in 'interval' milliseconds
		// Redisplay frame
	glutPostRedisplay();
		glutTimerFunc(interval, update, 0);

		
}
void drawDottedLines() {
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	for (int i = height; i+2>=0 ; i=i-10) {
		glVertex2f(width / 2, i);
		glVertex2f(width / 2, i+2);
	}
	glEnd();
}
void drawRacket(float x, float y, float width, float height) {
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}
void keyboard() {
	// left racket
	if (GetAsyncKeyState(VK_W) && (racket_left_y+racket_height)<=height) racket_left_y += racket_speed;
	if (GetAsyncKeyState(VK_S) && (racket_left_y) >= 0) racket_left_y -= racket_speed;

	// right racket
	if (GetAsyncKeyState(VK_UP) && (racket_right_y + racket_height) <= height) racket_right_y += racket_speed;
	if (GetAsyncKeyState(VK_DOWN) && (racket_right_y) >= 0) racket_right_y -= racket_speed;
}
void drawBall(float x, float y, float width, float height) {
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}
void updateBall() {
	// fly a bit
	ball_pos_x += ball_dir_x * ball_speed;
	ball_pos_y += ball_dir_y * ball_speed;
	// hit by left racket?
	if (ball_pos_x < racket_left_x + racket_width &&
		ball_pos_x > racket_left_x &&
		ball_pos_y < racket_left_y + racket_height &&
		ball_pos_y > racket_left_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = t;
	}

	// hit by right racket?
	if (ball_pos_x > racket_right_x &&
		ball_pos_x < racket_right_x + racket_width &&
		ball_pos_y < racket_right_y + racket_height &&
		ball_pos_y > racket_right_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = t;
	}

	// hit left wall?
	if (ball_pos_x < 0) {
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = 0;
	}

	// hit right wall?
	if (ball_pos_x > width) {
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = 0;
	}

	// hit top wall?
	if (ball_pos_y > height) {
		ball_dir_y = -fabs(ball_dir_y); // force it to be negative
	}

	// hit bottom wall?
	if (ball_pos_y < 0) {
		ball_dir_y = fabs(ball_dir_y); // force it to be positive
	}

	// make sure that length of dir stays at 1
	vec2_norm(ball_dir_x, ball_dir_y);
}
void vec2_norm(float& x, float &y) {
	// sets a vectors length to 1 (which means that x + y == 1)
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f) {
		length = 1.0f / length;
		x *= length;
		y *= length;
	}
}
void circleBall(float gx, float gy, float gd, float radius) {
	glColor3f(1, 1, 1);
	for (float a = 0; a < 2 * PI; a += 0.01) {
		glBegin(GL_POLYGON);
		glVertex2f(gx + cos(a)*radius, gy + sin(a)*radius);
		glVertex2f(gx + cos(a + 0.01)*radius, gy + sin(a + 0.01)*radius);
		glVertex2f(gx, gy);
		glEnd();
	}
}