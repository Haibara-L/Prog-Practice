#include <iostream>
using namespace std;
// 在此处补充你的代码
template<class T1,class T2,class T3>
class combine
{
    public:
    T1 fun1;
    T2 fun2;
    T3 arg;
    combine(T1 arg1,T2 arg2):fun1(arg1),fun2(arg2)
    {
    }
    T3 operator()(T3 n)
    {
        return fun1(fun1(n)+fun2(n));
    }
};
int main()
{
    auto Square = [] (double a) { return a * a; };
    auto Inc = [] (double a) { return a + 1; };
    cout << combine<decltype(Square),decltype(Inc),int>(Square,Inc)(3) << endl;
    cout << combine<decltype(Inc),decltype(Square),double>(Inc,Square)(2.5) << endl;

    return 0;
}