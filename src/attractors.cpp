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
