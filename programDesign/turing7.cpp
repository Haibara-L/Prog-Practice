#include<iostream>
using namespace std;
#include<string>
#include<stack>
int main()
{
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        string s;
        int k;
        cin>>s>>k;
        stack<char> st;
        st.push('0');
        for(int j=0;j<s.size();j++)
        {
            while(k>0&&st.top()>s[j])
            {
                st.pop();
                k--;
            }
            st.push(s[j]);
        }
        while(k>0)
        {
            st.pop();
            k--;
        }
        string result="";
        while(st.size()>1)
        {
            result.insert(result.begin(),st.top());
            st.pop();
        }
        cout<<result<<endl;

    }
    return 0;

}