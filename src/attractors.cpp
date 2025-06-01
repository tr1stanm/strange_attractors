#include "3DR.hpp"
#include "dynamicalsystems.hpp"
#include "attractors.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3/SDL_video.h"
#include <thread>
#include <sstream>
#include <iostream>

attractors::attractors() :
	projDepth(50),
	projScale(600),
	rotationAngle(1),
	xRotateScale(0),
	yRotateScale(0),
	zRotateScale(0),
	xDelta(0),
	yDelta(0),
	running(false),
	iterations(0),
	currentTypeID(0),
	currentShaderID(0),
	numShaders(6),
	numAttractors(10),
	toExport(false),
	sstime(0),
	currentW(CANVASSIZE),
	currentH(CANVASSIZE)
{
	initAttractor_typeID();
	attractors::initRMatrix();
	rTotal = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_identity(rTotal);
	initShaders();
	currentShader = shaders[0];
	testPoints = initTestPoints(attractor);
	attractors::initProjPoints();
	attractors::gfxInit("strange-attractors", CANVASSIZE, CANVASSIZE);

	char userResponse;
	printf("export frames to video? (y/n): ");
	scanf("%c", &userResponse);
	if(userResponse == 'y' || userResponse == 'Y') {
		toExport = true;
		system("mkdir ./.anim");
	}
}

attractors::~attractors() {
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		for(int j = 0; j < NUMPOINTS; ++j) {
			gsl_matrix_free(testPoints[i][j]);
		}
		delete testPoints[i];
		delete projPoints[i];
	}
	delete[] testPoints;
	delete[] projPoints;
	delete[] shaders;
	gsl_matrix_free(rMatrix);
	gsl_matrix_free(rTotal);
	for(int i = 0; i < NUM_TESTPTS; ++i) delete attractor[i];
	delete[] attractor;
	TTF_CloseFont(font);

	if(toExport) {
		system("[ ! -d anims ] && mkdir anims");
		system("ffmpeg -f image2 -framerate 60 -i ./.anim/frame_%04d.png -vcodec libx264 -pix_fmt yuv420p -crf 17 anims/anim.mp4");
		system("rm -r ./.anim/");
	}
	std::cout << "\rquitting program...\n";
}

void attractors::initShaders() {
	shaders = new shader[numShaders];

	shaders[0].rd = 255.0 / NUM_TESTPTS;
	shaders[0].gd = NUM_TESTPTS;
	shaders[0].bd = 510.0 / NUM_TESTPTS;
	shaders[0].background.r = 0;
	shaders[0].background.g = 0;
	shaders[0].background.b = 0;
	shaders[0].background.a = 255;

	shaders[1].rd = 120.0 / NUM_TESTPTS;
	shaders[1].gd = 0;
	shaders[1].bd = 700.0 / NUM_TESTPTS;
	shaders[1].background.r = 57;
	shaders[1].background.g = 37;
	shaders[1].background.b = 81;
	shaders[1].background.a = 255;

	shaders[2].rd = 500.0 / NUM_TESTPTS;
	shaders[2].gd = 60;
	shaders[2].bd = 120;
	shaders[2].background.r = 52;
	shaders[2].background.g = 112;
	shaders[2].background.b = 99;
	shaders[2].background.a = 255;

	shaders[3].rd = 510.0 / NUM_TESTPTS;
	shaders[3].gd = 1000.0 / NUM_TESTPTS;
	shaders[3].bd = 55;
	shaders[3].background.r = 48;
	shaders[3].background.g = 61;
	shaders[3].background.b = 133;
	shaders[3].background.a = 255;
	
	shaders[4].rd = 0;
	shaders[4].gd = 120.0 / NUM_TESTPTS;
	shaders[4].bd = 300.0 / NUM_TESTPTS;
	shaders[4].background.r = 8;
	shaders[4].background.g = 12;
	shaders[4].background.b = 62;
	shaders[4].background.a = 255;

	shaders[5].rd = 300.0 / NUM_TESTPTS;
	shaders[5].gd = 0;
	shaders[5].bd = 60.0 / NUM_TESTPTS;
	shaders[5].background.r = 100;
	shaders[5].background.g = 15;
	shaders[5].background.b = 15;
	shaders[5].background.a = 255;
}

