#### 运算符重载
运算符重载 
- 对已有的运算符(C++中预定义的运算符)赋予多重的含义 使同一运算符作用于不同类型的数据的行为时导致不同类型的行为
- 目的：扩展C++中提供的运算符的适用范围,以用于类所表示的抽象数据类型 
- 同一个运算符,对不同类型的操作数,所发生的行为不同
==运算符重载的本质是函数重载==
基本语法：
```
返回值类型 operator 运算符(形参列表)
{

}
```
程序编译时： 
- 把 运算符的表达式转化为运算符函数的调用 
- 把 运算符的操作数转化为运算符函数的参数 
- 运算符被多次重载时, 根据 实参的类型 决定调用哪个运算符函数 
- 运算符可以被重载成普通函数，也可以被重载成类的成员函数
###### 重载为普通函数
此时应当有两个参数，分别为参加运算的两个数据变量，例如
```
Complex operator+(const Complex & a, const Complex & b)
{ return Complex(a.real+b.real, a.imag+b.imag); //返回一个临时对象 }
```
此时，a+b 相当于 `operator+(a,b)`
###### 重载为类成员函数
此时只有一个参数，相当于运算符前面的变量调用这个函数，将运算符后面的变量作为参数传给这个函数进行运算
```
class Complex 
{ public: 
double real, imag; 
Complex( double r = 0.0, double i= 0.0 ):real(r), imag(i) { } 
Complex operator-(const Complex & c);
 };
```
此时a-b相当于 `a.operator-(b)`,此时调用这个运算符的对象就相当于重载为普通函数时的一个参数
#### 赋值运算符重载
- 赋值运算符 两边的类型 可以不匹配 
- 把一个 int 类型变量 赋值给一个 Complex 对象 
- 把一个 char * 类型的字符串 赋值给一个字符串对象 
- 需要 重载赋值运算符 '=' 
- 赋值运算符 '=' 只能重载为 成员函数

涉及指针时的赋值应当注意浅拷贝与深拷贝
###### 浅拷贝：
只进行地址的拷贝，即将一个指针指向的地址赋值给另一个指针，两个指针指向同一个内存区域
```
string S1, S2; 
S1 = "this"; 
S2 = "that"; 
S1 = S2;
```
问题：
1. 二者指向同一片内存区域
2. 如果s1消亡，析构函数将释放二者共同指向的空间，当s2消亡时再释放一次空间会出错
3. 如果将s1赋予其他的值，会首先释放s1指向的内存，然后重新分配内存空间，导致s2指针悬空
###### 深拷贝
进行值的拷贝，将一个对象中指针变量指向的内容复制到另一个对象中指针成员指向的地方
```
String & operator = (const String & s)
{ if( this == &s ) return * this; 
delete [] str; 
str = new char[strlen(s.str)+1]; 
strcpy( str, s.str); 
return * this; 
}
```
注意第一行的目的是防止出现s=s的语句导致s被删除，后续无法使用
==在编写复制构造函数时也会出现深拷贝和浅拷贝的问题，也要进行深拷贝==

注意 `string s=s1` 调用的是拷贝构造而不是赋值语句
#### 运算符重载为友元
- 一般情况下，推荐讲运算符重载为类的成员函数
- 重载问成员函数不可以满足要求时，重载为普通函数
- 为了使普通函数可以调用类的私有属性，需要将运算符重载为友元

例如，我们想重载复数类的加法，如果重载为成员函数，只能处理 c+1，相当于c.operator+(1)
但是遇到1+c,就会出问题了，此时就要将+重载为普通函数
#### 流运算符重载
###### 流插入运算符
cout 是在 iostream 中定义的, ostream 类的对象 "<<" 能用在 cout 上 因为在iostream里对 "<<" 进行了重载，实际上他重载了一些内置数据类型，但是对于自定义数据类型，需要我们自己重载
流运算符的特点是可以进行链式调用，因此必须返回一个指向std::ostream对象的引用
==注意<<只能重载为全局函数而不是重载为ostream的成员函数，因为iostream是标准库，不可以进行修改，无法添加处理自定义数据类型的方法，也不能重载为自定义数据类型的成员函数，因为如果这样就变成 "自定义数据类型>>cout"了，就不可以进行链式调用==
>流对象不可以进行拷贝构造，因此每次返回的都是引用而不是一个临时对象，这样在链式调用中每次调用的都是同一个流对象，并且如果重载流对象必须传入流对象的引用

