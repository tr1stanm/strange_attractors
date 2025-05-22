#include "3DR.hpp"
#include "SDL3/SDL_events.h"
#include <SDL3/SDL_video.h>
#include "dynamicalsystems.hpp"
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_pixels.h"
#include <sstream>

SDL_Renderer *renderer;
SDL_Window *window;
TTF_Font *font;
short isRunning = 0;

void init(const char *title, int w, int h, short isFullscreen) {
	window = SDL_CreateWindow(title, w, h, isFullscreen);
	if(window) {
		renderer = SDL_CreateRenderer(window, NULL);
		if(DEBUG) printf("window created.\n");
	}
	if(renderer) {
		isRunning = 1;
		if(DEBUG) printf("renderer created.\n");
	}
	TTF_Init();
	char filepath[256];
	snprintf(filepath, 256, "%s../assets/ByteBounce.ttf", SDL_GetBasePath());
	font = TTF_OpenFont(filepath, 16);
	if(DEBUG) {
		if(font) printf("font loaded.\n");
		else printf("font not opened. make sure your fonts are in the assets folder.\n");
	}
}

void handleEvents(gsl_matrix *&rMatrix) {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type) {
		case SDL_EVENT_QUIT:
			isRunning = 0;
			break;
		case SDL_EVENT_KEY_DOWN:
			switch(event.key.key) {
				case SDLK_S:
					X_ROTATE_SCALE -= 0.1;
					break;
				case SDLK_W:
					X_ROTATE_SCALE += 0.1;
					break;
				case SDLK_A:
					Y_ROTATE_SCALE -= 0.1;
					break;
				case SDLK_D:
					Y_ROTATE_SCALE += 0.1;
					break;
				case SDLK_Z:
					Z_ROTATE_SCALE -= 0.1;
					break;
				case SDLK_X:
					Z_ROTATE_SCALE += 0.1;
					break;
				case SDLK_Q:
					ROTATION_ANGLE -= 0.1;
					break;
				case SDLK_E:
					ROTATION_ANGLE += 0.1;
					break;
				case SDLK_UP:
					PROJ_DEPTH -= 1;
					break;
				case SDLK_DOWN:
					PROJ_DEPTH += 1;
					break;
				case SDLK_LEFT:
					PROJ_SCALE -= 10;
					break;
				case SDLK_RIGHT:
					PROJ_SCALE += 10;
					break;
				case SDLK_I:
					YDELTA -= 1;
					break;
				case SDLK_K:
					YDELTA += 1;
					break;
				case SDLK_J:
					XDELTA -= 1;
					break;
				case SDLK_L:
					XDELTA += 1;
					break;
				case SDLK_SPACE:
					X_ROTATE_SCALE = 0;
					Y_ROTATE_SCALE = 0;
					Z_ROTATE_SCALE = 0;
					ROTATION_ANGLE = 1;
					break;
				default:
					break;
			}
			gsl_matrix_free(rMatrix);
			rMatrix = initRMatrix();
			break;
	}
}

void renderText(int iterations, std::string name) {
	//char text[256];
	std::stringstream text;
	SDL_Color color;
	SDL_FRect dest;
	color.a = color.r = color.b = color.g = 255;
	text.setf(std::ios::fixed);
	text.precision(2);

	if(iterations > NUMPOINTS) iterations = NUMPOINTS;
	dest.x = 10;
	dest.y =  4 * CANVASSIZE / 5.0;
	dest.w = 3 * CANVASSIZE / 10.0;
	dest.h = CANVASSIZE / 5.0;
	text << name << " attractor\n";
	text << "x rotation: " << X_ROTATE_SCALE * ROTATION_ANGLE << "\n";
	text << "y rotation: " << Y_ROTATE_SCALE * ROTATION_ANGLE << "\n";
	text << "z rotation: " << Z_ROTATE_SCALE * ROTATION_ANGLE << "\n";
	text << "depth: " << PROJ_DEPTH << "\n";
	text << "scale: " << PROJ_SCALE << "\n";
	double tempyd;
	if(!YDELTA) tempyd = 0;
	else tempyd = -YDELTA;
	text << "current origin: (" << XDELTA << ", " << tempyd << ")\n";
	SDL_Surface *textSurf = TTF_RenderText_Solid_Wrapped(font, text.str().c_str(), 0, color, 0);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, textSurf);
	SDL_RenderTexture(renderer, tex, NULL, &dest);
	SDL_DestroySurface(textSurf);
	SDL_DestroyTexture(tex);
	
}

// set render draw color before using
void plotPoint(gsl_matrix *toPlot) {
	if(toPlot->size1 != 2 || toPlot->size2 != 1) {
		printf("error: cannot plot a point from a non-2x1 matrix.\n");
		return;
	}

	SDL_RenderPoint(renderer, gsl_matrix_get(toPlot, 0, 0) + QUADSIZE + XDELTA,
			              gsl_matrix_get(toPlot, 1, 0) + QUADSIZE + YDELTA);
}

void plotVector(gsl_matrix *p1, gsl_matrix* p2) {
	if(p1->size1 != 2 || p1->size2 != 1 || p2->size1 != 2 || p2->size2 != 1) {
		printf("error: cannot plot a vector from non-2x1 matrices.\n");
		return;
	}

	SDL_RenderLine(renderer, gsl_matrix_get(p1, 0, 0) + QUADSIZE + XDELTA, 
				     gsl_matrix_get(p1, 1, 0) + QUADSIZE + YDELTA,
	       		             gsl_matrix_get(p2, 0, 0) + QUADSIZE + XDELTA, 
				     gsl_matrix_get(p2, 1, 0) + QUADSIZE + YDELTA);
}
