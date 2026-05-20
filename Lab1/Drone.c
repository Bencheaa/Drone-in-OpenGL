#include "GLOS.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <math.h>


void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

static GLfloat rotX = 0.0;
static GLfloat inaltime = 0.0;
static GLfloat unghi_elice = 0.0;
static int spin = 0;
static GLfloat pozSursa[] = { 3.0, 5.0, 1.0, 1.0 };

void CALLBACK movelight(AUX_EVENTREC* event)
{
    spin = (spin + 30) % 360;
}
void CALLBACK PrivesteSus(void) { rotX += 5.0; }
void CALLBACK PrivesteJos(void) { rotX -= 5.0; }
void CALLBACK Urca(void) { inaltime += 0.1; }
void CALLBACK Coboara(void) { inaltime -= 0.1; }
void CALLBACK LuminaStanga() { pozSursa[0] -= 0.5;}
void CALLBACK LuminaDreapta() {pozSursa[0] += 0.5;}
void CALLBACK InvarteElice(void) {
    unghi_elice += 20.0;
    display();
    Sleep(10);
}

void calcCoeficientiPlan(float P[3][3], float coef[4]) {
    float v1[3], v2[3], length;
    static const int x = 0, y = 1, z = 2;

    v1[x] = P[0][x] - P[1][x];
    v1[y] = P[0][y] - P[1][y];
    v1[z] = P[0][z] - P[1][z];

    v2[x] = P[1][x] - P[2][x];
    v2[y] = P[1][y] - P[2][y];
    v2[z] = P[1][z] - P[2][z];

    coef[x] = v1[y] * v2[z] - v1[z] * v2[y];
    coef[y] = v1[z] * v2[x] - v1[x] * v2[z];
    coef[z] = v1[x] * v2[y] - v1[y] * v2[x]; 

    length = (float)sqrt((coef[x] * coef[x]) + (coef[y] * coef[y]) + (coef[z] * coef[z]));
    coef[x] /= length; coef[y] /= length; coef[z] /= length;
}

void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4]) {
    GLfloat coefPlan[4], temp;
    calcCoeficientiPlan(puncte, coefPlan);
    coefPlan[3] = -((coefPlan[0] * puncte[2][0]) + (coefPlan[1] * puncte[2][1]) + (coefPlan[2] * puncte[2][2]));

    temp = coefPlan[0] * pozSursa[0] + coefPlan[1] * pozSursa[1] + coefPlan[2] * pozSursa[2] + coefPlan[3] * pozSursa[3];

    mat[0][0] = temp - pozSursa[0] * coefPlan[0];
    mat[1][0] = 0.0f - pozSursa[0] * coefPlan[1];
    mat[2][0] = 0.0f - pozSursa[0] * coefPlan[2];
    mat[3][0] = 0.0f - pozSursa[0] * coefPlan[3];

    mat[0][1] = 0.0f - pozSursa[1] * coefPlan[0];
    mat[1][1] = temp - pozSursa[1] * coefPlan[1];
    mat[2][1] = 0.0f - pozSursa[1] * coefPlan[2];
    mat[3][1] = 0.0f - pozSursa[1] * coefPlan[3];

    mat[0][2] = 0.0f - pozSursa[2] * coefPlan[0];
    mat[1][2] = 0.0f - pozSursa[2] * coefPlan[1];
    mat[2][2] = temp - pozSursa[2] * coefPlan[2];
    mat[3][2] = 0.0f - pozSursa[2] * coefPlan[3];

    mat[0][3] = 0.0f - pozSursa[3] * coefPlan[0];
    mat[1][3] = 0.0f - pozSursa[3] * coefPlan[1];
    mat[2][3] = 0.0f - pozSursa[3] * coefPlan[2];
    mat[3][3] = temp - pozSursa[3] * coefPlan[3];
}

