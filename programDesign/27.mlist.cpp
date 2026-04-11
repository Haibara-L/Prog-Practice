#include<iostream>
#include<sstream>
#include<list>
#include<map>
#include<string>
using namespace std;

int main()
{
    int n;
    cin>>n;
    list<int> mp[10001];
    for(int i=0;i<n;i++)
    {
        string cmd;
        cin>>cmd;
        int arg1,arg2;
        if(cmd=="new")
        {
            cin>>arg1;
        }
        else if(cmd=="add")
        {
            cin>>arg1>>arg2;
            mp[arg1].push_back(arg2);
        }
        else if(cmd=="merge")
        {
            cin>>arg1>>arg2;
            if(arg1!=arg2)
            {
                mp[arg1].sort();
                mp[arg2].sort();
                mp[arg1].merge(mp[arg2]);
            }
        }
        else if(cmd=="unique")
        {
            cin>>arg1;
            mp[arg1].sort();
            mp[arg1].unique();
        }
        else if(cmd=="out")
        {
            cin>>arg1;
            mp[arg1].sort();
            for(auto it=mp[arg1].begin();it!=mp[arg1].end();it++)
            {
                if(it==mp[arg1].begin())
                {
                    cout<<*it;
                }
                else
                {
                    cout<<' '<<*it;
                }
            }
            cout<<endl;
        }
    }
    return 0;
}