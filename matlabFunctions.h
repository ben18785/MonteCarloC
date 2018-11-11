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

int iSeed = int(time(0));
default_random_engine generator(iSeed);

typedef SparseMatrix<double> SpMatrix;

//Pre-declarations
int mod(int,int);
double Rand();
int randi(int,int);
double normcdf(double);
inline int fProbabilitySwitch(double aProbability);
SpMatrix Laplace2DNeumann(int);
SpMatrix Laplace2DDirichlet(int);
template<typename T>
void WriteVectorToFile(vector<T> aVector ,string aFilename, int iNumComponents);
template<typename T>
void WriteToFile(T aThing ,string aFilename);

// Mimicks the action of mod in Matlab (C++'s native definition is different)
inline int mod(const int iA, const int iB)
{
    return iA-iB*floor(double(iA)/double(iB));
}

// Mimicks the action of mod in Matlab (C++'s native definition is different)
inline double mod(const double iA, const double iB)
{
    return iA-iB*floor(double(iA)/double(iB));
}

//Generates a random uniform double on 0,1
double Rand()
{
    uniform_real_distribution<double> uDistribution(0.0,1.0);
    double dRand = uDistribution(generator);
    return dRand;
}

//Generates a poisson distributed discrete random quantity with a mean
int PoissonRnd(double dLambda)
{
    poisson_distribution<int> poissonDistribution(dLambda);
    return poissonDistribution(generator);
}

//Generates a random standard normal double
double RandN()
{
    normal_distribution<double> uDistribution(0.0,1.0);
    double dRand = uDistribution(generator);
    return dRand;
}


//Generates a random uniform integer on iMin,iMax
int randi(int iMin, int iMax)
{
    double dRand = Rand()*(iMax-iMin) + iMin;
    int aRandInt = round(dRand);
    return aRandInt;
}

// Determines whether an event takes place given its prior probability
inline int fProbabilitySwitch(double aProbability)
{
    int cEvent;
    double aRand = Rand();
    if (aProbability > aRand) cEvent = 1;
    else cEvent = 0;
    return cEvent;
}

// Returns the normal cdf of a particular value of x
double normcdf(double x)
{
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x)/sqrt(2.0);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return 0.5*(1.0 + sign*y);
}

//Writes a vector of something to a file
template<typename T>
void WriteVectorToFile(vector<T> aVector ,string aFilename)
{
    ostringstream os;
    ofstream fileTemp;
    os<<aFilename;
    fileTemp.open(os.str().c_str());
    for (int t = 0; t < aVector.size(); ++t)
    {
        fileTemp<<aVector[t]<<"\n";
    }
    fileTemp.close();os.str("");
}

//Read a vector from a text from file
template<typename T>
vector<double> ReadVectorFromFile(T aFilename)
{
    string aStringFilename = string(aFilename);
    double input;
    vector<double> aVector;
    ifstream a_file (aFilename);
    while (a_file >> input)
    {
        aVector.push_back(input);
    }
    a_file.close();
    return aVector;
}


//Prints a vector of something to output
template<typename T>
void PrintVector(vector<T> aVector)
{
    for (int t = 0; t < aVector.size(); ++t)
    {
        cout<<aVector[t]<<endl;
    }
}

//Writes something to a file
template<typename T>
void WriteToFile(T aThing ,string aFilename)
{
    ostringstream os;
    ofstream fileTemp;
    os<<aFilename;
    fileTemp.open(os.str().c_str());
    fileTemp<<aThing;
    fileTemp.close();os.str("");
}


// Finds the Euclidean distance between two coordinates
inline double distance(double aX, double aY, double bX, double bY)
{
    return sqrt(pow(aX-bX,2) + pow(aY-bY,2));
}

#include <Eigen/Dense>
#include <vector>
#include <fstream>

using namespace Eigen;

template<typename M>
M load_csv (const std::string & path) {
    std::ifstream indata;
    indata.open(path);
    std::string line;
    std::vector<double> values;
    uint rows = 0;
    while (std::getline(indata, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ',')) {
            values.push_back(std::stod(cell));
        }
        ++rows;
    }
    return Map<const Matrix<typename M::Scalar, M::RowsAtCompileTime, M::ColsAtCompileTime, RowMajor>>(values.data(), rows, values.size()/rows);
}

//Time a bit of code. Can't get the tic toc thing to work as in Matlab, but use top line at start, and second line below it
//clock_t startTime = clock();
// some code here
// to compute its execution duration in runtime
//

// Example use of kd trees for search
//kdtree* kdTree = kd_create(2);
//kdres* kdResultsSet;
//double posBen[2];
//
//// Build a tree with 10 fixed points
//for (int i = 0; i < 10; ++i)
//{
//    posBen[0] = Rand();
//    posBen[1] = Rand();
//    assert(kd_insert(kd.kdTree, posBen,pTarget) == 0);
//}
//
//double pos1[2];
//double dSearchRadius;
//pos1[0] = 0.0;//Example test point
//pos1[1] = 0.0;//Example test point
//
//// Find the entities that are nearest the given point
//kdResultsSet = kd_nearest_range(kdTree, pos1,dSearchRadius);
//double pos[2];
//
//// Loop through the results using the iterator given in the library
//while( !kd_res_end( kdResultsSet ) ) {
//
///* get the data and position of the current result item, and return a pointer to a class target (which Ben created) */
//vTargetsWithinRadius.push_back((Target*)kd_res_item( kdResultsSet, pos ));
//
///* go to the next entry */
//kd_res_next(kdResultsSet );
//}
//// Frees the memory due to holding the results of the search
//kd_res_free(kdResultsSet);
