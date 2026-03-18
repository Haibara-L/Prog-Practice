//#include <iostream>
//#include <string>
//#include <cstring>
//#define _CRT_SECURE_NO_WARNINGS 
//using namespace std;
//class MyString {
//	char* p;
//public:
//	MyString(const char* s) {
//		if (s) {
//			p = new char[strlen(s) + 1];
//			strcpy(p, s);
//		}
//		else
//			p = NULL;
//
//	}
//	~MyString() { if (p) delete[] p; }
//	// 复制构造函数
//	MyString(MyString& s) {
//		if (s.p) {
//			p = new char[strlen(s.p) + 1];
//			strcpy(p, s.p);
//		}
//		else
//			p = NULL;
//
//	}
//	void Copy(char pp[])
//	{
//		if (p)
//		{
//			delete[] p;
//		}
//		if (pp)
//		{
//			p = new char[strlen(pp) + 1];
//			strcpy(p, pp);
//		}
//		else
//		{
//			p = NULL;
//		}
//	}
//	friend ostream& operator<<(ostream& o, const MyString& s)
//	{
//		o << s.p;
//		return o;
//	}
//	MyString& operator=(const MyString& s)
//	{
//		if (p)
//		{
//			delete[] p;
//		}
//		if (s.p)
//		{
//			p = new char[strlen(s.p) + 1];
//			strcpy(p, s.p);
//		}
//		else
//		{
//			p = NULL;
//		}
//		return *this;
//	}
//	MyString& operator=(const char* s)
//	{
//		if (p)
//		{
//			delete[] p;
//		}
//		if (s)
//		{
//			p = new char[strlen(s) + 1];
//			strcpy(p, s);
//		}
//		else
//		{
//			p = NULL;
//		}
//		return *this;
//	}
//
//};
//int main()
//{
//	char w1[200], w2[100];
//	while (cin >> w1 >> w2) {
//		MyString s1(w1), s2 = s1;
//		MyString s3(NULL);
//		s3.Copy(w1);
//		cout << s1 << "," << s2 << "," << s3 << endl;
//
//		s2 = w2;
//		s3 = s2;
//		s1 = s3;
//		cout << s1 << "," << s2 << "," << s3 << endl;
//
//	}
//}