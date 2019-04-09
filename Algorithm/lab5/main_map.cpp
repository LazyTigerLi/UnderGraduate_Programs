#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <ctime>
#include <queue>
#include <thread>
#include <mutex>
using namespace std;
map<int,int> vex_index_map;         //键值对是顶在数据集中的编号和在邻接表中的索引号
int count = 0;
mutex mu;
struct VNode
{
    list<int> ArcList;
    bool visited;
};

struct graph
{
    int vexnum,arcnum;
    VNode *vertices;
};

void BFSTranverse2(graph gra)
{
    queue<VNode *> q;
    for(int i = gra.vexnum - 1; i >= 0; i--)
    {
        if(!gra.vertices[i].visited)
        {
            gra.vertices[i].visited = true;
            mu.lock();
            count++;
            mu.unlock();
            q.push(&gra.vertices[i]);
            while(!q.empty())
            {
                VNode *temp = q.front();
                q.pop();
                for(list<int>::iterator it = gra.vertices[i].ArcList.begin();
                    it != gra.vertices[i].ArcList.end(); it++)
                {
                    int vex_index = vex_index_map.find(*it)->second;
                    if(!gra.vertices[vex_index].visited)
                    {
                        gra.vertices[vex_index].visited = true;
                        mu.lock();
                        count++;
                        mu.unlock();
                        q.push(&gra.vertices[vex_index]);
                    }
                }
            }
        }
    }
}

void BFSTranverse(graph gra)
{
    thread t(BFSTranverse2,gra);
    t.detach();
    queue<VNode *> q;
    for(int i = 0; i < gra.vexnum; i++)
    {
        if(!gra.vertices[i].visited)
        {
            gra.vertices[i].visited = true;
            mu.lock();
            count++;
            mu.unlock();
            q.push(&gra.vertices[i]);
            while(!q.empty())
            {
                VNode *temp = q.front();
                q.pop();
                for(list<int>::iterator it = gra.vertices[i].ArcList.begin();
                    it != gra.vertices[i].ArcList.end(); it++)
                {
                    int vex_index = vex_index_map.find(*it)->second;
                    if(!gra.vertices[vex_index].visited)
                    {
                        gra.vertices[vex_index].visited = true;
                        mu.lock();
                        count++;
                        mu.unlock();
                        q.push(&gra.vertices[vex_index]);
                    }
                }
            }
        }
    }
}

int main()
{   
    

    string filename;
    int vex_num;
    cin>>filename>>vex_num;
    graph gra;                                          //图
    gra.vertices = new VNode[vex_num];
    ifstream infile;
    infile.open(filename);
    string str;
    int index = 0;
    time_t t1 = time(0);
    cout<<"here\n";
    while(getline(infile,str))
    {
        char v1[10],v2[10];
        int pos = str.find(' ',0);
        str.copy(v1,pos,0);
        str.copy(v2,str.size() - pos - 1,pos + 1);
        v1[pos] = '\0';
        v2[str.size() - pos - 1] = '\0';
        
        int v1_num = atoi(v1),v2_num = atoi(v2);
        if(vex_index_map.find(v1_num) == vex_index_map.end())
        {  
            vex_index_map.insert(pair<int,int>(v1_num,index));
            gra.vertices[index].ArcList.push_back(v2_num);
            gra.vertices[index].visited = false;
            index++;
        }
        else
        {
            int temp_index = vex_index_map.find(v1_num)->second;
            gra.vertices[temp_index].ArcList.push_back(v2_num);
        }
        if(vex_index_map.find(v2_num) == vex_index_map.end())
        {  
            vex_index_map.insert(pair<int,int>(v2_num,index));
            index++;
        }
    }
    infile.close();
    gra.vexnum = index;

    time_t t2 = time(0);
    BFSTranverse(gra);
    time_t t3 = time(0);
    cout<<t1<<' '<<t2<<' '<<t3<<endl;
    cout<<count<<endl;
    return 0;
}