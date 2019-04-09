//dispatch.cpp
#include <iostream>
#include <algorithm>
#include <queue>
#include <ctime>
#define N 20
using namespace std;
int n,k;
int task[N],machine[N];                 //machine[i]表示机器i完成安排给它的所有任务所需时间
int curr_path[N];                            //path[i] = j表示任务i分配给了机器j
int shortest_path[N];
int min_time = INT_MAX;

bool cmp(int x,int y)
{
    return x > y;
}

void dispatch(int depth,int time)               //depth表示当前的递归深度，即正在安排第depth+1个任务，time表示当前搜索路径上各机器的最长时间
{
    if(depth >= n)                              //试探到叶结点
    {
        if(time < min_time)
        {
            for(int i = 0; i < n; i++)shortest_path[i] = curr_path[i];
            min_time = time;
        }
        return;
    } 
    if(time >= min_time)return;                 //剪枝，同样安排depth+1个任务，但各机器的最长时间更长，则舍去该分支
    for(int i = 0; i < k; i++)
        if(machine[i] + task[depth] <= min_time)
        {
            curr_path[depth] = i + 1;
            machine[i] += task[depth];
            dispatch(depth + 1,max(time,machine[i]));
            machine[i] -= task[depth];
        }
}

int main() {
    cin>>n>>k;
    for(int i = 0; i < k; i++)machine[i] = 0;
    for(int i = 0; i < n; i++)cin>>task[i];                         //输入每个任务所需要的时间
    time_t t1 = time(0);
    sort(task,task + n,cmp);                                        //启发式搜索，能更快找到最优解
    
    curr_path[0] = 1;                   //所有的机器都是等价的，故而第一个任务安排在任意一个机器上都是等价的，可以使运行时间变为原来的1/k                    
    machine[0] += task[0];
    dispatch(1,task[0]);
    cout<<min_time<<endl;
    queue <int> plan[k];
    for(int i = 0; i < n; i++)
        plan[shortest_path[i] - 1].push(task[i]);
    cout<<"machine\ttask\n";
    for(int i = 0; i < k; i++)
    {   
        cout<<i + 1<<'\t';
        while(!plan[i].empty())
        {
            cout<<plan[i].front()<<' ';
            plan[i].pop();
        }
        cout<<endl;
    }
    time_t t2 = time(0);
    cout<<"time:"<<t2 - t1<<endl;
    return 0;
}
