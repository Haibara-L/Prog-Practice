#include<iostream>
using namespace std;
#include<queue>
#include<map>
int getzys(int n) {
    int num = 0;
    int temp = n;
    for (int i = 2; i * i <= temp; i++) {
        if (temp % i == 0) {
            num++; // 发现一个质因数
            while (temp % i == 0) temp /= i; // 除尽它
        }
    }
    if (temp > 1&&temp<n) num++;
     // 剩下一个大于sqrt的质数
    return num; 
}
struct Node
{
    int num,ys;
    bool operator<(const Node& n) const
    {
        if(ys!=n.ys)
        {
            return ys<n.ys;
        }
        return num<n.num;
    }
    bool operator>(const Node& n) const
    {
        if(ys!=n.ys)
        {
            return ys>n.ys;
        }
        return num>n.num;
    }
    bool operator==(const Node& n) const
    {
        if(num==n.num)
        {
            return true;
        }
        return false;
    }

};

int main()
{
    priority_queue<Node> maxtop;
    priority_queue<Node,vector<Node>,greater<Node>> mintop;
    map<Node,int> mp;
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<10;j++)
        {
            int num;
            cin>>num;
            Node nd{num,getzys(num)};
            maxtop.push(nd);
            mintop.push(nd);
            mp[nd]=1;
        }
        while(1)
        {
            Node maxn=maxtop.top();
            maxtop.pop();
            if(mp[maxn]>0)
            {
                cout<<maxn.num<<' ';
                mp[maxn]--;
                break;
            }
        }
        while(1)
        {
            Node minn=mintop.top();
            mintop.pop();
            if(mp[minn]>0)
            {
                cout<<minn.num<<endl;
                mp[minn]--;
                break;
            }
        }
    }


}
