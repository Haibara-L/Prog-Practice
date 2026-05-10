#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>

using namespace std;

const string symbols[] = {"🍒", "🍋", "🍇", "💎", "🔔", "⭐"};
const int symbolCount = 6;

string spin() {
    return symbols[rand() % symbolCount];
}

void printSlot(string s1, string s2, string s3) {
    cout << "=====================\n";
    cout << "  " << s1 << " | " << s2 << " | " << s3 << " \n";
    cout << "=====================\n";
}

int checkWin(string s1, string s2, string s3) {
    if (s1 == s2 && s2 == s3) {
        return 3;
    } else if (s1 == s2 || s2 == s3 || s1 == s3) {
        return 2;
    }
    return 0;
}

int main() {
    srand(time(NULL));

    cout << "=== 老虎机游戏 ===\n";
    cout << "符号: 🍒 🍋 🍇 💎 🔔 ⭐\n";
    cout << "规则: 三个相同符号=大奖, 两个相同=小奖\n";
    cout << "每次下注10分\n\n";

    int balance = 100;

    while (balance >= 10) {
        cout << "当前余额: " << balance << " 分\n";
        cout << "按回车旋转 (输入q退出): ";

        string input;
        getline(cin, input);

        if (input == "q" || input == "Q") {
            break;
        }

        balance -= 10;

        string s1, s2, s3;

        cout << "\n";
        for (int i = 0; i < 5; i++) {
            cout << "\r旋转中... ";
            if (i < 4) {
                cout << spin() << " " << spin() << " " << spin();
            }
            Sleep(150);
        }

        s1 = spin();
        s2 = spin();
        s3 = spin();

        cout << "\r";
        printSlot(s1, s2, s3);

        int win = checkWin(s1, s2, s3);

        if (win == 3) {
            cout << "🎉 大奖! 获得50分! 🎉\n";
            balance += 50;
        } else if (win == 2) {
            cout << "✨ 小奖! 获得20分! ✨\n";
            balance += 20;
        } else {
            cout << "没中奖, 再接再厉!\n";
        }

        cout << "\n";
    }

    if (balance < 10) {
        cout << "余额不足, 游戏结束!\n";
    }

    cout << "最终得分: " << balance << "\n";
    cout << "谢谢游玩!\n";

    return 0;
}