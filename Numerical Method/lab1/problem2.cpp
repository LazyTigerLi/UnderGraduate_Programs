#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    double x[5] = {4040.045551380452,-2759471.276702747,-31.64291531266504,2755462.874010974,0.0000557052996742893};
    double x1 = x[3] + x[0] + x[4],x2 = x[1] + x[2];
    cout<<sizeof(double)<<'\t'<<sizeof(long double);
    cout<<scientific;
    cout<<setprecision(70);
    cout<<"(a) "<<x[0] + x[1] + x[2] + x[3] + x[4]<<endl
        <<"(b) "<<x[4] + x[3] + x[2] + x[1] + x[0]<<endl
        <<"(c) "<<x1 + x2<<endl;
    cout<<x[4]<<'\t'
        <<x[0]+x[1]<<'\t'<<x[0]+x[1]+x[2]<<'\t'<<x[0]+x[1]+x[2]+x[3]<<'\t'<<x[0]+x[1]+x[2]+x[3]+x[4]<<endl;
}