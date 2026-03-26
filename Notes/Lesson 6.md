#### 与输入输出流相关的类
istream 是用于输入的流类,cin 就是该类的对象 
ostream 是用于输出的流类,cout 就是该类的对象 
iostream 是既能用于输入,又能用于输出的类 
ifstream 是用于从文件读取数据的类 
ofstream 是用于向文件写入数据的类 
fstream 是既能从文件读取数据,又能向文件写入数据的类
#### 标准流对象
###### 输入流对象cin
- cin 与标准输入设备相连 
- cin 对应于标准输入流,用于从键盘读取数据, 
- 也可以被重定向为从文件中读取数据
- 重定向: 将输入的源或输出的目的地改变
`freopen("t.txt", "r", stdin) // 将标准输入重定向到文件t.txt`
###### 输出流对象
- cout 与标准输出设备相连 ，cout 对应于标准输出流,用于向屏幕输出数据, 也可以被重定向为从文件中写入数据  `freopen("test.txt", "w", stdout); // 将标准输出重定向到 test.txt 文件`
- cerr 与标准错误输出设备相连
- clog 与标准错误输出设备相连
cerr与clog 都是标准错误输出流 (均定义在 iostream头文件中), 但在缓冲机制和使用场景上有区别
- cerr (无缓冲) 
	- 数据会立即输出到设备, 如终端或日志文件, 不经过缓冲区; 
	- 适用于紧急错误、调试信息(确保即使程序崩溃也能输出)
- clog (带缓冲) 
	- 数据先存入缓冲区, 缓冲区满或手动刷新时才输出; 
	- 适用于非紧急日志记录, 性能更高(减少频繁I/O操作), 常规日志记录(如程序运行状态、非关键警告, 允许延迟输出)，
	- 可以通过`clog<<"Error"<<flush` 强制输出 
###### 判断输入流结束：
```
int x; 
while ( cin>>x ) { // … }
```
- 如果从键盘输入,则在单独一行输入Ctrl + Z代表输入流结束 
- 如果从文件输入,例如前面有 读到文件尾部,输入流就算结束
\>>本来返回的是输入流对象，但是这里却可以作为布尔值使用，这是因为istream或其基类里重载了operator bool
```
classMyCin{ 
boolbStop; 
public: 
MyCin():bStop(false) { } 

operator bool( ) { //重载类型强制转换运算符
bool return !bStop; } 

MyCin&operator >>(int &n){ 
cin >> n; 
if(n == 100) bStop = true; 
return *this; }
};
```
###### istream类的成员函数
==getline==
- istream & getline(char * buf, int bufSize);
	从输入流中读取 (bufSize-1) 个字符到缓冲区, 或读到 '\n' 为止 (哪个先到算哪个)
- istream & getline(char * buf, int bufSize, char delim); 
	从输入流中读取 (bufSize-1) 个字符到缓冲区, 或读到delim为止 
