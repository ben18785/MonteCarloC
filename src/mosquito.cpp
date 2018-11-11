#include "mosquito.hpp"


using namespace std;
Containers List;
Parameters pa;
Total to;
SimulationChoices sc;
KDTreeParameters kd;

double U = 1500.0;

Mosquito::Mosquito(double dX, double dY)
{
    x = dX;
    y = dY;
}

void Mosquito::moveDiffuseMosquito()
{
    double xnew, ynew;
    xnew = x + pa.stepSigma*RandN();
    ynew = y + pa.stepSigma*RandN();

    // Increment the distance moved by mosquito in lifetime
    distanceMoved += distance(x,y,xnew,ynew);

    x = mod(xnew,U);
    y = mod(ynew,U);

    // Need to test that steps are in the area
    assert(x > 0 || x < U || y > 0 || y < U);
}

void Mosquito::killMosquito()
{
    List.MosquitoList.erase(remove(List.MosquitoList.begin(), List.MosquitoList.end(), this),List.MosquitoList.end());
}

void Mosquito::moveToTargetMosquito(Target* pTarget)
{
    x = pTarget->getX();
    y = pTarget->getY();
    pTarget->addMosquito(this);
    pInTarget = pTarget;
}

void Mosquito::moveOutOfTargetMosquito(double dX, double dY)
{
    // Check pointer isn't null
    assert(pInTarget!=NULL);

    x = dX;
    y = dY;
    pInTarget->removeMosquito(this);
    pInTarget = NULL;

}

bool Mosquito::checkRadiusForTarget(Target* pTarget)
{
    // Don't want to carry out this operation for mosquitoes that are in targets
    assert(pInTarget==NULL);

    if (distance(x,y,pTarget->getX(),pTarget->getY()) <= pa.captureRadius)
    {
        return true;
    }
    return false;
}

vector<Target*> Mosquito::findTargetsWithinRadius()
{
    vector<Target*> vTargetsWithinRadius;
    for (vector<Target*>::iterator it = List.TargetList.begin() ; it != List.TargetList.end(); ++it)
    {
        if (checkRadiusForTarget(*it)) { vTargetsWithinRadius.push_back(*it);}// Still need to dereference pointer as it is a pointer to a pointer
    }
    return vTargetsWithinRadius;
}

vector<Target*> Mosquito::findTargetsWithinRadiusKd()
{
    vector<Target*> vTargetsWithinRadius;
    double pos1[2];
    pos1[0] = this->getX();
    pos1[1] = this->getY();
	kd.kdResultsSet = kd_nearest_range(kd.kdTree, pos1,pa.captureRadius);
	double pos[2];
	while( !kd_res_end( kd.kdResultsSet ) ) {

    /* get the data and position of the current result item */
    vTargetsWithinRadius.push_back((Target*)kd_res_item( kd.kdResultsSet, pos ));

    /* go to the next entry */
    kd_res_next( kd.kdResultsSet );
  }

	kd_res_free(kd.kdResultsSet);

    return vTargetsWithinRadius;
}


void Mosquito::moveToRandomTarget(vector<Target*> vPTarget)
{
    int iLen = vPTarget.size();
    if (iLen==0) {return;} //If no targets available then just return

    // Select random target, and move there
    int iRand = randi(0,iLen-1);
    // Check that the distance is below the search radius
    assert(distance(this->getX(),this->getY(),vPTarget[iRand]->getX(),vPTarget[iRand]->getY())<=pa.captureRadius);
    moveToTargetMosquito(vPTarget[iRand]);
}

void Mosquito::findTargetsMoveToTarget()
{
    vector<Target*> vPTargets = findTargetsWithinRadiusKd();
    moveToRandomTarget(vPTargets);
}

Target::Target(double dX, double dY)
{
    x = dX;
    y = dY;
}

void Target::addMosquito(Mosquito* pMosquito)
{
    TargetMosquitoList.push_back(pMosquito);
    to.iNumMosquitoesInTargets++;
    to.iNumMosquitoesOutsideTargets--;
    assert(to.iNumMosquitoesInTargets+to.iNumMosquitoesOutsideTargets==to.iNumMosquitoes);
    if (pMosquito->getMarked()==1) {to.iNumMarkedMosquitoesInTargets++; to.iNumMarkedMosquitoesOutsideTargets--;}

    assert(to.iNumMarkedMosquitoesInTargets+to.iNumMarkedMosquitoesOutsideTargets==to.iNumMarkedMosquitoes);
}

