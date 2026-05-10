#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const char* symbols[] = {"Cherry", "Lemon", "Grape", "Diamond", "Bell", "Star"};
const int symbolCount = 6;

HINSTANCE hInst;
HWND hwndMain, hwndSpin, hwndResult, hwndBalance;
int balance = 100;
bool spinning = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "SlotMachine";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, "Failed to register window class!", "Error", MB_ICONERROR);
        return 1;
    }

    hwndMain = CreateWindow("SlotMachine", "Slot Machine", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 350,
        NULL, NULL, hInstance, NULL);

    if (!hwndMain) {
        MessageBox(NULL, "Failed to create window!", "Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

string spin() {
    return symbols[rand() % symbolCount];
}

int checkWin(string s1, string s2, string s3) {
    if (s1 == s2 && s2 == s3) return 3;
    else if (s1 == s2 || s2 == s3 || s1 == s3) return 2;
    return 0;
}

void DrawSlot(HDC hdc, int x, int y, string symbol, bool highlight) {
    RECT rect = {x, y, x + 80, y + 80};
    HBRUSH bgBrush = CreateSolidBrush(highlight ? RGB(255, 255, 200) : RGB(240, 240, 240));
    FillRect(hdc, &rect, bgBrush);
    FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
    DeleteObject(bgBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));

    if (symbol == "Cherry") {
        SetTextColor(hdc, RGB(220, 20, 60));
    } else if (symbol == "Lemon") {
        SetTextColor(hdc, RGB(255, 200, 0));
    } else if (symbol == "Grape") {
        SetTextColor(hdc, RGB(128, 0, 128));
    } else if (symbol == "Diamond") {
        SetTextColor(hdc, RGB(0, 191, 255));
    } else if (symbol == "Bell") {
        SetTextColor(hdc, RGB(218, 165, 32));
    } else if (symbol == "Star") {
        SetTextColor(hdc, RGB(255, 215, 0));
    }

    const char* sym = symbol.c_str();
    DrawText(hdc, sym, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

DWORD WINAPI SpinThread(LPVOID lpParam) {
    string* results = (string*)lpParam;

    for (int i = 0; i < 10; i++) {
        results[0] = spin();
        results[1] = spin();
        results[2] = spin();
        InvalidateRect(hwndMain, NULL, FALSE);
        Sleep(50 + i * 20);
    }

    results[0] = spin();
    results[1] = spin();
    results[2] = spin();

    InvalidateRect(hwndMain, NULL, FALSE);

    int win = checkWin(results[0], results[1], results[2]);

    string resultText;
    if (win == 3) {
        balance += 50;
        resultText = "JACKPOT! +50!";
    } else if (win == 2) {
        balance += 20;
        resultText = "Small win! +20!";
    } else {
        resultText = "No win";
    }

    SetWindowText(hwndResult, resultText.c_str());

    char buf[32];
    sprintf(buf, "Balance: %d", balance);
    SetWindowText(hwndBalance, buf);

    spinning = false;
    EnableWindow(hwndSpin, TRUE);

    delete[] results;
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static string slot1, slot2, slot3;
    static int spinCount = 0;

    switch (message) {
        case WM_CREATE:
            srand(time(NULL));

            CreateWindow("STATIC", "老虎机", WS_CHILD | WS_VISIBLE | SS_CENTER,
                120, 10, 140, 30, hwnd, NULL, hInst, NULL);

            hwndBalance = CreateWindow("STATIC", "余额: 100", WS_CHILD | WS_VISIBLE | SS_CENTER,
                140, 45, 100, 25, hwnd, NULL, hInst, NULL);

            hwndSpin = CreateWindow("BUTTON", "旋转 (10分)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                130, 250, 120, 35, hwnd, (HMENU)1, hInst, NULL);

            hwndResult = CreateWindow("STATIC", "点击旋转开始游戏", WS_CHILD | WS_VISIBLE | SS_CENTER,
                100, 290, 180, 25, hwnd, NULL, hInst, NULL);

            slot1 = slot2 = slot3 = symbols[0];
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1 && !spinning) {
                if (balance < 10) {
                    MessageBox(hwnd, "余额不足!", "游戏结束", MB_OK);
                    break;
                }

                spinning = true;
                balance -= 10;
                char buf[32];
                sprintf(buf, "余额: %d", balance);
                SetWindowText(hwndBalance, buf);
                EnableWindow(hwndSpin, FALSE);
                SetWindowText(hwndResult, "旋转中...");

                string* results = new string[3];
                CreateThread(NULL, 0, SpinThread, results, 0, NULL);
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            DrawSlot(hdc, 60, 80, slot1, false);
            DrawSlot(hdc, 150, 80, slot2, false);
            DrawSlot(hdc, 240, 80, slot3, false);

            EndPaint(hwnd,&ps);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}