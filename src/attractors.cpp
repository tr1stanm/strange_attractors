#include "dynamicalsystems.hpp"
#include "3DR.hpp"

gsl_matrix*** initProjPoints() {
	gsl_matrix*** projPoints = new gsl_matrix**[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		projPoints[i] = new gsl_matrix*[NUMPOINTS];
		for(int j = 0; j < NUMPOINTS; ++j) {
			projPoints[i][j] = nullptr;
		}
	}
	return projPoints;
}

gsl_matrix* initRMatrix() {
	gsl_matrix* rMatrix = gsl_matrix_alloc(3, 3);
	gsl_matrix_set_identity(rMatrix);
	rMatrix = rotateZ(rotateY(rotateX(rMatrix, ROTATION_ANGLE * X_ROTATE_SCALE), 
						   ROTATION_ANGLE * Y_ROTATE_SCALE), 
						   ROTATION_ANGLE * Z_ROTATE_SCALE);
	return rMatrix;
}	

void drawAttractor(gsl_matrix*** projPoints, int iterations) {
	if(iterations >= NUMPOINTS) iterations = NUMPOINTS;
	for(int j = 0; j < NUM_TESTPTS; ++j) {
		for(int k = 0; k < iterations - 1; ++k) {
			SDL_SetRenderDrawColor(renderer, j * 255.0 / NUM_TESTPTS, 
							 j * NUM_TESTPTS, 
							 j * 510.0 / NUM_TESTPTS, 
							 255);
			plotVector(projPoints[j][k], projPoints[j][k+1]);
		}
	}
	for(int j = 0; j < NUM_TESTPTS; ++j) {
		for(int k = 0; k < iterations; ++k) {
			if(projPoints[j][k]) gsl_matrix_free(projPoints[j][k]);
			projPoints[j][k] = nullptr;
		}
	}
}

lorenz::lorenz(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	sigma(10),
	rho(28),
	beta(8/3.0) {}

lorenz::~lorenz() {}

void lorenz::iterate() {
	double dx, dy, dz;

	dx = (sigma * (y - x)) * dt;
	dy = (x * (rho - z) - y) * dt;
	dz = ((x * y) - (beta * z)) * dt;

	x += dx;
	y += dy;
	z += dz;
}

void lorenz::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

gsl_matrix* lorenz::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;
}

