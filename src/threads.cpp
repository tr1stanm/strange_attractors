#include "3DR.hpp"
#include "dynamicalsystems.hpp"
#include "threads.hpp"
#include <thread>

void multiThreadRotate(int numThreads, gsl_matrix*** testPts, gsl_matrix* rMatrix) {
	std::thread **threads = new std::thread*[numThreads];
	for(int i = 0; i < numThreads; ++i) {		// this should work fine for any numThreads val
		threads[i] = new std::thread(rotateInRange, testPts, rMatrix, 
				NUMPOINTS * i / numThreads, 
				NUMPOINTS * (i + 1) / numThreads);
	}
	for(int i = 0; i < numThreads; ++i) {
		threads[i]->join();
		delete threads[i];
	}
	delete[] threads;

}

void rotateInRange(gsl_matrix*** testPts, gsl_matrix* rMatrix, int start_totalPts, int end_totalPts) {

	for(int i = 0; i < NUM_TESTPTS; ++i) {
		for(int j = start_totalPts; j < end_totalPts; ++j) {
			gsl_matrix* newPt = matrixMul(rMatrix, testPts[i][j]);
			gsl_matrix_free(testPts[i][j]);
			testPts[i][j] = newPt;
		}
	}
}

void multiThreadPlot(gsl_matrix ***testPts, gsl_matrix ***projPts, int numThreads, int ptsToPlot) {
	if(ptsToPlot >= NUMPOINTS) ptsToPlot = NUMPOINTS;
	std::vector<std::thread> threads;
	for(int i = 0; i < numThreads; ++i) {
		threads.emplace_back(plotInRange, testPts, projPts,
					     ptsToPlot * i / numThreads,
					     ptsToPlot * (i + 1) / numThreads);
	}
	for(int i = 0; i < numThreads; ++i) {
		threads[i].join();
	}
}

void plotInRange(gsl_matrix ***testPts, gsl_matrix*** projPts, int start_totalPts, int end_totalPts) {
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		for(int j = start_totalPts; j < end_totalPts; ++j) {
			if(projPts[i][j]) gsl_matrix_free(projPts[i][j]);

			projPts[i][j] = project(testPts[i][j], PROJ_DEPTH);
			gsl_matrix_scale(projPts[i][j], PROJ_SCALE);
		}
	}
}
