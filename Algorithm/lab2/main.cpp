#include <iostream>
#include "rb_tree.h"
#include <fstream>
#include <string>
#include <cstdio>
using namespace std;

int main()
{
    RB_tree *T = new RB_tree;
    ifstream infile;
    infile.open("data.txt");
    string str;
   	while(getline(infile,str))
    {
        char low[5],high[5];
        int pos = str.find(',',0);
        str.copy(low,pos,0);
        str.copy(high,str.size() - pos - 1,pos + 1);
        low[pos] = '\0';
        high[str.size() - pos - 1] = '\0';          //不加\0的话会被上次的内容覆盖，不是很懂，明明是临时变量
        
        treenode *node = new treenode;
        node->i.high = atoi(high);
        node->i.low = atoi(low);
        RB_tree::RB_insert(T,node);

        str.clear();
    }
    infile.close();
    RB_tree::display(T->root,0);

    interval i = {1,2};
    treenode *result = RB_tree::interval_search(T,i);
    cout<<'['<<result->i.low<<','<<result->i.high<<"] | "<<result->max
            <<' '<<((result->c == black) ? "black" : "red")<<endl;

    RB_tree::RB_delete(T,result);
    RB_tree::display(T->root,0);

    return 0;
}
