#include "mosquito.hpp"

int T = 200;
int iNumTargets = 1000;

int main()
{
    initialiseRandomTargets(iNumTargets);
    initialiseSingleReleaseMosquitoes(1000,500.0,500.0);
    clock_t startTime = clock();
    evolveSystem(T,10,true);
    cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

//    screenPrintMosquitoAge();
//    screenPrintMosquitoSpatial();
//    screenPrintMosquitoDistance();


    return 0;
}
