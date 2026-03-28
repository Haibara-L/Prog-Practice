#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

int main() {
    std::ifstream right("right.txt");
    std::ifstream wrong("wrong.txt");

    if (!right.is_open()) {
        std::cerr << "错误：无法打开 right.txt" << std::endl;
        return 1;
    }
    if (!wrong.is_open()) {
        std::cerr << "错误：无法打开 wrong.txt" << std::endl;
        return 1;
    }

    std::string lineRight, lineWrong;
    int lineNum = 1;
    bool different = false;

    while (std::getline(right, lineRight) && std::getline(wrong, lineWrong)) {
        if (lineRight != lineWrong) {
            std::cout << "不同出现在第 " << lineNum << " 行：" << std::endl;
            std::cout << "right.txt: " << lineRight << std::endl;
            std::cout << "wrong.txt: " << lineWrong << std::endl;
            different = true;
            //break;
        }
        ++lineNum;
    }

    if (!different) {
        // 检查是否其中一个文件还有更多行
        if (std::getline(right, lineRight)) {
            std::cout << "第一个不同出现在第 " << lineNum << " 行：" << std::endl;
            std::cout << "right.txt: " << lineRight << std::endl;
            std::cout << "wrong.txt: (文件已结束)" << std::endl;
            different = true;
        } else if (std::getline(wrong, lineWrong)) {
            std::cout << "第一个不同出现在第 " << lineNum << " 行：" << std::endl;
            std::cout << "right.txt: (文件已结束)" << std::endl;
            std::cout << "wrong.txt: " << lineWrong << std::endl;
            different = true;
        }
    }

    if (!different) {
        std::cout << "两个文件完全相同。" << std::endl;
    }

    right.close();
    wrong.close();
    return 0;
}