#include "3DR.hpp"
#include "attractors.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

// current attractors supported: 
// lorenz, halvorsen, dadras, langford, threeScroll, rabinovichFabrikant, rossler, sprottB, sprottLinz, arneodo

const int CANVASSIZE = 600;		// initial window size (resizable on launch)
const int NUMPOINTS = 500;		// number of points drawn for each line
const int NUM_TESTPTS = 50;		// number of lines
const int FPS = 60;			// frame rate limiter
const bool DEBUG = false;		// set to true if functionality of certain gfx features needs to be checked

int main() {
	const double frameDelay = 1000.0/FPS;
	Uint64 frameStart;
	double frameTime;

	attractors attractor;
	
	while(attractor.isRunning()) {
		frameStart = SDL_GetTicks();
		attractor.update();
		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}
}