- 两个函数都会自动在缓冲区中读入数据的结尾添加 '\0' 
- '\n' 或 delim 都不会被读入缓冲区, 但会被从输入流中取走 
- 如果输入流 '\n' 或delim 之前的字符个数超过了bufSize个, 就导致读入出错, 其结果 就是: 本次读入已经完成, 但之后的读入就都会失败
- 可以用 if(!cin.getline(…)) 判断输入是否结束
==注意getline遇到换行符就返回，并且会吃掉换行符，而cin不会吃掉换行符，因此混合使用geiline和cin会导致错误==
bool eof(); //判断输入流是否结束 
int peek(); //返回下一个字符, 但不从流中去掉 
istream & putback( char c ); //将字符c放回输入流 
istream & ignore( int nCount = 1, int delim = EOF ）；//从流中删掉最多nCount 个字符, 遇到EOF 时结束
#### 流操纵算子
###### 整数流的基数
```
cout <<hex<<n<<endl//十六进制
<<dec<<n<<endl//十进制
<<oct<<n<<endl;//八进制
```
==这些算子一经设置对后面所有的全部生效，除非后面设置成别的==
###### 浮点数精度
浮点数的精度: precision, setprecision 
- precision是成员函数, 其调用方式为： cout.precision(5); 
- setprecision是流操作算子, 其调用方式为： cout << setprecision(5); // 可以连续输出 
- 功能相同： 指定输出浮点数的有效位数（非定点方式输出时）
- 指定输出浮点数的小数点后的有效位数（定点方式输出时） 
* 定点方式: 指定小数点的位数
###### 流格式操纵算子
流格式操纵算子: setiosflags 
- setiosflags(ios::fixed) 用定点方式表示实数 
- setiosflags(ios::scientific) 用科学计数法方式表示实数 
-  setiosflags(ios::fixed) 可以和 setprecision(n) 合用 控制小数点右边的数字个数
- setiosflags(ios::scientific) 可以和 setprecision(n) 合用 控制指数表示法的小数位数
- 在用浮点表示的输出中, setprecision(n)表示有效位数
- 在用定点/指数表示的输出中, setprecision(n)表示小数位数
- 小数位数截短显示时, 进行四舍五入处理
```
cout << setiosflags(ios::fixed) << setprecision(6) << x << endl << y << endl << n << endl << m;//以小数点位置固定 的方式输出

cout << setiosflags(ios::fixed) << setprecision(6) << x << endl << resetiosflags(ios::fixed) << x ;//取消以小数点位置 固定的方式输出
```

| **特性**                    | **defaultfloat (默认)** | **fixed (定点)**   | **scientific (科学计数法)** |
| ------------------------- | --------------------- | ---------------- | ---------------------- |
| **精度含义**                  | **总有效数字**             | **小数点后的位数**      | **小数点后的位数**            |
| **公式**                    | 整数 + 小数 = $n$ 位       | 小数点后固定 $n$ 位     | 科学计数法小数点后 $n$ 位        |
| **123.456** (precision=2) | `1.2e+02`             | `123.46`         | `1.23e+02`             |
| **1234567.89** (默认精度 6)   | `1.23457e+06`         | `1234567.890000` | `1.234568e+06`         |
| **123.456789** (默认精度 6)   | `123.457`             | `123.456789`     | `1.234568e+02`         |
###### 设置域宽
设置域宽: setw, width 
两者功能相同, 一个是流操作算子, 另一个是成员函数, 
调用方式不同： 
- cin >> setw(5); 或者 cin.width(5); 
- cout << setw(5); 或者 cout.width(5);
- 不含参数的width函数将输出当前域宽
基本语法与效果
`setw(n)` 表示设置下一个输出项的宽度为 `n`。
- **如果内容比 `n` 短：** 默认会在左侧填充空格（即**右对齐**）。
- **如果内容比 `n` 长：** `setw` 会“失效”，按内容的实际长度完整输出（保证数据不被截断）。
- 它是“一次性”的 (Non-sticky)，这与 `fixed` 或 `setprecision` 不同。`setw` 只对紧跟在它后面的**那一个**输出项起作用，因此需要注意的是在每次读入和输出之前都要设置宽度
```
cout << setw(10) << 1.23 << 4.56; 
// 1.23 占 10 位，4.56 紧跟其后，不再受 setw 影响
```
- 配合 `setfill` 使用，默认填充的是空格，但你可以用 `setfill` 改成其他字符（如 `0` 或 `*`）
```
cout << setfill('0') << setw(5) << 8; 
// 输出：00008 (常用于显示编号或时间)
```
- 默认为右对齐，即左边填入空格，但是可以修改成左对齐
```
cout << left << setw(10) << "Name" << "Score" << endl;
```
###### 本质
自定义流操纵算子，即之前的那些算子本质上都是函数
```
ostream & tab(ostream & output){ return output << '\t'; } 
cout << "aa" << tab << "bb" << endl;
```
- 因为 iostream 里对 << 进行了重载 (成员函数)
	ostream & operator << (ostream & (\*p)) (ostream &)); 
