#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

double f(double x)
{
	return 2 * pow(x,4) + 24 * pow(x,3) + 61 * pow(x,2) - 16 * x + 1;	//2x^4 + 24x^3 + 61x^2 - 16x + 1
}

double iterationNewton(double x)
{
	return x - f(x) / (8 * pow(x,3) + 72 * pow(x,2) + 122 * x - 16);
}

double iterationSecant(double x0,double x1)
{
	return x1 - (f(x1) * (x1 - x0)) / (f(x1) - f(x0));
}

std::vector<std::pair<double,double> > NewTon(double x0,double (*iteration)(double x),double (*f)(double x),double error)
{
	std::vector<std::pair<double,double> > result;
	while(true)
	{
        if(fabs(f(x0)) < error)
		{
            result.push_back(std::make_pair(x0,f(x0)));
			return result;
		}
        result.push_back(std::make_pair(x0,f(x0)));
		x0 = iteration(x0);
	}
}

std::vector<std::pair<double,double> > Secant(double x0,double x1,double (*iteration)(double x0,double x1),double (*f)(double x),double error)
{
	std::vector<std::pair<double,double> > result;
	while(true)
	{
        if(fabs(f(x0)) < error)
		{
            result.push_back(std::make_pair(x0,f(x0)));
			return result;
		}
        result.push_back(std::make_pair(x0,f(x0)));
		double temp = x1;
		x1 = iteration(x0,x1);
		x0 = temp;
	}
}

void display(std::vector<std::pair<double,double> > result)
{
	for(int i = 0; i < result.size(); i++)
		std::cout<<"k = "<<i<<'\t'<<"x = "<<result[i].first<<"\t"<<"f(x) = "<<result[i].second<<std::endl;
	std::cout<<std::endl;
}

int main()
{
    std::cout<<std::scientific;
    std::cout<<std::setprecision(11);
	double error = 10e-10;
	std::vector<std::pair<double,double> > result;
	result = NewTon(0,iterationNewton,f,error);
	display(result);
	result = NewTon(1,iterationNewton,f,error);
	display(result);
	result = Secant(0,0.1,iterationSecant,f,error);
	display(result);
	result = Secant(0.5,1,iterationSecant,f,error);
	display(result);
	return 0;
}
