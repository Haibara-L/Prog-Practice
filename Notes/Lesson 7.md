## 函数模板与类模板
以某个个性化的东西以模式化，统一的的东西进行包装，模板的信息都是笼统的，而非具体的信息
在类或者函数的基础上更进一步抽象了一层
#### 函数模板
对函数重载的推广：因为要处理不同的数据，使用不同的参数对函数进行区分
例如：排序问题：
- 实际应用中经常用到各种排序问题，采取的排序算法基本相同
- 不同类型的对象，在排序时，除了被排序数组, 存储被排序数组元素的变量的类型声明不同之外, 其它部分可以完全相同 
两种解决方法：
1. 函数重载
2. 函数模板
###### 函数重载
- 分别写两个同名的函数
- 由编译系统根据函数调用实参的类型，确定具体执行哪个函数
- 缺点：必须提前预知好，缺乏动态兼容性质
###### 函数模板

```
tempplate<class T>
returnType funName(T arg,……)
{}
```
- T为函数模板的类型参数，只是一个占位的虚指符号，标记符
- 由编译系统根据函数调用时实参的类型, 自动生成相应的 模板函数
- 调用的方式与具体的函数一样，当具体调用函数的时候就会对具体的参数进行实例化，进行类别的指定，就自动产生了一个由具体的类型替换T产生的实际的函数
- 如果在具体的处理过程有自定义数据类型，可能要对一些运算符进行重载，比如流输出运算符
- ==注意template后面没有分号==

一个函数模板可以有多个类型参数
```
tempplate<class T1，class T2>
returnType funName(T1 arg1,T2 arg2……)
{}
```
函数模板的参数类型
- 既可以使用类型参数说明符T
- 也可以使用基本数据类型或者其他类的说明
- 在具体调用时只会实例化类型参数

函数模板的类型参数可以用于函数模板的局部变量声明，此时这个局部变量就会变成实例化之后的类型
```
template<class T1, class T2> 
void print(T1 arg1, T2 arg2) 
{ T1 locVar = arg1; 
cout << locVar << " " << arg2 << endl; 
return; 
}
```

使用单一类型参数容易引起函数模板中类型参数的二义性
```
template<class T> 
T myFunction( T arg1, T arg2) 
{ cout<<arg1<<“ ”<<arg2<<“\n”; return arg1;}
myFunction(5, 7); //ok: replace T with int 
myFunction(5.8, 8.4); //ok: replace T with double 
myFunction(5, 8.4); //error: replace T with int or double? 二义性
```
如果传入参数可能类型不同，应当使用多个类型参数，避免二义性
```
template<class T1, class T2> 
T1 myFunction( T1 arg1, T2 arg2) 
{ cout<<arg1<<“ ”<<arg2<<“\n”; return arg1;}
```
也可以显式指定实参类型来避免二义性，此时会发生隐式转换
```
template<class T> 
void fun(T a) { 
cout << "class: " << typeid(T).name() << endl; 
} 
int main() { 
int c = 10; 
fun(c); // 类型推导：T = int 
fun<int>(c); // 显式指定：T = int（多余但正确） 
fun<double>(c); // 显式指定：T = double，会发生隐式转换！ 
return 0; }
```
###### 函数模板的重载
函数模板与函数模板的重载: 同一函数名, 参数的数量不同，不可以通过模板参数类型不同进行重载，因为模板参数在调用之前都是不确定的
```
template<class T> T 
myFunction( T arg ) 
{ cout<< “one argument\n”; return arg;} 

template<class T1, class T2> 
T1 myFunction( T1 arg1, T2 arg2 ) 
{ cout<< “two arguments\n”; return arg1;}
```
函数和模板的匹配顺序
1. 先找一个参数完全匹配的函数 
2. 再找一个参数个数完全匹配的模板 
3. 在没有二义性的前提下, 再找一个参数经过自动转换 后能够匹配的函数
4. 都找不到, 则报错
例如
```
template <class T> 
void Max( T a, T b ) 
{ cout << "TemplateMax" <<endl; return 0; } 

void Max(double a, double b)
{ cout << "MyMax" << endl; return 0; } 

main(){ 
int i=4, j=5; Max(1.2, 3.4); //输出MyMax 
Max(i, j); //输出TemplateMax 
Max(1.2, 3); //二义性, 强制类型转换3为double, 调用Max函数 
} 
```
#### 泛型程序设计
算法实现时不指定具体要操作的数据的类型
==泛型：算法实现一遍 → 适用于多种数据结构==，本质就是一种算法泛化到各种数据类型
优点
1. 减少重复代码的编写
2. 提供了统一的接口
大量编写模板, 使用模板的程序设计

