#ifndef MOSQUITO_HPP_INCLUDED
#define MOSQUITO_HPP_INCLUDED

#include <iostream>
#include "matlabFunctions.h"
#include <algorithm>
#include <assert.h>

//Declarations
class Mosquito;
class Target;
bool isMosquito(Mosquito);
struct Containers;
struct Parameters;
void CreateMosquito(double, double);
void CreateTarget(double, double);

struct Containers
{
    vector<Mosquito*> MosquitoList;
    vector<Target*> TargetList;
};

struct Parameters
{
    double captureRadius = 3.0;
};

class Mosquito
{
    double x, y;
    int marked;
    Target* pInTarget = NULL; // A pointer to a target, if the mosquito is in one

public:
    // Constructor
    Mosquito (double,double);

    // Accessors
    double getX() {return x;}
    double getY() {return y;}
    double getMarked() {return marked;}
    Target* getPTarget() {return pInTarget;}

    // Check for equality
    bool operator==(const Mosquito& OtherMosquito)
    {
        bool aBool = this == &OtherMosquito;
        return aBool;
    };

    // Diffuse
    void moveDiffuseMosquito();

    // Move to a target
    void moveToTargetMosquito(Target*);

    // Move away from a target
    void moveOutOfTargetMosquito(double,double);

    // Check for the presence of a target within target radius
    bool checkRadiusForTarget(Target*);

    // Check for the presence of any targets within target radius, and return a vector of target pointers
    vector<Target*> findTargetsWithinRadius();

    // Choose which target to move to from list probabilistically, and move there
    void moveToRandomTarget(vector<Target*>);

    // Check for targets, and move to one of them probabilistically (if there are any)
    void findTargetsMoveToTarget();

    // Die (remove mosquito from overall container, and kill it)
    void killMosquito();

    // Mark
    void mark() {marked = 1;}
};


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

#endif // MOSQUITO_HPP_INCLUDED