void attractors::updateShaderID(bool next) {
	if(next) {
		if(currentShaderID == numShaders - 1) currentShaderID = 0;
		else ++currentShaderID;
	}
	else {
		if(!currentShaderID) currentShaderID = numShaders - 1;
		else --currentShaderID;
	}
	currentShader = shaders[currentShaderID];
}

void attractors::gfxInit(const char *title, int w, int h) {
	window = SDL_CreateWindow(title, w, h, SDL_WINDOW_RESIZABLE);
	if(window) {
		renderer = SDL_CreateRenderer(window, NULL);
		if(DEBUG) printf("window created.\n");
	}
	if(renderer) {
		running = true;
		if(DEBUG) printf("renderer created.\n");
	}
	TTF_Init();
	char filepath[256];
	snprintf(filepath, 256, "%s../assets/ByteBounce.ttf", SDL_GetBasePath());
	font = TTF_OpenFont(filepath, 64);
	if(DEBUG) {
		if(font) printf("font loaded.\n");
		else printf("font not opened. make sure your fonts are in the assets folder.\n");
	}
}

void attractors::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type) {
		case SDL_EVENT_QUIT:
			running = false;
			break;
		case SDL_EVENT_WINDOW_RESIZED:
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			SDL_GetWindowSize(window, &currentW, &currentH);
			break;
		case SDL_EVENT_KEY_DOWN:
			switch(event.key.key) {
				case SDLK_S:
					xRotateScale -= 0.1;
					break;
				case SDLK_W:
					xRotateScale += 0.1;
					break;
				case SDLK_A:
					yRotateScale -= 0.1;
					break;
				case SDLK_D:
					yRotateScale += 0.1;
					break;
				case SDLK_Z:
					zRotateScale -= 0.1;
					break;
				case SDLK_X:
					zRotateScale += 0.1;
					break;
				case SDLK_Q:
					rotationAngle -= 0.1;
					break;
				case SDLK_E:
					rotationAngle += 0.1;
					break;
				case SDLK_UP:
					projDepth -= 1;
					break;
				case SDLK_DOWN:
					projDepth += 1;
					break;
				case SDLK_LEFT:
					projScale -= 10;
					break;
				case SDLK_RIGHT:
					projScale += 10;
					break;
				case SDLK_I:
					yDelta -= 5;
					break;
				case SDLK_K:
					yDelta += 5;
					break;
				case SDLK_J:
					xDelta -= 5;
					break;
				case SDLK_L:
					xDelta += 5;
					break;
				case SDLK_SPACE:
					xRotateScale = 0;
					yRotateScale = 0;
					zRotateScale = 0;
					rotationAngle = 1;
					break;
				case SDLK_PERIOD:
					updateTypeID(true);
					switchAttractor();
					break;
				case SDLK_COMMA:
					updateTypeID(false);
					switchAttractor();
					break;
				case SDLK_1:
					currentTypeID = 0;
					switchAttractor();
					break;
				case SDLK_2:
					currentTypeID = 1;
					switchAttractor();
					break;
				case SDLK_3:
					currentTypeID = 2;
					switchAttractor();
					break;
				case SDLK_4:
					currentTypeID = 3;
					switchAttractor();
					break;
				case SDLK_5:
					currentTypeID = 4;
					switchAttractor();
					break;
				case SDLK_6:
					currentTypeID = 5;
					switchAttractor();
					break;
				case SDLK_7:
					currentTypeID = 6;
					switchAttractor();
					break;
				case SDLK_8:
					currentTypeID = 7;
					switchAttractor();
					break;
				case SDLK_9:
					currentTypeID = 8;
					switchAttractor();
					break;
				case SDLK_0:
					currentTypeID = 9;
					switchAttractor();
					break;
				case SDLK_O:
					updateShaderID(false);
					break;
				case SDLK_P:
					updateShaderID(true);
					break;
				case SDLK_C:
					system("[ ! -d screenshots ] && mkdir screenshots");
					screenShot("screenshots");
					sstime = 255;
					break;
				default:
					break;
			}
			gsl_matrix_free(rMatrix);
			initRMatrix();
			break;
	}
}

void attractors::screenShot(std::string path) {
	char fname[200];

	snprintf(fname, 200, "./%s/frame_%06d.png", path.c_str(), iterations);
	SDL_Surface *surface = SDL_RenderReadPixels(renderer, NULL);
	IMG_SavePNG(surface, fname);
	SDL_DestroySurface(surface);
}

