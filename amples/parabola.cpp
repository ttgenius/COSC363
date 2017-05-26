#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <cmath>
#include <GL/freeglut.h>

// Real G = 0.0000000000667408
#define G    0.02 //Gravitational Constant

#define WIDTH 1536
#define HEIGHT 836
#define RATIO WIDTH/HEIGHT

struct Particle {
	float x;
	float y;
	float vx;
	float vy;
	float rad;
	float mass;
	float color[3];
};

struct Trace {
	float x;
	float y;
	float rad;
	float color[3];
};

struct Line {
	float x1;
	float y1;
	float x2;
	float y2;
};

void Timer(int = 0);
void Display();
void Mouse(int, int, int, int);
void MouseMotion(int, int);
void AddParticle(float, float, bool = true, float = 0, float = 0);
void AddTrace(float, float, float, bool = false);
void RemoveParticles();
void Keyboard(unsigned char, int, int);
void Resize(int width, int height);
void Evolve();

int mX, mY;
int gWin;
bool mLeft = false;
bool mRight = false;
bool mMiddle = false;
bool pause = false;
bool step = false;
bool trace = true;

std::vector<Particle> pVec;
std::vector<Trace> tVec;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(50, 50);
	gWin = glutCreateWindow("GravGL");

	glEnable(GL_MULTISAMPLE);

	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1 * WIDTH / 2, WIDTH / 2, HEIGHT / 2, -1 * HEIGHT / 2, 0, 1);

	glutDisplayFunc(Display);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Resize);
	Timer();

	glutMainLoop();
}

void Timer(int) {
	Display();

	if (mLeft) {
		AddParticle(1000000, 20, true, 0, 0);
		mLeft = false;
	}

	if (mRight){
		AddParticle(800000, 10, true, -200.f, 200.f);
		mRight = false;
	}

	if (mMiddle)
		RemoveParticles();

	if (trace && !pause) {
		for (size_t i = 0; i < pVec.size(); i++) {
			Particle &p = pVec[i];
			if (p.vx != 0 || p.vy != 0) AddTrace(p.x, p.y, p.rad, false);
		}
		if (tVec.size() > 100) {
			tVec.erase(tVec.begin());
		}
	}

	if (!pause)
		Evolve();
	else {
		if (step) {
			for (size_t i = 0; i < pVec.size(); i++) {
				Particle &p = pVec[i];
				if (p.vx != 0 || p.vy != 0) AddTrace(p.x, p.y, p.rad, false);
			}
			
			if (tVec.size() > 100) {
				tVec.erase(tVec.begin());
			}

			//for (int k = 0; k < 2; k++)
				Evolve();
			step = false;
		}
	}

	glutTimerFunc(1, Timer, 0);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw each trace in the vector
	for (size_t i = 0; i < tVec.size(); i++) {
		Trace &t = tVec[i];
		glColor3f(t.color[0], t.color[1], t.color[2]);
		glBegin(GL_POLYGON);
		for (float a = 0; a < 2 * M_PI; a += 0.2)
			glVertex2f(t.rad * cos(a) + t.x, t.rad * sin(a) + t.y);
		glEnd();
	}

	// Draw each particle in the vector
	for (size_t i = 0; i < pVec.size(); i++) {
		Particle &p = pVec[i];
		glColor3f(p.color[0], p.color[1], p.color[2]);
		glBegin(GL_POLYGON);
		for (float a = 0; a < 2 * M_PI; a += 0.2)
			glVertex2f(p.rad * cos(a) + p.x, p.rad * sin(a) + p.y);
		glEnd();
	}

	glFlush();
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	mX = x - WIDTH / 2;
	mY = y - HEIGHT / 2;

	if (button == GLUT_LEFT_BUTTON) mLeft = state == GLUT_DOWN;
	else if (button == GLUT_RIGHT_BUTTON) mRight = state == GLUT_DOWN;
	else if (button == GLUT_MIDDLE_BUTTON) mMiddle = state == GLUT_DOWN;
}

void MouseMotion(int x, int y) {
	mX = x - WIDTH / 2;
	mY = y - HEIGHT / 2;
}

void AddTrace(float x, float y, float r, bool randColor) {
	Trace t;
	t.x = x;
	t.y = y;
	t.rad = r / 2;
	if (randColor) {
		t.color[0] = rand() % 255 / 255.f;
		t.color[1] = rand() % 255 / 255.f;
		t.color[2] = rand() % 255 / 255.f;
	}
	else {
		t.color[0] = 34.f;
		t.color[1] = 34.f;
		t.color[2] = 34.f;
	}
	tVec.push_back(t);
}

void AddParticle(float m, float r, bool randColor, float vx, float vy) {
	Particle p;
	p.x = mX;
	p.y = mY;
	p.vx = vx / 30;
	p.vy = vy / 30;
	p.mass = m;
	p.rad = r;
	if (randColor) {
		p.color[0] = rand() % 255 / 255.f;
		p.color[1] = rand() % 255 / 255.f;
		p.color[2] = rand() % 255 / 255.f;
	}
	else {
		p.color[0] = 1;
		p.color[1] = 1;
		p.color[2] = 0;
	}
	pVec.push_back(p);
}

void RemoveParticles() {
	pVec.clear();
	tVec.clear();
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 0x1B:
		// Escape
		RemoveParticles();
		glutDestroyWindow(gWin);
		exit(0);
		break;
	case 'p':
		pause = !pause;
		break;
	case 'o':
		step = true;
		break;
	case 't':
		trace = !trace;
		break;
	case 'c':
		tVec.clear();
		break;
	}
}

void Resize(int width, int height) {
	glutReshapeWindow(WIDTH, HEIGHT);
}

void Evolve() {
	for (size_t i = 0; i < pVec.size(); i++) {
		Particle &p = pVec[i];
		bool destroy = false;
		
		for (size_t j = 0; j < pVec.size(); j++) {
			if (j == i || p.mass >= 1000000)
				continue;
			
			const Particle &p1 = pVec[j];
			
			// d = sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
			float d = sqrt((p1.x - p.x)*(p1.x - p.x) + (p1.y - p.y)*(p1.y - p.y));
			
			if (d > p1.rad && p.x > -2 * WIDTH && p.x < 2 * WIDTH && p.y > -2 * HEIGHT && p.y < 2 * HEIGHT) {
				// vx = G * mass / dist^2 * deltaX / dist
				// vy = G * mass / dist^2 * deltaY / dist
				p.vx += G * p1.mass / (d*d) * (p1.x - p.x) / d;
				p.vy += G * p1.mass / (d*d) * (p1.y - p.y) / d; 
			}
			else destroy = true;
		}
		
		if (destroy)
			pVec.erase(pVec.begin() + i);
		else {
			p.x += p.vx;
			p.y += p.vy;
		}
	}
}
