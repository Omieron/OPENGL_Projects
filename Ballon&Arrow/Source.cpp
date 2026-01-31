/*********
PROBLEMS:  Sometimes arrow can slip out of orbit, except this there are not any problems
----------
ADDITIONAL FEATURES: When you click the scren baloon color changes.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  17 // Period for the timer.
#define TIMER_ON        1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {

    int r, g, b;

}color_t;


typedef struct {

    vec_t pos;

    vec_t vel;

    color_t renk;

    bool color_change;

    bool right_left; // right false, left true

}baloon_t;

typedef struct {

    vec_t pos;

    vec_t vel;

    double angle;

    bool touch;  // touch or not 

    bool show; // display or not

}dart_t;

baloon_t balon = { {-600,200}, {0,0}, {255,0,0}, false, true };

dart_t dart = { {0,0}, {0,0}, 0 , false, false };

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void vertex(vec_t P, vec_t Tr, double angle) {
    double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}

void baloon(baloon_t B) {

    // rope

    glColor3ub(125, 125, 125);
    glBegin(GL_QUADS);
    vertex({ 2, -60 }, B.pos, 0);
    vertex({ -2, -60 }, B.pos, 0);
    vertex({ -2, -90 }, B.pos, 0);
    vertex({ 2, -90 }, B.pos, 0);
    glEnd();

    circle(B.pos.x, B.pos.y - 60, 2);


    // Main Body

    glColor3ub(balon.renk.r, balon.renk.g, balon.renk.b);
    circle(B.pos.x, B.pos.y, 50);

    glBegin(GL_TRIANGLES);
    vertex({ 0,-15 }, B.pos, 0);
    vertex({ 14,-60 }, B.pos, 0);
    vertex({ -14,-60 }, B.pos, 0);
    glEnd();



}

void dart_func(dart_t D) {

    double angle = D.angle * D2R;

    // Head of dart

    if (dart.touch == false)
        glColor3ub(50, 50, 50);
    else
        glColor3ub(250, 25, 0);

    glBegin(GL_TRIANGLES);
    vertex({ 0, 0 }, D.pos, angle);
    vertex({ 10, -15 }, D.pos, angle);
    vertex({ -10, -15 }, D.pos, angle);
    glEnd();

    // limb o front part

    glBegin(GL_QUADS);
    vertex({ 2, -12 }, D.pos, angle);
    vertex({ -2, -12 }, D.pos, angle);
    vertex({ -2, -30 }, D.pos, angle);
    vertex({ 2, -30 }, D.pos, angle);
    glEnd();

    // main patr of dart

    glBegin(GL_QUADS);
    vertex({ -5, -23 }, D.pos, angle);
    vertex({ 5, -23 }, D.pos, angle);
    vertex({ 5, -47 }, D.pos, angle);
    vertex({ -5, -47 }, D.pos, angle);
    glEnd();

    // back part 

    glBegin(GL_QUADS);
    vertex({ 2, -50 }, D.pos, angle);
    vertex({ -2, -50 }, D.pos, angle);
    vertex({ -2, -70 }, D.pos, angle);
    vertex({ 2, -70 }, D.pos, angle);
    glEnd();

    // Feather

    glBegin(GL_POLYGON);
    vertex({ -2, -60 }, D.pos, angle);
    vertex({ -7, -70 }, D.pos, angle);
    vertex({ -7, -95 }, D.pos, angle);
    vertex({ 0, -75 }, D.pos, angle);
    vertex({ 7, -95 }, D.pos, angle);
    vertex({ 7, -70 }, D.pos, angle);
    vertex({ 2, -60 }, D.pos, angle);
    glEnd();

}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0.03, 0.67, 0.6, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3ub(255, 255, 0);
    circle(-300, 200, 50);


    if (dart.show == true)

        dart_func(dart);

    glColor3ub(42, 2, 173);
    glRectf(-500, -90, 500, -300);


    baloon(balon);


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.


    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {

        dart.pos.x = x - winWidth / 2;

        dart.pos.y = winHeight / 2 - y;

        dart.angle = 0;

        dart.show = true;

        dart.touch = false;

        balon.color_change = true;

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.


    // Movemets of baloon

    if (balon.pos.x < 600 && balon.right_left == true)

        balon.pos.x += 5;

    if (balon.pos.x >= 600) {

        balon.pos.x = 600;

        balon.right_left = false;

        dart.show = false;

    }

    if (balon.pos.x > -600 && balon.right_left == false)

        balon.pos.x -= 5;

    if (balon.pos.x <= -600) {

        balon.pos.x = -600;

        balon.right_left = true;

        dart.show = false;

    }

    if (dart.pos.x < balon.pos.x)

        if (dart.angle > -115)

            dart.angle--;

    if (dart.pos.x > balon.pos.x)

        if (dart.angle < 115)

            dart.angle++;


    if (balon.pos.y + 50 >= dart.pos.y && balon.pos.y - 50 <= dart.pos.y && balon.pos.x - 50 <= dart.pos.x && balon.pos.x + 50 >= dart.pos.x)  // color change

        dart.touch = true;

    if (dart.pos.x > 500 && dart.pos.x < -500 && dart.pos.y < -300 && dart.pos.y > 300)

        dart.show = false;



    // Chase

    dart.vel = mulV(8, unitV(subV(balon.pos, dart.pos)));

    dart.pos = addV(dart.pos, dart.vel);

    if (balon.color_change == true) {


        balon.renk.r = rand() % 256;

        balon.renk.g = rand() % 256;

        balon.renk.b = rand() % 256;

        balon.color_change = false;

    }



    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(time(NULL));

}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("BallonandArrow");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}