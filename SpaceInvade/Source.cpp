/*********
PROBLEMS:

To be honest I don't have any idea about time func, beacuse of this I didn't do timer function, and also instead of remove enemy I added bomb affect!
----------
ADDITIONAL FEATURES:
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

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

// MY VARIABLES

bool condition = true, bullet_condition = false, activetime = true;

int bulletx = 0, bullety = -226;

typedef struct {

    int x, y;

}points_t;

typedef struct {

    points_t coordinate;
    int area_radius;
    bool condition;
    bool touching;

}assets_t;

assets_t Player = { 0, -250, 18 }, aliens[5];

typedef struct {

    int colR;
    int colG;
    int colB;

}color_t;

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

void pixel(int x, int y, color_t code) {

    glColor3ub(code.colR, code.colG, code.colB);
    glRectf(x, y, x + 3, y + 3);
}

void spacecraft(assets_t* q) {

    color_t green = { 0,255,0 },
        dark_green = { 21,150,19 },
        black = { 0,0,0 },
        white = { 255,255,255 },
        dark_grey = { 88,88,88 },
        grey = { 128,128,128 },
        yellow = { 255,242,0 },
        orange = { 255,127,39 },
        red = { 255,0,0 },
        blue = { 0,0,255 };

    int gap = -3, locx, locy;

    q->area_radius = 20;

    locx = q->coordinate.x;
    locy = q->coordinate.y;

    for (gap = -3; gap < 4; gap += 6) {

        pixel(gap * 0 + locx, 57 + locy, black);
        pixel(gap * 0 + locx, 54 + locy, grey);
        pixel(gap * 1 + locx, 54 + locy, black);
        pixel(gap * 0 + locx, 51 + locy, blue);
        pixel(gap * 1 + locx, 51 + locy, grey);
        pixel(gap * 2 + locx, 51 + locy, black);
        pixel(gap * 0 + locx, 48 + locy, blue);
        pixel(gap * 1 + locx, 48 + locy, black);
        pixel(gap * 2 + locx, 48 + locy, grey);
        pixel(gap * 3 + locx, 48 + locy, black);
        pixel(gap * 0 + locx, 45 + locy, blue);
        pixel(gap * 1 + locx, 45 + locy, black);
        pixel(gap * 2 + locx, 45 + locy, grey);
        pixel(gap * 3 + locx, 45 + locy, grey);
        pixel(gap * 4 + locx, 45 + locy, black);
        pixel(gap * 0 + locx, 42 + locy, white);
        pixel(gap * 1 + locx, 42 + locy, black);
        pixel(gap * 2 + locx, 42 + locy, grey);
        pixel(gap * 3 + locx, 42 + locy, grey);
        pixel(gap * 4 + locx, 42 + locy, dark_grey);
        pixel(gap * 5 + locx, 42 + locy, black);
        pixel(gap * 0 + locx, 39 + locy, blue);
        pixel(gap * 1 + locx, 39 + locy, black);
        pixel(gap * 2 + locx, 39 + locy, grey);
        pixel(gap * 3 + locx, 39 + locy, grey);
        pixel(gap * 4 + locx, 39 + locy, dark_grey);
        pixel(gap * 5 + locx, 39 + locy, black);
        pixel(gap * 0 + locx, 36 + locy, black);
        pixel(gap * 1 + locx, 36 + locy, grey);
        pixel(gap * 2 + locx, 36 + locy, grey);
        pixel(gap * 3 + locx, 36 + locy, grey);
        pixel(gap * 4 + locx, 36 + locy, dark_grey);
        pixel(gap * 5 + locx, 36 + locy, black);
        pixel(gap * 0 + locx, 33 + locy, grey);
        pixel(gap * 1 + locx, 33 + locy, grey);
        pixel(gap * 2 + locx, 33 + locy, grey);
        pixel(gap * 3 + locx, 33 + locy, grey);
        pixel(gap * 4 + locx, 33 + locy, dark_grey);
        pixel(gap * 5 + locx, 33 + locy, black);
        pixel(gap * 0 + locx, 30 + locy, grey);
        pixel(gap * 1 + locx, 30 + locy, grey);
        pixel(gap * 2 + locx, 30 + locy, grey);
        pixel(gap * 3 + locx, 30 + locy, grey);
        pixel(gap * 4 + locx, 30 + locy, dark_grey);
        pixel(gap * 5 + locx, 30 + locy, black);
        pixel(gap * 0 + locx, 27 + locy, black);
        pixel(gap * 1 + locx, 27 + locy, grey);
        pixel(gap * 2 + locx, 27 + locy, grey);
        pixel(gap * 3 + locx, 27 + locy, grey);
        pixel(gap * 4 + locx, 27 + locy, dark_grey);
        pixel(gap * 5 + locx, 27 + locy, black);
        pixel(gap * 0 + locx, 24 + locy, green);
        pixel(gap * 1 + locx, 24 + locy, black);
        pixel(gap * 2 + locx, 24 + locy, grey);
        pixel(gap * 3 + locx, 24 + locy, grey);
        pixel(gap * 4 + locx, 24 + locy, dark_grey);
        pixel(gap * 5 + locx, 24 + locy, black);
        pixel(gap * 8 + locx, 24 + locy, black);
        pixel(gap * 0 + locx, 21 + locy, green);
        pixel(gap * 1 + locx, 21 + locy, black);
        pixel(gap * 2 + locx, 21 + locy, grey);
        pixel(gap * 3 + locx, 21 + locy, grey);
        pixel(gap * 4 + locx, 21 + locy, dark_grey);
        pixel(gap * 5 + locx, 21 + locy, black);
        pixel(gap * 8 + locx, 21 + locy, black);
        pixel(gap * 0 + locx, 18 + locy, green);
        pixel(gap * 1 + locx, 18 + locy, black);
        pixel(gap * 2 + locx, 18 + locy, grey);
        pixel(gap * 3 + locx, 18 + locy, grey);
        pixel(gap * 4 + locx, 18 + locy, black);
        pixel(gap * 5 + locx, 18 + locy, white);
        pixel(gap * 6 + locx, 18 + locy, black);
        pixel(gap * 8 + locx, 18 + locy, black);
        pixel(gap * 0 + locx, 15 + locy, grey);
        pixel(gap * 1 + locx, 15 + locy, black);
        pixel(gap * 2 + locx, 15 + locy, grey);
        pixel(gap * 3 + locx, 15 + locy, black);
        pixel(gap * 4 + locx, 15 + locy, dark_grey);
        pixel(gap * 5 + locx, 15 + locy, white);
        pixel(gap * 6 + locx, 15 + locy, black);
        pixel(gap * 8 + locx, 15 + locy, black);
        pixel(gap * 0 + locx, 12 + locy, green);
        pixel(gap * 1 + locx, 12 + locy, black);
        pixel(gap * 2 + locx, 12 + locy, grey);
        pixel(gap * 3 + locx, 12 + locy, black);
        pixel(gap * 4 + locx, 12 + locy, dark_grey);
        pixel(gap * 5 + locx, 12 + locy, grey);
        pixel(gap * 6 + locx, 12 + locy, white);
        pixel(gap * 7 + locx, 12 + locy, black);
        pixel(gap * 8 + locx, 12 + locy, black);
        pixel(gap * 0 + locx, 9 + locy, green);
        pixel(gap * 1 + locx, 9 + locy, green);
        pixel(gap * 2 + locx, 9 + locy, black);
        pixel(gap * 3 + locx, 9 + locy, black);
        pixel(gap * 4 + locx, 9 + locy, dark_grey);
        pixel(gap * 5 + locx, 9 + locy, grey);
        pixel(gap * 6 + locx, 9 + locy, white);
        pixel(gap * 7 + locx, 9 + locy, black);
        pixel(gap * 8 + locx, 9 + locy, black);
        pixel(gap * 0 + locx, 6 + locy, green);
        pixel(gap * 1 + locx, 6 + locy, green);
        pixel(gap * 2 + locx, 6 + locy, green);
        pixel(gap * 3 + locx, 6 + locy, black);
        pixel(gap * 4 + locx, 6 + locy, dark_grey);
        pixel(gap * 5 + locx, 6 + locy, grey);
        pixel(gap * 6 + locx, 6 + locy, grey);
        pixel(gap * 7 + locx, 6 + locy, white);
        pixel(gap * 8 + locx, 6 + locy, black);
        pixel(gap * 0 + locx, 3 + locy, green);
        pixel(gap * 1 + locx, 3 + locy, green);
        pixel(gap * 2 + locx, 3 + locy, green);
        pixel(gap * 3 + locx, 3 + locy, black);
        pixel(gap * 4 + locx, 3 + locy, dark_grey);
        pixel(gap * 5 + locx, 3 + locy, grey);
        pixel(gap * 6 + locx, 3 + locy, grey);
        pixel(gap * 7 + locx, 3 + locy, grey);
        pixel(gap * 8 + locx, 3 + locy, white);
        pixel(gap * 9 + locx, 3 + locy, black);
        pixel(gap * 0 + locx, 0 + locy, green);
        pixel(gap * 1 + locx, 0 + locy, green);
        pixel(gap * 2 + locx, 0 + locy, green);
        pixel(gap * 3 + locx, 0 + locy, black);
        pixel(gap * 4 + locx, 0 + locy, dark_grey);
        pixel(gap * 5 + locx, 0 + locy, grey);
        pixel(gap * 6 + locx, 0 + locy, grey);
        pixel(gap * 7 + locx, 0 + locy, grey);
        pixel(gap * 8 + locx, 0 + locy, white);
        pixel(gap * 9 + locx, 0 + locy, black);
        pixel(gap * 0 + locx, -3 + locy, green);
        pixel(gap * 1 + locx, -3 + locy, green);
        pixel(gap * 2 + locx, -3 + locy, green);
        pixel(gap * 3 + locx, -3 + locy, black);
        pixel(gap * 4 + locx, -3 + locy, dark_grey);
        pixel(gap * 5 + locx, -3 + locy, grey);
        pixel(gap * 6 + locx, -3 + locy, grey);
        pixel(gap * 7 + locx, -3 + locy, grey);
        pixel(gap * 8 + locx, -3 + locy, grey);
        pixel(gap * 9 + locx, -3 + locy, white);
        pixel(gap * 10 + locx, -3 + locy, black);
        pixel(gap * 0 + locx, -6 + locy, green);
        pixel(gap * 1 + locx, -6 + locy, green);
        pixel(gap * 2 + locx, -6 + locy, black);
        pixel(gap * 3 + locx, -6 + locy, dark_grey);
        pixel(gap * 4 + locx, -6 + locy, black);
        pixel(gap * 5 + locx, -6 + locy, grey);
        pixel(gap * 6 + locx, -6 + locy, grey);
        pixel(gap * 7 + locx, -6 + locy, grey);
        pixel(gap * 8 + locx, -6 + locy, grey);
        pixel(gap * 9 + locx, -6 + locy, grey);
        pixel(gap * 10 + locx, -6 + locy, white);
        pixel(gap * 11 + locx, -6 + locy, black);
        pixel(gap * 0 + locx, -9 + locy, green);
        pixel(gap * 1 + locx, -9 + locy, black);
        pixel(gap * 2 + locx, -9 + locy, grey);
        pixel(gap * 3 + locx, -9 + locy, dark_grey);
        pixel(gap * 4 + locx, -9 + locy, black);
        pixel(gap * 5 + locx, -9 + locy, black);
        pixel(gap * 6 + locx, -9 + locy, grey);
        pixel(gap * 7 + locx, -9 + locy, grey);
        pixel(gap * 8 + locx, -9 + locy, grey);
        pixel(gap * 9 + locx, -9 + locy, grey);
        pixel(gap * 10 + locx, -9 + locy, grey);
        pixel(gap * 11 + locx, -9 + locy, white);
        pixel(gap * 12 + locx, -9 + locy, black);
        pixel(gap * 0 + locx, -12 + locy, green);
        pixel(gap * 1 + locx, -12 + locy, black);
        pixel(gap * 2 + locx, -12 + locy, grey);
        pixel(gap * 3 + locx, -12 + locy, dark_grey);
        pixel(gap * 4 + locx, -12 + locy, black);
        pixel(gap * 5 + locx, -12 + locy, dark_grey);
        pixel(gap * 6 + locx, -12 + locy, black);
        pixel(gap * 7 + locx, -12 + locy, grey);
        pixel(gap * 8 + locx, -12 + locy, grey);
        pixel(gap * 9 + locx, -12 + locy, grey);
        pixel(gap * 10 + locx, -12 + locy, grey);
        pixel(gap * 11 + locx, -12 + locy, grey);
        pixel(gap * 12 + locx, -12 + locy, white);
        pixel(gap * 13 + locx, -12 + locy, black);
        pixel(gap * 0 + locx, -15 + locy, green);
        pixel(gap * 1 + locx, -15 + locy, black);
        pixel(gap * 2 + locx, -15 + locy, grey);
        pixel(gap * 3 + locx, -15 + locy, dark_grey);
        pixel(gap * 4 + locx, -15 + locy, black);
        pixel(gap * 5 + locx, -15 + locy, dark_grey);
        pixel(gap * 6 + locx, -15 + locy, grey);
        pixel(gap * 7 + locx, -15 + locy, black);
        pixel(gap * 8 + locx, -15 + locy, grey);
        pixel(gap * 9 + locx, -15 + locy, grey);
        pixel(gap * 10 + locx, -15 + locy, grey);
        pixel(gap * 11 + locx, -15 + locy, grey);
        pixel(gap * 12 + locx, -15 + locy, grey);
        pixel(gap * 13 + locx, -15 + locy, black);
        pixel(gap * 14 + locx, -15 + locy, black);
        pixel(gap * 0 + locx, -18 + locy, grey);
        pixel(gap * 1 + locx, -18 + locy, black);
        pixel(gap * 2 + locx, -18 + locy, black);
        pixel(gap * 3 + locx, -18 + locy, black);
        pixel(gap * 4 + locx, -18 + locy, black);
        pixel(gap * 5 + locx, -18 + locy, dark_grey);
        pixel(gap * 6 + locx, -18 + locy, grey);
        pixel(gap * 7 + locx, -18 + locy, grey);
        pixel(gap * 8 + locx, -18 + locy, black);
        pixel(gap * 9 + locx, -18 + locy, grey);
        pixel(gap * 10 + locx, -18 + locy, grey);
        pixel(gap * 11 + locx, -18 + locy, grey);
        pixel(gap * 12 + locx, -18 + locy, grey);
        pixel(gap * 13 + locx, -18 + locy, black);
        pixel(gap * 14 + locx, -18 + locy, green);
        pixel(gap * 15 + locx, -18 + locy, black);
        pixel(gap * 0 + locx, -21 + locy, green);
        pixel(gap * 1 + locx, -21 + locy, black);
        pixel(gap * 2 + locx, -21 + locy, grey);
        pixel(gap * 3 + locx, -21 + locy, dark_grey);
        pixel(gap * 4 + locx, -21 + locy, black);
        pixel(gap * 5 + locx, -21 + locy, dark_grey);
        pixel(gap * 6 + locx, -21 + locy, grey);
        pixel(gap * 7 + locx, -21 + locy, grey);
        pixel(gap * 8 + locx, -21 + locy, black);
        pixel(gap * 9 + locx, -21 + locy, black);
        pixel(gap * 10 + locx, -21 + locy, grey);
        pixel(gap * 11 + locx, -21 + locy, grey);
        pixel(gap * 12 + locx, -21 + locy, grey);
        pixel(gap * 13 + locx, -21 + locy, black);
        pixel(gap * 14 + locx, -21 + locy, green);
        pixel(gap * 15 + locx, -21 + locy, black);
        pixel(gap * 0 + locx, -24 + locy, green);
        pixel(gap * 1 + locx, -24 + locy, black);
        pixel(gap * 2 + locx, -24 + locy, grey);
        pixel(gap * 3 + locx, -24 + locy, dark_grey);
        pixel(gap * 4 + locx, -24 + locy, black);
        pixel(gap * 5 + locx, -24 + locy, dark_grey);
        pixel(gap * 6 + locx, -24 + locy, grey);
        pixel(gap * 7 + locx, -24 + locy, grey);
        pixel(gap * 8 + locx, -24 + locy, black);
        pixel(gap * 9 + locx, -24 + locy, grey);
        pixel(gap * 10 + locx, -24 + locy, black);
        pixel(gap * 11 + locx, -24 + locy, grey);
        pixel(gap * 12 + locx, -24 + locy, grey);
        pixel(gap * 13 + locx, -24 + locy, black);
        pixel(gap * 14 + locx, -24 + locy, dark_green);
        pixel(gap * 15 + locx, -24 + locy, black);
        pixel(gap * 0 + locx, -27 + locy, green);
        pixel(gap * 1 + locx, -27 + locy, black);
        pixel(gap * 2 + locx, -27 + locy, grey);
        pixel(gap * 3 + locx, -27 + locy, dark_grey);
        pixel(gap * 4 + locx, -27 + locy, black);
        pixel(gap * 5 + locx, -27 + locy, dark_grey);
        pixel(gap * 6 + locx, -27 + locy, grey);
        pixel(gap * 7 + locx, -27 + locy, grey);
        pixel(gap * 8 + locx, -27 + locy, black);
        pixel(gap * 9 + locx, -27 + locy, grey);
        pixel(gap * 10 + locx, -27 + locy, grey);
        pixel(gap * 11 + locx, -27 + locy, black);
        pixel(gap * 12 + locx, -27 + locy, grey);
        pixel(gap * 13 + locx, -27 + locy, black);
        pixel(gap * 14 + locx, -27 + locy, dark_green);
        pixel(gap * 15 + locx, -27 + locy, green);
        pixel(gap * 16 + locx, -27 + locy, black);
        pixel(gap * 0 + locx, -30 + locy, green);
        pixel(gap * 1 + locx, -30 + locy, black);
        pixel(gap * 2 + locx, -30 + locy, black);
        pixel(gap * 3 + locx, -30 + locy, black);
        pixel(gap * 4 + locx, -30 + locy, black);
        pixel(gap * 5 + locx, -30 + locy, dark_grey);
        pixel(gap * 6 + locx, -30 + locy, grey);
        pixel(gap * 7 + locx, -30 + locy, grey);
        pixel(gap * 8 + locx, -30 + locy, black);
        pixel(gap * 9 + locx, -30 + locy, grey);
        pixel(gap * 10 + locx, -30 + locy, grey);
        pixel(gap * 11 + locx, -30 + locy, grey);
        pixel(gap * 12 + locx, -30 + locy, black);
        pixel(gap * 13 + locx, -30 + locy, black);
        pixel(gap * 14 + locx, -30 + locy, dark_green);
        pixel(gap * 15 + locx, -30 + locy, green);
        pixel(gap * 16 + locx, -30 + locy, black);
        pixel(gap * 0 + locx, -33 + locy, green);
        pixel(gap * 1 + locx, -33 + locy, black);
        pixel(gap * 2 + locx, -33 + locy, grey);
        pixel(gap * 3 + locx, -33 + locy, dark_grey);
        pixel(gap * 4 + locx, -33 + locy, black);
        pixel(gap * 5 + locx, -33 + locy, dark_grey);
        pixel(gap * 6 + locx, -33 + locy, grey);
        pixel(gap * 7 + locx, -33 + locy, grey);
        pixel(gap * 8 + locx, -33 + locy, black);
        pixel(gap * 9 + locx, -33 + locy, grey);
        pixel(gap * 10 + locx, -33 + locy, grey);
        pixel(gap * 11 + locx, -33 + locy, grey);
        pixel(gap * 12 + locx, -33 + locy, grey);
        pixel(gap * 13 + locx, -33 + locy, black);
        pixel(gap * 14 + locx, -33 + locy, dark_green);
        pixel(gap * 15 + locx, -33 + locy, green);
        pixel(gap * 16 + locx, -33 + locy, black);
        pixel(gap * 0 + locx, -36 + locy, black);
        pixel(gap * 1 + locx, -36 + locy, black);
        pixel(gap * 2 + locx, -36 + locy, black);
        pixel(gap * 3 + locx, -36 + locy, black);
        pixel(gap * 4 + locx, -36 + locy, black);
        pixel(gap * 5 + locx, -36 + locy, black);
        pixel(gap * 6 + locx, -36 + locy, dark_grey);
        pixel(gap * 7 + locx, -36 + locy, dark_grey);
        pixel(gap * 8 + locx, -36 + locy, black);
        pixel(gap * 9 + locx, -36 + locy, grey);
        pixel(gap * 10 + locx, -36 + locy, grey);
        pixel(gap * 11 + locx, -36 + locy, grey);
        pixel(gap * 12 + locx, -36 + locy, grey);
        pixel(gap * 13 + locx, -36 + locy, black);
        pixel(gap * 14 + locx, -36 + locy, dark_green);
        pixel(gap * 15 + locx, -36 + locy, green);
        pixel(gap * 16 + locx, -36 + locy, black);
        pixel(gap * 0 + locx, -39 + locy, black);
        pixel(gap * 1 + locx, -39 + locy, black);
        pixel(gap * 2 + locx, -39 + locy, black);
        pixel(gap * 3 + locx, -39 + locy, black);
        pixel(gap * 6 + locx, -39 + locy, black);
        pixel(gap * 7 + locx, -39 + locy, black);
        pixel(gap * 8 + locx, -39 + locy, black);
        pixel(gap * 9 + locx, -39 + locy, dark_grey);
        pixel(gap * 10 + locx, -39 + locy, grey);
        pixel(gap * 11 + locx, -39 + locy, grey);
        pixel(gap * 12 + locx, -39 + locy, grey);
        pixel(gap * 13 + locx, -39 + locy, black);
        pixel(gap * 14 + locx, -39 + locy, dark_green);
        pixel(gap * 15 + locx, -39 + locy, green);
        pixel(gap * 16 + locx, -39 + locy, black);
        pixel(gap * 0 + locx, -42 + locy, yellow);
        pixel(gap * 1 + locx, -42 + locy, orange);
        pixel(gap * 2 + locx, -42 + locy, red);
        pixel(gap * 9 + locx, -42 + locy, black);
        pixel(gap * 10 + locx, -42 + locy, dark_grey);
        pixel(gap * 11 + locx, -42 + locy, grey);
        pixel(gap * 12 + locx, -42 + locy, grey);
        pixel(gap * 13 + locx, -42 + locy, black);
        pixel(gap * 14 + locx, -42 + locy, dark_green);
        pixel(gap * 15 + locx, -42 + locy, green);
        pixel(gap * 16 + locx, -42 + locy, black);
        pixel(gap * 0 + locx, -45 + locy, orange);
        pixel(gap * 1 + locx, -45 + locy, orange);
        pixel(gap * 2 + locx, -45 + locy, red);
        pixel(gap * 10 + locx, -45 + locy, black);
        pixel(gap * 11 + locx, -45 + locy, dark_grey);
        pixel(gap * 12 + locx, -45 + locy, grey);
        pixel(gap * 13 + locx, -45 + locy, black);
        pixel(gap * 14 + locx, -45 + locy, dark_green);
        pixel(gap * 15 + locx, -45 + locy, green);
        pixel(gap * 16 + locx, -45 + locy, black);
        pixel(gap * 0 + locx, -48 + locy, yellow);
        pixel(gap * 1 + locx, -48 + locy, red);
        pixel(gap * 11 + locx, -48 + locy, black);
        pixel(gap * 12 + locx, -48 + locy, dark_grey);
        pixel(gap * 13 + locx, -48 + locy, black);
        pixel(gap * 14 + locx, -48 + locy, dark_green);
        pixel(gap * 15 + locx, -48 + locy, green);
        pixel(gap * 16 + locx, -48 + locy, black);
        pixel(gap * 0 + locx, -51 + locy, orange);
        pixel(gap * 1 + locx, -51 + locy, red);
        pixel(gap * 12 + locx, -51 + locy, black);
        pixel(gap * 13 + locx, -51 + locy, black);
        pixel(gap * 14 + locx, -51 + locy, dark_green);
        pixel(gap * 15 + locx, -51 + locy, green);
        pixel(gap * 16 + locx, -51 + locy, black);
        pixel(gap * 0 + locx, -54 + locy, red);
        pixel(gap * 13 + locx, -54 + locy, black);
        pixel(gap * 14 + locx, -54 + locy, dark_green);
        pixel(gap * 15 + locx, -54 + locy, green);
        pixel(gap * 16 + locx, -54 + locy, black);
        pixel(gap * 14 + locx, -57 + locy, black);
        pixel(gap * 15 + locx, -57 + locy, black);
        pixel(gap * 16 + locx, -57 + locy, black);
        pixel(gap * 15 + locx, -60 + locy, black);

    }


}

void aliencraft(assets_t* Q) {

    color_t red = { 255,0,0 },
        dark_red = { 136,0,27 },
        dark_dark_red = { 43,1,9 },
        dark_yellow = { 43,41,1 },
        yellow = { 136,142,20 },
        cream = { 242,164,61 },
        black = { 0,0,0 };

    int gap = -3, locx, locy;

    Q->area_radius = 19 * 3;

    locx = Q->coordinate.x;
    locy = Q->coordinate.y;

    for (gap = -3; gap < 4; gap += 6) {


        pixel(gap * 7 + locx, -33 + locy, cream);
        pixel(gap * 0 + locx, -30 + locy, black);
        pixel(gap * 7 + locx, -30 + locy, yellow);
        pixel(gap * 0 + locx, -27 + locy, red);
        pixel(gap * 1 + locx, -27 + locy, black);
        pixel(gap * 7 + locx, -27 + locy, dark_yellow);
        pixel(gap * 0 + locx, -24 + locy, dark_dark_red);
        pixel(gap * 1 + locx, -24 + locy, red);
        pixel(gap * 2 + locx, -24 + locy, black);
        pixel(gap * 6 + locx, -24 + locy, black);
        pixel(gap * 7 + locx, -24 + locy, yellow);
        pixel(gap * 0 + locx, -21 + locy, dark_red);
        pixel(gap * 1 + locx, -21 + locy, dark_dark_red);
        pixel(gap * 2 + locx, -21 + locy, red);
        pixel(gap * 3 + locx, -21 + locy, black);
        pixel(gap * 6 + locx, -21 + locy, black);
        pixel(gap * 7 + locx, -21 + locy, black);
        pixel(gap * 0 + locx, -18 + locy, dark_red);
        pixel(gap * 1 + locx, -18 + locy, dark_dark_red);
        pixel(gap * 2 + locx, -18 + locy, dark_red);
        pixel(gap * 3 + locx, -18 + locy, black);
        pixel(gap * 6 + locx, -18 + locy, black);
        pixel(gap * 7 + locx, -18 + locy, red);
        pixel(gap * 8 + locx, -18 + locy, black);
        pixel(gap * 0 + locx, -15 + locy, dark_red);
        pixel(gap * 1 + locx, -15 + locy, dark_dark_red);
        pixel(gap * 2 + locx, -15 + locy, dark_red);
        pixel(gap * 3 + locx, -15 + locy, black);
        pixel(gap * 6 + locx, -15 + locy, black);
        pixel(gap * 7 + locx, -15 + locy, black);
        pixel(gap * 8 + locx, -15 + locy, black);
        pixel(gap * 0 + locx, -12 + locy, red);
        pixel(gap * 1 + locx, -12 + locy, dark_red);
        pixel(gap * 2 + locx, -12 + locy, dark_dark_red);
        pixel(gap * 3 + locx, -12 + locy, dark_red);
        pixel(gap * 4 + locx, -12 + locy, black);
        pixel(gap * 6 + locx, -12 + locy, black);
        pixel(gap * 7 + locx, -12 + locy, dark_dark_red);
        pixel(gap * 8 + locx, -12 + locy, black);
        pixel(gap * 9 + locx, -12 + locy, black);
        pixel(gap * 10 + locx, -12 + locy, black);
        pixel(gap * 11 + locx, -12 + locy, black);
        pixel(gap * 12 + locx, -12 + locy, black);
        pixel(gap * 13 + locx, -12 + locy, black);
        pixel(gap * 0 + locx, -9 + locy, black);
        pixel(gap * 1 + locx, -9 + locy, red);
        pixel(gap * 2 + locx, -9 + locy, dark_red);
        pixel(gap * 3 + locx, -9 + locy, black);
        pixel(gap * 4 + locx, -9 + locy, black);
        pixel(gap * 5 + locx, -9 + locy, dark_dark_red);
        pixel(gap * 6 + locx, -9 + locy, dark_red);
        pixel(gap * 7 + locx, -9 + locy, dark_red);
        pixel(gap * 8 + locx, -9 + locy, dark_red);
        pixel(gap * 9 + locx, -9 + locy, dark_red);
        pixel(gap * 10 + locx, -9 + locy, dark_red);
        pixel(gap * 11 + locx, -9 + locy, dark_red);
        pixel(gap * 12 + locx, -9 + locy, dark_dark_red);
        pixel(gap * 13 + locx, -9 + locy, dark_red);
        pixel(gap * 14 + locx, -9 + locy, black);
        pixel(gap * 0 + locx, -6 + locy, yellow);
        pixel(gap * 1 + locx, -6 + locy, black);
        pixel(gap * 2 + locx, -6 + locy, red);
        pixel(gap * 3 + locx, -6 + locy, dark_dark_red);
        pixel(gap * 4 + locx, -6 + locy, dark_dark_red);
        pixel(gap * 5 + locx, -6 + locy, black);
        pixel(gap * 6 + locx, -6 + locy, black);
        pixel(gap * 7 + locx, -6 + locy, dark_red);
        pixel(gap * 8 + locx, -6 + locy, dark_red);
        pixel(gap * 9 + locx, -6 + locy, dark_red);
        pixel(gap * 10 + locx, -6 + locy, dark_red);
        pixel(gap * 11 + locx, -6 + locy, dark_dark_red);
        pixel(gap * 12 + locx, -6 + locy, red);
        pixel(gap * 13 + locx, -6 + locy, dark_red);
        pixel(gap * 14 + locx, -6 + locy, dark_red);
        pixel(gap * 15 + locx, -6 + locy, black);
        pixel(gap * 0 + locx, -3 + locy, cream);
        pixel(gap * 1 + locx, -3 + locy, yellow);
        pixel(gap * 2 + locx, -3 + locy, black);
        pixel(gap * 3 + locx, -3 + locy, dark_red);
        pixel(gap * 4 + locx, -3 + locy, dark_dark_red);
        pixel(gap * 5 + locx, -3 + locy, black);
        pixel(gap * 6 + locx, -3 + locy, black);
        pixel(gap * 7 + locx, -3 + locy, dark_red);
        pixel(gap * 8 + locx, -3 + locy, dark_red);
        pixel(gap * 9 + locx, -3 + locy, dark_red);
        pixel(gap * 10 + locx, -3 + locy, dark_dark_red);
        pixel(gap * 11 + locx, -3 + locy, red);
        pixel(gap * 12 + locx, -3 + locy, dark_red);
        pixel(gap * 13 + locx, -3 + locy, dark_red);
        pixel(gap * 14 + locx, -3 + locy, dark_red);
        pixel(gap * 15 + locx, -3 + locy, dark_red);
        pixel(gap * 16 + locx, -3 + locy, black);
        pixel(gap * 0 + locx, 0 + locy, cream);
        pixel(gap * 1 + locx, 0 + locy, yellow);
        pixel(gap * 2 + locx, 0 + locy, black);
        pixel(gap * 3 + locx, 0 + locy, dark_red);
        pixel(gap * 4 + locx, 0 + locy, dark_dark_red);
        pixel(gap * 5 + locx, 0 + locy, black);
        pixel(gap * 6 + locx, 0 + locy, dark_dark_red);
        pixel(gap * 7 + locx, 0 + locy, dark_red);
        pixel(gap * 8 + locx, 0 + locy, dark_red);
        pixel(gap * 9 + locx, 0 + locy, dark_dark_red);
        pixel(gap * 10 + locx, 0 + locy, red);
        pixel(gap * 11 + locx, 0 + locy, dark_red);
        pixel(gap * 12 + locx, 0 + locy, dark_red);
        pixel(gap * 13 + locx, 0 + locy, dark_red);
        pixel(gap * 14 + locx, 0 + locy, dark_red);
        pixel(gap * 15 + locx, 0 + locy, dark_red);
        pixel(gap * 16 + locx, 0 + locy, dark_red);
        pixel(gap * 17 + locx, 0 + locy, black);
        pixel(gap * 0 + locx, 3 + locy, cream);
        pixel(gap * 1 + locx, 3 + locy, cream);
        pixel(gap * 2 + locx, 3 + locy, black);
        pixel(gap * 3 + locx, 3 + locy, dark_red);
        pixel(gap * 4 + locx, 3 + locy, dark_dark_red);
        pixel(gap * 5 + locx, 3 + locy, black);
        pixel(gap * 6 + locx, 3 + locy, dark_dark_red);
        pixel(gap * 7 + locx, 3 + locy, dark_red);
        pixel(gap * 8 + locx, 3 + locy, dark_red);
        pixel(gap * 9 + locx, 3 + locy, dark_dark_red);
        pixel(gap * 10 + locx, 3 + locy, dark_red);
        pixel(gap * 11 + locx, 3 + locy, dark_red);
        pixel(gap * 12 + locx, 3 + locy, dark_red);
        pixel(gap * 13 + locx, 3 + locy, dark_red);
        pixel(gap * 14 + locx, 3 + locy, dark_red);
        pixel(gap * 15 + locx, 3 + locy, dark_red);
        pixel(gap * 16 + locx, 3 + locy, dark_red);
        pixel(gap * 17 + locx, 3 + locy, dark_red);
        pixel(gap * 18 + locx, 3 + locy, black);
        pixel(gap * 0 + locx, 6 + locy, cream);
        pixel(gap * 1 + locx, 6 + locy, cream);
        pixel(gap * 2 + locx, 6 + locy, black);
        pixel(gap * 3 + locx, 6 + locy, dark_red);
        pixel(gap * 4 + locx, 6 + locy, dark_dark_red);
        pixel(gap * 5 + locx, 6 + locy, black);
        pixel(gap * 6 + locx, 6 + locy, black);
        pixel(gap * 7 + locx, 6 + locy, dark_red);
        pixel(gap * 8 + locx, 6 + locy, dark_red);
        pixel(gap * 9 + locx, 6 + locy, dark_dark_red);
        pixel(gap * 10 + locx, 6 + locy, dark_red);
        pixel(gap * 11 + locx, 6 + locy, dark_red);
        pixel(gap * 12 + locx, 6 + locy, dark_red);
        pixel(gap * 13 + locx, 6 + locy, dark_red);
        pixel(gap * 14 + locx, 6 + locy, dark_dark_red);
        pixel(gap * 15 + locx, 6 + locy, dark_dark_red);
        pixel(gap * 16 + locx, 6 + locy, dark_red);
        pixel(gap * 17 + locx, 6 + locy, dark_red);
        pixel(gap * 18 + locx, 6 + locy, black);
        pixel(gap * 0 + locx, 9 + locy, cream);
        pixel(gap * 1 + locx, 9 + locy, black);
        pixel(gap * 2 + locx, 9 + locy, dark_red);
        pixel(gap * 3 + locx, 9 + locy, dark_red);
        pixel(gap * 4 + locx, 9 + locy, dark_dark_red);
        pixel(gap * 5 + locx, 9 + locy, black);
        pixel(gap * 6 + locx, 9 + locy, black);
        pixel(gap * 7 + locx, 9 + locy, dark_red);
        pixel(gap * 8 + locx, 9 + locy, dark_red);
        pixel(gap * 9 + locx, 9 + locy, dark_dark_red);
        pixel(gap * 10 + locx, 9 + locy, dark_red);
        pixel(gap * 11 + locx, 9 + locy, dark_red);
        pixel(gap * 12 + locx, 9 + locy, dark_red);
        pixel(gap * 13 + locx, 9 + locy, dark_dark_red);
        pixel(gap * 14 + locx, 9 + locy, black);
        pixel(gap * 15 + locx, 9 + locy, black);
        pixel(gap * 16 + locx, 9 + locy, dark_red);
        pixel(gap * 17 + locx, 9 + locy, dark_red);
        pixel(gap * 18 + locx, 9 + locy, black);
        pixel(gap * 0 + locx, 12 + locy, black);
        pixel(gap * 1 + locx, 12 + locy, dark_red);
        pixel(gap * 2 + locx, 12 + locy, dark_red);
        pixel(gap * 3 + locx, 12 + locy, dark_dark_red);
        pixel(gap * 4 + locx, 12 + locy, black);
        pixel(gap * 5 + locx, 12 + locy, black);
        pixel(gap * 6 + locx, 12 + locy, dark_dark_red);
        pixel(gap * 7 + locx, 12 + locy, dark_red);
        pixel(gap * 8 + locx, 12 + locy, dark_red);
        pixel(gap * 9 + locx, 12 + locy, dark_dark_red);
        pixel(gap * 10 + locx, 12 + locy, dark_red);
        pixel(gap * 11 + locx, 12 + locy, dark_red);
        pixel(gap * 12 + locx, 12 + locy, dark_dark_red);
        pixel(gap * 13 + locx, 12 + locy, black);
        pixel(gap * 16 + locx, 12 + locy, black);
        pixel(gap * 17 + locx, 12 + locy, black);
        pixel(gap * 18 + locx, 12 + locy, black);
        pixel(gap * 0 + locx, 15 + locy, black);
        pixel(gap * 1 + locx, 15 + locy, dark_red);
        pixel(gap * 2 + locx, 15 + locy, dark_red);
        pixel(gap * 3 + locx, 15 + locy, black);
        pixel(gap * 4 + locx, 15 + locy, black);
        pixel(gap * 6 + locx, 15 + locy, dark_red);
        pixel(gap * 7 + locx, 15 + locy, black);
        pixel(gap * 8 + locx, 15 + locy, black);
        pixel(gap * 9 + locx, 15 + locy, black);
        pixel(gap * 10 + locx, 15 + locy, dark_red);
        pixel(gap * 11 + locx, 15 + locy, dark_red);
        pixel(gap * 12 + locx, 15 + locy, black);
        pixel(gap * 1 + locx, 18 + locy, black);
        pixel(gap * 2 + locx, 18 + locy, black);
        pixel(gap * 5 + locx, 18 + locy, black);
        pixel(gap * 6 + locx, 18 + locy, black);
        pixel(gap * 7 + locx, 18 + locy, black);
        pixel(gap * 8 + locx, 18 + locy, black);
        pixel(gap * 9 + locx, 18 + locy, black);
        pixel(gap * 10 + locx, 18 + locy, black);
        pixel(gap * 11 + locx, 18 + locy, black);
        pixel(gap * 5 + locx, 21 + locy, black);
        pixel(gap * 6 + locx, 21 + locy, dark_red);
        pixel(gap * 7 + locx, 21 + locy, dark_red);
        pixel(gap * 8 + locx, 21 + locy, black);
        pixel(gap * 5 + locx, 24 + locy, dark_dark_red);
        pixel(gap * 6 + locx, 24 + locy, dark_dark_red);
        pixel(gap * 7 + locx, 24 + locy, dark_dark_red);
        pixel(gap * 5 + locx, 27 + locy, dark_dark_red);
        pixel(gap * 6 + locx, 27 + locy, dark_dark_red);
        pixel(gap * 5 + locx, 30 + locy, dark_dark_red);
        pixel(gap * 6 + locx, 30 + locy, dark_dark_red);
        pixel(gap * 5 + locx, 33 + locy, dark_dark_red);

    }

}

void createalien() {

    for (int mocha = 0; mocha < 5; mocha++) {

        aliencraft(&(aliens[mocha]));

    }

}

void explosion(assets_t Q) {

    color_t red = { 239,27,38 },
        yellow = { 248,237,51 },
        orange = { 247, 148, 29 },
        white = { 255,255,255 },
        brown = { 60, 37, 23 };

    int gap = 3, locx, locy;

    locx = Q.coordinate.x;
    locy = Q.coordinate.y;


    pixel(gap * 15 + locx, 13 * gap + locy, red);
    pixel(gap * 14 + locx, 13 * gap + locy, yellow);
    pixel(gap * 13 + locx, 13 * gap + locy, red);
    pixel(gap * 15 + locx, 12 * gap + locy, yellow);
    pixel(gap * 14 + locx, 12 * gap + locy, yellow);
    pixel(gap * 13 + locx, 12 * gap + locy, orange);
    pixel(gap * 12 + locx, 12 * gap + locy, brown);
    pixel(gap * 0 + locx, 12 * gap + locy, brown);
    pixel(gap * -1 + locx, 12 * gap + locy, brown);
    pixel(gap * -2 + locx, 12 * gap + locy, brown);
    pixel(gap * -3 + locx, 12 * gap + locy, brown);
    pixel(gap * -4 + locx, 12 * gap + locy, brown);
    pixel(gap * -15 + locx, 12 * gap + locy, red);
    pixel(gap * 15 + locx, 11 * gap + locy, red);
    pixel(gap * 14 + locx, 11 * gap + locy, orange);
    pixel(gap * 13 + locx, 11 * gap + locy, orange);
    pixel(gap * 12 + locx, 11 * gap + locy, red);
    pixel(gap * 11 + locx, 11 * gap + locy, brown);
    pixel(gap * 3 + locx, 11 * gap + locy, brown);
    pixel(gap * 2 + locx, 11 * gap + locy, brown);
    pixel(gap * 1 + locx, 11 * gap + locy, brown);
    pixel(gap * 0 + locx, 11 * gap + locy, red);
    pixel(gap * -1 + locx, 11 * gap + locy, red);
    pixel(gap * -2 + locx, 11 * gap + locy, red);
    pixel(gap * -3 + locx, 11 * gap + locy, red);
    pixel(gap * -4 + locx, 11 * gap + locy, red);
    pixel(gap * -5 + locx, 11 * gap + locy, brown);
    pixel(gap * -6 + locx, 11 * gap + locy, brown);
    pixel(gap * -7 + locx, 11 * gap + locy, brown);
    pixel(gap * -14 + locx, 11 * gap + locy, red);
    pixel(gap * -15 + locx, 11 * gap + locy, yellow);
    pixel(gap * -16 + locx, 11 * gap + locy, red);
    pixel(gap * 14 + locx, 10 * gap + locy, red);
    pixel(gap * 13 + locx, 10 * gap + locy, red);
    pixel(gap * 12 + locx, 10 * gap + locy, brown);
    pixel(gap * 11 + locx, 10 * gap + locy, brown);
    pixel(gap * 5 + locx, 10 * gap + locy, brown);
    pixel(gap * 4 + locx, 10 * gap + locy, brown);
    pixel(gap * 3 + locx, 10 * gap + locy, red);
    pixel(gap * 2 + locx, 10 * gap + locy, red);
    pixel(gap * 1 + locx, 10 * gap + locy, red);
    pixel(gap * 0 + locx, 10 * gap + locy, orange);
    pixel(gap * -1 + locx, 10 * gap + locy, orange);
    pixel(gap * -2 + locx, 10 * gap + locy, orange);
    pixel(gap * -3 + locx, 10 * gap + locy, orange);
    pixel(gap * -4 + locx, 10 * gap + locy, orange);
    pixel(gap * -5 + locx, 10 * gap + locy, red);
    pixel(gap * -6 + locx, 10 * gap + locy, red);
    pixel(gap * -7 + locx, 10 * gap + locy, red);
    pixel(gap * -8 + locx, 10 * gap + locy, brown);
    pixel(gap * -13 + locx, 10 * gap + locy, red);
    pixel(gap * -14 + locx, 10 * gap + locy, yellow);
    pixel(gap * -15 + locx, 10 * gap + locy, yellow);
    pixel(gap * -16 + locx, 10 * gap + locy, red);
    pixel(gap * 12 + locx, 9 * gap + locy, brown);
    pixel(gap * 11 + locx, 9 * gap + locy, brown);
    pixel(gap * 6 + locx, 9 * gap + locy, brown);
    pixel(gap * 5 + locx, 9 * gap + locy, red);
    pixel(gap * 4 + locx, 9 * gap + locy, red);
    pixel(gap * 3 + locx, 9 * gap + locy, red);
    pixel(gap * 2 + locx, 9 * gap + locy, orange);
    pixel(gap * 1 + locx, 9 * gap + locy, yellow);
    pixel(gap * 0 + locx, 9 * gap + locy, yellow);
    pixel(gap * -1 + locx, 9 * gap + locy, yellow);
    pixel(gap * -2 + locx, 9 * gap + locy, yellow);
    pixel(gap * -3 + locx, 9 * gap + locy, yellow);
    pixel(gap * -4 + locx, 9 * gap + locy, yellow);
    pixel(gap * -5 + locx, 9 * gap + locy, orange);
    pixel(gap * -6 + locx, 9 * gap + locy, orange);
    pixel(gap * -7 + locx, 9 * gap + locy, red);
    pixel(gap * -8 + locx, 9 * gap + locy, red);
    pixel(gap * -9 + locx, 9 * gap + locy, brown);
    pixel(gap * -12 + locx, 9 * gap + locy, brown);
    pixel(gap * -13 + locx, 9 * gap + locy, orange);
    pixel(gap * -14 + locx, 9 * gap + locy, orange);
    pixel(gap * -15 + locx, 9 * gap + locy, orange);
    pixel(gap * -16 + locx, 9 * gap + locy, red);
    pixel(gap * 6 + locx, 8 * gap + locy, brown);
    pixel(gap * 5 + locx, 8 * gap + locy, red);
    pixel(gap * 4 + locx, 8 * gap + locy, orange);
    pixel(gap * 3 + locx, 8 * gap + locy, orange);
    pixel(gap * 2 + locx, 8 * gap + locy, yellow);
    pixel(gap * 1 + locx, 8 * gap + locy, yellow);
    pixel(gap * 0 + locx, 8 * gap + locy, yellow);
    pixel(gap * -1 + locx, 8 * gap + locy, white);
    pixel(gap * -2 + locx, 8 * gap + locy, white);
    pixel(gap * -3 + locx, 8 * gap + locy, white);
    pixel(gap * -4 + locx, 8 * gap + locy, yellow);
    pixel(gap * -5 + locx, 8 * gap + locy, yellow);
    pixel(gap * -6 + locx, 8 * gap + locy, orange);
    pixel(gap * -7 + locx, 8 * gap + locy, orange);
    pixel(gap * -8 + locx, 8 * gap + locy, red);
    pixel(gap * -9 + locx, 8 * gap + locy, red);
    pixel(gap * -10 + locx, 8 * gap + locy, brown);
    pixel(gap * -12 + locx, 8 * gap + locy, red);
    pixel(gap * -13 + locx, 8 * gap + locy, red);
    pixel(gap * -14 + locx, 8 * gap + locy, red);
    pixel(gap * -15 + locx, 8 * gap + locy, red);
    pixel(gap * -16 + locx, 8 * gap + locy, brown);
    pixel(gap * 7 + locx, 7 * gap + locy, brown);
    pixel(gap * 6 + locx, 7 * gap + locy, red);
    pixel(gap * 5 + locx, 7 * gap + locy, red);
    pixel(gap * 4 + locx, 7 * gap + locy, red);
    pixel(gap * 3 + locx, 7 * gap + locy, orange);
    pixel(gap * 2 + locx, 7 * gap + locy, yellow);
    pixel(gap * 1 + locx, 7 * gap + locy, white);
    pixel(gap * 0 + locx, 7 * gap + locy, white);
    pixel(gap * -1 + locx, 7 * gap + locy, white);
    pixel(gap * -2 + locx, 7 * gap + locy, white);
    pixel(gap * -3 + locx, 7 * gap + locy, white);
    pixel(gap * -4 + locx, 7 * gap + locy, white);
    pixel(gap * -5 + locx, 7 * gap + locy, yellow);
    pixel(gap * -6 + locx, 7 * gap + locy, yellow);
    pixel(gap * -7 + locx, 7 * gap + locy, yellow);
    pixel(gap * -8 + locx, 7 * gap + locy, red);
    pixel(gap * -9 + locx, 7 * gap + locy, red);
    pixel(gap * -10 + locx, 7 * gap + locy, brown);
    pixel(gap * -12 + locx, 7 * gap + locy, brown);
    pixel(gap * -13 + locx, 7 * gap + locy, brown);
    pixel(gap * -14 + locx, 7 * gap + locy, brown);
    pixel(gap * -15 + locx, 7 * gap + locy, brown);
    pixel(gap * 9 + locx, 6 * gap + locy, brown);
    pixel(gap * 8 + locx, 6 * gap + locy, brown);
    pixel(gap * 7 + locx, 6 * gap + locy, brown);
    pixel(gap * 6 + locx, 6 * gap + locy, red);
    pixel(gap * 5 + locx, 6 * gap + locy, red);
    pixel(gap * 4 + locx, 6 * gap + locy, orange);
    pixel(gap * 3 + locx, 6 * gap + locy, yellow);
    pixel(gap * 2 + locx, 6 * gap + locy, yellow);
    pixel(gap * 1 + locx, 6 * gap + locy, white);
    pixel(gap * 0 + locx, 6 * gap + locy, white);
    pixel(gap * -1 + locx, 6 * gap + locy, white);
    pixel(gap * -2 + locx, 6 * gap + locy, white);
    pixel(gap * -3 + locx, 6 * gap + locy, white);
    pixel(gap * -4 + locx, 6 * gap + locy, white);
    pixel(gap * -5 + locx, 6 * gap + locy, white);
    pixel(gap * -6 + locx, 6 * gap + locy, white);
    pixel(gap * -7 + locx, 6 * gap + locy, yellow);
    pixel(gap * -8 + locx, 6 * gap + locy, yellow);
    pixel(gap * -9 + locx, 6 * gap + locy, orange);
    pixel(gap * -10 + locx, 6 * gap + locy, red);
    pixel(gap * -11 + locx, 6 * gap + locy, brown);
    pixel(gap * 13 + locx, 5 * gap + locy, brown);
    pixel(gap * 12 + locx, 5 * gap + locy, brown);
    pixel(gap * 11 + locx, 5 * gap + locy, brown);
    pixel(gap * 10 + locx, 5 * gap + locy, brown);
    pixel(gap * 9 + locx, 5 * gap + locy, brown);
    pixel(gap * 8 + locx, 5 * gap + locy, brown);
    pixel(gap * 7 + locx, 5 * gap + locy, brown);
    pixel(gap * 6 + locx, 5 * gap + locy, red);
    pixel(gap * 5 + locx, 5 * gap + locy, red);
    pixel(gap * 4 + locx, 5 * gap + locy, orange);
    pixel(gap * 3 + locx, 5 * gap + locy, yellow);
    pixel(gap * 2 + locx, 5 * gap + locy, yellow);
    pixel(gap * 1 + locx, 5 * gap + locy, white);
    pixel(gap * 0 + locx, 5 * gap + locy, white);
    pixel(gap * -1 + locx, 5 * gap + locy, white);
    pixel(gap * -2 + locx, 5 * gap + locy, white);
    pixel(gap * -3 + locx, 5 * gap + locy, white);
    pixel(gap * -4 + locx, 5 * gap + locy, white);
    pixel(gap * -5 + locx, 5 * gap + locy, white);
    pixel(gap * -6 + locx, 5 * gap + locy, white);
    pixel(gap * -7 + locx, 5 * gap + locy, white);
    pixel(gap * -8 + locx, 5 * gap + locy, yellow);
    pixel(gap * -9 + locx, 5 * gap + locy, orange);
    pixel(gap * -10 + locx, 5 * gap + locy, red);
    pixel(gap * -11 + locx, 5 * gap + locy, red);
    pixel(gap * -12 + locx, 5 * gap + locy, brown);
    pixel(gap * -13 + locx, 5 * gap + locy, brown);
    pixel(gap * 14 + locx, 4 * gap + locy, brown);
    pixel(gap * 13 + locx, 4 * gap + locy, red);
    pixel(gap * 12 + locx, 4 * gap + locy, red);
    pixel(gap * 11 + locx, 4 * gap + locy, red);
    pixel(gap * 10 + locx, 4 * gap + locy, red);
    pixel(gap * 9 + locx, 4 * gap + locy, red);
    pixel(gap * 8 + locx, 4 * gap + locy, brown);
    pixel(gap * 7 + locx, 4 * gap + locy, brown);
    pixel(gap * 6 + locx, 4 * gap + locy, red);
    pixel(gap * 5 + locx, 4 * gap + locy, red);
    pixel(gap * 4 + locx, 4 * gap + locy, orange);
    pixel(gap * 3 + locx, 4 * gap + locy, orange);
    pixel(gap * 2 + locx, 4 * gap + locy, yellow);
    pixel(gap * 1 + locx, 4 * gap + locy, white);
    pixel(gap * 0 + locx, 4 * gap + locy, yellow);
    pixel(gap * -1 + locx, 4 * gap + locy, yellow);
    pixel(gap * -2 + locx, 4 * gap + locy, white);
    pixel(gap * -3 + locx, 4 * gap + locy, white);
    pixel(gap * -4 + locx, 4 * gap + locy, white);
    pixel(gap * -5 + locx, 4 * gap + locy, white);
    pixel(gap * -6 + locx, 4 * gap + locy, white);
    pixel(gap * -7 + locx, 4 * gap + locy, white);
    pixel(gap * -8 + locx, 4 * gap + locy, yellow);
    pixel(gap * -9 + locx, 4 * gap + locy, yellow);
    pixel(gap * -10 + locx, 4 * gap + locy, orange);
    pixel(gap * -11 + locx, 4 * gap + locy, red);
    pixel(gap * -12 + locx, 4 * gap + locy, red);
    pixel(gap * -13 + locx, 4 * gap + locy, brown);
    pixel(gap * -14 + locx, 4 * gap + locy, brown);
    pixel(gap * -15 + locx, 4 * gap + locy, brown);
    pixel(gap * -16 + locx, 4 * gap + locy, brown);
    pixel(gap * 14 + locx, 3 * gap + locy, brown);
    pixel(gap * 13 + locx, 3 * gap + locy, red);
    pixel(gap * 12 + locx, 3 * gap + locy, orange);
    pixel(gap * 11 + locx, 3 * gap + locy, yellow);
    pixel(gap * 10 + locx, 3 * gap + locy, orange);
    pixel(gap * 9 + locx, 3 * gap + locy, orange);
    pixel(gap * 8 + locx, 3 * gap + locy, brown);
    pixel(gap * 7 + locx, 3 * gap + locy, brown);
    pixel(gap * 6 + locx, 3 * gap + locy, red);
    pixel(gap * 5 + locx, 3 * gap + locy, red);
    pixel(gap * 4 + locx, 3 * gap + locy, orange);
    pixel(gap * 3 + locx, 3 * gap + locy, orange);
    pixel(gap * 2 + locx, 3 * gap + locy, yellow);
    pixel(gap * 1 + locx, 3 * gap + locy, yellow);
    pixel(gap * 0 + locx, 3 * gap + locy, yellow);
    pixel(gap * -1 + locx, 3 * gap + locy, yellow);
    pixel(gap * -2 + locx, 3 * gap + locy, yellow);
    pixel(gap * -3 + locx, 3 * gap + locy, white);
    pixel(gap * -4 + locx, 3 * gap + locy, white);
    pixel(gap * -5 + locx, 3 * gap + locy, yellow);
    pixel(gap * -6 + locx, 3 * gap + locy, yellow);
    pixel(gap * -7 + locx, 3 * gap + locy, yellow);
    pixel(gap * -8 + locx, 3 * gap + locy, yellow);
    pixel(gap * -9 + locx, 3 * gap + locy, yellow);
    pixel(gap * -10 + locx, 3 * gap + locy, orange);
    pixel(gap * -11 + locx, 3 * gap + locy, orange);
    pixel(gap * -12 + locx, 3 * gap + locy, red);
    pixel(gap * -13 + locx, 3 * gap + locy, red);
    pixel(gap * -14 + locx, 3 * gap + locy, red);
    pixel(gap * -15 + locx, 3 * gap + locy, red);
    pixel(gap * -16 + locx, 3 * gap + locy, red);
    pixel(gap * -17 + locx, 3 * gap + locy, brown);
    pixel(gap * 15 + locx, 2 * gap + locy, brown);
    pixel(gap * 14 + locx, 2 * gap + locy, brown);
    pixel(gap * 13 + locx, 2 * gap + locy, red);
    pixel(gap * 12 + locx, 2 * gap + locy, yellow);
    pixel(gap * 11 + locx, 2 * gap + locy, white);
    pixel(gap * 10 + locx, 2 * gap + locy, yellow);
    pixel(gap * 9 + locx, 2 * gap + locy, orange);
    pixel(gap * 8 + locx, 2 * gap + locy, red);
    pixel(gap * 7 + locx, 2 * gap + locy, brown);
    pixel(gap * 6 + locx, 2 * gap + locy, brown);
    pixel(gap * 5 + locx, 2 * gap + locy, red);
    pixel(gap * 4 + locx, 2 * gap + locy, red);
    pixel(gap * 3 + locx, 2 * gap + locy, orange);
    pixel(gap * 2 + locx, 2 * gap + locy, orange);
    pixel(gap * 1 + locx, 2 * gap + locy, orange);
    pixel(gap * 0 + locx, 2 * gap + locy, yellow);
    pixel(gap * -1 + locx, 2 * gap + locy, yellow);
    pixel(gap * -2 + locx, 2 * gap + locy, yellow);
    pixel(gap * -3 + locx, 2 * gap + locy, yellow);
    pixel(gap * -4 + locx, 2 * gap + locy, yellow);
    pixel(gap * -5 + locx, 2 * gap + locy, yellow);
    pixel(gap * -6 + locx, 2 * gap + locy, yellow);
    pixel(gap * -7 + locx, 2 * gap + locy, yellow);
    pixel(gap * -8 + locx, 2 * gap + locy, yellow);
    pixel(gap * -9 + locx, 2 * gap + locy, orange);
    pixel(gap * -10 + locx, 2 * gap + locy, orange);
    pixel(gap * -11 + locx, 2 * gap + locy, orange);
    pixel(gap * -12 + locx, 2 * gap + locy, red);
    pixel(gap * -13 + locx, 2 * gap + locy, red);
    pixel(gap * -14 + locx, 2 * gap + locy, orange);
    pixel(gap * -15 + locx, 2 * gap + locy, red);
    pixel(gap * -16 + locx, 2 * gap + locy, red);
    pixel(gap * -17 + locx, 2 * gap + locy, brown);
    pixel(gap * 15 + locx, 1 * gap + locy, brown);
    pixel(gap * 14 + locx, 1 * gap + locy, red);
    pixel(gap * 13 + locx, 1 * gap + locy, red);
    pixel(gap * 12 + locx, 1 * gap + locy, orange);
    pixel(gap * 11 + locx, 1 * gap + locy, yellow);
    pixel(gap * 10 + locx, 1 * gap + locy, white);
    pixel(gap * 9 + locx, 1 * gap + locy, white);
    pixel(gap * 8 + locx, 1 * gap + locy, yellow);
    pixel(gap * 7 + locx, 1 * gap + locy, orange);
    pixel(gap * 6 + locx, 1 * gap + locy, red);
    pixel(gap * 5 + locx, 1 * gap + locy, brown);
    pixel(gap * 4 + locx, 1 * gap + locy, red);
    pixel(gap * 3 + locx, 1 * gap + locy, red);
    pixel(gap * 2 + locx, 1 * gap + locy, red);
    pixel(gap * 1 + locx, 1 * gap + locy, orange);
    pixel(gap * 0 + locx, 1 * gap + locy, orange);
    pixel(gap * -1 + locx, 1 * gap + locy, orange);
    pixel(gap * -2 + locx, 1 * gap + locy, orange);
    pixel(gap * -3 + locx, 1 * gap + locy, yellow);
    pixel(gap * -4 + locx, 1 * gap + locy, yellow);
    pixel(gap * -5 + locx, 1 * gap + locy, yellow);
    pixel(gap * -6 + locx, 1 * gap + locy, yellow);
    pixel(gap * -7 + locx, 1 * gap + locy, yellow);
    pixel(gap * -8 + locx, 1 * gap + locy, orange);
    pixel(gap * -9 + locx, 1 * gap + locy, orange);
    pixel(gap * -10 + locx, 1 * gap + locy, orange);
    pixel(gap * -11 + locx, 1 * gap + locy, red);
    pixel(gap * -12 + locx, 1 * gap + locy, red);
    pixel(gap * -13 + locx, 1 * gap + locy, red);
    pixel(gap * -14 + locx, 1 * gap + locy, yellow);
    pixel(gap * -15 + locx, 1 * gap + locy, orange);
    pixel(gap * -16 + locx, 1 * gap + locy, red);
    pixel(gap * -17 + locx, 1 * gap + locy, brown);
    pixel(gap * 15 + locx, 0 * gap + locy, brown);
    pixel(gap * 14 + locx, 0 * gap + locy, red);
    pixel(gap * 13 + locx, 0 * gap + locy, red);
    pixel(gap * 12 + locx, 0 * gap + locy, orange);
    pixel(gap * 11 + locx, 0 * gap + locy, white);
    pixel(gap * 10 + locx, 0 * gap + locy, yellow);
    pixel(gap * 9 + locx, 0 * gap + locy, orange);
    pixel(gap * 8 + locx, 0 * gap + locy, red);
    pixel(gap * 7 + locx, 0 * gap + locy, brown);
    pixel(gap * 6 + locx, 0 * gap + locy, red);
    pixel(gap * 5 + locx, 0 * gap + locy, red);
    pixel(gap * 4 + locx, 0 * gap + locy, red);
    pixel(gap * 3 + locx, 0 * gap + locy, red);
    pixel(gap * 2 + locx, 0 * gap + locy, red);
    pixel(gap * 1 + locx, 0 * gap + locy, red);
    pixel(gap * 0 + locx, 0 * gap + locy, orange);
    pixel(gap * -1 + locx, 0 * gap + locy, orange);
    pixel(gap * -2 + locx, 0 * gap + locy, yellow);
    pixel(gap * -3 + locx, 0 * gap + locy, yellow);
    pixel(gap * -4 + locx, 0 * gap + locy, yellow);
    pixel(gap * -5 + locx, 0 * gap + locy, orange);
    pixel(gap * -6 + locx, 0 * gap + locy, orange);
    pixel(gap * -7 + locx, 0 * gap + locy, red);
    pixel(gap * -8 + locx, 0 * gap + locy, red);
    pixel(gap * -9 + locx, 0 * gap + locy, red);
    pixel(gap * -10 + locx, 0 * gap + locy, red);
    pixel(gap * -11 + locx, 0 * gap + locy, brown);
    pixel(gap * -12 + locx, 0 * gap + locy, brown);
    pixel(gap * -13 + locx, 0 * gap + locy, orange);
    pixel(gap * -14 + locx, 0 * gap + locy, yellow);
    pixel(gap * -15 + locx, 0 * gap + locy, yellow);
    pixel(gap * -16 + locx, 0 * gap + locy, red);
    pixel(gap * -17 + locx, 0 * gap + locy, brown);
    pixel(gap * 15 + locx, -1 * gap + locy, brown);
    pixel(gap * 14 + locx, -1 * gap + locy, red);
    pixel(gap * 13 + locx, -1 * gap + locy, orange);
    pixel(gap * 12 + locx, -1 * gap + locy, orange);
    pixel(gap * 11 + locx, -1 * gap + locy, yellow);
    pixel(gap * 10 + locx, -1 * gap + locy, yellow);
    pixel(gap * 9 + locx, -1 * gap + locy, yellow);
    pixel(gap * 8 + locx, -1 * gap + locy, orange);
    pixel(gap * 7 + locx, -1 * gap + locy, red);
    pixel(gap * 6 + locx, -1 * gap + locy, brown);
    pixel(gap * 5 + locx, -1 * gap + locy, brown);
    pixel(gap * 4 + locx, -1 * gap + locy, brown);
    pixel(gap * 3 + locx, -1 * gap + locy, brown);
    pixel(gap * 2 + locx, -1 * gap + locy, red);
    pixel(gap * 1 + locx, -1 * gap + locy, red);
    pixel(gap * 0 + locx, -1 * gap + locy, red);
    pixel(gap * -1 + locx, -1 * gap + locy, red);
    pixel(gap * -2 + locx, -1 * gap + locy, red);
    pixel(gap * -3 + locx, -1 * gap + locy, orange);
    pixel(gap * -4 + locx, -1 * gap + locy, orange);
    pixel(gap * -5 + locx, -1 * gap + locy, orange);
    pixel(gap * -6 + locx, -1 * gap + locy, orange);
    pixel(gap * -7 + locx, -1 * gap + locy, red);
    pixel(gap * -8 + locx, -1 * gap + locy, red);
    pixel(gap * -9 + locx, -1 * gap + locy, red);
    pixel(gap * -10 + locx, -1 * gap + locy, red);
    pixel(gap * -11 + locx, -1 * gap + locy, brown);
    pixel(gap * -12 + locx, -1 * gap + locy, red);
    pixel(gap * -13 + locx, -1 * gap + locy, orange);
    pixel(gap * -14 + locx, -1 * gap + locy, yellow);
    pixel(gap * -15 + locx, -1 * gap + locy, yellow);
    pixel(gap * -16 + locx, -1 * gap + locy, red);
    pixel(gap * -17 + locx, -1 * gap + locy, brown);
    pixel(gap * 15 + locx, -2 * gap + locy, brown);
    pixel(gap * 14 + locx, -2 * gap + locy, red);
    pixel(gap * 13 + locx, -2 * gap + locy, red);
    pixel(gap * 12 + locx, -2 * gap + locy, orange);
    pixel(gap * 11 + locx, -2 * gap + locy, orange);
    pixel(gap * 10 + locx, -2 * gap + locy, orange);
    pixel(gap * 9 + locx, -2 * gap + locy, orange);
    pixel(gap * 8 + locx, -2 * gap + locy, orange);
    pixel(gap * 7 + locx, -2 * gap + locy, red);
    pixel(gap * 6 + locx, -2 * gap + locy, brown);
    pixel(gap * 5 + locx, -2 * gap + locy, brown);
    pixel(gap * 4 + locx, -2 * gap + locy, brown);
    pixel(gap * 3 + locx, -2 * gap + locy, brown);
    pixel(gap * 2 + locx, -2 * gap + locy, brown);
    pixel(gap * 1 + locx, -2 * gap + locy, red);
    pixel(gap * 0 + locx, -2 * gap + locy, red);
    pixel(gap * -1 + locx, -2 * gap + locy, red);
    pixel(gap * -2 + locx, -2 * gap + locy, red);
    pixel(gap * -3 + locx, -2 * gap + locy, red);
    pixel(gap * -4 + locx, -2 * gap + locy, orange);
    pixel(gap * -5 + locx, -2 * gap + locy, orange);
    pixel(gap * -6 + locx, -2 * gap + locy, red);
    pixel(gap * -7 + locx, -2 * gap + locy, red);
    pixel(gap * -8 + locx, -2 * gap + locy, red);
    pixel(gap * -9 + locx, -2 * gap + locy, brown);
    pixel(gap * -10 + locx, -2 * gap + locy, brown);
    pixel(gap * -11 + locx, -2 * gap + locy, brown);
    pixel(gap * -12 + locx, -2 * gap + locy, red);
    pixel(gap * -13 + locx, -2 * gap + locy, orange);
    pixel(gap * -14 + locx, -2 * gap + locy, orange);
    pixel(gap * -15 + locx, -2 * gap + locy, orange);
    pixel(gap * -16 + locx, -2 * gap + locy, red);
    pixel(gap * -17 + locx, -2 * gap + locy, brown);
    pixel(gap * 15 + locx, -3 * gap + locy, brown);
    pixel(gap * 14 + locx, -3 * gap + locy, red);
    pixel(gap * 13 + locx, -3 * gap + locy, brown);
    pixel(gap * 12 + locx, -3 * gap + locy, red);
    pixel(gap * 11 + locx, -3 * gap + locy, orange);
    pixel(gap * 10 + locx, -3 * gap + locy, orange);
    pixel(gap * 9 + locx, -3 * gap + locy, red);
    pixel(gap * 8 + locx, -3 * gap + locy, red);
    pixel(gap * 7 + locx, -3 * gap + locy, red);
    pixel(gap * 6 + locx, -3 * gap + locy, brown);
    pixel(gap * 5 + locx, -3 * gap + locy, red);
    pixel(gap * 4 + locx, -3 * gap + locy, red);
    pixel(gap * 3 + locx, -3 * gap + locy, brown);
    pixel(gap * 2 + locx, -3 * gap + locy, brown);
    pixel(gap * 1 + locx, -3 * gap + locy, brown);
    pixel(gap * 0 + locx, -3 * gap + locy, red);
    pixel(gap * -1 + locx, -3 * gap + locy, red);
    pixel(gap * -2 + locx, -3 * gap + locy, red);
    pixel(gap * -3 + locx, -3 * gap + locy, red);
    pixel(gap * -4 + locx, -3 * gap + locy, red);
    pixel(gap * -5 + locx, -3 * gap + locy, red);
    pixel(gap * -6 + locx, -3 * gap + locy, red);
    pixel(gap * -7 + locx, -3 * gap + locy, brown);
    pixel(gap * -8 + locx, -3 * gap + locy, brown);
    pixel(gap * -9 + locx, -3 * gap + locy, red);
    pixel(gap * -10 + locx, -3 * gap + locy, red);
    pixel(gap * -11 + locx, -3 * gap + locy, brown);
    pixel(gap * -12 + locx, -3 * gap + locy, red);
    pixel(gap * -13 + locx, -3 * gap + locy, red);
    pixel(gap * -14 + locx, -3 * gap + locy, red);
    pixel(gap * -15 + locx, -3 * gap + locy, orange);
    pixel(gap * -16 + locx, -3 * gap + locy, red);
    pixel(gap * -17 + locx, -3 * gap + locy, red);
    pixel(gap * 15 + locx, -4 * gap + locy, brown);
    pixel(gap * 14 + locx, -4 * gap + locy, brown);
    pixel(gap * 13 + locx, -4 * gap + locy, red);
    pixel(gap * 12 + locx, -4 * gap + locy, red);
    pixel(gap * 11 + locx, -4 * gap + locy, red);
    pixel(gap * 10 + locx, -4 * gap + locy, red);
    pixel(gap * 9 + locx, -4 * gap + locy, red);
    pixel(gap * 8 + locx, -4 * gap + locy, red);
    pixel(gap * 7 + locx, -4 * gap + locy, red);
    pixel(gap * 6 + locx, -4 * gap + locy, red);
    pixel(gap * 5 + locx, -4 * gap + locy, brown);
    pixel(gap * 4 + locx, -4 * gap + locy, red);
    pixel(gap * 3 + locx, -4 * gap + locy, orange);
    pixel(gap * 2 + locx, -4 * gap + locy, yellow);
    pixel(gap * 1 + locx, -4 * gap + locy, red);
    pixel(gap * 0 + locx, -4 * gap + locy, red);
    pixel(gap * -1 + locx, -4 * gap + locy, red);
    pixel(gap * -2 + locx, -4 * gap + locy, brown);
    pixel(gap * -3 + locx, -4 * gap + locy, red);
    pixel(gap * -4 + locx, -4 * gap + locy, brown);
    pixel(gap * -5 + locx, -4 * gap + locy, brown);
    pixel(gap * -6 + locx, -4 * gap + locy, brown);
    pixel(gap * -7 + locx, -4 * gap + locy, brown);
    pixel(gap * -8 + locx, -4 * gap + locy, red);
    pixel(gap * -9 + locx, -4 * gap + locy, red);
    pixel(gap * -10 + locx, -4 * gap + locy, red);
    pixel(gap * -11 + locx, -4 * gap + locy, red);
    pixel(gap * -12 + locx, -4 * gap + locy, brown);
    pixel(gap * -13 + locx, -4 * gap + locy, brown);
    pixel(gap * -14 + locx, -4 * gap + locy, red);
    pixel(gap * -15 + locx, -4 * gap + locy, red);
    pixel(gap * -16 + locx, -4 * gap + locy, red);
    pixel(gap * -17 + locx, -4 * gap + locy, brown);
    pixel(gap * 14 + locx, -5 * gap + locy, brown);
    pixel(gap * 13 + locx, -5 * gap + locy, brown);
    pixel(gap * 12 + locx, -5 * gap + locy, red);
    pixel(gap * 11 + locx, -5 * gap + locy, red);
    pixel(gap * 10 + locx, -5 * gap + locy, red);
    pixel(gap * 9 + locx, -5 * gap + locy, brown);
    pixel(gap * 8 + locx, -5 * gap + locy, brown);
    pixel(gap * 7 + locx, -5 * gap + locy, brown);
    pixel(gap * 6 + locx, -5 * gap + locy, orange);
    pixel(gap * 5 + locx, -5 * gap + locy, yellow);
    pixel(gap * 4 + locx, -5 * gap + locy, white);
    pixel(gap * 3 + locx, -5 * gap + locy, white);
    pixel(gap * 2 + locx, -5 * gap + locy, orange);
    pixel(gap * 1 + locx, -5 * gap + locy, red);
    pixel(gap * 0 + locx, -5 * gap + locy, red);
    pixel(gap * -1 + locx, -5 * gap + locy, brown);
    pixel(gap * -2 + locx, -5 * gap + locy, red);
    pixel(gap * -3 + locx, -5 * gap + locy, red);
    pixel(gap * -4 + locx, -5 * gap + locy, brown);
    pixel(gap * -5 + locx, -5 * gap + locy, brown);
    pixel(gap * -6 + locx, -5 * gap + locy, red);
    pixel(gap * -7 + locx, -5 * gap + locy, red);
    pixel(gap * -8 + locx, -5 * gap + locy, orange);
    pixel(gap * -9 + locx, -5 * gap + locy, red);
    pixel(gap * -10 + locx, -5 * gap + locy, brown);
    pixel(gap * -11 + locx, -5 * gap + locy, brown);
    pixel(gap * -12 + locx, -5 * gap + locy, brown);
    pixel(gap * -13 + locx, -5 * gap + locy, brown);
    pixel(gap * -14 + locx, -5 * gap + locy, brown);
    pixel(gap * -15 + locx, -5 * gap + locy, red);
    pixel(gap * -16 + locx, -5 * gap + locy, brown);
    pixel(gap * -17 + locx, -5 * gap + locy, brown);
    pixel(gap * 13 + locx, -6 * gap + locy, brown);
    pixel(gap * 12 + locx, -6 * gap + locy, brown);
    pixel(gap * 11 + locx, -6 * gap + locy, red);
    pixel(gap * 10 + locx, -6 * gap + locy, brown);
    pixel(gap * 9 + locx, -6 * gap + locy, brown);
    pixel(gap * 8 + locx, -6 * gap + locy, brown);
    pixel(gap * 7 + locx, -6 * gap + locy, red);
    pixel(gap * 6 + locx, -6 * gap + locy, yellow);
    pixel(gap * 5 + locx, -6 * gap + locy, white);
    pixel(gap * 4 + locx, -6 * gap + locy, white);
    pixel(gap * 3 + locx, -6 * gap + locy, yellow);
    pixel(gap * 2 + locx, -6 * gap + locy, orange);
    pixel(gap * 1 + locx, -6 * gap + locy, orange);
    pixel(gap * 0 + locx, -6 * gap + locy, red);
    pixel(gap * -1 + locx, -6 * gap + locy, brown);
    pixel(gap * -2 + locx, -6 * gap + locy, brown);
    pixel(gap * -3 + locx, -6 * gap + locy, red);
    pixel(gap * -4 + locx, -6 * gap + locy, brown);
    pixel(gap * -5 + locx, -6 * gap + locy, red);
    pixel(gap * -6 + locx, -6 * gap + locy, red);
    pixel(gap * -7 + locx, -6 * gap + locy, orange);
    pixel(gap * -8 + locx, -6 * gap + locy, yellow);
    pixel(gap * -9 + locx, -6 * gap + locy, orange);
    pixel(gap * -10 + locx, -6 * gap + locy, red);
    pixel(gap * -11 + locx, -6 * gap + locy, red);
    pixel(gap * -12 + locx, -6 * gap + locy, red);
    pixel(gap * -13 + locx, -6 * gap + locy, red);
    pixel(gap * -14 + locx, -6 * gap + locy, brown);
    pixel(gap * -15 + locx, -6 * gap + locy, brown);
    pixel(gap * -16 + locx, -6 * gap + locy, brown);
    pixel(gap * 12 + locx, -7 * gap + locy, brown);
    pixel(gap * 11 + locx, -7 * gap + locy, brown);
    pixel(gap * 10 + locx, -7 * gap + locy, brown);
    pixel(gap * 9 + locx, -7 * gap + locy, red);
    pixel(gap * 8 + locx, -7 * gap + locy, red);
    pixel(gap * 7 + locx, -7 * gap + locy, orange);
    pixel(gap * 6 + locx, -7 * gap + locy, yellow);
    pixel(gap * 5 + locx, -7 * gap + locy, yellow);
    pixel(gap * 4 + locx, -7 * gap + locy, yellow);
    pixel(gap * 3 + locx, -7 * gap + locy, yellow);
    pixel(gap * 2 + locx, -7 * gap + locy, orange);
    pixel(gap * 1 + locx, -7 * gap + locy, orange);
    pixel(gap * 0 + locx, -7 * gap + locy, red);
    pixel(gap * -1 + locx, -7 * gap + locy, brown);
    pixel(gap * -2 + locx, -7 * gap + locy, red);
    pixel(gap * -3 + locx, -7 * gap + locy, red);
    pixel(gap * -4 + locx, -7 * gap + locy, brown);
    pixel(gap * -5 + locx, -7 * gap + locy, red);
    pixel(gap * -6 + locx, -7 * gap + locy, orange);
    pixel(gap * -7 + locx, -7 * gap + locy, yellow);
    pixel(gap * -8 + locx, -7 * gap + locy, white);
    pixel(gap * -9 + locx, -7 * gap + locy, yellow);
    pixel(gap * -10 + locx, -7 * gap + locy, orange);
    pixel(gap * -11 + locx, -7 * gap + locy, orange);
    pixel(gap * -12 + locx, -7 * gap + locy, orange);
    pixel(gap * -13 + locx, -7 * gap + locy, red);
    pixel(gap * -14 + locx, -7 * gap + locy, red);
    pixel(gap * -15 + locx, -7 * gap + locy, brown);
    pixel(gap * 12 + locx, -8 * gap + locy, brown);
    pixel(gap * 11 + locx, -8 * gap + locy, brown);
    pixel(gap * 10 + locx, -8 * gap + locy, red);
    pixel(gap * 9 + locx, -8 * gap + locy, red);
    pixel(gap * 8 + locx, -8 * gap + locy, orange);
    pixel(gap * 7 + locx, -8 * gap + locy, orange);
    pixel(gap * 6 + locx, -8 * gap + locy, yellow);
    pixel(gap * 5 + locx, -8 * gap + locy, orange);
    pixel(gap * 4 + locx, -8 * gap + locy, orange);
    pixel(gap * 3 + locx, -8 * gap + locy, orange);
    pixel(gap * 2 + locx, -8 * gap + locy, red);
    pixel(gap * 1 + locx, -8 * gap + locy, red);
    pixel(gap * 0 + locx, -8 * gap + locy, red);
    pixel(gap * -1 + locx, -8 * gap + locy, brown);
    pixel(gap * -2 + locx, -8 * gap + locy, brown);
    pixel(gap * -3 + locx, -8 * gap + locy, red);
    pixel(gap * -4 + locx, -8 * gap + locy, red);
    pixel(gap * -5 + locx, -8 * gap + locy, brown);
    pixel(gap * -6 + locx, -8 * gap + locy, red);
    pixel(gap * -7 + locx, -8 * gap + locy, orange);
    pixel(gap * -8 + locx, -8 * gap + locy, yellow);
    pixel(gap * -9 + locx, -8 * gap + locy, yellow);
    pixel(gap * -10 + locx, -8 * gap + locy, yellow);
    pixel(gap * -11 + locx, -8 * gap + locy, yellow);
    pixel(gap * -12 + locx, -8 * gap + locy, orange);
    pixel(gap * -13 + locx, -8 * gap + locy, orange);
    pixel(gap * -14 + locx, -8 * gap + locy, red);
    pixel(gap * -15 + locx, -8 * gap + locy, brown);
    pixel(gap * 11 + locx, -9 * gap + locy, brown);
    pixel(gap * 10 + locx, -9 * gap + locy, red);
    pixel(gap * 9 + locx, -9 * gap + locy, orange);
    pixel(gap * 8 + locx, -9 * gap + locy, orange);
    pixel(gap * 7 + locx, -9 * gap + locy, orange);
    pixel(gap * 6 + locx, -9 * gap + locy, orange);
    pixel(gap * 5 + locx, -9 * gap + locy, orange);
    pixel(gap * 4 + locx, -9 * gap + locy, red);
    pixel(gap * 3 + locx, -9 * gap + locy, red);
    pixel(gap * 2 + locx, -9 * gap + locy, red);
    pixel(gap * 1 + locx, -9 * gap + locy, red);
    pixel(gap * 0 + locx, -9 * gap + locy, brown);
    pixel(gap * -1 + locx, -9 * gap + locy, brown);
    pixel(gap * -2 + locx, -9 * gap + locy, brown);
    pixel(gap * -3 + locx, -9 * gap + locy, brown);
    pixel(gap * -4 + locx, -9 * gap + locy, brown);
    pixel(gap * -5 + locx, -9 * gap + locy, brown);
    pixel(gap * -6 + locx, -9 * gap + locy, brown);
    pixel(gap * -7 + locx, -9 * gap + locy, red);
    pixel(gap * -8 + locx, -9 * gap + locy, orange);
    pixel(gap * -9 + locx, -9 * gap + locy, orange);
    pixel(gap * -10 + locx, -9 * gap + locy, orange);
    pixel(gap * -11 + locx, -9 * gap + locy, orange);
    pixel(gap * -12 + locx, -9 * gap + locy, orange);
    pixel(gap * -13 + locx, -9 * gap + locy, red);
    pixel(gap * -14 + locx, -9 * gap + locy, red);
    pixel(gap * -15 + locx, -9 * gap + locy, brown);
    pixel(gap * 11 + locx, -10 * gap + locy, brown);
    pixel(gap * 10 + locx, -10 * gap + locy, brown);
    pixel(gap * 9 + locx, -10 * gap + locy, red);
    pixel(gap * 8 + locx, -10 * gap + locy, red);
    pixel(gap * 7 + locx, -10 * gap + locy, red);
    pixel(gap * 6 + locx, -10 * gap + locy, red);
    pixel(gap * 5 + locx, -10 * gap + locy, red);
    pixel(gap * 4 + locx, -10 * gap + locy, red);
    pixel(gap * 3 + locx, -10 * gap + locy, red);
    pixel(gap * 2 + locx, -10 * gap + locy, red);
    pixel(gap * 1 + locx, -10 * gap + locy, brown);
    pixel(gap * 0 + locx, -10 * gap + locy, brown);
    pixel(gap * -1 + locx, -10 * gap + locy, brown);
    pixel(gap * -2 + locx, -10 * gap + locy, brown);
    pixel(gap * -3 + locx, -10 * gap + locy, brown);
    pixel(gap * -4 + locx, -10 * gap + locy, brown);
    pixel(gap * -5 + locx, -10 * gap + locy, brown);
    pixel(gap * -6 + locx, -10 * gap + locy, brown);
    pixel(gap * -7 + locx, -10 * gap + locy, red);
    pixel(gap * -8 + locx, -10 * gap + locy, red);
    pixel(gap * -9 + locx, -10 * gap + locy, red);
    pixel(gap * -10 + locx, -10 * gap + locy, red);
    pixel(gap * -11 + locx, -10 * gap + locy, red);
    pixel(gap * -12 + locx, -10 * gap + locy, red);
    pixel(gap * -13 + locx, -10 * gap + locy, red);
    pixel(gap * -14 + locx, -10 * gap + locy, brown);
    pixel(gap * -15 + locx, -10 * gap + locy, brown);
    pixel(gap * 0 + locx, -11 * gap + locy, brown);
    pixel(gap * 1 + locx, -11 * gap + locy, brown);
    pixel(gap * 2 + locx, -11 * gap + locy, brown);
    pixel(gap * 3 + locx, -11 * gap + locy, brown);
    pixel(gap * 4 + locx, -11 * gap + locy, red);
    pixel(gap * 5 + locx, -11 * gap + locy, red);
    pixel(gap * 6 + locx, -11 * gap + locy, red);
    pixel(gap * 7 + locx, -11 * gap + locy, red);
    pixel(gap * 8 + locx, -11 * gap + locy, red);
    pixel(gap * 9 + locx, -11 * gap + locy, brown);
    pixel(gap * 10 + locx, -11 * gap + locy, red);
    pixel(gap * 11 + locx, -11 * gap + locy, brown);
    pixel(gap * -1 + locx, -11 * gap + locy, brown);
    pixel(gap * -6 + locx, -11 * gap + locy, brown);
    pixel(gap * -7 + locx, -11 * gap + locy, brown);
    pixel(gap * -8 + locx, -11 * gap + locy, brown);
    pixel(gap * -9 + locx, -11 * gap + locy, red);
    pixel(gap * -10 + locx, -11 * gap + locy, red);
    pixel(gap * -11 + locx, -11 * gap + locy, red);
    pixel(gap * -12 + locx, -11 * gap + locy, brown);
    pixel(gap * -13 + locx, -11 * gap + locy, brown);
    pixel(gap * -14 + locx, -11 * gap + locy, brown);
    pixel(gap * 1 + locx, -12 * gap + locy, brown);
    pixel(gap * 2 + locx, -12 * gap + locy, brown);
    pixel(gap * 3 + locx, -12 * gap + locy, red);
    pixel(gap * 4 + locx, -12 * gap + locy, red);
    pixel(gap * 5 + locx, -12 * gap + locy, brown);
    pixel(gap * 6 + locx, -12 * gap + locy, brown);
    pixel(gap * 7 + locx, -12 * gap + locy, brown);
    pixel(gap * 8 + locx, -12 * gap + locy, brown);
    pixel(gap * 9 + locx, -12 * gap + locy, brown);
    pixel(gap * 10 + locx, -12 * gap + locy, brown);
    pixel(gap * 11 + locx, -12 * gap + locy, brown);
    pixel(gap * -9 + locx, -12 * gap + locy, brown);
    pixel(gap * -10 + locx, -12 * gap + locy, brown);
    pixel(gap * -11 + locx, -12 * gap + locy, brown);
    pixel(gap * -12 + locx, -12 * gap + locy, brown);
    pixel(gap * -13 + locx, -12 * gap + locy, brown);
    pixel(gap * 3 + locx, -13 * gap + locy, brown);
    pixel(gap * 4 + locx, -13 * gap + locy, brown);
    pixel(gap * 5 + locx, -13 * gap + locy, brown);
    pixel(gap * 6 + locx, -13 * gap + locy, brown);
    pixel(gap * 7 + locx, -13 * gap + locy, brown);
    pixel(gap * 8 + locx, -13 * gap + locy, brown);
    pixel(gap * 9 + locx, -13 * gap + locy, brown);



}

void bullet(assets_t p) {

    color_t purple = { 0,0,0 };

    int gap = -3;

    if (bullety == -226)

        bulletx = p.coordinate.x;


    for (int mocha = -24; mocha < 25; mocha += 48) {

        pixel(bulletx + mocha, bullety, purple);
        pixel(bulletx + mocha, bullety - 3, purple);
        pixel(bulletx + mocha, bullety - 6, purple);

    }

}

void hitbox(assets_t aliens[]) {

    int gap = 3;

    for (int mocha = 0; mocha < 5; mocha++) {

        if (bulletx <= (aliens + mocha)->coordinate.x + 17 * gap && bulletx >= (aliens + mocha)->coordinate.x - 16 * gap && bullety <= (aliens + mocha)->coordinate.y + 6 * gap && bullety >= (aliens + mocha)->coordinate.y - gap * 1) {

            bullet_condition = false;

            bullety = -250;

            bulletx = 1000;

            aliens[mocha].condition = true;

        }

        if (bulletx <= (aliens + mocha)->coordinate.x + 9 * gap && bulletx >= (aliens + mocha)->coordinate.x - 8 * gap && bullety <= (aliens + mocha)->coordinate.y + 5 * gap && bullety >= (aliens + mocha)->coordinate.y - 10 * gap) {

            bullet_condition = false;

            bullety = -250;

            bulletx = 1000;

            aliens[mocha].condition = true;

        }

    }

}

void createexplosion() {

    for (int mocha = 0; mocha < 5; mocha++) {

        if (aliens[mocha].condition == true)

            explosion(aliens[mocha]);

    }

}

double dist(points_t p, points_t a) {

    return sqrt((p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y));
}

void touch_enemy(assets_t Player, assets_t aliens[]) {

    for (int mocha = 0; mocha < 5; mocha++)

        if (Player.area_radius >= dist(Player.coordinate, aliens[mocha].coordinate)) {

            Player.touching = true;

            aliens[mocha].touching = true;

            activetime = false;

        }
}

void asset_circle(assets_t* P, assets_t alien[]) {

    assets_t temp;

    temp = *P;

    touch_enemy(temp, alien);

    *P = temp;

    glColor3ub(125, 0, 255);
    circle_wire(P->coordinate.x, P->coordinate.y, P->area_radius);

    for (int mocha = 0; mocha < 5; mocha++) {

        if (alien[mocha].touching == true)

            circle_wire(alien[mocha].coordinate.x, alien[mocha].coordinate.y, P->area_radius);

    }

}



// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0.25, 0.25, 0.25, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    bullet(Player);

    spacecraft(&Player);

    createalien();

    hitbox(aliens);

    createexplosion();

    asset_circle(&Player, aliens);

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

    if (key == ' ') {

        bullet_condition = true;

    }
    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    //glutPostRedisplay();
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

    if (key == GLUT_KEY_F1)

        activetime != activetime;

    // to refresh the window it calls display() function
    //glutPostRedisplay();
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
    //glutPostRedisplay();
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



    // to refresh the window it calls display() function
    //glutPostRedisplay();
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
    //glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
   // glutPostRedisplay();
}





// if the player hits anyone of the icons, it returns true.
/*bool hitToIcons(assets_t* Player, assets_t aliens[], int size) {
    for (int i = 0; i < size; i++) {
        double d = dist(p->pos, icons[i].pos);
        if (d <= (p->radius + icons[i].radius)) {
            icons[i].hit = true;
            p->hit = true;
        }
    }
    return p->hit;
}
*/


