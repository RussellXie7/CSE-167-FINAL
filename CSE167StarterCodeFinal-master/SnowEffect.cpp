// This is to create snow effect, better to have only one instance in your scene

/**

Usage: include "SnowEffect.h" at the top of your Window.cpp, then, make sure you have shader.h:


 */

#include "SnowEffect.h"
#include "Window.h"

#include <iostream>

using namespace glm;


// particles
struct particle
{
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLfloat xd, yd, zd;
	GLfloat cs;
} p[10000];

GLfloat windspeed = 0.005f;
int winddir = 45;


SnowEffect::SnowEffect(){

	for (int i = 0; i<2000; i++)
	{
		if (p[i].y <= p[i].cs)
		{
			p[i].xd = -(rand() / 32767.0f - 0.5f) / 200.0f;
			p[i].zd = -(rand() / 32767.0f - 0.5f) / 200.0f;
			p[i].yd = -rand() / 32767.0f / 100.0f;
			p[i].x = (rand() / 32767.0f - 0.5f) * 3.0f;
			p[i].y = 1.2f * (rand() / 32767.0f + 0.5f);
			p[i].z = (rand() / 32767.0f - 0.5f) * 5.0f;
			p[i].b = rand() / 32767.0f;
			p[i].g = p[i].b;
			p[i].r = p[i].b;
			p[i].cs = -0.8f * (rand() / 32767.0f + 0.5f);
		}
	}
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

SnowEffect::~SnowEffect(){}

void SnowEffect::drawSnow() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -2.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(sin(winddir*.0174532925f)*windspeed*2.0f, cos(winddir*.0174532925f)*windspeed*2.0f, 0.0f);
	glEnd();
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i<2000; i++)
	{
		p[i].x += cos(winddir*.0174532925f)*windspeed;
		p[i].y += p[i].yd;
		p[i].z += sin(winddir*.0174532925f)*windspeed;
		p[i].yd -= rand() / 32767.0f / 10000.0f;

		if (p[i].y <= p[i].cs)
		{
			p[i].xd = -(rand() / 32767.0f - 0.5f) / 200.0f;
			p[i].zd = -(rand() / 32767.0f - 0.5f) / 200.0f;
			p[i].yd = -rand() / 32767.0f / 100.0f;
			p[i].x = (rand() / 32767.0f - 0.5f) * 5.0f;
			p[i].y = 1.2f * (rand() / 32767.0f + 0.5f);
			p[i].z = (rand() / 32767.0f - 0.5f) * 5.0f;
			p[i].b = rand() / 32767.0f;
			p[i].g = p[i].b;
			p[i].r = p[i].b;
			p[i].cs = -0.8f * (rand() / 32767.0f + 0.5f);
		}
		glColor3f(p[i].r, p[i].g, p[i].b);
		glVertex3f(p[i].x, p[i].y, p[i].z);
	}
	glEnd();
}

// controls
void SnowEffect::increase_windSpeed() {
	windspeed += 0.0005f;
}

void SnowEffect::decrease_windSpeed() {
	windspeed -= 0.0005f;
}

void SnowEffect::increase_windDirection() {
	winddir++;
}

void SnowEffect::decrease_windDirection() {
	winddir--;
}