- 该函数内部会调用 p 所指向的函数, 且以\*this 作为参数
- hex / dec / oct 都是函数,即<<相应的流操纵算子相当于调用相应的函数

#### 文件读写
###### 数据的层次
- 位 bit 
- 字节 byte 
- 域 / 记录
- 将所有记录顺序地写入一个文件, 称为 顺序文件
文件读写的本质：将顺序文件看作一个有限字符构成的顺序字符流 然后像对 cin / cout 一样的读写
###### 建立顺序文件
```
#include<fstream> // 包含头文件 
ofstream outFile("clients.dat", ios::out|ios::binary); //打开文件
```
ofstream 是 fstream 中定义的类 
outFile 是我们定义的 ofstream 类的对象 
"clients.dat" 是将要建立的文件的文件名 
ios::out 是打开并建立文件的选项 
- ios::out 输出到文件, 删除原有内容 
- ios::in 读入文件
- ios::app 输出到文件, 保留原有内容, 总是在尾部添加 
- ios::ate 输出到文件, 保留原有内容, 初始在尾部, 可在文件任意位置添加  
- ios::binary 以二进制文件格式打开文件
也可以先创建ofstream对象, 再用open函数打开
```
ofstream fout; 
fout.open("test.out", ios::out|ios::binary);
```
一般在打开文件后要判断打开是否成功
```
if (!fout) { cerr << "File open error!" <<endl;}
```
文件名可以给出绝对路径, 也可以给相对路径没有交代路径信息, 就是在当前文件夹下找文件
###### 文件的读写指针
对于输入文件, 有一个读指针 
对于输出文件, 有一个写指针 
对于输入输出文件, 有一个读写指针标识文件操作的当前位置, 
该指针在哪里, 读写操作就在哪里进行

写指针 (Put Pointer)
使用 `ofstream` 或 `fstream` 对象时，通过 `p` 系列函数操作“写”的位置。
```
ofstream fout("a1.out", ios::ate); // ios::ate 表示打开后立即移到文件末尾
long location = fout.tellp();      // 取得写指针当前的位置 (tell put)

location = 10L;
fout.seekp(location);              // 将写指针移动到第 10 个字节处 -> 10

// 相对位置移动 (seek put)
fout.seekp(location, ios::beg);    // 从开头数 location 个字节 -> 10
fout.seekp(location, ios::cur);    // 从当前位置数 location 个字节 -> 20
fout.seekp(location, ios::end);    // 从尾部数 location 个字节 -> 文件总长度 + 10
// 注：location 可以为负值，表示向文件头方向移动
```

读指针 (Get Pointer)
使用 `ifstream` 或 `fstream` 对象时，通过 `g` 系列函数操作“读”的位置。
```
ifstream fin("a1.in", ios::ate);  // 打开文件
long location = fin.tellg();      // 取得读指针当前的位置 (tell get)

location = 10L;
fin.seekg(location);              // 将读指针移动到第 10 个字节处

// 相对位置移动 (seek get)
fin.seekg(location, ios::beg);    // 从头数 location
fin.seekg(location, ios::cur);    // 从当前位置数 location
fin.seekg(location, ios::end);    // 从尾部数 location
// 注：location 可以为负值
```
- **后缀 `p` (Put)**：对应写操作，意为“把数据**放**进去”。
- **后缀 `g` (Get)**：对应读操作，意为“从文件**拿**数据”。
- **`tell`**：询问当前位置。
- **`seek`**：主动跳转位置。
注意以上的文件模式都可以修改指针位置

