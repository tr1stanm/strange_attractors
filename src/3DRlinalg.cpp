//#include "../../FPT.h"
#include "3DR.hpp"
#include "math.h"

gsl_matrix* project(gsl_matrix *toProject, double distance) {
	gsl_matrix *projMatrix;
	if(toProject->size1 == 3) projMatrix = gsl_matrix_alloc(2, 3);
	else if(toProject->size1 == 4) projMatrix = gsl_matrix_alloc(2, 4);
	else return NULL;

	double xp = 1 / (distance - gsl_matrix_get(toProject, 2, 0));
	double yp = 1 / (distance - gsl_matrix_get(toProject, 2, 0));
	// this doesn't work with the cube bc it multiplies both points by the 1/z of the first
	// the cube needs to be represented with points that are 3x1 or 4x1 format
	// let's keep it 4x1 for now

	gsl_matrix_set_zero(projMatrix);
	gsl_matrix_set(projMatrix, 0, 0, xp);
	gsl_matrix_set(projMatrix, 1, 1, yp);
	
	gsl_matrix* rval = matrixMul(projMatrix, toProject);
	gsl_matrix_free(projMatrix);
	return rval;
}

gsl_matrix* orthoProject(gsl_matrix* toProject) {
	gsl_matrix *projMatrix;
	if(toProject->size1 == 3) projMatrix = gsl_matrix_alloc(2, 3);
	else if(toProject->size1 == 4) projMatrix = gsl_matrix_alloc(2, 4);
	else return NULL;

	gsl_matrix_set_zero(projMatrix);
	gsl_matrix_set(projMatrix, 0, 0, 1);
	gsl_matrix_set(projMatrix, 1, 1, 1);
	
	gsl_matrix* rval = matrixMul(projMatrix, toProject);
	gsl_matrix_free(projMatrix);
	return rval;
}

// the following four functions are in charge of allocation, not freeing. be sure to free memory when you use them
gsl_matrix* setPoint(double x, double y) {
	gsl_matrix *newPt = gsl_matrix_alloc(2, 1);
	gsl_matrix_set(newPt, 0, 0, x);
	gsl_matrix_set(newPt, 1, 0, y);
	return newPt;
}

gsl_matrix* set2DVector(double x1, double y1, double x2, double y2) {
	gsl_matrix *newPt = gsl_matrix_alloc(2, 2);
	gsl_matrix_set(newPt, 0, 0, x1);
	gsl_matrix_set(newPt, 1, 0, y1);
	gsl_matrix_set(newPt, 0, 1, x2);
	gsl_matrix_set(newPt, 1, 1, y2);
	return newPt;
}

gsl_matrix* set2DVectorWithMatrices(gsl_matrix *p1, gsl_matrix *p2) {
	gsl_matrix *newPt = gsl_matrix_alloc(2, 2);
	gsl_matrix_set(newPt, 0, 0, gsl_matrix_get(p1, 0, 0));
	gsl_matrix_set(newPt, 1, 0, gsl_matrix_get(p1, 1, 0));
	gsl_matrix_set(newPt, 0, 1, gsl_matrix_get(p2, 0, 0));
	gsl_matrix_set(newPt, 1, 1, gsl_matrix_get(p2, 1, 0));
	return newPt;
}

gsl_matrix* set3DVector(double p1[3], double p2[3]) {
	gsl_matrix *newPt = gsl_matrix_alloc(3, 2);
	gsl_matrix_set(newPt, 0, 0, p1[0]);
	gsl_matrix_set(newPt, 1, 0, p1[1]);
	gsl_matrix_set(newPt, 2, 0, p1[2]);
	gsl_matrix_set(newPt, 0, 1, p2[0]);
	gsl_matrix_set(newPt, 1, 1, p2[1]);
	gsl_matrix_set(newPt, 2, 1, p2[2]);
	return newPt;
}

gsl_matrix* set3DPoint(double x, double y, double z) {
	gsl_matrix *newPt = gsl_matrix_alloc(3, 1);
	gsl_matrix_set(newPt, 0, 0, x);
	gsl_matrix_set(newPt, 1, 0, y);
	gsl_matrix_set(newPt, 2, 0, z);
	return newPt;
}

