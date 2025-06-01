#include "3DR.hpp"
#include "attractors.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

// current attractors supported: 
// lorenz, halvorsen, dadras, langford, threeScroll, rabinovichFabrikant, rossler, sprottB
const int CANVASSIZE = 600;
const double QUADSIZE = CANVASSIZE / 2.0;
const int NUMPOINTS = 500;
const int NUM_TESTPTS = 50;
const int FPS = 60;
const bool DEBUG = false;

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
