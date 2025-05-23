#include "3DR.hpp"
#include "attractors.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <iostream>

// current attractors supported: 
// lorenz, halvorsen, dadras, langford, threeScroll, rabinovichFabrikant, rossler, sprottB
const int CANVASSIZE = 900;
const double QUADSIZE = CANVASSIZE / 2.0;
const int NUMPOINTS = 400;
const int NUM_TESTPTS = 100;
const int FPS = 120;
const bool DEBUG = false;

int main() {
	const double frameDelay = 1000.0/FPS;
	Uint64 frameStart;
	double frameTime;
	char fname[200];
	char userResponse;
	bool toExport = false;

	attractors attractor;
	printf("export frames to video? (y/n): ");
	scanf("%c", &userResponse);
	if(userResponse == 'y' || userResponse == 'Y') {
		toExport = true;
		system("mkdir ../anim");
	}
	
	while(attractor.isRunning()) {
		attractor.update();
		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}

	// stitching video from the exported frames
	if(toExport) {
		system("ffmpeg -f image2 -framerate 60 -i ../anim/frame_%04d.png -vcodec libx264 -pix_fmt yuv420p -crf 17 anim.mp4");
		system("rm -r ../anim/");
	}
	std::cout << "\rquitting program...\n";
}

