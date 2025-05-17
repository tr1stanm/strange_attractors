#include "gsl/gsl_matrix.h"

class lorenz {
	public:
		lorenz(double = 0, double = 0, double = 0, double = 0.005);
		~lorenz();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();

	private:
		double x, y, z, dt;
		double sigma, rho, beta;
};

class halvorsen {
	public:
		halvorsen(double = 0, double = 0, double = 0, double = 0.005);
		~halvorsen();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double a;
};

class dadras {
	public:
		dadras(double = 0, double = 0, double = 0, double = 0.005);
		~dadras();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double a, b, c, d, e;
};

class langford {
	public:
		langford(double = 0, double = 0, double = 0, double = 0.005);
		~langford();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double a, b, c, d, e, f;
};

class threeScroll {
	public:
		threeScroll(double = 0, double = 0, double = 0, double = 0.005);
		~threeScroll();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double a, b, c, d, e, f;
};

class rabinovichFabrikant {
	public:
		rabinovichFabrikant(double = 0, double = 0, double = 0, double = 0.005);
		~rabinovichFabrikant();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double alpha, gamma;
};
