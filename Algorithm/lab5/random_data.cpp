//random_data.cpp
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <iostream>
using namespace std;
int main()
{
    int arcnum;
    cin>>arcnum;
    std::ofstream outfile;
    outfile.open("random_data.txt");
    outfile.clear();
    srand((int)time(0));
    
    cout<<arcnum;
    map<int,int> arcs;                                  //存储弧
    for(int i = 0; i < arcnum; i++)
    {
        int v1 = rand() % (10 * arcnum) + 1;
        int v2 = rand() % (10 * arcnum) + 1;
        if((arcs.find(v1) == arcs.end() && arcs.find(v2) == arcs.end())
            || (arcs.find(v1)->second != v2 && (arcs.find(v2) == arcs.end() || arcs.find(v2)->second != v1))
            || (arcs.find(v2)->second != v1 && (arcs.find(v1) == arcs.end() || arcs.find(v2)->second != v2)))
        {
            arcs.insert(pair<int,int>(v1,v2));
            outfile<<v1<<' '<<v2<<' ';
            if(i != arcnum - 1)outfile<<'\n';
        }
        else i--;
    }
    outfile.close();
}