#### 类模板
为了多快好省地定义出⼀批相似的类, 可以定义 **类模板**
通过类模板生成不同的类(注意不是对象)
例如，对于不同的数组类，除了元素类型不同，其他的完全相同，因此可以抽象成一个类模板
==类模板：模板，模板类：实例化出的一个具体的类 对象：用具体的模板类实例化的对象==
类模板 
- 在定义类的时候给它⼀个/多个参数, 这个/些参数表示不同的数据类型 
- 在调⽤类模板时, 指定参数, 由编译系统根据参数提供的数据类型⾃动 产⽣相应的模板类

类模板的写法
```
template <class T1,class T2,……> 
class 类模板名 
{ 
成员函数和成员变量 
};
```
类模板里的成员函数，在类模板外定义
```
template <类型参数表> 
返回值类型 类模板名<类型参数表>::成员函数名(函数参数表)
{ ... }
```
用类模板定义对象：

```
类模板名 <真实类型参数表> 对象名(构造函数实际参数表);
```
如果类模板具有无参构造函数
```
类模板名 <真实类型参数表> 对象名;
```
模板类：为类模板中各类型参数指定了具体的数据类型后, 即得到⼀个模板类
- 编译系统⾃动⽤具体的数据类型替换类模板中的类型参数, ⽣成模板类的代码
- 编译系统⾃动⽤具体的数据类型替换类模板中的类型参数, ⽣成模板类的代码
- 在具体使用的时候必须指定类型参数的具体类型，才可以声明对象
==使用同一个类模板可以生成不同的模板类，生成的不同模板类不兼容，不可以进行赋值操作==

###### 函数模板作为类模板
类模板中的成员函数可以是⼀个函数模板, 该成员函数只有在被调⽤时才会被实例化
函数模板的类型参数不可以与类模板的类型参数同名，否则会造成二义性，即类的类型参数会掩盖函数模板的类型参数，编译时会报错
```
template <class T> 
class A{ 
public: 
template <class T2> 
void Func(T2 t) { cout << t; } //成员函数模板 };
```

###### 类模板与非类型参数
在类模板的类型参数表中可以包括非类型参数，即确定类型的参数
- 非类型参数: 用来说明类模板中的属性, 常用于定义数组大小、设置模板内的配置常量 
- 类型参数: 用来说明类模板中的属性类型, 成员函数的参数类型和返回值类型
- 非类型参数与成员变量的区别
	1. 类中并没有添加这个非类型参数作为成员变量，编译后会替换为对应的数值
	2. 通常类模板参数声明中的非类型参数可以提高程序执行效率  在编译或链接期间即可确定参数的值
	3. 传入的非类型参数定义的是不同的类
```
template <class T, int size> 
class CArray{ 
T array[size]; 
public: 
void Print() 
{ for(int i = 0; i < size; ++i) cout << array[i] << endl; 
} 
}; 
CArray<double, 40> a2; 
CArray<int, 50> a3;
//类中并没有添加一 个int size的成员变量, 编译过后被替换为40/50 
//a2和a3属于不同的类
```
###### 类模板与派生
- 类模板派⽣出类模板 
- 模板类(即类模板中类型/⾮类型参数实例化后的类)派出类模板 
- 普通类派⽣出类模板 
- 模板类派⽣出普通类

