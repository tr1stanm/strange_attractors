#include "3DR.hpp"
#include "dynamicalsystems.hpp"
#include "threads.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include "SDL3/SDL_surface.h"
#include "SDL3_image/SDL_image.h"
#include <iostream>

// current attractors supported: 
// lorenz, halvorsen, dadras, langford, threeScroll, rabinovichFabrikant, rossler, sprottB
#define ATTRACTOR dadras
const int CANVASSIZE = 700;
const double QUADSIZE = CANVASSIZE / 2.0;
const int NUMPOINTS = 500;
const int NUM_TESTPTS = 50;
const double RANDOM_SCALE = 8;
double PROJ_DEPTH = 50;
double PROJ_SCALE = 400;
double ROTATION_ANGLE = 1;
double X_ROTATE_SCALE = 0;
double Y_ROTATE_SCALE = 0;
double Z_ROTATE_SCALE = 0;
double XDELTA = 0;
double YDELTA = 0;
const double DT = 0.005;
const int FPS = 120;				// FPS limiter

int main() {
	const double frameDelay = 1000.0/FPS;
	Uint64 frameStart;
	double frameTime;
	char fname[200];
	char userResponse;
	bool toExport = false;
	int pointsCharted = 0;
	int iterations = 0;
	
	// initialization
	ATTRACTOR *test = initAttractor<ATTRACTOR>();
	gsl_matrix*** testPoints = initTestPoints(test);
	gsl_matrix*** projPoints = initProjPoints();
	gsl_matrix* rMatrix = initRMatrix();
	gsl_matrix* rTotal = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_identity(rTotal);

	printf("export frames to video? (y/n): ");
	scanf("%c", &userResponse);
	if(userResponse == 'y' || userResponse == 'Y') {
		toExport = true;
		system("mkdir ../anim");
	}
	
	init("strange-attractors", CANVASSIZE, CANVASSIZE, 0);

	while(isRunning) {
		handleEvents(rMatrix);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if(iterations >= NUMPOINTS) {
			for(int j = 0; j < NUM_TESTPTS; ++j) {
				for(int k = 0; k < NUMPOINTS-1; ++k) {
					gsl_matrix_swap(testPoints[j][k], testPoints[j][k+1]);
				}
				gsl_matrix_free(testPoints[j][NUMPOINTS-1]);
				testPoints[j][NUMPOINTS-1] = test[j].currentCoord();
				test[j].iterate();
			}
		}

		// transformation, plotting, drawing
		multiThreadPlot(testPoints, projPoints, rTotal, 200, iterations);
		drawAttractor(projPoints, iterations);
		gsl_matrix *temp = matrixMul(rMatrix, rTotal);
		gsl_matrix_free(rTotal);
		rTotal = temp;

		// exporting (optional obv)
		if(toExport) {
			snprintf(fname, 200, "../anim/frame_%04d.png", iterations);
			SDL_Surface *surface = SDL_RenderReadPixels(renderer, NULL);
			IMG_SavePNG(surface, fname);
			std::cout << "\rexporting frames... ";
			printf("%.2f", (double)iterations * 100 / NUMPOINTS);
			std::cout << "\% done." << std::flush;
		}

		// rendering
		SDL_RenderPresent(renderer);
		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);

		++iterations;
	}

	// destruction
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		for(int j = 0; j < NUMPOINTS; ++j) {
			gsl_matrix_free(testPoints[i][j]);
		}
		delete testPoints[i];
		delete projPoints[i];
	}
	delete[] testPoints;
	delete[] projPoints;
	gsl_matrix_free(rMatrix);
	gsl_matrix_free(rTotal);
	destroyAttractor(test);

	// stitching video from the exported frames
	if(toExport) {
		system("ffmpeg -f image2 -framerate 60 -i ../anim/frame_%04d.png -vcodec libx264 -pix_fmt yuv420p -crf 17 anim.mp4");
		system("rm -r ../anim/");
		std::cout << "100% done." << std::endl;
	}
	std::cout << "\rquitting program...     \n";
}