|**打开模式**|**初始指针位置**|**是否可以移动指针？**|
|---|---|---|
|**`ios::in`**|文件开头 (`beg`)|**可以**。你可以 `seekg` 到任何地方读取。|
|**`ios::out`**|文件开头（会清空原内容）|**可以**。你可以 `seekp` 到指定位置覆盖写入。|
|**`ios::ate`**|**文件末尾 (`end`)**|**可以**。虽然起点在末尾，但你可以立刻 `seek` 回开头。|
|**`ios::app`**|文件末尾|**受限**。这是唯一的例外（见下文）。|
真正的“禁区”：`ios::app` (追加模式)
在所有的模式中，只有 **`ios::app` (append)** 会让 `seekp` 变得“软弱无力”：
- 在 `ios::app` 模式下，无论你如何调用 `fout.seekp(0, ios::beg)` 尝试回到开头，**只要你执行写入操作，指针都会被强制拉回到文件末尾**。
- 这是为了保证每一条新数据都绝对是在最后，不会破坏已有数据。
 **总结：** 如果你需要“先移动到中间修改，再移动到末尾添加”，你应该使用 `ios::in | ios::out | ios::binary`（且不加 `ios::app`）。
因为文件流也是流, 所以前面讲过的流的成员函数和流操作算子也同样 适用于文件流
###### 二进制文件读写
1. 文件的打开方式
二进制读写必须在打开文件时指定 `ios::binary` 标志，否则系统会对换行符（如 `\n`）进行自动转换，导致数据损坏。
- **写模式：** `ofstream fout("data.bin", ios::out | ios::binary);`
- **读模式：** `ifstream fin("data.bin", ios::in | ios::binary);`
- **读写模式：** `fstream fio("data.bin", ios::in | ios::out | ios::binary);`
1. 核心读写函数
二进制操作不使用 `<<` 和 `>>`，而是使用 `write` 和 `read`。它们的参数统一要求为 `char*` 类型的指针。
写操作：`write`
```
// 原型：ostream& write(const char* buffer, streamsize size);
double d = 3.14;
fout.write((char*)&d, sizeof(d));
```
读操作：read
```
// 原型：istream& read(char* buffer, streamsize size);
double d;
fin.read((char*)&d, sizeof(d));
```
3. 结构体与类的整体读写
这是二进制读写最强大的地方：你可以一次性读写整个对象。
```
struct Student {
    int id;
    char name[20];
    double score;
};

Student s1 = {1001, "Tom", 95.5};

// 写入整个结构体
fout.write((char*)&s1, sizeof(s1));

// 读取整个结构体
Student s2;
fin.read((char*)&s2, sizeof(s2));
```
4. 位置定位与随机读写
	二进制文件通常用于“随机访问”（即直接跳到文件的某一部分进行修改）。
- **`seekg` / `seekp`**：移动读/写指针。
    - `fio.seekg(n, ios::beg);` // 移动到开头起第 n 个字节。
    - `fio.seekg(-n, ios::end);` // 移动到倒数第 n 个字节。
- **`tellg` / `tellp`**：获取当前指针位置。
    - 常用于计算文件大小：先 `seek` 到末尾，再 `tell`。
 常用技巧：获取文件大小
```
fin.seekg(0, ios::end);       // 移到末尾
long size = fin.tellg();      // 获取位置（即大小）
fin.seekg(0, ios::beg);       // 记得移回开头以便后续读取
```
6. 三大致命红线
① 绝对不要直接读写包含 `std::string` 或指针的对象
`std::string` 内部存储的是指向堆内存的指针。如果你用 `write` 把对象存入文件，存进去的只是一个**内存地址**。当你下次运行程序读取时，那个地址已经失效了，程序会直接崩溃。
- **解决方案：** 使用定长字符数组 `char name[20]`，或者手动进行序列化
② 记得检查流状态
在循环读取二进制文件时，使用 `read` 后应该检查是否成功：
```
while (fin.read((char*)&obj, sizeof(obj))) {
    // 处理读取到的数据
}
```

③ 注意字节对齐（进阶）
不同的编译器或平台对结构体的填充（Padding）可能不同。如果你的二进制文件要在不同机器间传输，建议使用 `#pragma pack(push, 1)` 来取消字节对齐