void attractors::renderSSText() {
	std::stringstream text;
	SDL_Color color;
	SDL_FRect dest;
	color.r = color.b = color.g = 255;
	color.a = sstime;
	sstime -= 2;
	if(sstime < 0) sstime = 0;

	double textScaleFactor;
	if(currentH < currentW) textScaleFactor = currentH;
	else textScaleFactor = currentW;
	dest.w = 3 * textScaleFactor / 4.0;
	dest.h = 15;
	dest.x = currentW - dest.w;
	dest.y = 10;

	text << "screenshot saved in " << SDL_GetBasePath() << "screenshots/.";

	SDL_Surface *textSurf = TTF_RenderText_Solid_Wrapped(font, text.str().c_str(), 0, color, 0);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, textSurf);
	SDL_RenderTexture(renderer, tex, NULL, &dest);
	SDL_DestroySurface(textSurf);
	SDL_DestroyTexture(tex);
}

void attractors::renderText() {
	std::stringstream text;
	SDL_Color color;
	SDL_FRect dest;
	color.a = color.r = color.b = color.g = 255;
	text.setf(std::ios::fixed);
	text.precision(2);

	double textScaleFactor;
	if(currentH < currentW) textScaleFactor = currentH;
	else textScaleFactor = currentW;
	dest.w = 3 * textScaleFactor / 10.0;
	dest.h = textScaleFactor / 5.0;
	dest.x = 10;
	dest.y = currentH - dest.h;

	text << attractor[0]->attractorName << " attractor\n";
	text << "x rotation: " << xRotateScale * rotationAngle << "\n";
	text << "y rotation: " << yRotateScale * rotationAngle << "\n";
	text << "z rotation: " << zRotateScale * rotationAngle << "\n";
	text << "depth: " << projDepth << "\n";
	text << "scale: " << projScale << "\n";
	double tempyd;
	if(!yDelta) tempyd = 0;
	else tempyd = -yDelta;
	text << "current origin: (" << xDelta << ", " << tempyd << ")\n";
	SDL_Surface *textSurf = TTF_RenderText_Solid_Wrapped(font, text.str().c_str(), 0, color, 0);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, textSurf);
	SDL_RenderTexture(renderer, tex, NULL, &dest);
	SDL_DestroySurface(textSurf);
	SDL_DestroyTexture(tex);
	
}

void attractors::initAttractor_typeID() {
	switch(currentTypeID) {
		case 0: 
			attractor = initAttractor<lorenz>(0.005, 10);
			projDepth = 65;
			projScale = 250;
			break;
		case 1: 
			attractor = initAttractor<halvorsen>(0.005, 5);
			break;
		case 2: 
			attractor = initAttractor<dadras>(0.005, 5);
			projDepth = 40;
			projScale = 800;
			break;
		case 3: 
			attractor = initAttractor<langford>(0.005, 2);
			projDepth = 10;
			projScale = 1000;
			break;
		case 4: 
			attractor = initAttractor<threeScroll>(0.0005, 5);
			projDepth = 225;
			projScale = 350;
			break;
		case 5: 
			attractor = initAttractor<rabinovichFabrikant>(0.0005, 5);
			projDepth = 30;
			projScale = 700;
			break;
		case 6: 
			attractor = initAttractor<rossler>(0.01, 5);
			break;
		case 7: 
			attractor = initAttractor<sprottLinz>(0.01, .1);
			projDepth = 10;
			projScale = 800;
			break;
		case 8: 
			attractor = initAttractor<sprottB>(0.05, 5);
			projDepth = 25;
			break;
		case 9:
			attractor = initAttractor<arneodo>(0.005, 2);
			projDepth = 30;
			projScale = 800;
			break;
		default:
			// this would be bad
			std::cerr << "error: no attractor with typeID " << currentTypeID << "exists.\n";
			break;
	}
}

void attractors::initRMatrix() {
	rMatrix = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_identity(rMatrix);
	rMatrix = rotateZ(rotateY(rotateX(rMatrix, rotationAngle * xRotateScale), 
						   rotationAngle * yRotateScale), 
						   rotationAngle * zRotateScale);
}

