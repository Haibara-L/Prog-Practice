#include<iostream>
using namespace std;
#include<vector>
#include<sstream>
#include<algorithm>
#include<map>
int main()
{
    string s;
    while(1)
    {
        map<int,int> mp;
        int num=0;
        getline(cin,s);
        if(s=="0")
        {
            break;
        }
        stringstream ss(s);
        vector<int> v;
        int n;
        while(ss>>n)
        {
            mp[n]++;
            num++;
        }
        if(num%3!=2)
        {
            cout<<"XIANGGONG"<<endl;
            continue;
        }
        bool found=false;
        for(auto it=mp.begin();it!=mp.end();it++)
        {
            if(it->second<2)
            {
                continue;
            }
            map<int,int> tempmp=mp;
            tempmp[it->first]-=2;
            bool flag=true;
            for(auto it1=tempmp.begin();it1!=tempmp.end();it1++)
            {
                if(it1->second>=3)
                {
                    it1->second-=3;
                }
                while(it1->second>0)
                {
                    auto it2=it1;
                    it2++;
                    if(it2==tempmp.end())
                    {
                        flag=false;
                        break;
                    }
                    auto it3=it2;
                    it3++;
                    if(it3==tempmp.end())
                    {
                        flag=false;
                        break;
                    }
                    if(it2->first!=it1->first+1||it3->first!=it2->first+1)
                    {
                        flag=false;
                        break;
                    }
                    if(it2->second<it1->second||it3->second<it1->second)
                    {
                        flag=false;
                        break;
                    }
                    it2->second-=it1->second;
                    it3->second-=it1->second;
                    it1->second=0;
                }
                if(!flag)
                {
                    break;
                }
            }
            if(flag)
            {
                cout<<"HU"<<endl;
                found=true;
            }
        }
        if(!found)
        {
            cout<<"BUHU"<<endl;
        }
    }
    return 0;
}