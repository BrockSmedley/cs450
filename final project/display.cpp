#include "display.h"
#include "bezier.cpp"
#include <stdlib.h>
#include <stdio.h>


/* non-constant variables */
float *wire_colors[3];               // list to hold wire color vectors
State STATE;
Curve CurvesStatic[NUMCURVES];      // if you are creating a pattern of other curves
Curve Stem;                         // if you are not
int lastPick = 0;                       // hold a copy of the pick to compare


void initDisplayModule(){
    return;
}


// display function for my stuff
void myDisplay(int doAnimate, void (*Animate)(), float dTime, bool DebugOn = false){
    // control animation w/ KB
    if (doAnimate)
        glutIdleFunc(Animate);
    else
        glutIdleFunc(NULL);

    // control state based on time
    if (dTime < 0.35){
        STATE = idle;
    }
    else if (dTime < 0.5){
        STATE = receiving;
    }
    else if (dTime < 0.85){
        STATE = mining;
    }
    else if (dTime < 1.){
        STATE = committing;
        if (pick == lastPick)
            lastPick++;
    }

    for (int i = 0; i < 3; i++)
    {
        wire_colors[i] = wireColorByState(STATE);
    }

    

    if (STATE == idle && lastPick != pick){
        // shift pick by 1 for each round
        pick = (pick + 1) % 3;
        lastPick = pick;
    }

    

    // draw boxes
    drawBoxes(STATE);

    // draw text to show the state
    const char *wc = stateName(STATE);
    float dx = len(stateName(STATE)) * -0.08;
    glTranslatef(dx, 0., 0.);
    DoStrokeString(0., 2., 0., 0.4, stateName(STATE));
    glTranslatef(-dx, 0., 0.);

    // represent the internet as a sphere
    glTranslatef(0., -1., 2.);
    glColor3f(0.1, 0.6, 1.);
    glutSolidSphere(0.3, 20, 20);
    glTranslatef(0., 1., -2.);

    // draw node connections
    drawWires();

    // draw some fuckin stars
    drawStars();
}


void drawBoxes(State s){
    float** colors;
    colors = boxColorsByState(s);

    glTranslatef(-3., 0., 0.);

    for (int i = 0; i < 3; i++)
    {
        glTranslatef(1.5, 0., 0.);
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        glutSolidCube(1.);
    }
    glTranslatef(-1.5, 0., 0.);
}

void drawStars(){
    for (int i = 0; i < 100; i++){
        float d = (float)((rand() % 1000) - 500) / 100.0 + 2;
        float e = (float)((rand() % 1000) - 500) / 100.0 + 2;
        float f = (float)((rand() % 1000) - 500) / 100.0 + 2;

        glColor3f(d/5., e/5., f/5.);
        glTranslatef(d, e, f);
        glutSolidSphere(0.01, 20, 10);
        glTranslatef(-d, -e, -f);
    }
}


// draw wires connecting nodes to sphere
void drawWires(){
    placeLeg(&CurvesStatic[1], 0., -1., 2., 0., -0.5, 0.);
    drawEazierCurve(wire_colors[1], CurvesStatic[1]);

    placeLeg(&CurvesStatic[0], 0., -1., 2., -1.5, -0.5, 0.);
    drawEazierCurve(wire_colors[0], CurvesStatic[0]);

    placeLeg(&CurvesStatic[2], 0., -1., 2., 1.5, -0.5, 0.);
    drawEazierCurve(wire_colors[2], CurvesStatic[2]);
}


// use glut to display a string of characters using a raster font:

void DoRasterString(float x, float y, float z, char *s)
{
    glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);

    char c; // one character to print
    for (; (c = *s) != '\0'; s++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

// use glut to display a string of characters using a stroke font:

void DoStrokeString(float x, float y, float z, float ht, char *s)
{
    glPushMatrix();
    glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
    float sf = ht / (119.05f + 33.33f);
    glScalef((GLfloat)sf, (GLfloat)sf, (GLfloat)sf);
    char c; // one character to print
    for (; (c = *s) != '\0'; s++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();
}