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
