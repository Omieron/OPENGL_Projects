/*********
PROBLEMS:  When you press to r unfortunately it doesn't restart whole animation, it just re-animate intro part
           Also the ammo aim is not right , when you remove helicopter the ammo also remove
           Points system doesn't work
----------
ADDITIONAL FEATURES:

 There are cycle for animation.
 The helicopter can be removd by arrow keys
 The helicopter can fire ammo
 When animation finish, helicopter will explode itself in the middle of screen
 You can press button, and according to the mouse location its color change
 Top of the screen, there are timer which is reduced
 There are normally two end, but I did not do second finish because of point sysem error so there are just a one end


*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

// My global variables

#define INTRO 0
#define GAME 1
#define END 2


bool activatetime = true, explot = false, bomb_button_activate = false, deneme = false, time = false, restart = false;

int locXt = -200,         //   
locXh = 500, locYh = 50,
locx = 0, locy = 0,
locxA = 0, locyA = 0,
app_position = INTRO, points = 0;

double time_circle = 380;


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

void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
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
    vsprintf_s(str, string, ap);
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


void walltexture() {

    // Baklava pieces

    int x = 0, y = 0;

    for (x = 0; x <= WINDOW_WIDTH; x += 50)

        for (y = 0; y >= -WINDOW_HEIGHT; y -= 60) {

            glColor3ub(133, 80, 30);
            glBegin(GL_QUADS);
            glVertex2f(-700 + x, 370 + y);
            glVertex2f(-675 + x, 340 + y);
            glVertex2f(-650 + x, 370 + y);
            glVertex2f(-675 + x, 400 + y);
            glEnd();

        }
}

void outside() {

    int x = 0, y = 0;

    glColor3ub(66, 154, 237);         // Outside base color
    glBegin(GL_QUADS);
    glVertex2f(-569, 305);
    glVertex2f(569, 305);
    glColor3ub(137, 244, 243);
    glVertex2f(569, -90);
    glVertex2f(-569, -90);
    glEnd();

    glColor3ub(193, 116, 64);
    glRectf(-569, -170, 569, -90);
    glRectf(-569, -305, 569, -280);

    // BUILDING 1

    glColor3ub(0, 0, 0);                // Building 1
    glRectf(-569, -120, -469, 200);

    for (x = -569; x < -469; x += 35)   // Buiilding 1 windows

        for (y = -120; y < 170; y += 30) {

            glColor3ub(255, 255, 255);
            glRectf(x + 7, y + 15, x + 17, y + 30);

        }



    // BUILDING 2

    glColor3ub(64, 176, 211);         // Building 2 
    glRectf(-469, -120, -250, 150);

    for (y = -120; y < 120; y += 35) {    // Buiilding 2 windows

        glColor3ub(24, 117, 144);
        glRectf(-459, y + 25, -260, y + 45);

    }



    // BUILDING 3

    glColor3ub(232, 126, 116);        // Buildiing 3
    glRectf(-200, -120, 0, 280);


    for (x = 0; x < 150; x += 65)    // Building 3 windows

        for (y = 0; y < 350; y += 55) {
            glColor3ub(24, 117, 100);
            glRectf(-188 + x, -100 + y, -140 + x, -60 + y);

        }



    // BUILDING 4

    glColor3ub(126, 234, 144);            // Building 4 
    glRectf(0, -120, 250, 180);


    for (x = 0; x < 250; x += 65)    // Building 4 windows

        for (y = 0; y < 250; y += 55) {
            glColor3ub(196, 230, 227);
            glRectf(10 + x, -100 + y, 40 + x, -60 + y);

        }


    // BUILDING 5

    glColor3ub(255, 255, 161);        // Building 5
    glRectf(250, -120, 569, 100);

    glColor3ub(95, 225, 182);
    circle(410, 50, 35);

    glRectf(300, -10, 519, -36);
    glColor3ub(0, 0, 0);
    glRectf(360, -120, 460, -60);
    glColor3ub(95, 225, 182);
    glRectf(364, -120, 456, -64);
    glColor3ub(0, 0, 0);
    glRectf(415, -120, 405, -60);

    vprint(385, -28, GLUT_BITMAP_HELVETICA_12, "SCHOOL");

    // TREE

    for (int treex = 0; treex < 1000; treex += 110) {

        glColor3ub(109, 61, 10);
        glRectf(-500 + treex, -75, -490 + treex, -150);
        glColor3ub(59, 143, 7);
        circle(-495 + treex, -80, 35);

    }

    // ROAD

    glColor3ub(86, 86, 86);             // Road
    glBegin(GL_QUADS);
    glVertex2f(-569, -280);
    glVertex2f(-569, -170);
    glVertex2f(569, -170);
    glVertex2f(569, -280);
    glEnd();

    for (x = 0; x + (-569) < 569; x += 240) {   // Road white line

        glColor3ub(255, 255, 255);
        glRectf(-549 + x, -220, -419 + x, -230);

    }
}

void window() {

    // WINDOW 

    glColor3ub(0, 0, 0);       // outer window outline
    glRectf(-587, 323, 587, -323);

    glColor3ub(255, 255, 255);    // window structure
    glRectf(-583, 319, 583, -319);

    glColor3ub(0, 0, 0);    // inner window outline
    glRectf(-573, 309, 573, -309);

    vprint(-200, -350, GLUT_BITMAP_TIMES_ROMAN_24, "OMER FARUK ASIL FIRST GAME ATTEMPT");

}

void background() {

    walltexture(); // Baklava Pieces

    window();

    outside();

    // WALL TRAITS 

    glColor3ub(244, 230, 200);       //  Left Candle
    glRectf(-635, 110, -650, 160);

    glColor3ub(0, 0, 0);             // Left Candle wick
    glRectf(-642, 160, -643, 170);

    glColor3ub(255, 242, 0);          // Candle Fires
    circle(-643, 170, 6);

    glColor3ub(255, 127, 39);          // Candle Fires
    circle(-643, 170, 4);

    glColor3ub(236, 28, 36);           // Candle Fires  
    circle(-643, 170, 2);

    glColor3ub(119, 60, 17);        //Left corner stain
    glRectf(-615, 120, -670, 110);
    glRectf(-620, 110, -665, 100);
    glRectf(-625, 100, -660, 90);
    glRectf(-630, 90, -655, 80);
    glRectf(-635, 80, -650, 70);
    glRectf(-640, 70, -645, 60);

    glColor3ub(0, 0, 0);             //Left corner stain black lines
    glRectf(-615, 116, -670, 114);
    glRectf(-620, 106, -665, 104);
    glRectf(-625, 96, -660, 94);
    glRectf(-630, 86, -655, 84);
    glRectf(-635, 76, -650, 74);
    glRectf(-640, 66, -645, 64);


    glColor3ub(244, 230, 200);       //  Right Candle
    glRectf(635, 110, 650, 160);

    glColor3ub(0, 0, 0);             // Right Candle wick
    glRectf(642, 160, 643, 170);

    glColor3ub(255, 242, 0);          // Candle Fires
    circle(643, 170, 6);

    glColor3ub(255, 127, 39);          // Candle Fires
    circle(643, 170, 4);

    glColor3ub(236, 28, 36);           // Candle Fires  
    circle(643, 170, 2);

    glColor3ub(119, 60, 17);        //Right corner stain
    glRectf(615, 120, 670, 110);
    glRectf(620, 110, 665, 100);
    glRectf(625, 100, 660, 90);
    glRectf(630, 90, 655, 80);
    glRectf(635, 80, 650, 70);
    glRectf(640, 70, 645, 60);

    glColor3ub(0, 0, 0);             //Right corner stain black lines
    glRectf(615, 116, 670, 114);
    glRectf(620, 106, 665, 104);
    glRectf(625, 96, 660, 94);
    glRectf(630, 86, 655, 84);
    glRectf(635, 76, 650, 74);
    glRectf(640, 66, 645, 64);

}

void tank(int x) {

    int k, p;

    glColor3ub(0, 250, 0);            // Body part of tank
    glRectf(-600 + x, -230, -430 + x, -150);

    glColor3ub(0, 125, 0);             // Head of tank
    glRectf(-545 + x, -150, -490 + x, -115);

    glColor3ub(86, 76, 45);             // tank barrel
    glRectf(-490 + x, -135, -370 + x, -125);

    glColor3ub(86, 76, 45);             // Main part of tires
    glRectf(-600 + x, -230, -430 + x, -270);

    glColor3ub(86, 76, 45);             // Smooth pass for left part of tires 

    circle(-600 + x, -250, 20);

    glColor3ub(86, 76, 45);             // Smooth pass for right part of tires
    circle(-430 + x, -250, 20);

    for (k = 0; k < 200; k += 40) {    // Tires

        glColor3ub(0, 0, 0);
        circle(-595 + k + x, -250, 15);

    }
}

void duration() {  // Show how many time left


    glColor3ub(125, 125, 0);
    glRectf(-380, 370, 380, 340);


    if (time == true) {

        glColor3ub(255, 0, 0);
        glRectf(-380, 370, time_circle, 340);
    }



}

void helicopter(int x, int y) {

    // HELICOPTER TAIL

    glColor3ub(52, 107, 33);  // Main tail of helicopter
    glRectf(50 + x, 25 + y, 195 + x, 35 + y);

    glColor3ub(0, 0, 0);  // Top tail of helicopter
    glBegin(GL_QUADS);
    glVertex2f(180 + x, 35 + y);
    glVertex2f(190 + x, 50 + y);
    glVertex2f(200 + x, 50 + y);
    glVertex2f(190 + x, 35 + y);
    glEnd();

    glColor3ub(0, 0, 0);  // Bottom tail of helicopter
    glBegin(GL_QUADS);
    glVertex2f(180 + x, 25 + y);
    glVertex2f(190 + x, 15 + y);
    glVertex2f(200 + x, 15 + y);
    glVertex2f(190 + x, 25 + y);
    glEnd();

    glColor3ub(52, 107, 33);   // Helicopter body part 
    glBegin(GL_QUADS);
    glVertex2f(0 + x, 0 + y);
    glVertex2f(90 + x, 0 + y);
    glVertex2f(120 + x, 50 + y);
    glVertex2f(30 + x, 50 + y);
    glEnd();

    glColor3ub(0, 0, 0);      // Conection between body and propoller
    glRectf(67 + x, 50 + y, 73 + x, 65 + y);

    glColor3ub(0, 0, 0);      // Propoller
    glRectf(20 + x, 65 + y, 130 + x, 71 + y);

    circle(20 + x, 68 + y, 3);  // smooth angle for propoller's right side
    circle(130 + x, 68 + y, 3); // smooth angle for propoller's right side

    glColor3ub(0, 0, 0);  // Front window
    glBegin(GL_POLYGON);
    glVertex2f(24 + x, 40 + y);
    glVertex2f(30 + x, 37 + y);
    glVertex2f(40 + x, 34 + y);
    glVertex2f(45 + x, 27 + y);
    glVertex2f(50 + x, 15 + y);
    glVertex2f(9 + x, 15 + y);
    glEnd();


    glColor3ub(140, 255, 251);  // Side window 
    glRectf(70 + x, 15 + y, 80 + x, 40 + y);


    glColor3ub(255, 255, 251);  // line on the helicopter
    glBegin(GL_QUADS);
    glVertex2f(88 + x, 40 + y);
    glVertex2f(89 + x, 43 + y);
    glVertex2f(96 + x, 18 + y);
    glVertex2f(95 + x, 15 + y);
    glEnd();

    // BOTTOM PART OF HELICOPTER

    glColor3ub(88, 88, 88);    //Above part of gun
    glRectf(10 + x, 5 + y, 50 + x, -5 + y);
    circle(10 + x, 0 + y, 5);
    circle(50 + x, 0 + y, 5);

    glBegin(GL_QUADS);      // Gun
    glVertex2f(30 + x, -10 + y);
    glVertex2f(33 + x, -14 + y);
    glVertex2f(9 + x, -22 + y);
    glVertex2f(6 + x, -18 + y);
    glEnd();

    glColor3ub(88, 88, 88);  //bottom part of helicopter
    glRectf(25 + x, 5 + y, 35 + x, -10 + y);
    circle(30 + x, -10 + y, 5);  // smooth botom part

}

// Button texture
void button() {

    glColor3ub(208, 208, 218);       // Bomb anten
    glRectf(642, -50, 648, 20);
    circle(646, 20, 2);

    glColor3ub(0, 0, 0);             // Bomb control outline          
    glRectf(612, -46, 682, -152);

    glColor3ub(86, 86, 86);             // Bomb control           
    glRectf(612, -46, 676, -146);

    glColor3ub(175, 0, 0);           // Bomb button  
    circle(645, -100, 20);

    if (bomb_button_activate == true) {

        glColor3ub(255, 0, 0);           // Bomb button  
        circle(645, -100, 20);

    }

}

// For tank and heliopter extra texture
// I made this because of avoiding some bugs 
void kaplama() {

    // LEFT SIDE

    glColor3ub(155, 102, 62);
    glRectf(-700, -400, -586, -20);

    int x = 0, y = 0;

    for (x = 0; x < 99; x += 50)

        for (y = 0; y >= -WINDOW_HEIGHT; y -= 60) {   // Wall texture for tank

            glColor3ub(133, 80, 30);
            glBegin(GL_QUADS);
            glVertex2f(-700 + x, -50 + y);
            glVertex2f(-675 + x, -80 + y);
            glVertex2f(-650 + x, -50 + y);
            glVertex2f(-675 + x, -20 + y);
            glEnd();

        }

    for (y = 0; y >= -WINDOW_HEIGHT; y -= 60) {  // Continue to wall design

        glColor3ub(133, 80, 30);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(-600, -50 + y);
        glVertex2f(-575, -80 + y);
        glVertex2f(-575, -20 + y);
        glEnd();

    }


    glColor3ub(0, 0, 0);               // Black line
    glRectf(-587, 319, -583, -323);

    glColor3ub(255, 255, 255);         // White line
    glRectf(-583, 315, -570, -319);

    glColor3ub(0, 0, 0);               //  Black line
    glRectf(-570, 309, -566, -308);


    // RIGHT SIDE

    glColor3ub(155, 102, 62);
    glRectf(586, -400, 700, -20);

    for (x = 0; x < 99; x += 50)

        for (y = 0; y >= -360; y -= 60) {   // Wall texture for tank

            glColor3ub(133, 80, 30);
            glBegin(GL_QUADS);
            glVertex2f(600 + x, -50 + y);
            glVertex2f(625 + x, -80 + y);
            glVertex2f(650 + x, -50 + y);
            glVertex2f(625 + x, -20 + y);
            glEnd();

        }

    for (y = 0; y >= -WINDOW_HEIGHT; y -= 60) {  // Continue to wall design

        glColor3ub(133, 80, 30);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(600, -50 + y);
        glVertex2f(586, -66 + y);
        glVertex2f(586, -34 + y);
        glEnd();

    }

    glColor3ub(0, 0, 0);               // Black line
    glRectf(583, 319, 587, -323);

    glColor3ub(255, 255, 255);         // White line
    glRectf(570, 315, 583, -319);

    glColor3ub(0, 0, 0);               // Black line
    glRectf(566, 309, 570, -308);

    button();

}

void ammo(int x, int y) {   // Ammo animation

    int tempx, tempy;

    glColor3ub(255, 255, 255);
    circle(7 + x + locxA, -20 + y + locyA, 3);

    tempx = 7 + x + locxA;

    tempy = -20 + y + locyA;

    if (tempx >= (-600 + locXt) && tempx <= (-430 + x) && tempy <= (-150 + y) && tempy >= (-230 + y))  // How many points did you earn

        points++;

}

void explosion(int x, int y) {   // Explotion animation

    glColor3ub(255, 150, 0);
    glBegin(GL_POLYGON);
    glVertex2f(10 + x, 25 + y);
    glVertex2f(0 + x, 45 + y);
    glVertex2f(40 + x, 50 + y);
    glVertex2f(90 + x, 70 + y);
    glVertex2f(110 + x, 55 + y);
    glVertex2f(150 + x, 45 + y);
    glVertex2f(120 + x, 25 + y);
    glVertex2f(145 + x, -10 + y);
    glVertex2f(115 + x, -15 + y);
    glVertex2f(90 + x, -20 + y);
    glVertex2f(60 + x, -5 + y);
    glVertex2f(20 + x, -10 + y);
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

void menu_intro() {

    glColor3f(1, 1, 0);
    vprint(-100, 100, GLUT_BITMAP_TIMES_ROMAN_24, "Helicopter Defender");

    vprint(-220, 50, GLUT_BITMAP_HELVETICA_12, "Welcome, in this game your aim is to give max damage to the tank until time finish.");

    vprint(-110, 0, GLUT_BITMAP_HELVETICA_12, "You can use arrow keys to move helicopter.");

    vprint(420, -160, GLUT_BITMAP_HELVETICA_12, "To start press this button");

}

void button_arrow() {             // Show the control button

    glColor3ub(255, 255, 0);
    glBegin(GL_POLYGON);
    glVertex2f(550, -100);
    glVertex2f(510, -140);
    glVertex2f(510, -120);
    glVertex2f(445, -120);
    glVertex2f(445, -80);
    glVertex2f(510, -80);
    glVertex2f(510, -60);
    glEnd();

}

void finish_screen() {

    glColor3ub(125, 255, 125);
    glRectf(-100, 50, 100, -50);


    glColor3ub(0, 0, 0);
    vprint(-45, -10, GLUT_BITMAP_TIMES_ROMAN_24, "DEFEAT");


    glColor3ub(125, 255, 125);
    glRectf(-120, -75, 120, -95);


    glColor3ub(0, 0, 0);
    vprint(-95, -93, GLUT_BITMAP_TIMES_ROMAN_24, "Press 'r' to restart!");

}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0.609375, 0.3984375, 0.2421875, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (app_position) {

    case 0:  // Introduction scenerio

        walltexture();

        window();

        button();

        menu_intro();

        button_arrow();

        break;

    case 1:    // Game scenerio



        background();

        duration();

        tank(locXt);

        helicopter(locXh, locYh);

        ammo(locXh, locYh);

        kaplama();

        if (time_circle <= -380) {

            app_position++;
        }

        break;

    case 2:

        if (points < 100) { // If you dont have enough points this execute 

            deneme = true;

            walltexture();

            window();

            background();

            helicopter(locXh, locYh);

            kaplama();

            if (locYh <= -250 && locXh == -100)

                explosion(locXh, locYh);

            if (locYh <= 250) {

                finish_screen();

                restart = true;

            }

        }

        // I didnt add any win condition, so no one win this 


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

    if (key == ' ')

        activatetime = !activatetime;

    if (restart == true)

        if (key == 'r') {

            app_position = INTRO;



        }

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

    int tempx, tempy;

    tempx = x - winWidth / 2;
    tempy = winHeight / 2 - y;

    if (bomb_button_activate == true && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)

        if (app_position < 1) {

            app_position++;

            time = true;

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

bool incircle(double mx, double my, double x, double y, double radius) {  // Check mouse in circle or not

    double tempx, tempy, tempr;

    tempx = mx - x;     // Find x coordinates
    tempy = my - y;     // Find y coordinates
    tempr = sqrtf(tempx * tempx + tempy * tempy);  // find radius

    return tempr <= radius;  // it is true or not
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    int tempx, tempy;

    tempx = x - winWidth / 2;
    tempy = winHeight / 2 - y;

    bomb_button_activate = incircle(tempx, tempy, 645, -100, 20);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1

void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    if (time == true) {

        if (activatetime == true) {

            // TANK MOVEMENT

            locXt += 3;

            if (locXt > 1200)

                locXt = -200;

            // AMMO MOVEMENT

            locxA -= 4;

            locyA -= 4;

            if (locxA <= -573 || locyA <= -309) {

                locxA = 0;

                locyA = 0;

            }

            // TIMER 

            if (time_circle > -380)

                time_circle -= 1; // -1/5

        }

        // HELİCOPTER PROGRESS


        if (deneme == false) {

            if (locXh > 370)

                locXh -= 5;

            if (up)

                if (locYh < 230)

                    locYh += 3;

            if (down)

                if (locYh > -280)

                    locYh -= 3;

            if (left)

                if (locXh > -573)

                    locXh -= 3;

            if (right)

                if (locXh < 573)

                    locXh += 3;
        }

        if (deneme == true) {

            if (locXh > -100)        // If locXh is right side helicopter's crash place

                locXh -= 1;

            else if (locXh < -100)   // If locXh is left side helicopter's crash place

                locXh += 1;

            else

                locXh = -100;        // When locXh is equal to -100  


            if (locXh == -100)       //Helicopter will fall

                if (locYh > -250)

                    locYh -= 3;

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

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Helicopter Defender");

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