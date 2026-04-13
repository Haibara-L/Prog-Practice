#include<iostream>
using namespace std;
#include<vector>
#include<map>
#include<algorithm>
struct line
{
    long long A;
    long long B;
    long long C;
};
bool compare(line l1,line l2)
{
    return l1.A*l2.C<l1.C*l2.A;
}
int main()
{
    long long cardnum,toynum,reclx,recly,recrx,recry;
    long long num=0;
    while(1)
    {
        cin>>cardnum;
        if(cardnum==0)
        {
            break;
        }
        cin>>toynum>>reclx>>recly>>recrx>>recry;
        if(num!=0)
        {
            cout<<endl;
        }
        num++;
        vector<line> lines;
        lines.push_back(line{1,0,-reclx});
        for(long long i=0;i<cardnum;i++)
        {
            long long x1,x2;
            cin>>x1>>x2;
            lines.push_back(line{recly-recry,x2-x1,recry*x1-recly*x2});
        }
        lines.push_back(line{1,0,-recrx});
        sort(lines.begin(),lines.end(),compare);
        vector<long long> boxes(cardnum+1,0);
        for(long long i=0;i<toynum;i++)
        {
            long long x,y;
            cin>>x>>y;
            long long leftptr=0;
            long long rightptr=cardnum;
            while(leftptr<=rightptr)
            {
                long long mid=(leftptr+rightptr)/2;
                line l1=lines[mid];
                line l2=lines[mid+1];
                if(l1.A*x+l1.B*y+l1.C<0)
                {
                    rightptr=mid-1;
                }
                else if(l2.A*x+l2.B*y+l2.C>0)
                {
                    leftptr=mid+1;
                }
                else
                {
                    boxes[mid]++;
                    break;
                }
            }
        }
        for(long long i=0;i<=cardnum;i++)
        {
            cout<<i<<": "<<boxes[i]<<endl;
        }
    }
    return 0;
}