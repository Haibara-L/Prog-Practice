##### 统一的初始化方法
统一使用{ }进行初始化
##### 成员变量初始值
##### auto关键字
编译器自动通过赋初值的类型对变量确定类型
注意还是在编译时确定类型，因此如果一定要使用auto，必须赋初值

函数返回值也可以使用auto，让编译器在编译时自动推断返回值是什么
注意要额外声明，让编译器自动推断
```
auto add(T1 x, T2 y) -> decltype(x + y) 
{ return x+y; }
```
##### decltype关键字
让编译器自动推断编译器的类型
```
int i; 
double t; 
struct A { double x; }; 
const A* a = new A(); 

decltype(a) x1; // x1 is A * 
decltype(i) x2; // x2 is int 
decltype(a->x) x3; // x3 is double 
decltype((a->x)) x4 = t; // x4 is double&
```
##### 智能指针
头文件： \<memory\> 
通过shared_ptr的构造函数，可以让shared_ptr对象托管一个new运算符返 回的指针，
 shared_ptr\<T\> ptr(new T); // T 可以是 int ,char, 类名等各种类型 
 此后ptr就可以像 T* 类型的指针一样来使用，即 \*ptr 就是用new动态分配的那 个对象，而且不必操心释放内存的事,系统会自动delete这个指针。  
 多个shared_ptr对象可以同时托管一个指针，系统会维护一个托管计数。当无shared_ptr托管该指针时，delete该指针。 
 shared_ptr对象不能托管指向动态分配的数组的指针，否则程序运行会出错

智能指针解除托管
```
spr.reset() //放弃托管
spr.reset(ptr2) //更换托管对象
```
获取智能指针托管对象
```
ptr=spr.get() //获取托管的指针
```
判断智能指针是否托管了对象
```
if(spr)
{
}
```

shared_ptr的问题：





unique_ptr

| 特性       | unique_ptr               | shared_ptr                              |
|------------|--------------------------|------------------------------------------|
| 所有权     | 独占                     | 共享                                     |
| 拷贝       | 不允许拷贝               | 允许拷贝，计数 + 1                        |
| 转移       | 使用 std::move           | 直接拷贝即可                              |
| 内存开销   | 极小                     | 额外存引用计数                            |
| 性能       | 更快                     | 稍慢（计数原子操作）                      |
| 释放时机   | 自身销毁时立即释放        | 计数为 0 时释放                           |
| 循环引用   | 无问题                   | 会内存泄漏，需搭配 weak_ptr               |

##### 基于范围的for循环


```
for(int e:arr)
{
}
//取到每一个元素的拷贝，不会修改原来的数组
```
```
for(int& e:arr)
{
}
//取到每一个元素的引用，可以修改原来的数组
```
##### 右值引用
右值：一般来说，不能取地址的表达式，如临时变量，就是右值， 能取地址的，就是左值
```
class A { }; 
A & r = A(); // error , A()是无名变量，是右值 
A && r = A(); //ok, r 是右值引用
```
主要目的是提高程序运行的效率，减少需要进行深拷贝的对象进行深拷贝 的次数
右值引用本身不会自动避免深拷贝，但通过**移动语义**和**完美转发**，可以避免不必要的深拷贝。

核心机制：移动语义
右值引用（`T&&`）标识的是**即将销毁的临时对象**，可以"窃取"其资源，而不是复制。
1. 实现移动构造函数和移动赋值运算符

```cpp
class MyString {
    char* data;
    size_t size;
public:
    // 移动构造函数 - 窃取资源，避免深拷贝
    MyString(MyString&& other) noexcept 
        : data(other.data), size(other.size) {
        other.data = nullptr;  // 将源对象置空
        other.size = 0;
    }
    
    // 拷贝构造函数 - 会深拷贝
    MyString(const MyString& other) {
        data = new char[other.size];
        size = other.size;
        memcpy(data, other.data, size);
    }
};
```

2. 使用 `std::move` 触发移动语义

```cpp
std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2 = std::move(v1);  // 移动，O(1)，无深拷贝

// v1 现在处于有效但未指定的状态
```
3. 函数返回值自动使用移动

```cpp
std::vector<int> createVector() {
    std::vector<int> v = {1, 2, 3};
    return v;  // 自动移动（C++11起），无深拷贝
}

std::vector<int> result = createVector();  // 高效
```

实际应用对比

```cpp
// ❌ 深拷贝 - 性能差
void process(std::vector<int> v) { }  // 传值会拷贝
std::vector<int> data(1000000);
process(data);  // 深拷贝 1M 个元素

// ✅ 移动 - 高效
void process(std::vector<int> v) { }
std::vector<int> data(1000000);
process(std::move(data));  // 移动，O(1)，不拷贝

// ✅ 引用 - 避免拷贝
void process(const std::vector<int>& v) { }  // 只读场景
process(data);  // 不拷贝
```

