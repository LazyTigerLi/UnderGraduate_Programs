#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main()
{
    float x = 1;
    cout<<scientific;
    cout<<setprecision(30);
    for(int i = 0; i < 10; i++)
    {
        x = x / 8;
        cout<<x<<'\t'<<sqrtf(x * x + 4) - 2<<'\t'<<x * x / (sqrtf(x * x + 4) + 2)<<endl;
        cout<<x*x<<endl;
    }
    return 0;
}
