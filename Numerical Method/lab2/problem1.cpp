#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#define N 8
using namespace std;
int main()
{
    vector<pair<double,double> > point;
    double lagrange[501];
    double error[501];
    double max_error;
    cout<<scientific;
    cout<<setprecision(11);
    for(int i = 0; i <= N; i++)
    {
        double x = -5 + 10 * (double)i / N;
        point.push_back(pair<double,double>(x,1 / (4 + x + x * x)));
    }
    //求出插值节点

    max_error = 0;
    for(int i = 0; i <= 500; i++)
    {
        double y = i / 50.0 - 5;
        lagrange[i] = 0;
        for(int j = 0; j <= N; j++)
        {
            double l = 1;
            for(int k = 0; k <= j - 1; k++)
                l *= (y - point[k].first) / (point[j].first - point[k].first);
            for(int k = j + 1; k <= N; k++)
                l *= (y - point[k].first) / (point[j].first - point[k].first);
            lagrange[i] += l * point[j].second;
        }
        error[i] = fabs(1 / (4 + y + y * y) - lagrange[i]);
        if(error[i] >= max_error)max_error = error[i];
    }
    cout<<max_error;
    return 0;
}
