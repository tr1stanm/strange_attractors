#include "gsl/gsl_matrix.h"
#include "SDL3/SDL.h"
#include <string>
#pragma once

extern const int CANVASSIZE;
extern const int NUMPOINTS, NUM_TESTPTS;
extern const bool DEBUG;

// linear algebra
gsl_matrix* matrixMul(gsl_matrix*, gsl_matrix*);
gsl_matrix* matrixPower(gsl_matrix*, int);
void printMatrix(gsl_matrix*);
void plotPoint(SDL_Renderer*, gsl_matrix*, double, double);
void plotVector(SDL_Renderer*, gsl_matrix*, gsl_matrix*, double, double, double, double);
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
void handleEvents(gsl_matrix*&);
void renderText(int, std::string);
