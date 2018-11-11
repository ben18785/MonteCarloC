#include "mathematical.hpp"

default_random_engine generator(iSeed);

// Mimicks the action of mod in Matlab (C++'s native definition is different)
int mod(const int iA, const int iB)
{
    return iA-iB*floor(double(iA)/double(iB));
}

// Mimicks the action of mod in Matlab (C++'s native definition is different)
double mod(const double iA, const double iB)
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
int fProbabilitySwitch(double aProbability)
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
double distance(double aX, double aY, double bX, double bY)
{
    return sqrt(pow(aX-bX,2) + pow(aY-bY,2));
}

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
