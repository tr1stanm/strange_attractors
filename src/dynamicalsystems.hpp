#include "3DR.hpp"
#include <string>
#pragma once

class strangeAttractor {
	public:
		strangeAttractor(double = 0, double = 0, double = 0, double = 0.005);
		virtual ~strangeAttractor();
		virtual void iterate() = 0;
		void setVals(double, double, double, double);
		gsl_matrix* currentCoord();

		std::string attractorName;
	protected:
		double x, y, z, dt;
};

class lorenz : public strangeAttractor {
	public:
		lorenz(double = 0, double = 0, double = 0, double = 0.005);
		~lorenz();
		void iterate();
	private:
		double sigma, rho, beta;
};

class halvorsen : public strangeAttractor {
	public:
		halvorsen(double = 0, double = 0, double = 0, double = 0.005);
		~halvorsen();
		void iterate();
	private:
		double a;
};

class dadras : public strangeAttractor {
	public:
		dadras(double = 0, double = 0, double = 0, double = 0.005);
		~dadras();
		void iterate();
	private:
		double a, b, c, d, e;
};

class langford : public strangeAttractor {
	public:
		langford(double = 0, double = 0, double = 0, double = 0.005);
		~langford();
		void iterate();
	private:
		double a, b, c, d, e, f;
};

class threeScroll : public strangeAttractor {
	public:
		threeScroll(double = 0, double = 0, double = 0, double = 0.005);
		~threeScroll();
		void iterate();
	private:
		double a, b, c, d, e, f;
};

class rabinovichFabrikant : public strangeAttractor {
	public:
		rabinovichFabrikant(double = 0, double = 0, double = 0, double = 0.005);
		~rabinovichFabrikant();
		void iterate();
	private:
		double alpha, gamma;
};

class rossler : public strangeAttractor {
	public:
		rossler(double = 0, double = 0, double = 0, double = 0.005);
		~rossler();
		void iterate();
	private:
		double a, b, c;
};

class quadratic3D : public strangeAttractor {
	public:
		quadratic3D(double = 0, double = 0, double = 0, double = 0.005);
		~quadratic3D();
		void iterate();
	private:
		double p[30];
};

class sprottLinz : public strangeAttractor {
	public:
		sprottLinz(double = 0, double = 0, double = 0, double = 0.005);
		~sprottLinz();
		void iterate();
	private:
		double a;
};

class sprottB : public strangeAttractor {
	public:
		sprottB(double = 0, double = 0, double = 0, double = 0.005);
		~sprottB();
		void iterate();
	private:
		double a, b, c;
};

class arneodo : public strangeAttractor {
	public:
		arneodo(double = 0, double = 0, double = 0, double = 0.005);
		~arneodo();
		void iterate();
	private:
		double a, b, c;
};

template<typename TYPE>
strangeAttractor** initAttractor(double dt, double randomScale) {
	srand(time(0));
	strangeAttractor **test = new strangeAttractor*[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		test[i] = new TYPE;
		test[i]->setVals(((double)rand() / RAND_MAX) * randomScale, 
				((double)rand() / RAND_MAX) * randomScale, 
				((double)rand() / RAND_MAX) * randomScale, dt);
	}
	return test;
}

template<typename TYPE>
gsl_matrix*** initTestPoints(TYPE *test) {
	gsl_matrix*** testPoints = new gsl_matrix**[NUM_TESTPTS];
	for(int i = 0; i < NUM_TESTPTS; ++i) {
		testPoints[i] = new gsl_matrix*[NUMPOINTS];
		for(int j = 0; j < NUMPOINTS; ++j) {
			testPoints[i][j] = test[i]->currentCoord();
			test[i]->iterate();
		}
	}
	return testPoints;
}
