#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

long double trapeziumIntegrate(long double h,std::vector<long double> point)      //参数为小区间长度、积分节点
{
    long double t = 0;
    for(int i = 1; i < point.size() - 1; i++)
        t += point[i];
    t += 0.5 * (point[0] + point[point.size() - 1]);
    return h * t;
}

long double simpsonIntegrate(long double h,std::vector<long double> point)
{
    long double s1 = 0,s2 = 0,s = 0;
    int m = (point.size() - 1) / 2;
    for(int i = 0; i <= m - 1; i++)
        s1 += point[2 * i + 1];
    for(int i = 1; i <= m - 1; i++)
        s2 += point[2 * i];
    s += point[0] + point[point.size() - 1] + 4 * s1 + 2 * s2;
    return s * h / 3;
}

int main()
{
    long double a = 1,b = 6;
    long double realResult = cosl(1) - cosl(6);
    long double error[13];
    std::vector<long double> point;
    for(int i = 0; i <= 12; i++)
    {
        std::cout<<std::scientific;
        std::cout<<std::setprecision(11);

        long double h = (b - a) / (1 << i);
        point.push_back(sinl(a));
        for(int j = 1; j < 1 << i; j++)
            point.push_back(sinl(a + j * h));
        point.push_back(sinl(b));
        long double result = trapeziumIntegrate(h,point);
        std::cout<<"k="<<i<<" ,e"<<i<<"="<<result<<'\t'<<realResult - result<<'\t';
        error[i] = realResult - result;

        std::cout<<std::fixed;
        std::cout<<std::setprecision(4);
        if(i == 0)std::cout<<std::endl;
        if(i >= 1)std::cout<<"d = "<<-logl(error[i] / error[i - 1]) / logl(2)<<std::endl;
        point.clear();
    }

    for(int i = 1; i <= 12; i++)
    {
        std::cout<<std::scientific;
        std::cout<<std::setprecision(11);

        long double h = (b - a) / (1 << i);
        point.push_back(sinl(a));
        for(int j = 1; j < 1 << i; j++)
            point.push_back(sinl(a + j * h));
        point.push_back(sinl(b));
        long double result = simpsonIntegrate(h,point);
        std::cout<<"k="<<i<<" ,e"<<i<<"="<<result<<'\t'<<realResult - result<<'\t';
        error[i] = realResult - result;

        std::cout<<std::fixed;
        std::cout<<std::setprecision(4);
        if(i == 1)std::cout<<std::endl;
        if(i >= 2)std::cout<<"d = "<<-logl(error[i] / error[i - 1]) / logl(2)<<std::endl;
        point.clear();
    }
    return 0;
}