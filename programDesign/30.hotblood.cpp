#include<iostream>
using namespace std;
#include<map>
#include<cmath>
int main()
{
    map<int,int> mp;
    mp[1000000000]=1;
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        int id,power;
        cin>>id>>power;
        auto it=mp.lower_bound(power);
        auto it1=it;
        it1--;
        if(it==mp.end())
        {
            it--;
            cout<<id<<' '<<it->second<<endl;
            mp[power]=id;
            continue;
        }
        if(it==mp.begin())
        {
            cout<<id<<' '<<it->second<<endl;
            mp[power]=id;
            continue;

        }
        int dis1=abs(it1->first-power);
        int dis2=abs(it->first-power);
        if(dis1<=dis2)
        {
            cout<<id<<' '<<it1->second<<endl;
        }
        else
        {
            cout<<id<<' '<<it->second<<endl;
        }
        mp[power]=id;
    }
    return 0;
}
