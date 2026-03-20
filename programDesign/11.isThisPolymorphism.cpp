#include <iostream>
using namespace std;
class B { 
	private: 
		int nBVal; 
	public: 
		void Print() 
		{ cout << "nBVal="<< nBVal << endl; } 
		void Fun() 
		{cout << "B::Fun" << endl; } 
		B ( int n ) { nBVal = n;} 
};
// 在此处补充你的代码
class D:public B
{
    public:
    int DVal;
    D(int n):B(3*n)
    {
        DVal=n;
    }
    void Fun()
    {
        cout << "D::Fun" << endl;
    }
    void Print()
    {
        B::Print();
        cout << "nDVal="<< DVal << endl;
    }

};
int main() { 
	B * pb; D * pd; 
	D d(4); d.Fun(); //DF
	pb = new B(2); pd = new D(8); 
	pb -> Fun(); pd->Fun(); //BF DF
	pb->Print (); pd->Print (); //B2 B24
	pb = & d; pb->Fun(); //
	pb->Print(); //B12
	return 0;
}