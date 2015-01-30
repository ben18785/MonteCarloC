#include "mosquito.hpp"

using namespace std;
Containers List;

int main()
{

    Target aTarget(4,5);
    Mosquito aMos(3,2);
    Mosquito bMos(6,7);
    aTarget.addMosquito(&aMos);
    aTarget.addMosquito(&bMos);
    cout<<aMos.getX()<<endl;
    aTarget.removeMosquito(&aMos);
    Mosquito* pMos = aTarget.getTargetMosquitoList()[0];
    cout<<pMos->getX()<<endl;


    return 0;
}
