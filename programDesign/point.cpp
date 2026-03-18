//#include <iostream> 
//using namespace std;
//class Point {
//private:
//	int x;
//	int y;
//public:
//	Point() {};
//	//请在此输入代码
//	friend istream& operator>>(istream& i,Point& p)
//	{
//		i >> p.x >> p.y;
//		return i;
//	}
//	friend ostream& operator<<(ostream& o, Point& p)
//	{
//		o << p.x <<',' << p.y;
//		return o;
//	}
//};
//int main()
//{
//	Point p;
//	while (cin >> p) {
//		cout << p << endl;
//	}
//	return 0;
//}