gsl_matrix* rotate2D(gsl_matrix *toRotate, double angle) {
	gsl_matrix *rMatrix = gsl_matrix_alloc(2, 2);
	gsl_matrix_set(rMatrix, 0, 0, cos(angle));
	gsl_matrix_set(rMatrix, 1, 0, sin(angle));
	gsl_matrix_set(rMatrix, 0, 1, -sin(angle));
	gsl_matrix_set(rMatrix, 1, 1, cos(angle));

	gsl_matrix* rval = matrixMul(rMatrix, toRotate);
	gsl_matrix_free(rMatrix);
	gsl_matrix_free(toRotate);
	return rval;
}

gsl_matrix* rotateX(gsl_matrix* toRotate, double angle) {
	angle *= M_PI/180;

	gsl_matrix *rMatrix = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_zero(rMatrix);
	gsl_matrix_set(rMatrix, 0, 0, 1);
	gsl_matrix_set(rMatrix, 1, 1, cos(angle));
	gsl_matrix_set(rMatrix, 2, 1, sin(angle));
	gsl_matrix_set(rMatrix, 1, 2, -sin(angle));
	gsl_matrix_set(rMatrix, 2, 2, cos(angle));

	gsl_matrix* rval = matrixMul(rMatrix, toRotate);
	gsl_matrix_free(rMatrix);
	gsl_matrix_free(toRotate);
	return rval;
}

gsl_matrix* rotateY(gsl_matrix* toRotate, double angle) {
	angle *= M_PI/180;

	gsl_matrix *rMatrix = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_zero(rMatrix);
	gsl_matrix_set(rMatrix, 1, 1, 1);
	gsl_matrix_set(rMatrix, 0, 0, cos(angle));
	gsl_matrix_set(rMatrix, 0, 2, sin(angle));
	gsl_matrix_set(rMatrix, 2, 0, -sin(angle));
	gsl_matrix_set(rMatrix, 2, 2, cos(angle));

	gsl_matrix* rval = matrixMul(rMatrix, toRotate);
	gsl_matrix_free(rMatrix);
	gsl_matrix_free(toRotate);
	return rval;
}

gsl_matrix* rotateZ(gsl_matrix* toRotate, double angle) {
	angle *= M_PI/180;

	gsl_matrix *rMatrix = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_zero(rMatrix);
	gsl_matrix_set(rMatrix, 0, 0, cos(angle));
	gsl_matrix_set(rMatrix, 1, 0, sin(angle));
	gsl_matrix_set(rMatrix, 0, 1, -sin(angle));
	gsl_matrix_set(rMatrix, 1, 1, cos(angle));
	gsl_matrix_set(rMatrix, 2, 2, 1);

	gsl_matrix* rval = matrixMul(rMatrix, toRotate);
	gsl_matrix_free(rMatrix);
	gsl_matrix_free(toRotate);
	return rval;
}

// matrix multiplication DON'T TOUCH!!!
// consider that the two matrices passed in remain untouched. maybe dealloc those matrices when not in use
gsl_matrix* matrixMul(gsl_matrix *M1, gsl_matrix *M2) {
	if(M1->size2 != M2->size1) {
		printf("error: cannot multiply these matrices.\n");
		return NULL;
	}
	gsl_matrix *M3 = gsl_matrix_alloc(M1->size1, M2->size2);

	for(int row = 0; row < M3->size1; ++row) {
		for(int col = 0; col < M3->size2; ++col) {
			double sum = 0;
			for(int k = 0; k < M1->size2; ++k) {
				sum += gsl_matrix_get(M1, row, k) * gsl_matrix_get(M2, k, col);
			}
			gsl_matrix_set(M3, row, col, sum);
		}
	}
			
	return M3;
}

// O(lg(n)) for powers of n.
gsl_matrix* matrixPower(gsl_matrix* M, int power) {
	gsl_matrix *result = gsl_matrix_alloc(3, 3);
	gsl_matrix *base = gsl_matrix_alloc(3, 3);
	gsl_matrix *temp = nullptr;
	gsl_matrix_set_identity(result);
	gsl_matrix_memcpy(base, M);

	while(power > 0) {
		if(power % 2 == 1) {
			if(temp) gsl_matrix_free(temp);
			temp = matrixMul(result, base);
			gsl_matrix_memcpy(result, temp);
		}
		if(temp) gsl_matrix_free(temp);
		temp = matrixMul(base, base);
		gsl_matrix_memcpy(base, temp);
		power /= 2;
	}
	gsl_matrix_free(temp);
	gsl_matrix_free(base);
	return result;
}

void printMatrix(gsl_matrix *M) {
	for(int row = 0; row < M->size1; ++row) {
		for(int col = 0; col < M->size2; ++col) {
			printf("%f ", gsl_matrix_get(M, row, col));
		}
		printf("\n");
	}
}
