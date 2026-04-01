#include <iostream>
using namespace std;
class MyCin
{
// 在此处补充你的代码
public:
bool istop=false;
MyCin& operator>>(int& n)
{
    cin>>n;
    if(n==-1)
    {
        istop=true;
    }
    return *this;
}
operator bool()
{
    return !istop;
}
};
int main()
{
    MyCin m;
    int n1,n2;
    while( m >> n1 >> n2) 
        cout  << n1 << " " << n2 << endl;
    return 0;
}