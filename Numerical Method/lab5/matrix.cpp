#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

bool stop(double *X1,double *X2,int n,double error)
{
    for(int i = 0; i < n; i++)
        if(fabs(X1[i] - X2[i]) >= error)return false;
    return true;
}

std::pair<std::vector<double>,int> Jacobi(double **A,double *b,double *intialization,int n,double error)
{
    double R[n][n],g[n];
    for(int i = 0; i < n; i++)
    {
        g[i] = b[i] / A[i][i];
        for(int j = 0; j < n; j++)
            if(i == j)R[i][j] = 0;
            else R[i][j] = -A[i][j] / A[i][i];
    }

    double X1[n],X2[n];
    int steps = 0;
    for(int i = 0; i < n; i++)X1[i] = intialization[i];
    while(true)
    {
        //X2 = R * X1 + g
        steps++;
        for(int i = 0; i < n; i++)
        {
            X2[i] = 0;
            for(int j = 0; j < n; j++)
                X2[i] += R[i][j] * X1[j];
            X2[i] += g[i];
        }
        if(stop(X1,X2,9,error))break;
        //X1 = X2
        for(int i = 0; i < n; i++)X1[i] = X2[i];
    }
    std::vector<double> result(X2,X2 + n);
    return std::make_pair(result,steps);
}

std::pair<std::vector<double>,int> Gauss_Seidel(double **A,double *b,double *intialization,int n,double error)
{
    double R[n][n],g[n];
    for(int i = 0; i < n; i++)
    {
        g[i] = b[i] / A[i][i];
        for(int j = 0; j < n; j++)
            if(i == j)R[i][j] = 0;
            else R[i][j] = -A[i][j] / A[i][i];
    }

    double X1[n],X2[n];  
    int steps = 0; 
    for(int i = 0; i < n; i++)X1[i] = intialization[i];
    while(true)
    {
        steps++;
        //X2 = R * X1 + g
        for(int i = 0; i < n; i++)
        {
            X2[i] = 0;
            for(int j = 0; j < i; j++)
                X2[i] += R[i][j] * X2[j];
            for(int j = i; j < n; j++)
                X2[i] += R[i][j] * X1[j];
            X2[i] += g[i];
        }
        if(stop(X1,X2,9,error))break;
        //X1 = X2
        for(int i = 0; i < n; i++)X1[i] = X2[i];
    }
    std::vector<double> result(X2,X2 + n);
    return std::make_pair(result,steps);
}

int main()
{
    std::cout<<std::fixed;
    std::cout<<std::setprecision(6);
    double temp[9][9] = {{31,-13,0,0,0,-10,0,0,0},
                        {-13,35,-9,0,-11,0,0,0,0},
                        {0,-9,31,-10,0,0,0,0,0},
                        {0,0,-10,79,-30,0,0,0,-9},
                        {0,0,0,-30,57,-7,0,-5,0},
                        {0,0,0,0,-7,47,-30,0,0},
                        {0,0,0,0,0,-30,41,0,0},
                        {0,0,0,0,-5,0,0,27,-2},
                        {0,0,0,-9,0,0,0,-2,29}};
    double *A[9];
    for(int i = 0; i < 9; i++)A[i] = temp[i];
    double b[9] = {-15,27,-23,0,-20,12,-7,7,10};
    double error = 1e-5;
    double initialization[9] = {0,0,0,0,0,0,0,0,0};
    
    std::pair<std::vector<double>,int> result = Jacobi(A,b,initialization,9,error);
    std::cout<<result.second<<std::endl;
    for(int i = 0; i < result.first.size(); i++)
        std::cout<<result.first[i]<<'\t';
    std::cout<<std::endl;
    
    result = Gauss_Seidel(A,b,initialization,9,error);
    std::cout<<result.second<<std::endl;
    for(int i = 0; i < result.first.size(); i++)
        std::cout<<result.first[i]<<'\t';
    std::cout<<std::endl;
    return 0; 
}