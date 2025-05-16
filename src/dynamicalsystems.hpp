#include "gsl/gsl_matrix.h"

class lorenz {
	public:
		lorenz(double = 0, double = 0, double = 0, double = 1);
		~lorenz();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();

	private:
		double x, y, z, dt;
		double sigma, rho, beta;
};
