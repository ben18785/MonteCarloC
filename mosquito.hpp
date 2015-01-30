#ifndef MOSQUITO_HPP_INCLUDED
#define MOSQUITO_HPP_INCLUDED

#include <iostream>
#include "matlabFunctions.h"
#include <algorithm>

//Declarations
class Mosquito;
class Target;
bool isMosquito(Mosquito);

struct Containers
{
    vector<Mosquito*> MosquitoList;
    vector<Target*> TargetList;
};

class Mosquito
{
    double x, y;
    int marked;

public:
    // Constructor
    Mosquito (double,double);

    // Accessors
    double getX() {return x;}
    double getY() {return y;}
    double getMarked() {return marked;}

    // Check for equality
    bool operator==(const Mosquito& OtherMosquito)
    {
        bool aBool = this == &OtherMosquito;
        return aBool;
    };

    // Diffuse
    void moveDiffuseMosquito();

    // Move to a target
    void moveToTargetMosquito();

    // Die (remove mosquito from overall container, and kill it)
    void killMosquito(Containers&);

    // Mark
    void mark() {marked = 1;}
};

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

void Mosquito::killMosquito(Containers& aContainer)
{
    aContainer.MosquitoList.erase(remove(aContainer.MosquitoList.begin(), aContainer.MosquitoList.end(), this),aContainer.MosquitoList.end());
}

class Target
{
    double x, y;

    // Container of mosquitoes
    vector<Mosquito*> TargetMosquitoList;

public:
    // Constructor
    Target(double,double);

    //Accessors
    double getX() {return x;}
    double getY() {return y;}
    vector<Mosquito*> getTargetMosquitoList() {return TargetMosquitoList;}

    // Add mosquito to its container
    void addMosquito(Mosquito*);

    // Remove mosquito from its container
    void removeMosquito(Mosquito*);

    // Check if there are any mosquitoes within a radius and add them to their container. I think I will need to flip a coin
    // if there are two (or more) targets who have this mosquito within a radius. Perhaps we can go through and make
    // temporary lists... Or perhaps don't iterate through traps, simply do by mosquitoes?
};

Target::Target(double dX, double dY)
{
    x = dX;
    y = dY;
}

void Target::addMosquito(Mosquito* aMosquito)
{
    TargetMosquitoList.push_back(aMosquito);
}

void Target::removeMosquito(Mosquito* aMosquito)
{
    TargetMosquitoList.erase(remove(TargetMosquitoList.begin(), TargetMosquitoList.end(), aMosquito),TargetMosquitoList.end());
}


void CreateMosquito(double dX, double dY, Containers& aContainer)
{
    Mosquito* pMos;
    pMos = new Mosquito(dX,dY);
    aContainer.MosquitoList.push_back(pMos);
}

void killMosquito(Mosquito* pMosquito, Containers& aContainer)
{
    vector<Mosquito*> aMosquitoPList = aContainer.MosquitoList;
    aMosquitoPList.erase(remove(aMosquitoPList.begin(), aMosquitoPList.end(), pMosquito),aMosquitoPList.end());
    aContainer.MosquitoList = aMosquitoPList;
}

#endif // MOSQUITO_HPP_INCLUDED
