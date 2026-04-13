#include<iostream>
#include<string>
using namespace std;
#include<vector>
#include<algorithm>
int main()
{
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        string s,p;
        cin>>s>>p;
        int len1=s.size();
        int len2=p.size();
        vector<vector<int> >dp(len2+1,vector<int>(len1+1,0));
        dp[0][0]=1;
        for(int i=1;i<=len2;i++)
        {
            for(int j=0;j<=len1;j++)
            {
                if(j==0)
                {
                    if(p[i-1]>='a'&&p[i-1]<='z')
                    {
                        dp[i][j]=false;
                    }
                    else if(p[i-1]=='?')
                    {
                        dp[i][j]=false;
                    }
                    else
                    {
                        dp[i][j]=dp[i-1][j];
                    }
                    continue;
                }
                if(p[i-1]>='a'&&p[i-1]<='z')
                {
                    if(p[i-1]==s[j-1])
                    {
                        dp[i][j]=dp[i-1][j-1];
                    }
                    else
                    {
                        dp[i][j]=false;
                    }
                }
                else if(p[i-1]=='?')
                {
                    dp[i][j]=dp[i-1][j-1];
                }
                else if(p[i-1]=='*')
                {
                    dp[i][j]=dp[i-1][j]||dp[i][j-1];
                }
            }
        }
        cout<<(dp[len2][len1]==1?"yes":"no")<<endl;
    }
    return 0;
}