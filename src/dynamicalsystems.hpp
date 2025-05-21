#include "gsl/gsl_matrix.h"
#include "3DR.hpp"

template<typename TYPE>
TYPE* initAttractor() {
	srand(time(0));
	TYPE *test = new TYPE[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		test[i].setVals(((double)rand() / RAND_MAX) * RANDOM_SCALE, 
				((double)rand() / RAND_MAX) * RANDOM_SCALE, 
				((double)rand() / RAND_MAX) * RANDOM_SCALE, DT);
	}
	return test;
}

template<typename TYPE>
void destroyAttractor(TYPE* attractor) {
	delete[] attractor;
}

template<typename TYPE>
gsl_matrix*** initTestPoints(TYPE *test) {
	gsl_matrix*** testPoints = new gsl_matrix**[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		testPoints[i] = new gsl_matrix*[NUMPOINTS];
		for(int j = 0; j < NUMPOINTS; ++j) {
			testPoints[i][j] = test[i].currentCoord();
			test[i].iterate();
		}
	}
	return testPoints;
}

gsl_matrix*** initProjPoints();
gsl_matrix* initRMatrix();
void drawAttractor(gsl_matrix***, int);

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

class rossler {
	public:
		rossler(double = 0, double = 0, double = 0, double = 0.005);
		~rossler();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double a, b, c;
};

class quadratic3D {
	public:
		quadratic3D(double = 0, double = 0, double = 0, double = 0.005);
		~quadratic3D();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double p[30];
};

class sprottLinz {
	public:
		sprottLinz(double = 0, double = 0, double = 0, double = 0.005);
		~sprottLinz();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double a;
};

class sprottB {
	public:
		sprottB(double = 0, double = 0, double = 0, double = 0.005);
		~sprottB();
		void setVals(double, double, double, double);
		void iterate();
		gsl_matrix* currentCoord();
	private:
		double x, y, z, dt;
		double a, b, c;
};
