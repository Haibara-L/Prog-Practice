#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

class CHugeInt {
private:
    char* s;  // 存储数值的字符串表示
    int n;    // 保存整数值；当 s 为 nullptr 时使用

    // 进行字符串数字相加
    static char* addStrings(const char* a, const char* b) {
        int lenA = strlen(a);
        int lenB = strlen(b);
        int maxLen = max(lenA, lenB) + 2;  // +2 为结束符 '\0' 预留空间
        char* result = new char[maxLen];

        int i = lenA - 1;
        int j = lenB - 1;
        int k = 0;
        int carry = 0;

        // 从个位开始相加
        while (i >= 0 || j >= 0 || carry) {
            int digitA = (i >= 0) ? a[i--] - '0' : 0;
            int digitB = (j >= 0) ? b[j--] - '0' : 0;

            int sum = digitA + digitB + carry;
            result[k++] = (sum % 10) + '0';
            carry = sum / 10;
        }

        // 翻转结果
        for (int m = 0; m < k / 2; m++) {
            swap(result[m], result[k - 1 - m]);
        }
        result[k] = '\0';

        return result;
    }

    // 将整数加到字符串数字上
    static char* addInt(const char* a, int b) {
        if (b == 0) {
            char* result = new char[strlen(a) + 1];
            strcpy(result, a);
            return result;
        }

        int len = strlen(a);
        int maxLen = len + 20;  // 预留足够空间
        char* result = new char[maxLen];
        strcpy(result, a);

        int carry = b;

        // 从个位开始加
        for (int i = len - 1; i >= 0 && carry > 0; i--) {
            int digit = result[i] - '0' + carry;
            result[i] = (digit % 10) + '0';
            carry = digit / 10;
        }

        // 如果还有进位，需要加到前面
        if (carry > 0) {
            char* newResult = new char[len + 20];
            sprintf(newResult, "%d", carry);
            strcat(newResult, result);
            delete[] result;
            return newResult;
        }

        return result;
    }

public:
    CHugeInt() : s(nullptr), n(-1) {}

    CHugeInt(const char s[]) {
        this->s = new char[strlen(s) + 1];
        strcpy(this->s, s);
        this->n = -1;
    }

    CHugeInt(int n) {
        this->n = n;
        this->s = nullptr;
    }

    CHugeInt(const CHugeInt& other) {
        if (other.s) {
            s = new char[strlen(other.s) + 1];
            strcpy(s, other.s);
            n = -1;
        }
        else {
            s = nullptr;
            n = other.n;
        }
    }

    ~CHugeInt() {
        delete[] s;
    }

    friend ostream& operator<<(ostream& o, const CHugeInt& c) {
        if (c.s) {
            o << c.s;
        }
        else {
            o << c.n;
        }
        return o;
    }

    // CHugeInt + CHugeInt
    CHugeInt operator+(const CHugeInt& other) const {
        // 处理字符串数字相加模式
        if (s && other.s) {
            char* resultStr = addStrings(s, other.s);
            CHugeInt result(resultStr);
            delete[] resultStr;
            return result;
        }
        // this 是字符串，other 是整数
        else if (s && other.n != -1) {
            char* resultStr = addInt(s, other.n);
            CHugeInt result(resultStr);
            delete[] resultStr;
            return result;
        }
        // this 是整数，other 是字符串
        else if (n != -1 && other.s) {
            char* resultStr = addInt(other.s, n);
            CHugeInt result(resultStr);
            delete[] resultStr;
            return result;
        }
        // 两者都是整数
        else {
            return CHugeInt(n + other.n);
        }
    }

    // CHugeInt + int
    CHugeInt operator+(int num) const {
        if (s) {
            char* resultStr = addInt(s, num);
            CHugeInt result(resultStr);
            delete[] resultStr;
            return result;
        }
        else {
            return CHugeInt(n + num);
        }
    }

    // int + CHugeInt 友元函数
    friend CHugeInt operator+(int num, const CHugeInt& obj) {
        return obj + num;  // 调用成员 operator+(int)
    }

    CHugeInt& operator+=(int num) {
        if (s) {
            char* newS = addInt(s, num);
            delete[] s;
            s = newS;
            n = -1;
        }
        else if (n != -1) {
            n += num;
        }
        return *this;
    }

    CHugeInt& operator++() {
        if (s) {
            char* newS = addInt(s, 1);
            delete[] s;
            s = newS;
            n = -1;
        }
        else if (n != -1) {
            n++;
        }
        return *this;
    }

    CHugeInt operator++(int) {
        CHugeInt tmp(*this);
        if (s) {
            char* newS = addInt(s, 1);
            delete[] s;
            s = newS;
            n = -1;
        }
        else if (n != -1) {
            n++;
        }
        return tmp;
    }
};
int  main()
{
	char s[210];
	int n;

	while (cin >> s >> n) {
		CHugeInt a(s);
		CHugeInt b(n);

		cout << a + b << endl;
		cout << n + a << endl;
		cout << a + n << endl;
		b += n;
		cout << ++b << endl;
		cout << b++ << endl;
		cout << b << endl;
	}
	return 0;
}