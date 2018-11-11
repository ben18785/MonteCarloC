#ifndef MATHEMATICAL_HPP_INCLUDED
#define MATHEMATICAL_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <math.h>
#include <random>
#include <time.h>
#include <list>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>
#include <unsupported/Eigen/SparseExtra>
#include <algorithm>
#include <functional>
#include <Eigen/Dense>
#include <vector>
#include <fstream>

using namespace Eigen;

using namespace std;
using Eigen::MatrixXi;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::SparseMatrix;
using Eigen::ConjugateGradient;
using Eigen::SimplicialLLT;
using Eigen::SimplicialLDLT;
using Eigen::BiCGSTAB;
using Eigen::SparseLU;

const int iSeed = int(time(0));

typedef SparseMatrix<double> SpMatrix;

//Pre-declarations
int mod(int,int);
double Rand();
double RandN();
int randi(int,int);
double normcdf(double);
int PoissonRnd(double dLambda);
int fProbabilitySwitch(double aProbability);
SpMatrix Laplace2DNeumann(int);
SpMatrix Laplace2DDirichlet(int);
template<typename T>
void WriteVectorToFile(vector<T> aVector ,string aFilename, int iNumComponents);
template<typename T>
void WriteToFile(T aThing ,string aFilename);
double distance(double aX, double aY, double bX, double bY);

#endif
