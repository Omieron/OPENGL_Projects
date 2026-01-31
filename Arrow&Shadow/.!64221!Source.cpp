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

