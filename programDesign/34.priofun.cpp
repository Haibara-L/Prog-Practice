#include <iostream>
using namespace std;
// 在此处补充你的代码
#include<functional>
template<class T1,class T2>
auto  f(T1 arg1)->function<T2(T2)>
{
    return [=](T2 arg2) {return arg2+arg1;};
}
int main()
{
   cout << f<int,int>(7)(9) << endl;   //16
   cout << f<string,string> (" hello!")("world")  <<endl; // world hello!
   cout << f<char,string> ('!')("world") << endl;
   
   
   return 0;    //world!
}