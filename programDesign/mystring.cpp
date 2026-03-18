#include <cstdlib>
#include <iostream>
using namespace std;
int strlen(const char * s) 
{	int i = 0;
	for(; s[i]; ++i);
	return i;
}
void strcpy(char * d,const char * s)
{
	int i = 0;
	for( i = 0; s[i]; ++i)
		d[i] = s[i];
	d[i] = 0;
		
}
int strcmp(const char * s1,const char * s2)
{
	for(int i = 0; s1[i] && s2[i] ; ++i) {
		if( s1[i] < s2[i] )
			return -1;
		else if( s1[i] > s2[i])
			return 1;
	}
	return 0;
}
void strcat(char * d,const char * s)
{
	int len = strlen(d);
	strcpy(d+len,s);
}
class MyString
{
public:
char* s;
MyString(){
    s=new char[1];
    s[0]='\0';
}
MyString(const char* temps)
{
    s=new char[strlen(temps)+1];
    strcpy(s,temps);
}
MyString(const MyString& temps)
{
    if(temps.s)
    {
        s=new char[strlen(temps.s)+1];
        strcpy(s,temps.s);
    }
    else
    {
        s=NULL;
    }
}
bool operator<(const MyString& s2)
{
    if(strcmp(s,s2.s)==-1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
bool operator==(const MyString& s2)
{
    if(strcmp(s,s2.s)==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
bool operator>(const MyString& s2)
{
    if(strcmp(s,s2.s)==1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
friend ostream& operator<<(ostream& o,const MyString& temps)
{
    if(temps.s)
    {
    o<<temps.s;
    }
    else
    {
    o<<"";
    }
    return o;
}
MyString& operator=(const MyString& temps)
{
    if(s[0]=='\0')
    {
        delete[] s;
        s=new char[strlen(temps.s)+1];
        strcpy(s,temps.s);
        return *this;
    }
    if(*this==temps)
    {
        return *this;
    }
    if(s)
    {
        delete[] s;
    }
    if(temps.s)
    {
        s=new char[strlen(temps.s)+1];
        strcpy(s,temps.s);
    }
    else
    {
        s=NULL;
    }
    return *this;
}
MyString& operator=(const char* temps)
{
    if(s)
    {
        delete[] s;
    }
    if(temps)
    {
    s=new char[strlen(temps)+1];
    strcpy(s,temps);
    }
    else
    {
        s=NULL;
    }
    return *this;
}
MyString operator+(const MyString& temps)
{
    int newLen = strlen(s) + strlen(temps.s) + 1;
    char* s1 = new char[newLen];
    strcpy(s1, s);
    strcat(s1, temps.s);
    MyString result(s1);
    delete[] s1;
    return result;
}
MyString operator+(const char* temps)
{
    int newLen = strlen(s) + strlen(temps) + 1;
    char* s1 = new char[newLen];
    strcpy(s1, s);
    strcat(s1, temps);
    MyString result(s1);
    delete[] s1;
    return result;
}
friend MyString operator+(const char* temps,const MyString& s)
{
    int newLen = strlen(s.s) + strlen(temps) + 1;
    char* s1 = new char[newLen];
    strcpy(s1, temps);
    strcat(s1, s.s);
    MyString result(s1);
    delete[] s1;
    return result;
}
char& operator[](int index)
{
    return s[index];
}
MyString& operator+=(const char* temps)
{
    int len=strlen(s)+strlen(temps)+1;
    char* s1=new char[len];
    strcpy(s1,s);
    strcat(s1,temps);
    delete[] s;
    s=s1;
    return *this;
}
char* operator()(int start,int num)
{
    char* s1=new char[num+1];
    for(int i=start;i<start+num;i++)
    {
        s1[i-start]=s[i];
    }
    s1[num]='\0';
    return s1;
}
};


int CompareString( const void * e1, const void * e2)
{
	MyString * s1 = (MyString * ) e1;
	MyString * s2 = (MyString * ) e2;
	if( * s1 < *s2 )
	return -1;
	else if( *s1 == *s2)
	return 0;
	else if( *s1 > *s2 )
	return 1;
}
int main()
{
	MyString s1("abcd-"),s2,s3("efgh-"),s4(s1);
	MyString SArray[4] = {"big","me","about","take"};
	cout << "1. " << s1 << s2 << s3<< s4<< endl;
	s4 = s3;
	s3 = s1 + s3;
	cout << "2. " << s1 << endl;
	cout << "3. " << s2 << endl;
	cout << "4. " << s3 << endl;
	cout << "5. " << s4 << endl;
	cout << "6. " << s1[2] << endl;
	s2 = s1;
	s1 = "ijkl-";
	s1[2] = 'A' ;
	cout << "7. " << s2 << endl;
	cout << "8. " << s1 << endl;
	s1 += "mnop";
	cout << "9. " << s1 << endl;
	s4 = "qrst-" + s2;
	cout << "10. " << s4 << endl;
	s1 = s2 + s4 + " uvw " + "xyz";
	cout << "11. " << s1 << endl;
	qsort(SArray,4,sizeof(MyString),CompareString);
	for( int i = 0;i < 4;i ++ )
	cout << SArray[i] << endl;
	//s1的从下标0开始长度为4的子串
	cout << s1(0,4) << endl;
	//s1的从下标5开始长度为10的子串
    cout << s1(5,10) << endl;
	return 0;
}