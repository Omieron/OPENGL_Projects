/*********
PROBLEMS: Sometimes arrow goes backward.
----------
ADDITIONAL FEATURES:  I added color reflection and when target disappear, arrow also disappear until you click the mouse
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
#include <stdarg.h>
#include <time.h>
#include "vec.h"

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  17 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

#define TARGET_SPEED 5

float A = 100, //genlik
fq = 0.5,  //frekas
C = 900,   //yatay gecis
B = 0;   //dikey gecis

typedef struct {

    double r, g, b;

}colour_t;   // color variables

typedef struct {

    vec_t coordinates;  // Coordinates of arrows

    vec_t vel; // velocity

    colour_t color;

    double rad;

    double angle;        // angle of arrow

    bool position;

    double yorunge; // ters ya da düz

    bool color_change;

    bool condition;

}arrow_t;

typedef struct {

    vec_t pos;

    bool right_left; // Check the direction of target // TRUE = LEFT,  FALSE = RIGHT

}target_t;

typedef struct {
    vec_t pos;
    vec_t N;
} vertex_t;

target_t positionoftarget = { { 0, 0}, true };

arrow_t OK = { {-1000,0}, {0,0,},{0.5,0.5,0.5},false, false, true, 1 , false, false };


colour_t mulColor(float k, colour_t c) {
    colour_t tmp = { k * c.r, k * c.g, k * c.b };
    return tmp;
}

colour_t addColor(colour_t c1, colour_t c2) {
    colour_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
    return tmp;
}

double distanceImpact(double d) {
    return (-1.0 / 350.0) * d + 1.0;
}

colour_t calculateColor(arrow_t source, vertex_t v) {
    vec_t L = subV(source.coordinates, v.pos);
    vec_t uL = unitV(L);
    float factor = dotP(uL, v.N) * distanceImpact(magV(L));
    return mulColor(factor, source.color);
}

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

void filled_circle(float x, float y, float r) {
    glBegin(GL_POLYGON);
    for (float angle = 0; angle <= 180; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
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

float quadratic_function(float x) {

    return (A * sin((fq * x + C) * D2R) + B);

}

// Apply translate (move) and rotation transformations
void vertex(vec_t P, vec_t Tr, double angle) {
    double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}

void background() {

    glColor3ub(19, 11, 85);
    glRectf(700, 400, -700, -400);

}

void arrow(arrow_t Arrow) {

    double aci = Arrow.angle * D2R;  // convert to radius type


    if (OK.color_change == false)
        glColor3ub(86, 86, 86);
    else
        glColor3ub(255, 125, 0);



    glBegin(GL_QUADS);  // Main body
    vertex({ 3, -20 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -3, -20 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -3, -120 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ 3, -120 * OK.yorunge }, Arrow.coordinates, aci);
    glEnd();

    glBegin(GL_QUADS);  // Right back tail
    vertex({ 4, -95 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ 4, -120 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ 9, -135 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ 9, -110 * OK.yorunge }, Arrow.coordinates, aci);
    glEnd();

    glBegin(GL_QUADS);  // left back tail
    vertex({ -4, -95 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -4, -120 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -9, -135 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -9, -110 * OK.yorunge }, Arrow.coordinates, aci);
    glEnd();

    glBegin(GL_QUADS);  // Right front tail
    vertex({ 4, -80 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ 4, -92 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ 9, -107 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ 9, -95 * OK.yorunge }, Arrow.coordinates, aci);
    glEnd();

    glBegin(GL_QUADS);  // Left front tail 
    vertex({ -4, -80 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -4, -92 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -9, -107 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -9, -95 * OK.yorunge }, Arrow.coordinates, aci);
    glEnd();

    glBegin(GL_TRIANGLES);  // Arrow body
    vertex({ 6, -20 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ -6, -20 * OK.yorunge }, Arrow.coordinates, aci);
    vertex({ 0, 0 * OK.yorunge }, Arrow.coordinates, aci);
    glEnd();


}

void target_board(target_t A) {

    glColor3ub(242, 27, 27);
    circle(A.pos.x, A.pos.y, 40);
    glColor3ub(240, 240, 240);
    circle(A.pos.x, A.pos.y, 30);
    glColor3ub(242, 27, 27);
    circle(A.pos.x, A.pos.y, 20);
    glColor3ub(240, 240, 240);
    circle(A.pos.x, A.pos.y, 10);


}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    background();

    target_board(positionoftarget);

    if (OK.condition == true)
        arrow(OK);

    for (int x = -700; x <= 700; x++) {
        vertex_t P = { { (double)x, -100.0 }, { 0, 1 } };

        colour_t res = { 0.34, 0.1, 0.01 };

        res = addColor(res, calculateColor(OK, P));

        //line segments will be used to paint the area in depth
        glBegin(GL_LINES);
        //from the surface
        glColor3f(res.r, res.g, res.b);
        glVertex2f(x, -200);
        //to the bottom
        glColor3ub(89, 26, 3);
        glVertex2f(x, -400);
        glEnd();
    }

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

        OK.coordinates.x = x - winWidth / 2;

        OK.coordinates.y = winHeight / 2 - y;

        OK.color_change = false;

        OK.condition = true;

        /************************************************************************************/
        if (fabs(OK.coordinates.y - positionoftarget.pos.y) < 50) // HORIZONTAL ARROW

            OK.angle = 90;

        else

            OK.angle = 0;  // NORMALLY


        /************************************************************************************/

        // UPSIDE OR DOWN

        if (OK.coordinates.y >= positionoftarget.pos.y) {

            OK.yorunge = -1;

            OK.position = false;

        }

        else {

            OK.yorunge = 1;

            OK.position = true;

        }

    }


    //  OK.coordinates = addV(OK.coordinates, OK.vel);

      // Reflection from Walls.
    if (OK.coordinates.x > 400 || OK.coordinates.x < -400) {
        OK.vel.x *= -1;
    }

    if (OK.coordinates.y > 240 || OK.coordinates.y < -90) {
        OK.vel.y *= -1;
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
    /************************************************************************************/


    // TARGET MOVEMENTS 

    positionoftarget.pos.y = quadratic_function(positionoftarget.pos.x);  // ORBIT OF TARGET

    if (positionoftarget.right_left == true) {

        if (positionoftarget.pos.x < 800)

            positionoftarget.pos.x += TARGET_SPEED;

        else {

            positionoftarget.right_left = !positionoftarget.right_left;

            positionoftarget.pos.x = rand() % ((850 + 1) - 750) + 750;

            OK.condition = false;

        }
    }

    else {

        if (positionoftarget.pos.x > -800)
            positionoftarget.pos.x -= TARGET_SPEED;

        else {

            positionoftarget.right_left = !positionoftarget.right_left;


            positionoftarget.pos.x = rand() % ((-750 + 1) - -850) + -850;

            OK.condition = false;

        }
    }

    /************************************************************************************/

    // ARROW's Movements

    if (OK.coordinates.y <= positionoftarget.pos.y && OK.coordinates.x <= positionoftarget.pos.x) {

        if (OK.position == true && OK.angle >= -90)

            OK.angle -= 1;

        if (OK.position == false && OK.angle <= 180)

            OK.angle += 1;

    }
    if (OK.coordinates.y >= positionoftarget.pos.y && OK.coordinates.x <= positionoftarget.pos.x) {

        if (OK.position == true && OK.angle >= -180)

            OK.angle -= 1;

        if (OK.position == false && OK.angle <= 90)

            OK.angle += 1;

    }

    if (OK.coordinates.y <= positionoftarget.pos.y && OK.coordinates.x >= positionoftarget.pos.x) {

        if (OK.position == true && OK.angle <= 90)

            OK.angle += 1;

        if (OK.position == false && OK.angle >= -180)

            OK.angle -= 1;

    }

    if (OK.coordinates.y >= positionoftarget.pos.y && OK.coordinates.x >= positionoftarget.pos.x) {

        if (OK.position == true && OK.angle <= 180)

            OK.angle += 1;

        if (OK.position == false && OK.angle >= -90)

            OK.angle -= 1;

    }

    /************************************************************************************/

    // ARROW LOCKING ON TARGET

    OK.vel = mulV(TARGET_SPEED + 5, unitV(subV(positionoftarget.pos, OK.coordinates)));

    OK.coordinates = addV(OK.coordinates, OK.vel);

    /************************************************************************************/

    if (positionoftarget.pos.x + 50 >= OK.coordinates.x && positionoftarget.pos.x - 50 <= OK.coordinates.x && positionoftarget.pos.y + 50 >= OK.coordinates.y && positionoftarget.pos.y - 50 <= OK.coordinates.y)

        OK.color_change = true;

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    srand(time(NULL)); // random root

}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Arrow and Shadow");

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