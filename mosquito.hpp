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
    vector<Mosquito> MosquitoList;
    vector<Target> TargetList;
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
        //cout<<this<<"\n";
        //cout<<&OtherMosquito<<"\n";
        return aBool;
    };

    // Diffuse
    void diffuseMosquito();

    // Move to a target

    // Die (remove mosquito from overall container, and kill it)

    // Mark
    void mark() {marked = 1;}
};

Mosquito::Mosquito(double dX, double dY)
{
    x = dX;
    y = dY;
    marked = 0;

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
    Mosquito aMosquito(dX,dY);
    aContainer.MosquitoList.push_back(aMosquito);
}


#endif // MOSQUITO_HPP_INCLUDED
