//#include <iostream>
//#include <cstring>
//using namespace std;
//
//class Array2 {
//    // 二维数组类的定义
//public:
//    int** arr;
//    int rows;
//    int cols;
//    Array2()
//    {
//        arr = NULL;
//    }
//    Array2(int a, int b):rows(a),cols(b)
//    {
//        arr = new int* [a];
//        for (int i = 0; i < a; i++)
//        {
//            arr[i] = new int[b];
//        }
//    }
//    Array2& operator=(const Array2& a)
//    {
//        rows = a.rows;
//        cols = a.cols;
//        arr = new int* [rows];
//        for (int i = 0; i < rows; i++)
//        {
//            arr[i] = new int[cols];
//        }
//        for (int i = 0; i < rows; i++)
//        {
//            for (int j = 0; j < cols; j++)
//            {
//                arr[i][j] = a.arr[i][j];
//            }
//        }
//        return *this;
//    }
//    int operator()(int a, int b)
//    {
//        return arr[a][b];
//    }
//    int* operator[](int n)
//    {
//        return arr[n];
//    }
//};
//
//int main() {
//    Array2 a(3, 4);
//    int i, j;
//    for (i = 0; i < 3; ++i)
//        for (j = 0; j < 4; j++)
//            a[i][j] = i * 4 + j;
//    for (i = 0; i < 3; ++i) {
//        for (j = 0; j < 4; j++) {
//            cout << a(i, j) << ",";
//        }
//        cout << endl;
//    }
//    cout << "next" << endl;
//    Array2 b;     b = a;
//    for (i = 0; i < 3; ++i) {
//        for (j = 0; j < 4; j++) {
//            cout << b[i][j] << ",";
//        }
//        cout << endl;
//    }
//    return 0;
//}