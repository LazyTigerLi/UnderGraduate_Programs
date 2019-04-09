#include <iostream>
#include <string>
using namespace std;

enum direction{Left,Upper,Upper_left};
struct result
{
    direction **b;
    int **c;
};

result *LCS_length(string X,string Y)
{
    int m = X.length();
    int n = Y.length();
    result *res = new result;
    res->b = new direction *[m + 1];
    for(int i = 0; i < m + 1; i++)
        res->b[i] = new direction [n + 1];
    res->c = new int *[m + 1];
    for(int i = 0; i < m + 1; i++)
        res->c[i] = new int [n + 1];

    for(int i = 1; i <= m; i++)res->c[i][0] = 0;
    for(int j = 0; j <= n; j++)res->c[0][j] = 0;

    for(int i = 1; i <= m; i++)
        for(int j = 1; j <= n; j++)
        {
            if(X[i - 1] == Y[j - 1])
            {
                res->c[i][j] = res->c[i - 1][j - 1] + 1;
                res->b[i][j] = Upper_left;
            }
            else if(res->c[i - 1][j] >= res->c[i][j - 1])
            {
                res->c[i][j] = res->c[i - 1][j];
                res->b[i][j] = Upper;
            }
            else
            {
                res->c[i][j] = res->c[i][j - 1];
                res->b[i][j] = Left;
            }
        }
    return res;
}

void print_LCS(direction **b,string X,int i,int j)
{
    if(i == 0 || j == 0)return;
    if(b[i][j] == Upper_left)
    {
        print_LCS(b,X,i - 1,j - 1);
        cout<<X[i - 1];
    }
    else if(b[i][j] == Upper)
        print_LCS(b,X,i - 1,j);
    else print_LCS(b,X,i,j - 1);
}

int main()
{
    string X,Y;
    cin>>X>>Y;
    result *res = LCS_length(X,Y);
    print_LCS(res->b,X,X.length(),Y.length());
    delete res;
    return 0;
}