void Target::removeMosquito(Mosquito* pMosquito)
{
    TargetMosquitoList.erase(remove(TargetMosquitoList.begin(), TargetMosquitoList.end(), pMosquito),TargetMosquitoList.end());
    to.iNumMosquitoesInTargets--;
    to.iNumMosquitoesOutsideTargets++;
    assert(to.iNumMosquitoesInTargets+to.iNumMosquitoesOutsideTargets==to.iNumMosquitoes);
    if (pMosquito->getMarked()==1) {to.iNumMarkedMosquitoesInTargets--; to.iNumMarkedMosquitoesOutsideTargets++;}
    assert(to.iNumMarkedMosquitoesInTargets+to.iNumMarkedMosquitoesOutsideTargets==to.iNumMarkedMosquitoes);
}


void CreateMosquito(double dX, double dY)
{
    Mosquito* pMos;
    pMos = new Mosquito(dX,dY);
    List.MosquitoList.push_back(pMos);
    to.iNumMosquitoes++;
    to.iNumMosquitoesOutsideTargets++;
    assert(to.iNumMosquitoes==List.MosquitoList.size());
}

void killMosquito(Mosquito* pMosquito)
{
    // Check if in a target or not
    Target* pTargetTemp = pMosquito->getPTarget();
    if(pTargetTemp!=NULL)
    {
        pTargetTemp->removeMosquito(pMosquito);
    }
    to.iNumMosquitoesOutsideTargets--;
    if (pMosquito->getMarked()==1) {to.iNumMarkedMosquitoes--;to.iNumMarkedMosquitoesOutsideTargets--;}

    // Remove from master list
    List.MosquitoList.erase(remove(List.MosquitoList.begin(), List.MosquitoList.end(), pMosquito),List.MosquitoList.end());
    to.iNumMosquitoes --;
    assert(to.iNumMosquitoes==List.MosquitoList.size());
}


void CreateTarget(double dX, double dY, int iKnown)
{
    Target* pTarget;
    pTarget = new Target(dX,dY);
    List.TargetList.push_back(pTarget);
    if (iKnown==1){List.KnownTargetsList.push_back(pTarget);}
    double pos[2];
    pos[0] = dX;
    pos[1] = dY;
    assert(kd_insert(kd.kdTree, pos,pTarget) == 0);
    to.iNumTargets += 1;
    assert(to.iNumTargets==List.TargetList.size());
}

void initialiseRandom(int iNumTargets, int iNumMosquitoes)
{
    // Create targets randomly according to Poisson process
    for (int i = 0; i < iNumTargets; ++i)
    {
        double dX, dY;
        dX = U*Rand();
        dY = U*Rand();
        CreateTarget(dX,dY,0);
    }

    for (int i = 0; i < iNumMosquitoes; ++i)
    {
        double dX, dY;
        dX = U*Rand();
        dY = U*Rand();
        CreateMosquito(dX,dY);
    }
}

pair<vector<double>,vector<double>> spatialTargetInformation()
{
    pair<vector<double>,vector<double>> pairSpatial;
    Target* pTargetTemp;
    double dX, dY;
    for (vector<Target*>::iterator it = List.TargetList.begin() ; it != List.TargetList.end(); ++it)
    {
        pTargetTemp = *it;
        dX = pTargetTemp->getX();
        dY = pTargetTemp->getY();

        // Check that all the targets are within the target area
        assert(dX > 0 || dX < U || dY > 0 || dY < U);
        pairSpatial.first.push_back(dX);
        pairSpatial.second.push_back(dY);
    }
    return pairSpatial;
}

pair<vector<double>,vector<double>> spatialMosquitoInformation()
{
    pair<vector<double>,vector<double>> pairSpatial;
    Mosquito* pMosquitoTemp;
    double dX, dY;
    for (vector<Mosquito*>::iterator it = List.MosquitoList.begin() ; it != List.MosquitoList.end(); ++it)
    {
        pMosquitoTemp = *it;
        dX = pMosquitoTemp->getX();
        dY = pMosquitoTemp->getY();

        // Check that all the targets are within the target area
        assert(dX > 0 || dX < U || dY > 0 || dY < U);
        pairSpatial.first.push_back(dX);
        pairSpatial.second.push_back(dY);
    }
    return pairSpatial;
}