|**操作**|**函数/标志**|**关键点**|
|---|---|---|
|**打开**|`ios::binary`|必须添加，防止换行符转换|
|**写入**|`write(char*, size)`|强制类型转换 `(char*)`|
|**读取**|`read(char*, size)`|确保缓冲区大小足够|
|**跳转**|`seekg` / `seekp`|配合 `ios::beg/cur/end` 使用|
|**位置**|`tellg` / `tellp`|返回当前字节偏移量|
#### 显示关闭文件
```
f.close()
```
#### 文件拷贝
推荐用get和put进行读写，这样是进行一个一个字符的拷贝，相对安全
```
/* 用法示例：
   mycopy src.dat dest.dat
   即将 src.dat 拷贝到 dest.dat
   如果 dest.dat 原来就有，则原来的文件会被覆盖
*/

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char * argv[])
{
    // 检查命令行参数，argc 应为 3 (程序名 + 源文件名 + 目标文件名)
    if ( argc != 3 ) {
        cout << "File name missing!" << endl;
        return 0;
    }

    // 以二进制输入模式打开源文件
    ifstream inFile(argv[1], ios::binary | ios::in); 
    if ( !inFile ) {
        cout << "Source file open error." << endl;
        return 0;
    }

    // 以二进制输出模式打开目标文件
    ofstream outFile(argv[2], ios::binary | ios::out); 
    if ( !outFile ) {
        cout << "New file open error." << endl;
        inFile.close();  // 打开的文件一定要关闭
        return 0;
    }

    char c;
    // 逐字节读取并写入，确保所有字符（含空白符）均被完整拷贝
    while ( inFile.get(c) )  // 每次读取一个字符
    {
        outFile.put(c);      // 每次写入一个字符
    }

    // 关闭文件流
    outFile.close();
    inFile.close();

    return 0;
}
```
或者使用read，write
```
// 定义缓冲区大小，通常为 4KB 或 8KB (磁盘块大小的整数倍)
const int BUFFER_SIZE = 4096; 
char buffer[BUFFER_SIZE];

// 循环读取，直到文件结束
// read() 返回流对象本身，在布尔上下文中失败或结束会返回 false
while (inFile.read(buffer, BUFFER_SIZE)) {
    // 正常读取了 BUFFER_SIZE 个字节，直接全部写入
    outFile.write(buffer, BUFFER_SIZE);
}

// 关键步骤：处理最后一次读取（不满 BUFFER_SIZE 的部分）
// gcount() 返回上一次输入操作实际读取的字节数
if (inFile.gcount() > 0) {
    outFile.write(buffer, inFile.gcount());
}
```
实习考试避坑点：`gcount()` 的重要性
很多同学会漏掉最后那句 `inFile.gcount()`。
- 如果文件大小是 4100 字节，缓冲区是 4096。 
- 第一次循环：读了 4096 字节，写入 4096。  
- 第二次循环：只能读到剩下的 4 字节，此时 `read()` 可能返回 false（触碰了 EOF）。
- 如果不写最后那句 `write(..., gcount())`，**文件末尾的 4 个字节就会丢失**。
- 在 C++ 实习或考试中，只要你用到了 `read()` 函数，脑子里就要立刻反射出 `gcount()`。它是保证二进制文件**不多写、不少写**的唯一“精确称量工具”
- 核心作用：防止“数据污染”
当你搬运数据时，它能确保你**“读到多少，就写多少”**。
如果不使用 `gcount()`，你的程序就像一个不负责任的搬运工。比如：
- **最后一次读取**：文件只剩 **10 个字节**了。
- **缓冲区**：你准备了一个 **1024 字节**的大桶。
- **后果**：你如果不问 `gcount()`，你就会把这 10 个新字节和桶里剩下的 **1014 个旧垃圾数据**一起写进新文件。
**用了 `gcount()`，你就能精准地只切下那 10 个字节。**
在 C++ 实习的填空题或大作业里，`gcount()` 经常出现在处理**最后一块数据**的逻辑中
```
// 典型的二进制文件处理循环
while (!inFile.eof()) {
    inFile.read(buffer, sizeof(buffer));
    // 即使读取失败（到了文件尾），也要把最后抓到的这部分写出去
    outFile.write(buffer, inFile.gcount()); 
}
```