#include "GLOS.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <math.h>


void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
static GLfloat rotX = 20.0; 
static GLfloat inaltime = 0.0;
static GLfloat unghi_elice = 0.0;
static int spin = 0;

void CALLBACK movelight(AUX_EVENTREC* event)
{
    spin = (spin + 30) % 360;
}

void CALLBACK PrivesteSus(void) { rotX += 5.0; }
void CALLBACK PrivesteJos(void) { rotX -= 5.0; }
void CALLBACK Urca(void) { inaltime += 0.1; }
void CALLBACK Coboara(void) {inaltime -= 0.1;}

void CALLBACK InvarteElice(void) {
    unghi_elice += 15.0; 
    display();   
    Sleep(10);
}

void deseneazaEliceDubla() {
    glPushMatrix();
    glRotatef(unghi_elice, 0, 1, 0); 
    glColor3f(1.0, 1.0, 0.0);       
    auxSolidBox(0.8, 0.02, 0.1);
    glRotatef(90, 0, 1, 0);
    auxSolidBox(0.8, 0.02, 0.1);
    glPopMatrix();
}

void myinit(void) {
    glClearColor(0.2, 0.2, 0.2, 1.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_DEPTH_TEST);
}


void CALLBACK display(void) {
    GLfloat position[] = { 0.0, 3.0, 1.5, 1.0 };
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity();

    glTranslatef(0.0, inaltime, -5.0);
    glRotatef(rotX, 1.0, 0.0, 0.0);
    
    glPushMatrix();
    glRotated((GLdouble)spin, 2.0, 0.3, 0.0);
    glRotated(0.0, 1.0, 0.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, position); 	
    glTranslated(0.0, 0.0, 1.5);
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 1.0, 1.0);
    auxWireCube(0.1);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    /*
   //podeaua
   glDisable(GL_LIGHTING);
   glColor3f(0.3, 0.3, 0.3);
   glBegin(GL_QUADS);
   glVertex3f(-5, -2.0, -5);
   glVertex3f(5, -2.0, -5);
   glVertex3f(5, -2.0, 5);
   glVertex3f(-5, -2.0, 5);
   glEnd();
   glEnable(GL_LIGHTING);

   //umbra dronei
   glDisable(GL_LIGHTING);
   glPushMatrix();
   glTranslatef(0.0, -1.99, 0.0);
   glColor3f(0.1, 0.1, 0.1);
   glRotatef(90, 1, 0, 0);
   GLUquadricObj* q = gluNewQuadric();
   float razaUmbra = 0.6 / (1.0 + inaltime * 0.2);
   gluDisk(q, 0.0, razaUmbra, 32, 1);
   gluDeleteQuadric(q);
   glPopMatrix();
   glEnable(GL_LIGHTING);

   */

	//corpul dronei
    glPushMatrix();
    glColor3f(0.0, 0.5, 1.0);
    glTranslatef(0.0, -0.9, 0.0);
    auxSolidCylinder(0.5, 0.4);

    glRotatef(-90.0, 5.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 1.0);
    GLUquadricObj* quad = gluNewQuadric(); 
    gluDisk(quad, 0.0, 0.5, 32, 1);       

    glPopMatrix();

	//bratul 1
    glPushMatrix();
    glColor3f(0.4, 0.4, 0.4); 
    glRotatef(0, 0.0, 1.0, 0.0); 
    glTranslatef(0.0, 0.0, 0.0);  
    auxSolidBox(3.0, 0.1, 0.2);  
    glPopMatrix();

    //bratul 2
    glPushMatrix();
    glColor3f(0.4, 0.4, 0.4);
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.0);
    auxSolidBox(3.0, 0.1, 0.2);
    glPopMatrix();


    //ELICE PENTRU BRATUL 1
    glPushMatrix();
    glTranslatef(1.5, 0.05, 0.0);
    deseneazaEliceDubla();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, 0.05, 0.0);
    deseneazaEliceDubla();
    glPopMatrix();

    //ELICE PENTRU BRATUL 2 
  
    glPushMatrix();

    glRotatef(90, 0, 1, 0);
    
    glPushMatrix();
    glTranslatef(1.5, 0.05, 0.0);
    deseneazaEliceDubla();
    glPopMatrix();

    
    glPushMatrix();
    glTranslatef(-1.5, 0.05, 0.0);
    deseneazaEliceDubla();
    glPopMatrix();

    glPopMatrix();


    glFlush();
}


void CALLBACK myReshape(GLsizei w, GLsizei h) {
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    auxInitDisplayMode(AUX_SINGLE | AUX_RGB);
    auxInitPosition(0, 0, 800, 600);
    auxInitWindow("Proiect Drona");

    myinit();

    auxKeyFunc(AUX_UP, PrivesteSus);
    auxKeyFunc(AUX_DOWN, PrivesteJos);
    auxKeyFunc(AUX_RIGHT, Urca);
    auxKeyFunc(AUX_LEFT, Coboara);
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, movelight);

    auxIdleFunc(InvarteElice);
    auxReshapeFunc(myReshape);
    auxMainLoop(display);

    return 0;
}