void screenPrintSpatial (pair<vector<double>,vector<double>> pairSpatial)
{
    int iLen = pairSpatial.first.size();
    for (int i = 0; i < iLen; ++i)
    {
        cout<<pairSpatial.first[i]<<", "<<pairSpatial.second[i]<<endl;
    }
}

void screenPrintTargetSpatial()
{
    pair<vector<double>,vector<double>> pairSpatial = spatialTargetInformation();
    screenPrintSpatial(pairSpatial);
}

void screenPrintMosquitoSpatial()
{
    pair<vector<double>,vector<double>> pairSpatial = spatialMosquitoInformation();
    screenPrintSpatial(pairSpatial);
}


void initialiseRandomTargets(int iNumTargets)
{
    // Assign the number of targets which are known
    int iNumKnownTargets = (int)round(iNumTargets*pa.dProportionKnownTargets);
    int iCountKnown = 0;
    // Create targets randomly according to Poisson process
    for (int i = 0; i < iNumTargets; ++i)
    {
        double dX, dY;
        dX = U*Rand();
        dY = U*Rand();
        if (iCountKnown < iNumKnownTargets) {CreateTarget(dX,dY,1);iCountKnown++;}
        else {CreateTarget(dX,dY,0);}
    }
}

void initialiseSingleReleaseMosquitoes(int iNumMosquitoes, double dX, double dY)
{
    // Release all mosquitoes at the same location
    for (int i = 0; i < iNumMosquitoes; ++i)
    {
        CreateMosquito(dX,dY);
    }
}

// Evolves each of the mosquitoes over the course of a day
void stepMosquitoes()
{
    Mosquito* pMosquitoTemp;
    bool MovedOutTarget;
    vector<Mosquito*> vPMosquitoListTemp = List.MosquitoList;

    for (vector<Mosquito*>::iterator it = vPMosquitoListTemp.begin() ; it != vPMosquitoListTemp.end(); ++it)
    {
        pMosquitoTemp = *it;
        MovedOutTarget = checkInTargetMoveMosquitoOut(pMosquitoTemp);

        // Let mosquitoes die
        if (fProbabilitySwitch(pa.dDailyDeathProbability)==1)
        {
            killMosquito(pMosquitoTemp);
        }
        else
        {
            // Increment age
            pMosquitoTemp->ageMosquito();

                // Check whether mosquito has not just been moved out of a target. If so, step it.
            if (!MovedOutTarget) {pMosquitoTemp->moveDiffuseMosquito();}

            // Check that all mosquitoes are now out of targets at this point
            assert(pMosquitoTemp->getPTarget()==NULL);

            // Check for targets
            pMosquitoTemp->findTargetsMoveToTarget();
        }
    }
}

// Checks whether each of the mosquitoes is in a target, and if so moves them out
bool checkInTargetMoveMosquitoOut(Mosquito* pMosquito)
{
    if (pMosquito->getPTarget()==NULL) {return false;}

    // If in a target, then move out to a new random location
    double dX, dY;
    dX = pMosquito->getX();
    dY = pMosquito->getY();
    dX += pa.stepSigma*RandN();
    dY += pa.stepSigma*RandN();

    pMosquito->moveOutOfTargetMosquito(dX,dY);
    assert(pMosquito->getPTarget()==NULL);
    return true;
}

// Evolve system for T periods
void evolveSystem(int T, int iTimeFirstRelease, bool bPrintScreen)
{
    for (int t = 0; t < T; ++t)
    {
        if (t == iTimeFirstRelease) {releaseInSingleLocation(1000,500,500);}
        stepMosquitoes();
        if (sc.bCarryingCapacity) {createRandomSpatialMosquitoes();}
        if (bPrintScreen) {cout<<t<<"     "<<to.iNumMosquitoes<<"     "<<to.iNumMosquitoesInTargets<<"     "<<to.iNumMarkedMosquitoes<<endl;}
    }
}