void deseneazaEliceDubla() {
    glPushMatrix();
    glRotatef(unghi_elice, 0, 1, 0); 
    //glColor3f(1.0, 1.0, 0.0);       
    auxSolidBox(0.8, 0.02, 0.1);
    glRotatef(90, 0, 1, 0);
    auxSolidBox(0.8, 0.02, 0.1);
    glPopMatrix();
}

void deseneazaDrona(){
    //corpul dronei
    glPushMatrix();
    //glColor3f(0.0, 0.5, 1.0);
    glTranslatef(0.0, -0.9, 0.0);
    auxSolidCylinder(0.5, 0.4);

    glRotatef(-90.0, 5.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 1.0);
    GLUquadricObj* quad = gluNewQuadric();
    gluDisk(quad, 0.0, 0.5, 32, 1);

    glPopMatrix();

	//bratele dronei
    glPushMatrix();
    //glColor3f(0.4, 0.4, 0.4);
    glRotatef(0, 0.0, 1.0, 0.0);
    auxSolidBox(3.0, 0.1, 0.2);
    glRotatef(90, 0.0, 1.0, 0.0);
    auxSolidBox(3.0, 0.1, 0.2);
    glPopMatrix();

	//elice bratul 1
    glPushMatrix();
    glTranslatef(1.5, 0.05, 0.0);
    deseneazaEliceDubla();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, 0.05, 0.0);
    deseneazaEliceDubla();
    glPopMatrix();

    //elice bratul 2

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
}

void myinit(void) {
    glClearColor(0.6, 0.8, 1.0, 1.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}


void CALLBACK display(void) {
    GLfloat matUmbra[4][4];
    GLfloat punctePlan[3][3] = {
    { 50.0f, -1.99f,  50.0f}, 
    { 50.0f, -1.99f, -50.0f},
    {-50.0f, -1.99f, -50.0f}  
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -15.0);
    glRotatef(rotX, 1.0, 0.0, 0.0);

	//lumina
    glPushMatrix();
    glRotated((GLdouble)spin, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozSursa);
    glDisable(GL_LIGHTING);
    glTranslatef(pozSursa[0], pozSursa[1], pozSursa[2]);
    glColor3f(1.0, 1.0, 0.0);
    auxSolidSphere(0.5);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    //iarba/podeaua
    glDisable(GL_LIGHTING);
    glColor3f(0.1, 0.5, 0.1);
    glBegin(GL_QUADS);
    glVertex3f(-50.0, -2.0, -50.0); 
    glVertex3f(50.0, -2.0, -50.0);
    glVertex3f(50.0, -2.0, 50.0); 
    glVertex3f(-50.0, -2.0, 50.0);
    glEnd();
    glEnable(GL_LIGHTING);

    //umbra dronei
    MatriceUmbra(punctePlan, pozSursa, matUmbra);
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glRotated((GLdouble)spin, 0.0, 1.0, 0.0);
	glMultMatrixf((GLfloat*)matUmbra);//proiecteaza umbra
    glTranslatef(0.0, inaltime, 0.0);
    glColor3f(0.0, 0.0, 0.0); 
    deseneazaDrona();
    glPopMatrix();
    glPopAttrib();

    //desenare drona
    glPushMatrix();
    glTranslatef(0.0, inaltime, 0.0);
    glColor3f(0.5, 0.5, 0.5);
    deseneazaDrona();
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
    auxInitDisplayMode(AUX_SINGLE | AUX_RGB | AUX_DEPTH);
    auxInitPosition(0, 0, 800, 600);
    auxInitWindow("Proiect Drona");

    myinit();

    auxKeyFunc(AUX_UP, PrivesteSus);
    auxKeyFunc(AUX_DOWN, PrivesteJos);
    auxKeyFunc(AUX_RIGHT, Urca);
    auxKeyFunc(AUX_LEFT, Coboara);
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, movelight);
    auxKeyFunc('A', LuminaStanga);
    auxKeyFunc('D', LuminaDreapta);

    auxIdleFunc(InvarteElice);
    auxReshapeFunc(myReshape);
    auxMainLoop(display);

    return 0;
}