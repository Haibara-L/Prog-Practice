#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main()
{
    int n;
    while(1)
    {
        cin>>n;
        if(n==0)
        {
            break;
        }
        vector<int> C(n);
        vector<int> S(n);
        for(int i=0;i<n;i++)
        {
            cin>>C[i];
        }
        for(int i=0;i<n;i++)
        {
            cin>>S[i];
        }
        
        sort(C.begin(),C.end());
        sort(S.begin(),S.end());
        int maxpsr=S.size()-1;
        int maxpsl=0;
        int maxpcr=C.size()-1;
        int maxpcl=0;
        int minpsl=0;
        int minpsr=S.size()-1;
        int minpcr=C.size()-1;
        int minpcl=0;
        int maxs=0;
        int maxc=0;
        for(int i=0;i<n;i++)
        {
            if(S[maxpsr]>C[maxpcr])
            {
                maxs+=3;
                maxpsr--;
                maxpcr--;
            }
            else if(S[maxpsl]>C[maxpcl])
            {
                maxs+=3;
                maxpsl++;
                maxpcl++;
            }
            else
            {
                if(S[maxpsl]==C[maxpcr])
                {
                    maxs+=2;
                }
                else
                {
                    maxs+=1;
                }
                maxpcr--;
                maxpsl++;
            }
            
            if(C[minpcr]>S[minpsr])
            {
                maxc+=3;
                minpsr--;
                minpcr--;
            }
            else if(S[minpsl]<C[minpcl])
            {
                maxc+=3;
                minpsl++;
                minpcl++;
            }
            else
            {
                if(C[minpcl]==S[minpsr])
                {
                    maxc+=2;
                }
                else
                {
                    maxc+=1;
                }
                minpsr--;
                minpcl++;
            }
        }
        cout<<maxs<<' '<<4*n-maxc<<endl;
    }
    return 0;
}