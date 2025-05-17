#include "dynamicalsystems.hpp"
#include "3DR.hpp"

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