#if TIMER_ON == 1
void onTimer(int v) {


    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (activetime) {

        if (left == true)

            if (Player.coordinate.x > -645)

                Player.coordinate.x -= 8;

        if (right == true)

            if (Player.coordinate.x < 645)

                Player.coordinate.x += 8;

        for (int latte = 0; latte < 5; latte++) {

            aliens[latte].coordinate.y -= 4;
        }

        for (int latte = 0; latte < 5; latte++) {

            if (condition == true)

                if (aliens[latte].coordinate.y < -420) {

                    aliens[latte].coordinate.y = 400 + rand() % 250;

                    switch (latte) {

                    case 0:

                        aliens[latte].coordinate.x = -700 + rand() % ((1350 + 1) - 1120) + 1120;

                        aliens[latte].condition = false;

                        break;

                    case 1:

                        aliens[latte].coordinate.x = -700 + rand() % ((1120 + 1) - 840) + 840;

                        aliens[latte].condition = false;

                        break;

                    case 2:

                        aliens[latte].coordinate.x = -700 + rand() % ((840 + 1) - 560) + 560;

                        aliens[latte].condition = false;

                        break;

                    case 3:

                        aliens[latte].coordinate.x = -700 + rand() % ((560 + 1) - 280) + 280;

                        aliens[latte].condition = false;

                        break;

                    default:

                        aliens[latte].coordinate.x = -700 + rand() % ((280 + 1) - 50) + 50;

                        aliens[latte].condition = false;
                    }

                }

        }


        if (bullet_condition == true) {

            bullety += 8;

            if (bullety > 450) {

                bullet_condition = false;

                bullety = -226;

            }

        }

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
    glutCreateWindow("Space Invade");

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