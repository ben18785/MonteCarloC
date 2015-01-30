#include "mosquito.hpp"

using namespace std;
Containers List;
Parameters pa;

int main()
{
    CreateMosquito(3,5);
    CreateMosquito(45,51);
    CreateTarget(45,50);
    CreateTarget(45,50);

    int cCount = 0;
    for (int i = 0; i < 100; ++i)
    {
        Mosquito aMosquito(45,50);
        aMosquito.findTargetsMoveToTarget();
        cCount += List.TargetList[1]->getTargetMosquitoList().size();
        cout<<List.TargetList[1]->getTargetMosquitoList().size()<<endl;
    }
    cout<<cCount-2000<<endl;

    return 0;
}

Mosquito::Mosquito(double dX, double dY)
{
    x = dX;
    y = dY;
    marked = 0;

}

void Mosquito::moveDiffuseMosquito()
{
    double dStepLength = 0.1;
    x += dStepLength*RandN();
    y += dStepLength*RandN();

    // Need to handle the case of where the steps take it outside the simulation area
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

void Mosquito::moveToRandomTarget(vector<Target*> vPTarget)
{
    int iLen = vPTarget.size();
    if (iLen==0) {return;} //If no targets available then just return

    // Select random target, and move there
    int iRand = randi(0,iLen-1);
    moveToTargetMosquito(vPTarget[iRand]);
}

void Mosquito::findTargetsMoveToTarget()
{
    vector<Target*> vPTargets = findTargetsWithinRadius();
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
}

void Target::removeMosquito(Mosquito* pMosquito)
{
    TargetMosquitoList.erase(remove(TargetMosquitoList.begin(), TargetMosquitoList.end(), pMosquito),TargetMosquitoList.end());
}


void CreateMosquito(double dX, double dY)
{
    Mosquito* pMos;
    pMos = new Mosquito(dX,dY);
    List.MosquitoList.push_back(pMos);
}

void killMosquito(Mosquito* pMosquito)
{
    vector<Mosquito*> aMosquitoPList = List.MosquitoList;
    aMosquitoPList.erase(remove(aMosquitoPList.begin(), aMosquitoPList.end(), pMosquito),aMosquitoPList.end());
    List.MosquitoList = aMosquitoPList;
}


void CreateTarget(double dX, double dY)
{
    Target* pTarget;
    pTarget = new Target(dX,dY);
    List.TargetList.push_back(pTarget);
}