```
ostream & operator<<( ostream & o, int n )
{ Output(n); 
return o;
}
```
两种重载方式
1. 声明和函数体写在类外，此时是全局函数，如果要访问类的私有属性要声明友员，否则不需要
2. 声明和函数体写在类内，此时必须在前面加上友元声明，此时他表示这是一个全局函数，只是写在类内了，例如
```
class Point {
private:
	int x;
	int y;
public:
	Point() {};
	friend istream& operator>>(istream& i,Point& p)
	{
		i >> p.x >> p.y;
		return i;
	}
	friend ostream& operator<<(ostream& o, Point& p)
	{
		o << p.x <<',' << p.y;
		return o;
	}
};
int main()
{
	Point p;
	while (cin >> p) {
		cout << p << endl;
	}
	return 0;
}
```
###### 流提取运算符
```
istream & operator<<( istream & i, int n )
{ Output(n); 
return i;
}
```
cin和cout是类似的，模仿使用即可
atof库函数可以把const char\*指向的内容转换成float
#### 类型转换运算符重载
类型转换运算符重载允许**把一个类的对象自动转换成其他类型**（比如转成 `int`、`double`，甚至另一个类）
语法格式：
```
operator 目标类型() const {
    // 返回目标类型的值
    return 转换结果;
}
```
特点：
- 没有返回类型（但实际要返回目标类型的值）
- 没有参数
- 通常加 `const`（转换不应该修改对象）
- 可以隐式调用，也可以显式调用
例子：
```
class MyInt {
    int nVal;
public:
    MyInt(int n) : nVal(n) { }
    
    // 转换成 int
    operator int() const {
        return nVal;
    }
};

int main() {
    MyInt obj(42);
    
    int a = obj;           // 隐式转换：obj 自动变成 42
    int b = int(obj);      // 显式转换：C++风格
    int c = (int)obj;      // 显式转换：C风格
    
    cout << a << b << c;   // 都输出 42
}
```
#### 数组运算符重载
当创建了一个自定义数据类型的数组，希望通过\[\]返回对象的某一个成员变量而不是成员本身，就需要重载\[\]运算符，返回自己需要返回的变量的引用，返回引用的目的是可以对他进行修改，从而实现赋值等功能
例如：
```
class Array{ 
public: 
Array(int n = 10) : size(n)
{ ptr = new int[n]; } 
~Array() 
{ delete [] ptr; } 
int & operator[](int subscript)
{ return ptr[subscript]; } 
private: 
int size;
 int *ptr; 
 }
```
如果我们还想实现变量之间的复制，可以重载赋值号

```
const Array & operator=( constArray & a)
 { 
 if( ptr == a.ptr ) return * this;
  delete [] ptr; 
  ptr = new int[ a.size ]; 
  memcpy( ptr, a.ptr, sizeof(int ) * a.size); 
  size = a.size; return * this; } 
  //返回const array & 类型是为了高效实现 //a = b = c; 形式，即进行链式调用
  }
```
补充：memcpy是内存拷贝函数,要 \#include\<memory\> 它将从a.ptr起的sizeof(int) * a.size 个字节拷贝到地址 ptr
此外，为了实现复制构造函数的深拷贝，还要编写复制构造函数
```
Array(Array & a)
 { ptr = new int[ a.size ]; 
 memcpy( ptr, a.ptr, sizeof(int) * a.size); 
 size = a.size;
  } 完成形如Array b(a); 方式的初始化
```
==凡是涉及重载赋值运算符和复制构造函数，即将一个成员赋给另一个成员，都要考虑进行重载，写成深拷贝，否则无法完成数组元素空间的分配==
#### 自增自减运算符重载
有前置和后置的区分，后置先执行再自增，前置先自增再执行
为了区分二者，C++规定
- 前置运算符 作为一元 运算符重载
```
T &operator++() //成员函数 
T &operator--() 
T &operator++( T & ) //全局函数 
T &operator--( T & ) 
//++obj, obj.operator++() 或者operator++(obj) 都调用上述函数
```
- 后置运算符作为二元运算符重载，多写一个int参数，只要写一个int即可，不用写参数名
```
 T operator++(int) //成员函数 
 T operator--(int) 
 T operator++( T &, int ) //全局函数 
 T operator--( T &, int ) 
 //obj++, obj.operator++(0)或者operator++(obj, 0) 都调用上函数
```
具体方法
```
CDemo & CDemo::operator++() { //前置 ++
 n ++; 
 return * this; 
 } // ++s即为: s.operator++(); 

CDemo CDemo::operator++( int k ) { //后置 ++ 
CDemo tmp(*this); //记录修改前的对象
 n ++; 
 return tmp; //返回修改前的对象
 } // s++即为: s.operator++(0); 

CDemo & operator--(CDemo & d) {//前置-- 
d.n--; 
return d; 
} //--s即为: operator--(s); 

CDemo operator--(CDemo & d,int) {//后置-- 
CDemo tmp(d);
 d.n --;
 return tmp; 
 } //s--即为: operator--(s, 0);
```
#### 运算符重载的注意事项
- C++不允许定义新的运算符
- 重载后运算符的含义应当符合日常习惯，又实际意义
- 运算符重载不改变运算符优先级
- .   ::   ?:  sizeof运算符不可以被重载
- 重载运算符(),\[\],->或者赋值运算符=时,运算符重载 函数必须声明为类的成员函数
- 重载运算符是为了让它能作用于对象,因此重载运算符不允许操作数都不是对象，至少要有一个对象
- 有一个操作数是枚举类型也可以，枚举类型（enum）是用户定义的类型 ，但它本质上是一个整数类型