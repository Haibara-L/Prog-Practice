#include<iostream>
using namespace std;
#include<vector>
#include<stack>
int main()
{
    int m,n;
    cin>>m>>n;
    vector<vector<int> > v(m,vector<int>(n));
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            cin>>v[i][j];
        }
    }
    vector<int> row(n,0);
    int maxs=0;
    for(int i=0;i<m;i++)
    {
        if(i==0)
        {
            for(int j=0;j<n;j++)
            {
                row[j]=1-v[0][j];
                row.push_back(0);
            }
        }
        else
        {
            for(int j=0;j<n;j++)
            {
                row[j]=(v[i][j]==0?row[j]+1:0);
            }
        }
        stack<pair<int,int>> st;
        st.push(make_pair(-1,0));
        for(int j=0;j<=n;j++)
        {
            while(!st.empty()&&st.top().second>=row[j])
            {
                int h=st.top().second;
                st.pop();
                if(st.empty())
                {
                    maxs=max(maxs,h*j);
                }
                else
                {
                    int id1=st.top().first;
                    maxs=max(maxs,h*(j-id1-1));
                }
            }
            st.push(make_pair(j,row[j]));
            
        }

    }
    cout<<maxs<<endl;
    return 0;
    
}