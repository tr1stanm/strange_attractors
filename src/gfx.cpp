#include "3DR.hpp"
#include "SDL3/SDL_events.h"
#include <SDL3/SDL_video.h>
#include <iostream>
#include "dynamicalsystems.hpp"

SDL_Renderer *renderer;
SDL_Window *window;
short isRunning = 0;

void init(const char *title, int w, int h, short isFullscreen) {
	window = SDL_CreateWindow(title, w, h, isFullscreen);
	if(window) renderer = SDL_CreateRenderer(window, NULL);
	if(renderer) isRunning = 1;
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
					PROJ_DEPTH -= 5;
					break;
				case SDLK_DOWN:
					PROJ_DEPTH += 5;
					break;
				case SDLK_LEFT:
					PROJ_SCALE -= 10;
					break;
				case SDLK_RIGHT:
					PROJ_SCALE += 10;
					break;
				default:
					break;
			}
			gsl_matrix_free(rMatrix);
			rMatrix = initRMatrix();
			break;
		case SDL_EVENT_KEY_UP:
			break;

	default:
			break;
	}
}

// set render draw color before using
void plotPoint(gsl_matrix *toPlot) {
	if(toPlot->size1 != 2 || toPlot->size2 != 1) {
		printf("error: cannot plot a point from a non-2x1 matrix.\n");
		return;
	}

	SDL_RenderPoint(renderer, gsl_matrix_get(toPlot, 0, 0) + QUADSIZE,
			              gsl_matrix_get(toPlot, 1, 0) + QUADSIZE);
}

void plotVector(gsl_matrix *p1, gsl_matrix* p2) {
	if(p1->size1 != 2 || p1->size2 != 1 || p2->size1 != 2 || p2->size2 != 1) {
		printf("error: cannot plot a vector from non-2x1 matrices.\n");
		return;
	}

	SDL_RenderLine(renderer, gsl_matrix_get(p1, 0, 0) + QUADSIZE, 
				     gsl_matrix_get(p1, 1, 0) + QUADSIZE,
	       		             gsl_matrix_get(p2, 0, 0) + QUADSIZE, 
				     gsl_matrix_get(p2, 1, 0) + QUADSIZE);
}
