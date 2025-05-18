#include "gsl/gsl_matrix.h"
#include "SDL3/SDL.h"
#pragma once

extern const int CANVASSIZE;
extern const double QUADSIZE;
extern const int NUMPOINTS, NUM_TESTPTS;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern short isRunning;

// linear algebra
gsl_matrix* matrixMul(gsl_matrix*, gsl_matrix*);
void printMatrix(gsl_matrix*);
void plotPoint(gsl_matrix*);
void plotVector(gsl_matrix*, gsl_matrix*);
gsl_matrix* setPoint(double, double);
gsl_matrix* set3DPoint(double, double, double);
gsl_matrix* set2DVectorWithMatrices(gsl_matrix*, gsl_matrix*);
gsl_matrix* set2DVector(double, double, double, double);
gsl_matrix* set3DVector(double p1[3], double p2[3]);
gsl_matrix* rotate2D(gsl_matrix*, double);
gsl_matrix* rotateX(gsl_matrix*, double);
gsl_matrix* rotateY(gsl_matrix*, double);
gsl_matrix* rotateZ(gsl_matrix*, double);
gsl_matrix* project(gsl_matrix*, double);
gsl_matrix* orthoProject(gsl_matrix*);

// gfx functions
void init(const char*, int, int, short);
void handleEvents();
void rotateInRange(gsl_matrix***, gsl_matrix*, int, int, int, int);
void multiThreadRotate(int, gsl_matrix***, gsl_matrix*, int, int, int, int);