关键点总结

| 场景 | 避免深拷贝的方法 |
|------|------------------|
| 函数参数（只读） | 用 `const T&` |
| 函数参数（需要所有权） | 用 `T` + `std::move` |
| 返回值 | 直接返回，编译器自动优化 |
| 容器元素转移 | 用 `std::move` 包裹 |
| 自定义类 | 实现移动构造/移动赋值 |
常见误区

```cpp
// ❌ 错误：右值引用本身不会避免拷贝
void func(std::vector<int>&& vec) {
    std::vector<int> local = vec;  // 仍然是拷贝！
}

// ✅ 正确：需要显式 move
void func(std::vector<int>&& vec) {
    std::vector<int> local = std::move(vec);  // 移动，无拷贝
}
```

**核心原则**：右值引用只是一个类型修饰符，真正避免深拷贝的是**移动语义的实现**和**显式的 `std::move`**。
函数返回值为对象时，返回值对象如何初始化？ 
- 只写复制构造函数 
	- return 局部对象 -> 复制 
	- return 全局对象 ->复制 
- 只写移动构造函数 
	- return 局部对象 -> 移动 
	- return 全局对象 ->默认复制 
	- return move(全局对向） -〉移动 
- 同时写 复制构造函数和 移动构造函数: 
	- return 局部对象 -> 移动 
	- return 全局对象 -> 复制 
	- return move(全局对向） -〉移动 
所以不必担心return 一个局部 vector对象会导致复制整个数组耗费时间 
##### 哈希表
unordered_map
哈希表插入和查询的时间复杂度几乎是常数,效率非常高
但是unordered_map内部的元素无序
##### 正则表达式
头文件regex
用于字符串匹配
##### lambda表达式
匿名函数：只使用一次的简单函数
形式
```
[外部变量访问方式说明符](参数表) ->返回值类型 { 语句组 } 
```
\[] 不使用任何外部变量 
\[=] 以传值的形式使用所有外部变量 
\[&] 以引用形式使用所有外部变量 
\[x, &y] x 以传值形式使用，y 以引用形式使用 
\[=,&x,&y] x,y 以引用形式使用，其余变量以传值形式使用 
\[&,x,y] x,y 以传值的形式使用，其余变量以引用形式使用
->返回值类型”也可以没有， 没有则编译器自动判断返回值类型

lambda表达式最经常用于在原来传一个函数指针的地方可以传一个lambda表达式，例如sort，foreach等算法
或者也可以用一个auto对象ff接受一个lambda表达式，然后通过ff()调用函数

对于复杂的lambda表达式，如果要使用需要functional头文件，此时类型不可以用auto

```
#include <functional> function<int(int)> fib = [&fib](int n) { return n <= 2 ? 1 : fib(n-1) + fib(n-2);}; 
cout << fib(5) << endl; 
//输出5 function<int(int)> 表示返回值为 int, 有一个int参数的函数
```

##### 多线程
头文件：thread
创建并立即启动线程
```
thread th(线程执行的对象)
```
### C++14改进
##### 二进制常量
```
cout << 0b1101 << endl; //>>13 
```
##### 函数返回值自动推断
C++14：可以直接用 auto 当返回值，编译器自动推类型
##### auto lambda表达式



##### make_unique
用来创建unique_ptr
```
std::unique_ptr<int> ptr = std::make_unique<int>(20); 
std::cout << *ptr << "\n"; //>>20 
```


### C++17改进
##### 类模板实参推导



##### 结构化绑定
tuple是c++11开始引入的可以存放任意多个不同类型的数据的序列 
```
auto [id, name] = make_pair(1, "Alice"); 
auto [a, b, c] = tuple{10, 3.14, "hello"}; //CTAD 
// 解构结构体 
struct Point { int x; int y; }; 
Point p{10, 20}; 
auto [x, y] = p; 
```
##### if/switch初始化语句


##### 折叠表达式

##### optional 表示可能为空的值
类似于多提供了python中的None这种类型，用来替代特殊的返回值
```
#include <optional> using namespace std; optional<int> func(int x) { // 返回值为int或空值 if(x > 0) return x; return nullopt; // 空值 } void print(int n) { auto res = func(n); if (res) cout << *res << endl; else cout << "NULL" <<endl; } int main() { print(20); //>>20 print(-2); //>>NULL return 0; }
```
##### any 安全存储任意类型的数据
