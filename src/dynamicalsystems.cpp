#include "3DR.hpp"
#include "dynamicalsystems.hpp"
#include "gsl/gsl_matrix.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include "SDL3/SDL_surface.h"
#include "SDL3_image/SDL_image.h"
#include <thread>


const int CANVASSIZE = 800;
const double QUADSIZE = CANVASSIZE / 2.0;
const int NUMPOINTS = 2000;
const int NUM_TESTPTS = 50;
const double RANDOM_SCALE = 20;
const double PROJ_DEPTH = 100;
const double PROJ_SCALE = 500;
const double ROTATION_ANGLE = .5;
const int FPS = 60;				// FPS limiter

int main() {
	const double frameDelay = 1000.0/FPS;
	Uint64 frameStart;
	double frameTime;
	char fname[200];
	char userResponse;
	short toExport = 0;
	int increment = 0;
	srand(time(0));

	class lorenz test[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		test[i].setVals(((double)rand() / RAND_MAX) * RANDOM_SCALE, 
				((double)rand() / RAND_MAX) * RANDOM_SCALE, 
				((double)rand() / RAND_MAX) * RANDOM_SCALE, 0.005);
	}

	gsl_matrix*** testPoints = new gsl_matrix**[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		testPoints[i] = new gsl_matrix*[NUMPOINTS];
		for(int j = 0; j < NUMPOINTS; ++j) {
			testPoints[i][j] = test[i].currentCoord();
			test[i].iterate();
		}
	}

	gsl_matrix* rMatrix = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_identity(rMatrix);
	rMatrix = rotateZ(rotateY(rotateX(rMatrix, ROTATION_ANGLE * 1.2), ROTATION_ANGLE), ROTATION_ANGLE * 0.75);

	printf("export frames to video? (y/n): ");
	scanf("%c", &userResponse);
	if(userResponse == 'y' || userResponse == 'Y') toExport = 1;
	
	init("lorenz", CANVASSIZE, CANVASSIZE, 0);

	if(toExport) system("mkdir ../anim");
	while(isRunning) {
		for(int i = 0; i < NUMPOINTS; ++i) {
			handleEvents();
			if(isRunning) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);

				// rotation
				multiThreadRotate(50, testPoints, rMatrix, 0, NUM_TESTPTS, 0, NUMPOINTS);

				// transformation and plotting
				for(int j = 0; j < i - 1; ++j) {
					for(int k = 0; k < NUM_TESTPTS; ++k) {
						SDL_SetRenderDrawColor(renderer, k * 255.0 / NUM_TESTPTS, k * NUM_TESTPTS, k * 510.0 / NUM_TESTPTS, 255);
						gsl_matrix *proj1 = project(testPoints[k][j], PROJ_DEPTH);
						gsl_matrix *proj2 = project(testPoints[k][j+1], PROJ_DEPTH);
						gsl_matrix_scale(proj1, PROJ_SCALE);
						gsl_matrix_scale(proj2, PROJ_SCALE);
						plotVector(proj1, proj2);
						gsl_matrix_free(proj1);
						gsl_matrix_free(proj2);
					}
				}

				// exporting (optional obv)
				if(toExport) {
					snprintf(fname, 200, "../anim/frame_%04d.png", (int)increment++);
					SDL_Surface *surface = SDL_RenderReadPixels(renderer, NULL);
					IMG_SavePNG(surface, fname);
				}

				// rendering
				SDL_RenderPresent(renderer);
				frameTime = SDL_GetTicks() - frameStart;
				if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
			}
		}
		isRunning = 0; // if all the points are drawn ends the program here

	}

	for(int i = 0; i < NUM_TESTPTS; ++i) {
		for(int j = 0; j < NUMPOINTS; ++j) {
			gsl_matrix_free(testPoints[i][j]);
		}
		delete[] testPoints[i];
	}
	delete[] testPoints;
	gsl_matrix_free(rMatrix);

	if(toExport) {
		system("ffmpeg -f image2 -framerate 60 -i ../anim/frame_%04d.png -vcodec libx264 -pix_fmt yuv420p -crf 17 anim.mp4");
		system("rm -r ../anim/");
	}
}

void rotateInRange(gsl_matrix*** testPts, gsl_matrix* rMatrix, 
		   int start_testPts, int end_testPts, 
		   int start_totalPts, int end_totalPts) {

	for(int i = start_testPts; i < end_testPts; ++i) {
		for(int j = start_totalPts; j < end_totalPts; ++j) {
			gsl_matrix* newPt = matrixMul(rMatrix, testPts[i][j]);
			gsl_matrix_free(testPts[i][j]);
			testPts[i][j] = newPt;
		}
	}
}

void multiThreadRotate(int numThreads, gsl_matrix*** testPts, gsl_matrix* rMatrix,
		       int start_testPts, int end_testPts,
		       int start_totalPts, int end_totalPts) {
	
	std::thread **threads = new std::thread*[numThreads];
	for(int i = 0; i < numThreads; ++i) {
		threads[i] = new std::thread(rotateInRange, testPts, rMatrix, 
				NUM_TESTPTS * i / numThreads, 
				NUM_TESTPTS * (i + 1) / numThreads,
				0, end_totalPts);
	}
	for(int i = 0; i < numThreads; ++i) {
		threads[i]->join();
		delete threads[i];
	}
	delete[] threads;

}
