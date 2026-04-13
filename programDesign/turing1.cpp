#include<iostream>
using namespace std;
int main()
{
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        int bene,harm;
        cin>>harm>>bene;
        int hour=0;
        while(1)
        {
            hour++;
            harm-=bene;
            if(harm<=0)
            {
                break;
            } 
            harm*=2;
            bene=bene*1.05;
            if(harm>1000000)
            {
                harm=1000000;
            }
            
        }
        cout<<hour<<endl;
    }
    return  0;
}