#include <gsl/gsl_matrix.h>

void rotateInRange(gsl_matrix***, gsl_matrix*, int, int);
void multiThreadRotate(int, gsl_matrix***, gsl_matrix*);

void plotInRange(gsl_matrix***, gsl_matrix***, gsl_matrix*, int, int);
void multiThreadPlot(gsl_matrix***, gsl_matrix***, gsl_matrix*, int, int);
