#include<iostream>
using namespace std;
#include<set>
#include<algorithm>

int main()
{
    multiset<int> s;
    set<int> hiss;
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        string cmd;
        int n;
        cin>>cmd>>n;
        if(cmd=="add")
        {
            s.insert(n);
            hiss.insert(n);
            cout<<s.count(n)<<endl;
        }
        else if(cmd=="ask")
        {
            int num=s.count(n);
            cout<<(hiss.count(n)==0?0:1)<<' '<<num<<endl;
        }
        else
        {
            cout<<s.count(n)<<endl;
            s.erase(n);
        }
    }

}