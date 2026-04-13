#include<iostream>
#include<string>
using namespace std;
#include<vector>
#include<map>
int main()
{
    int n;
    string s;
    cin>>n>>s;
    
    vector<pair<string,int>> mp;
    map<string,int> hasemerge;
    int num=0;
    for(int i=0;i+n<=s.size();i++)
    {
        string s1=s.substr(i,n);
        if(hasemerge.count(s1)==0)
        {
            mp.push_back(make_pair(s1,1));
            num++;
            hasemerge[s1]=num;
        }
        else
        {
            int id=hasemerge[s1];
            mp[id-1].second++;
        }
    }
    int maxtime=1;
    vector<string> maxstring;
    for(auto it=mp.begin();it!=mp.end();it++)
    {
        if(it->second>maxtime)
        {
            maxtime=it->second;
            maxstring.clear();
            maxstring.push_back(it->first);
        }
        else if(it->second==maxtime)
        {
            maxstring.push_back(it->first);
        }
    }
    if(maxtime==1)
    {
        cout<<"NO"<<endl;
    }
    else
    {
        cout<<maxtime<<endl;
        for(int i=0;i<maxstring.size();i++)
        {
            cout<<maxstring[i]<<endl;
        }
    }
    return 0;

}