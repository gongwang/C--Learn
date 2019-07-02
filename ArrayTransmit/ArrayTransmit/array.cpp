#include <stdio.h>
#include <iostream>
using namespace std;

void arrayCheck(int Param[])
{
    int i = Param[0];
    int j = Param[1];
    cout<<"i="<<i<<endl;
    cout<<"j="<<j<<endl;
}

void arrayCheck2(double *Param)
{
    double array[2] = {};
    array[0] = (double)*Param;

    array[1] = (double)*(Param+1);

}

void main()
{
    
    double arrry[2] = {0,1};

 //   arrayCheck(arrry);

    arrayCheck2(arrry);

    system("pause");
}