// Create a number of mosquitoes with a mean equal to the death rate
void createRandomSpatialMosquitoes()
{
    double dMeanMosquitoNumber = pa.dDailyDeathProbability*pa.iPopulationCarryingCapacity;

    // Poisson distributed number of mosquitoes to create
    int iNumCreate = PoissonRnd(dMeanMosquitoNumber);

    for (int i = 0; i < iNumCreate; ++i) {createRandomSpatialMosquito();}
}

// Create a single mosquito at random spatial coordinates
void createRandomSpatialMosquito()
{
    double dX, dY;
    dX = U*Rand();
    dY = U*Rand();
    CreateMosquito(dX,dY);
}

// print ages of mosquitoes to screen
void screenPrintMosquitoAge()
{
    Mosquito* pMosquito;
    for (vector<Mosquito*>::iterator it = List.MosquitoList.begin() ; it != List.MosquitoList.end(); ++it)
    {
        pMosquito = *it;
        cout<<pMosquito->getAge()<<endl;
    }
}

// print ages of mosquitoes to screen
void screenPrintMosquitoDistance()
{
    Mosquito* pMosquito;
    for (vector<Mosquito*>::iterator it = List.MosquitoList.begin() ; it != List.MosquitoList.end(); ++it)
    {
        pMosquito = *it;
        cout<<pMosquito->getDistance()<<endl;
    }
}

// Release in a given location
void releaseInSingleLocation(int iNumMarkedMosquitoesReleased, double dX, double dY)
{
    for (int i = 0; i < iNumMarkedMosquitoesReleased; ++i) {CreateMarkedMosquito(dX,dY);}
}

// Create a marked mosquito in a given location
void CreateMarkedMosquito(double dX, double dY)
{
    Mosquito* pMos;
    pMos = new Mosquito(dX,dY);
    pMos->mark();
    List.MosquitoList.push_back(pMos);
    to.iNumMosquitoes++;
    to.iNumMosquitoesOutsideTargets++;
    to.iNumMarkedMosquitoes++;
    to.iNumMarkedMosquitoesOutsideTargets++;
    assert(to.iNumMosquitoes==List.MosquitoList.size());
}

// Return number of mosquitoes at a given target as vectors of marked and unmarked mosquitoes
pair<int,int> sampleAndKillMosquitoesAtTarget(Target* pTarget)
{
    int iCountMarked = 0;
    int iCountUnMarked = 0;
    vector<Mosquito*> vPMosquitoesTemp = pTarget->getTargetMosquitoList();
    Mosquito* pMosquito;
    for (vector<Mosquito*>::iterator it = vPMosquitoesTemp.begin() ; it != vPMosquitoesTemp.end(); ++it)
    {
        pMosquito = *it;
        if (pMosquito->getMarked()==0)
        {
            iCountUnMarked++;
        }
        else
        {
            iCountMarked++;
        }
        killMosquito(pMosquito);
    }
    pair<int,int> pairMarkedUnmarkedNumberMosquitoes;
    pairMarkedUnmarkedNumberMosquitoes.first = iCountUnMarked;
    pairMarkedUnmarkedNumberMosquitoes.second = iCountMarked;

    return pairMarkedUnmarkedNumberMosquitoes;
}

// Return number of mosquitoes at a given target as vectors of marked and unmarked mosquitoes
pair<int,int> sampleAndKillMosquitoesAtListOfTargets(vector<Target*> vPTargets)
{
    Target* pTargetTemp;
    int iCountMarked = 0;
    int iCountUnmarked = 0;
    pair<int,int> pairMarkedUnmarkedTemp;
    for (vector<Target*>::iterator it = vPTargets.begin() ; it != vPTargets.end(); ++it)
    {
        pTargetTemp = *it;
        pairMarkedUnmarkedTemp = sampleAndKillMosquitoesAtTarget(pTargetTemp);
        iCountMarked += pairMarkedUnmarkedTemp.first;
        iCountUnmarked += pairMarkedUnmarkedTemp.second;
    }
    pair<int,int> pairMarkedUnmarkedFinal;
    pairMarkedUnmarkedFinal.first = iCountMarked;
    pairMarkedUnmarkedFinal.second = iCountUnmarked;
    return pairMarkedUnmarkedFinal;
}
