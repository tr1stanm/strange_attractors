#include "3DR.hpp"
#include "dynamicalsystems.hpp"
#include "threads.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include "SDL3/SDL_surface.h"
#include "SDL3_image/SDL_image.h"
#include <iostream>


const int CANVASSIZE = 800;
const double QUADSIZE = CANVASSIZE / 2.0;
const int NUMPOINTS = 2000;
const int NUM_TESTPTS = 30;
const double RANDOM_SCALE = 30;
const double PROJ_DEPTH = 500;
const double PROJ_SCALE = 300;
const double ROTATION_ANGLE = .5;
const int FPS = 120;				// FPS limiter

int main() {
	const double frameDelay = 1000.0/FPS;
	Uint64 frameStart;
	double frameTime;
	char fname[200];
	char userResponse;
	short toExport = 0;
	int increment = 0;
	srand(time(0));

	class threeScroll test[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		test[i].setVals(((double)rand() / RAND_MAX) * RANDOM_SCALE, 
				((double)rand() / RAND_MAX) * RANDOM_SCALE, 
				((double)rand() / RAND_MAX) * RANDOM_SCALE, 0.001);
	}

	gsl_matrix*** testPoints = new gsl_matrix**[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		testPoints[i] = new gsl_matrix*[NUMPOINTS];
		for(int j = 0; j < NUMPOINTS; ++j) {
			testPoints[i][j] = test[i].currentCoord();
			test[i].iterate();
		}
	}
	gsl_matrix*** projPoints = new gsl_matrix**[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		projPoints[i] = new gsl_matrix*[NUMPOINTS];
		for(int j = 0; j < NUMPOINTS; ++j) {
			projPoints[i][j] = nullptr;
		}
	}

	gsl_matrix* rMatrix = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_identity(rMatrix);
	rMatrix = rotateZ(rotateY(rotateX(rMatrix, ROTATION_ANGLE * 2.1), ROTATION_ANGLE), ROTATION_ANGLE * 0.75);

	printf("export frames to video? (y/n): ");
	scanf("%c", &userResponse);
	if(userResponse == 'y' || userResponse == 'Y') toExport = 1;
	
	init("strange-attractors", CANVASSIZE, CANVASSIZE, 0);

	if(toExport) system("mkdir ../anim");

	int i = 0;
	while(isRunning) {
		handleEvents();

		/*
		if(i == NUMPOINTS) {
			for(int j = 0; j < NUM_TESTPTS; ++j) {
				for(int k = 0; k < i-1; ++k) {
					gsl_matrix_swap(testPoints[j][k], testPoints[j][k+1]);
				}
				gsl_matrix_free(testPoints[j][i-1]);
				testPoints[j][i-1] = test[j].currentCoord();
				test[j].iterate();
				// the new points aren't rotated as much as they should be
				// fix that
			}
			--i;
		}
		*/
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// rotation
		multiThreadRotate(200, testPoints, rMatrix);

		// transformation and plotting
		multiThreadPlot(testPoints, projPoints, 200, i);
		for(int j = 0; j < NUM_TESTPTS; ++j) {
			for(int k = 0; k < i - 1; ++k) {
				SDL_SetRenderDrawColor(renderer, j * 255.0 / NUM_TESTPTS, 
								 j * NUM_TESTPTS, 
								 j * 510.0 / NUM_TESTPTS, 
								 255);
				plotVector(projPoints[j][k], projPoints[j][k+1]);
			}
		}
		for(int j = 0; j < NUM_TESTPTS; ++j) {
			for(int k = 0; k < i; ++k) {
				if(projPoints[j][k]) gsl_matrix_free(projPoints[j][k]);
				projPoints[j][k] = nullptr;
			}
		}

		// exporting (optional obv)
		if(toExport) {
			snprintf(fname, 200, "../anim/frame_%04d.png", (int)increment++);
			SDL_Surface *surface = SDL_RenderReadPixels(renderer, NULL);
			IMG_SavePNG(surface, fname);
			//std::cout << std::flush;
			std::cout << "\rexporting frames... ";
			printf("%.2f", (double)i * 100 / NUMPOINTS);
			std::cout << "\% done." << std::flush;
		}

		// rendering
		SDL_RenderPresent(renderer);
		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);

		if(i == NUMPOINTS) isRunning = 0;
		else ++i;

	}

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

	if(toExport) {
		system("ffmpeg -f image2 -framerate 60 -i ../anim/frame_%04d.png -vcodec libx264 -pix_fmt yuv420p -crf 17 anim.mp4");
		system("rm -r ../anim/");
		std::cout << "100% done." << std::endl;
	}
}

