#include<iostream>
using namespace std;
#include<string>
#include<vector>
string get_string();
int n;
vector<string> v;
bool isNumber(string s) 
{
    if (s.empty() || s.length() > 5) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    // 转化为整数判断范围
    int val = stoi(s);
    return val >= 0 && val <= 99999;
}
int get_int()
{
    string s;
    cin>>s;
    if(s=="find")
    {
        string ms=get_string();
        int n=get_int();
        int pos=v[n-1].find(ms);
        if(pos==string::npos)
        {
            return v[n-1].size();
        }
        else
        {
            return pos;
        }
    }
    else if(s=="rfind")
    {
        string ms=get_string();
        int n=get_int();
        int pos=v[n-1].rfind(ms);
        if(pos==string::npos)
        {
            return v[n-1].size();
        }
        else
        {
            return pos;
        }
    }
    else
    {
        return stoi(s);
    }
}
string get_string()
{
    string s;
    cin>>s;
    if(s=="copy")
    {
        int n=get_int();
        int x=get_int();
        int l=get_int();
        return v[n-1].substr(x,l);
    }
    else if(s=="add")
    {
        string s1=get_string();
        string s2=get_string();
        if(isNumber(s1)&&isNumber(s2))
        {
            int n1=stoi(s1);
            int n2=stoi(s2);
            return to_string(n1+n2);
        }
        else
        {
            return s1+s2;
        }
    }
    else
    {
        return s;
    }
}
int main()
{
    cin>>n;
    v.resize(n);
    for(int i=0;i<n;i++)
    {
        cin>>v[i];
    }
    string cmd;
    while(1)
    {
        cin>>cmd;
        if(cmd=="over")
        {
            break;
        }
        else if(cmd=="insert")
        {
            string s=get_string();
            int num=get_int();
            int x=get_int();
            v[num-1].insert(x,s);
        }
        else if(cmd=="reset")
        {
            string s=get_string();
            int num=get_int();
            v[num-1]=s;
        }
        else if(cmd=="print")
        {
            int num=get_int();
            cout<<v[num-1]<<endl;
        }
        else if(cmd=="printall")
        {
            for(int i=0;i<n;i++)
            {
                cout<<v[i]<<endl;
            }
        }
        
    }
    return 0;

}