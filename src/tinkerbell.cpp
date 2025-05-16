#include "3DR.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

const int CANVASSIZE = 800;
const double QUADSIZE = CANVASSIZE / 2.0;
const double NUMPOINTS = 10000;
const int FPS = 24;				// FPS limiter

int main() {
	double x = -0.72;
	double y = -0.64;
	double a = 0.9;
	double b = -0.6013;
	double c = 2.0;
	double d = 0.5;
	short increaseD = 0;

	const double frameDelay = 1000.0/FPS;
	Uint32 frameStart;
	double frameTime;
	
	init("tinkerbell", CANVASSIZE, CANVASSIZE, 0);
	gsl_matrix** pointsArray = (gsl_matrix**)malloc(sizeof(gsl_matrix*) * NUMPOINTS);


	while(isRunning) {
		frameStart = SDL_GetTicks();
		handleEvents();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for(int i = 0; i < NUMPOINTS; ++i) {
			pointsArray[i] = rotate2D(setPoint(x, y), 90);
			double temp = x;		// this is important
			x = x*x - y*y + a*x + b*y;
			y = 2*temp*y + c*temp + d*y;
		}
		if(a > 0.8 && !increaseD) {
			a -= 0.0005;
			d -= 0.0005;
		}
		else { 
			a += 0.0005;
			d += 0.0005;
			increaseD = 1;
		}
		if(a == 0.9) increaseD = 0;

		for(int i = 0; i < NUMPOINTS-1; ++i) {
			gsl_matrix_scale(pointsArray[i], QUADSIZE / 2);
			plotPoint(pointsArray[i]);
			gsl_matrix_scale(pointsArray[i], 2 / QUADSIZE);
		}

		SDL_RenderPresent(renderer);
		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}
	for(int i = 0; i < NUMPOINTS; ++i) gsl_matrix_free(pointsArray[i]);
	free(pointsArray);
}