void attractors::initProjPoints() {
	projPoints = new gsl_matrix**[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		projPoints[i] = new gsl_matrix*[NUMPOINTS];
		for(int j = 0; j < NUMPOINTS; ++j) {
			projPoints[i][j] = nullptr;
		}
	}
}

void attractors::switchAttractor() {
	// resets to defaults
	gsl_matrix_set_identity(rTotal);
	projDepth = 50;
	projScale = 600;
	xRotateScale = yRotateScale = zRotateScale = 0;
	xDelta = yDelta = 0;
	rotationAngle = 1;
	iterations = 0;
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		for(int j = 0; j < NUMPOINTS; ++j) gsl_matrix_free(testPoints[i][j]);
		delete[] testPoints[i];
		delete attractor[i];
	}
	delete[] attractor;
	delete[] testPoints;
	// initializes new attractor based on current typeID
	initAttractor_typeID();
	testPoints = initTestPoints(attractor);
}

void attractors::update() {
	handleEvents();
	SDL_SetRenderDrawColor(renderer, currentShader.background.r,
					 currentShader.background.g,
					 currentShader.background.b,
					 currentShader.background.a);
	SDL_RenderClear(renderer);

	if(iterations >= NUMPOINTS) {
		for(int j = 0; j < NUM_TESTPTS; ++j) {
			for(int k = 0; k < NUMPOINTS-1; ++k) {
				gsl_matrix_swap(testPoints[j][k], testPoints[j][k+1]);
			}
			gsl_matrix_free(testPoints[j][NUMPOINTS-1]);
			testPoints[j][NUMPOINTS-1] = attractor[j]->currentCoord();
			attractor[j]->iterate();
		}
	}

	// transformation, plotting, drawing
	plot();
	drawAttractor();
	gsl_matrix *temp = matrixMul(rMatrix, rTotal);
	gsl_matrix_free(rTotal);
	rTotal = temp;

	// rendering
	renderText();
	if(toExport) screenShot(".anim");
	if(sstime) renderSSText();
	SDL_RenderPresent(renderer);

	++iterations;
}

void attractors::plot() {
	int ptsToPlot = iterations;
	if(ptsToPlot >= NUMPOINTS) ptsToPlot = NUMPOINTS;
	std::vector<std::thread> threads;
	int threadCount = 8;
	for(int i = 0; i < threadCount; ++i) {
		threads.emplace_back(&attractors::plotInRange, this,
				     ptsToPlot * i / threadCount,
				     ptsToPlot * (i + 1) / threadCount);
	}
	for(int i = 0; i < threadCount; ++i) {
		threads[i].join();
	}

}

void attractors::plotInRange(int start_totalPts, int end_totalPts) {
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		for(int j = start_totalPts; j < end_totalPts; ++j) {
			if(projPoints[i][j]) gsl_matrix_free(projPoints[i][j]);

			gsl_matrix *rotated = matrixMul(rTotal, testPoints[i][j]);
			projPoints[i][j] = project(rotated, projDepth);
			gsl_matrix_scale(projPoints[i][j], projScale);
			gsl_matrix_free(rotated);
		}
	}
}

void attractors::drawAttractor() {
	int ptsToDraw = iterations;
	if(iterations >= NUMPOINTS) ptsToDraw = NUMPOINTS;
	for(int j = 0; j < NUM_TESTPTS; ++j) {
		for(int k = 0; k < ptsToDraw - 1; ++k) {
			SDL_SetRenderDrawColor(renderer, j * currentShader.rd, 
							 j * currentShader.gd, 
							 j * currentShader.bd, 
							 255);
			plotVector(renderer, projPoints[j][k], projPoints[j][k+1],
				   currentW, currentH, xDelta, yDelta);
		}
	}
	for(int j = 0; j < NUM_TESTPTS; ++j) {
		for(int k = 0; k < ptsToDraw; ++k) {
			if(projPoints[j][k]) gsl_matrix_free(projPoints[j][k]);
			projPoints[j][k] = nullptr;
		}
	}
}

bool attractors::isRunning() {
	return running;
}

void attractors::updateTypeID(bool next) {
	if(next) {
		if(currentTypeID == numAttractors - 1) currentTypeID = 0;
		else ++currentTypeID;
	}
	else if(!next) {
		if(!currentTypeID) currentTypeID = numAttractors - 1;
		else --currentTypeID;
	}
}
