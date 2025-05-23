#include "dynamicalsystems.hpp"
#include "SDL3/SDL_render.h"
#include <SDL3/SDL_timer.h>
#include "SDL3_ttf/SDL_ttf.h"

struct shader {
	double rd;
	double gd;
	double bd;
};

class attractors {
	public:
		attractors();
		~attractors();
		bool isRunning();
		void update();
	private:
		void initAttractor_typeID();
		void gfxInit(const char*, int, int, bool);
		void initShaders();
		void handleEvents();
		void plot();
		void plotInRange(int, int);
		void drawAttractor();
		void renderText();
		void initRMatrix();
		void switchAttractor();
		void initProjPoints();
		void updateTypeID(bool);
		void updateShaderID(bool);

		gsl_matrix *rMatrix, *rTotal;
		gsl_matrix ***testPoints, ***projPoints;
		double projDepth;
		double projScale;
		double rotationAngle;
		double xRotateScale, yRotateScale, zRotateScale;
		double xDelta, yDelta;
		strangeAttractor **attractor;
		shader *shaders;
		shader currentShader;

		SDL_Renderer *renderer;
		SDL_Window *window;
		TTF_Font *font;
		bool running;
		int iterations;
		int currentTypeID;	// update this as attractors are switched
		int currentShaderID;
		int numShaders;
};