**类模板从类模板派生**
```
template <class T1, class T2> 
class A { T1 v1; T2 v2; }; 

template <class T1, class T2> 
class B: public A<T2, T1> { T1 v3; T2 v4; }; 

template <class T> 
class C: public B<T,T> { T v5; }; 
int main(){ 
B<int, double> obj1; 
C<int> obj2; 
return 0; 
}
```
==注意 类名<类型参数表>是一个整体，因此在继承时也要把类型参数表写上去，并且在顺序上具有对应关系==
此时并没有指定类型参数具体类型，因此是从类模板派生
**类模板从模板类派生**
```
template <class T1, class T2> 
class A { T1 v1; T2 v2; }; 

template <class T> 
class B: public A <int, double> { T v; }; 
int main() { 
B<char> obj1; //自动生成两个模板类: A<int, double>和B<char> 
return 0; }
```
此时指定了类型参数具体类型，因此是从模板类派生
**类模板从普通类派生**
```
class A { int v1; }; 
template <class T> 
class B: public A { //所有从B实例化得到的类, 都以A为基类 T v; }; 
int main() { 
B<char> obj1; 
return 0; 
}
```
**普通类从模板类派生**
```
template <class T> 
class A { T v1; int n; }; 
class B: public A<int> { double v; }; 
int main() { 
B obj1; 
return 0;
} 
```
==注意不可以从类模板派生普通类，因为类模板派生的类一定要给基类的类型参数赋值==
###### 友元
**函数/类/类的成员函数作为类模板的友元**
在类内声明friend即可
**函数模板作为类模板的友元**
注意要把templarte写上去
```
template <class T1, class T2> 
class Pair{ 
T1 key; //关键字 
T2 value; //值 
public: Pair(T1 k, T2 v):key(k), value(v) { }; 
bool operator < ( const Pair<T1,T2> & p ) const; 

template <class T3, class T4> 
friend ostream & operator<<( ostream & o, const Pair<T3, T4> & p ); //函数模版 
};

template <class T3, class T4> 
ostream & operator<< (ostream & o, const Pair<T3, T4> & p)
{ 
o << "(" << p.key << "," << p.value << ")" ; 
return o; 
}
```
任意从 `template <class T3, class T4> ostream & operator<< (ostream & o, const Pair<T3, T4> & p)`{ }生成的函数, 都是任意Pair模板类的友元
**函数模板作为类的友元**
同样要声明template
```
class A 
{ 
int v; 
public: A(int n):v(n) { } 
template <class T> 
friend void Print(const T & p); 
}; 
template <class T> 
void Print(const T & p)
{ cout << p.v; }
```
任意从 `template <class T> void Print(const T & p)` 生成的函数, 都成为A的友元
**类模板作为类模板的友元**
同样要声明template
```
template <class T> 
class A { 
public: void Func( const T & p ) 
{ cout << p.v; } 
}; 

template <class T> 
class B { 
T v; 
public: B(T n):v(n) { } 
template <class T2> 
friend class A; //把类模板A声明为友元 
};
```
注意参数必须对应想用,否则不可以成为友元
###### static成员
类模板中可以定义静态成员, 那么从该类模板实例化得到的模板 类的所有对象, 都包含同样的静态成员
==注意不同实例化得到的模板类是不同的模板类，他们的静态成员是无关的，只有同一个模板类实例化的对象才共享同一份静态成员==
```
template< > int A<int>::count = 0; 
template< > int A<double>::count = 0;
\\A<int>和A<double>是不同的 模板类, 不能共享静态变量 count, 因而需要分别初始化
```

#### string 类
string类是⼀个模板类, 它的定义如下
```
typedef basic_string<char> string;
```
三种初始化方式
```
string s1("Hello"); // ⼀个参数的构造函数 
string s2(8, 'x'); // 两个参数的构造函数 
string month = "March";
```
类中不提供以字符和整数为参数的构造函数，因为库里没有包含这样的构造函数
```
string error1 = 'c'; // 错 
string error2('u'); // 错 
string error3 = 22; // 错 
string error4(8); // 错
```
可以将字符赋值给string对象 string 类 
###### **1. 初始化与构造**
- **常用构造方式**：
    - `string s1("Hello");`：使用字符串常量初始化。
    - `string s2(8, 'x');`：创建包含 8 个连续字符 'x' 的字符串。
    - `string month = "March";`：直接通过赋值方式初始化。
- **注意**：不提供以单个字符或整数作为参数的构造函数（如 `string s = 'c';` 或 `string s(8);` 均错误）。
###### **2. 基本属性与访问**
- **长度获取**：使用 `s.length()` 或 `s.size()` 读取字符串中的字符数。
- **字符访问**：
    - 使用下标运算符 `[]`：如 `s[i]`，不进行范围检查。
    - 使用成员函数 `at()`：如 `s.at(i)`，会做范围检查，超出范围抛出 `out_of_range` 异常。
###### **3. 赋值与复制**
- **赋值运算符**：使用 `=` 直接赋值（如 `s2 = s1;`）。
- **`assign` 成员函数**：
    - `s3.assign(s1);`：完全复制。
    - `s3.assign(s1, 1, 3);`：部分复制（从下标 1 开始复制 3 个字符）
###### **4. 连接与添加**
- **`+` 或 `+=` 运算符**：直接连接两个字符串。
- **`append` 成员函数**：
    - `s1.append(s2);`：在末尾添加字符串。
    - `s2.append(s1, 3, s1.size());`：部分添加（从指定下标开始添加指定长度）。
###### **5. 字符串比较**
- **关系运算符**：支持 `==`, `>`, `>=`, `<`, `<=`, `!=`。返回值均为 `bool` 类型。
- **`compare` 成员函数**：
    - 返回值：相等返回 `0`；大于返回正整数；小于返回负整数。
    - 支持复杂比较，如 `s1.compare(1, 2, s3, 0, 3);`（将 `s1` 的子串与 `s3` 的子串进行比较）。
###### **6. 输入与输出**
- **流读取**：支持 `cin >> s;`（读取到空格或换行符停止）。
- **整行读取**：使用全局函数 `getline(cin, s);`（读取一行，包括空格，直到遇到换行符）。
- **流输出**：支持 `cout << s;`。
###### **7. 其他特性**
- **异常处理**：如果构造的字符串长度超过表达范围，会抛出 `length_error` 异常。
- **内存输入输出**：支持使用 `istringstream` 和 `ostringstream` 在字符串上进行输入输出操作。
- 更多复杂的见PPT