halvorsen::halvorsen(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	a(1.89) {}

halvorsen::~halvorsen() {}

void halvorsen::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void halvorsen::iterate() {
	double dx, dy, dz;

	dx = ((-a * x) - (4 * y) - (4 * z) - (y * y)) * dt;
	dy = ((-a * y) - (4 * z) - (4 * x) - (z * z)) * dt;
	dz = ((-a * z) - (4 * x) - (4 * y) - (x * x)) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* halvorsen::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;
}

dadras::dadras(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	a(3),
	b(2.7),
	c(1.7),
	d(2),
	e(9) {}

dadras::~dadras() {}

void dadras::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void dadras::iterate() {
	double dx, dy, dz;

	dx = (y - (a * x) + (b * y * z)) * dt;
	dy = ((c * y) - (x * z) + z) * dt;
	dz = ((d * x * y) - (e * z)) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* dadras::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;
}

langford::langford(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	a(0.95),
	b(0.7),
	c(0.6),
	d(3.5),
	e(0.25),
	f(0.1) {}

langford::~langford() {}

void langford::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void langford::iterate() {
	double dx, dy, dz;

	dx = (((z - b) * x) - (d * y)) * dt;
	dy = ((d * x) + ((z - b) * y)) * dt;
	dz = (c + (a * z) - ((z * z * z) / 3.0) 
	     - ((x * x) + (y * y)) + (1 + (e * z))
	     + (f * z * x * x * x)) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* langford::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;
}

threeScroll::threeScroll(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	a(40),
	b(1.833),
	c(55),
	d(0.16),
	e(0.65),
	f(20) {}

threeScroll::~threeScroll() {}

void threeScroll::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void threeScroll::iterate() {
	double dx, dy, dz;

	dx = (a * (y - x) + (d * x * z)) * dt;
	dy = ((c * x) - (x * z) + (f * y)) * dt;
	dz = ((b * z) + (x * y) - (e * x * x)) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* threeScroll::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;
}

rabinovichFabrikant::rabinovichFabrikant(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	alpha(0.14),
	gamma(0.10) {}

rabinovichFabrikant::~rabinovichFabrikant() {}

void rabinovichFabrikant::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void rabinovichFabrikant::iterate() {
	double dx, dy, dz;

	dx = (y * (z - 1 + (x * x)) + (gamma * x)) * dt;
	dy = (x * ((3 * z) + 1 - (x * x))) * dt;
	dz = ((-2 * z) * (alpha + (x * y))) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* rabinovichFabrikant::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;
}

rossler::rossler(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	a(0.2),
	b(0.2),
	c(5.7) {}

rossler::~rossler() {}

void rossler::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void rossler::iterate() {
	double dx, dy, dz;

	dx = (-(y + z)) * dt;
	dy = (x + (a * y)) * dt;
	dz = (b + (z * (x - c))) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* rossler::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;
}

quadratic3D::quadratic3D(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t) {
	p[0] = -0.875;
	p[1] = -0.173;
	p[2] = 0.307;
	p[3] = -0.436;
	p[4] = 0.598;
	p[5] = 0.003;
	p[6] = -0.039;
	p[7] = 0.96;
	p[8] = -0.84;
	p[9] = 0.855;
	p[10] = 0.774;
	p[11] = 0.281;
	p[12] = -0.015;
	p[13] = 0.585;
	p[14] = 0.442;
	p[15] = -0.18;
	p[16] = -0.535;
	p[17] = -0.151;
	p[18] = -0.971;
	p[19] = -0.48;
	p[20] = 0.777;
	p[21] = 0.418;
	p[22] = 0.185;
	p[23] = 0.006;
	p[24] = 0.45;
	p[25] = -0.066;
	p[26] = 0.498;
	p[27] = 0.142;
	p[28] = -0.246;
	p[29] = -0.939;
}

quadratic3D::~quadratic3D() {}


void quadratic3D::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void quadratic3D::iterate() {
	double dx, dy, dz;

	dx = (p[0] + (p[1] * x) + (p[2] * y) + (p[3] * z) +
	     (p[4] * x * y) + (p[5] * x * z) + (p[6] * y * z) +
	     (p[7] * x * x) + (p[8] * y * y) + (p[9] * z * z)) * dt;
	dy = (p[10] + (p[11] * x) + (p[12] * y) + (p[13] * z) +
	     (p[14] * x * y) + (p[15] * x * z) + (p[16] * y * z) +
	     (p[17] * x * x) + (p[18] * y * y) + (p[19] * z * z)) * dt;
	dz = (p[20] + (p[21] * x) + (p[22] * y) + (p[23] * z) +
	     (p[24] * x * y) + (p[25] * x * z) + (p[26] * y * z) +
	     (p[27] * x * x) + (p[28] * y * y) + (p[29] * z * z)) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* quadratic3D::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;
}

sprottLinz::sprottLinz(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	a(0.5) {}

sprottLinz::~sprottLinz() {}

void sprottLinz::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void sprottLinz::iterate() {
	double dx, dy, dz;

	dx = (y + z) * dt;
	dy = (-x + (a * y)) * dt;
	dz = ((x * x) - z) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* sprottLinz::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;

}

sprottB::sprottB(double xo, double yo, double zo, double t) :
	x(xo),
	y(yo),
	z(zo),
	dt(t),
	a(0.4),
	b(1.2),
	c(1) {}

sprottB::~sprottB() {}

void sprottB::setVals(double xo, double yo, double zo, double t) {
	x = xo;
	y = yo;
	z = zo;
	dt = t;
}

void sprottB::iterate() {
	double dx, dy, dz;

	dx = (a * y * z) * dt;
	dy = (x - (b * y)) * dt;
	dz = (c - (x * y)) * dt;

	x += dx;
	y += dy;
	z += dz;
}

gsl_matrix* sprottB::currentCoord() {
	gsl_matrix* current = set3DPoint(x, y, z);
	return current;

}
