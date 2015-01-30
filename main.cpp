#include "mosquito.hpp"

using namespace std;
Containers List;

int main()
{
    CreateMosquito(3,5,List);
    CreateMosquito(4,7,List);

    cout<<List.MosquitoList[0]->getX()<<", "<<List.MosquitoList[0]->getY()<<endl;
    List.MosquitoList[0]->moveDiffuseMosquito();
    cout<<List.MosquitoList[0]->getX()<<", "<<List.MosquitoList[0]->getY()<<endl;

    return 0;
}
