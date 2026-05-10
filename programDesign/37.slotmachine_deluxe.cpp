#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <algorithm>

#pragma comment(lib, "gdiplus.lib")

using std::string;
using std::vector;
using std::wstring;
using namespace Gdiplus;

namespace {

const int kWindowWidth = 1040;
const int kWindowHeight = 700;
const int kReelCount = 3;
const int kSymbolCount = 10;
const int kSpinCost = 20;

struct SymbolInfo {
    const char* name;
    const char* fileName;
    COLORREF primary;
    COLORREF secondary;
    COLORREF accent;
    int tripleReward;
    int pairReward;
    double cropX;
    double cropY;
    double cropW;
    double cropH;
};

const SymbolInfo kSymbols[kSymbolCount] = {
    {"Conan",    "conan.png",    RGB(95, 160, 255),  RGB(235, 245, 255), RGB(220, 70, 70),  260,  80, 0.08, 0.02, 0.84, 0.68},
    {"Ran",      "ran.png",      RGB(255, 130, 180), RGB(255, 240, 248), RGB(70, 120, 255), 220,  70, 0.10, 0.02, 0.80, 0.72},
    {"Ai",       "ai.png",       RGB(180, 150, 255), RGB(242, 238, 255), RGB(110, 220, 255),300,  90, 0.10, 0.02, 0.80, 0.70},
    {"Kaito",    "kaito.png",    RGB(255, 255, 255), RGB(225, 240, 255), RGB(80, 180, 255), 420, 120, 0.08, 0.00, 0.84, 0.72},
    {"Heiji",    "heiji.png",    RGB(120, 220, 120), RGB(240, 255, 240), RGB(255, 210, 80), 240,  75, 0.10, 0.02, 0.80, 0.72},
    {"Shinichi", "shinichi.png", RGB(90, 200, 255),  RGB(235, 250, 255), RGB(255, 150, 60), 320, 100, 0.12, 0.02, 0.76, 0.74},
    {"Gin",      "gin.png",      RGB(190, 190, 205), RGB(240, 240, 250), RGB(180, 70, 255), 520, 150, 0.12, 0.00, 0.76, 0.74},
    {"Vermouth", "vermouth.png", RGB(255, 210, 120), RGB(255, 246, 226), RGB(255, 95, 160), 360, 110, 0.06, 0.00, 0.88, 0.78},
    {"Kogoro",   "kogoro.png",   RGB(110, 170, 255), RGB(235, 245, 255), RGB(255, 220, 100),280,  85, 0.12, 0.02, 0.76, 0.72},
    {"Kazuha",   "kazuha.png",   RGB(255, 170, 120), RGB(255, 244, 232), RGB(110, 220, 170),260,  80, 0.08, 0.02, 0.84, 0.74}
};

struct ReelState {
    int symbol = 0;
    bool locked = false;
};

struct Particle {
    int x = 0;
    int y = 0;
    int vx = 0;
    int vy = 0;
    int life = 0;
    COLORREF color = RGB(255, 255, 255);
};

HWND gMainWindow = nullptr;
RECT gSpinButton = {0, 0, 0, 0};
RECT gResetButton = {0, 0, 0, 0};
RECT gMinusButton = {0, 0, 0, 0};
RECT gPlusButton = {0, 0, 0, 0};
RECT gApplyButton = {0, 0, 0, 0};

ReelState gReels[kReelCount];
vector<Particle> gParticles;
int gBalance = 200;
int gConfiguredStartBalance = 200;
int gLastWin = 0;
int gSpinStep = 0;
bool gSpinning = false;
bool gConfigured = false;
bool gGameOver = false;
string gMessage = "Set the starting credits, then press APPLY";
DWORD gStopTick[kReelCount] = {0, 0, 0};
ULONG_PTR gGdiToken = 0;
Image* gPortraits[kSymbolCount] = {nullptr};

wstring ToWide(const string& text) {
    int size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    if (size <= 0) {
        return L"";
    }
    vector<wchar_t> buffer(static_cast<size_t>(size));
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, buffer.data(), size);
    return wstring(buffer.data());
}

string GetExeDir() {
    char path[MAX_PATH] = {};
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    string full(path);
    size_t pos = full.find_last_of("\\/");
    return pos == string::npos ? "." : full.substr(0, pos);
}

COLORREF MixColor(COLORREF a, COLORREF b, int ratio) {
    int inv = 255 - ratio;
    int r = (GetRValue(a) * inv + GetRValue(b) * ratio) / 255;
    int g = (GetGValue(a) * inv + GetGValue(b) * ratio) / 255;
    int bl = (GetBValue(a) * inv + GetBValue(b) * ratio) / 255;
    return RGB(r, g, bl);
}

void FillVerticalGradient(HDC hdc, const RECT& rc, COLORREF top, COLORREF bottom) {
    int height = rc.bottom - rc.top;
    if (height <= 0) {
        return;
    }
    for (int i = 0; i < height; ++i) {
        int ratio = (255 * i) / height;
        HPEN pen = CreatePen(PS_SOLID, 1, MixColor(top, bottom, ratio));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        MoveToEx(hdc, rc.left, rc.top + i, nullptr);
        LineTo(hdc, rc.right, rc.top + i);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }
}

void FillHorizontalGradient(HDC hdc, const RECT& rc, COLORREF leftColor, COLORREF rightColor) {
    int width = rc.right - rc.left;
    if (width <= 0) {
        return;
    }
    for (int i = 0; i < width; ++i) {
        int ratio = (255 * i) / width;
        HPEN pen = CreatePen(PS_SOLID, 1, MixColor(leftColor, rightColor, ratio));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        MoveToEx(hdc, rc.left + i, rc.top, nullptr);
        LineTo(hdc, rc.left + i, rc.bottom);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }
}

void DrawCenteredText(HDC hdc, const RECT& rc, const string& text, COLORREF color, int fontSize, int weight) {
    HFONT font = CreateFontA(
        fontSize, 0, 0, 0, weight, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        VARIABLE_PITCH, "Segoe UI"
    );
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    DrawTextA(hdc, text.c_str(), -1, const_cast<RECT*>(&rc), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, oldFont);
    DeleteObject(font);
}

void DrawGlowFrame(HDC hdc, const RECT& rc, COLORREF color, int thickness, int radius) {
    for (int i = thickness; i >= 1; --i) {
        HPEN pen = CreatePen(PS_SOLID, 1, MixColor(color, RGB(0, 0, 0), 40 + i * 18));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
        RoundRect(hdc, rc.left - i, rc.top - i, rc.right + i, rc.bottom + i, radius, radius);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(pen);
    }
}

void DrawButton(HDC hdc, const RECT& rc, const string& label, COLORREF leftColor, COLORREF rightColor, bool active) {
    FillHorizontalGradient(hdc, rc, leftColor, rightColor);
    DrawGlowFrame(hdc, rc, active ? RGB(255, 255, 255) : RGB(110, 110, 110), 3, 24);
    DrawCenteredText(hdc, rc, label, RGB(20, 20, 28), 24, FW_BOLD);
}

void AddBurst(int cx, int cy, COLORREF color, int count) {
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.x = cx;
        p.y = cy;
        p.vx = (rand() % 15) - 7;
        p.vy = -(rand() % 12) - 2;
        p.life = 18 + rand() % 12;
        p.color = color;
        gParticles.push_back(p);
    }
}

void UpdateParticles() {
    for (size_t i = 0; i < gParticles.size();) {
        Particle& p = gParticles[i];
        p.x += p.vx;
        p.y += p.vy;
        p.vy += 1;
        --p.life;
        if (p.life <= 0) {
            gParticles.erase(gParticles.begin() + static_cast<long long>(i));
        } else {
            ++i;
        }
    }
}

void RandomizeReels() {
    for (int i = 0; i < kReelCount; ++i) {
        gReels[i].symbol = rand() % kSymbolCount;
        gReels[i].locked = false;
    }
}

bool LoadPortraits() {
    string baseDir = GetExeDir() + "\\slot_assets\\";
    for (int i = 0; i < kSymbolCount; ++i) {
        delete gPortraits[i];
        gPortraits[i] = nullptr;
        wstring path = ToWide(baseDir + kSymbols[i].fileName);
        Image* image = new Image(path.c_str());
        if (image->GetLastStatus() != Ok) {
            delete image;
            return false;
        }
        gPortraits[i] = image;
    }
    return true;
}

void ReleasePortraits() {
    for (int i = 0; i < kSymbolCount; ++i) {
        delete gPortraits[i];
        gPortraits[i] = nullptr;
    }
}

void ApplyConfiguredBalance() {
    gBalance = gConfiguredStartBalance;
    gLastWin = 0;
    gSpinStep = 0;
    gSpinning = false;
    gConfigured = true;
    gGameOver = false;
    gParticles.clear();
    RandomizeReels();
    KillTimer(gMainWindow, 1);
    gMessage = "Starting credits applied. Pull the lever!";
}

int EvaluateWin() {
    const int a = gReels[0].symbol;
    const int b = gReels[1].symbol;
    const int c = gReels[2].symbol;

    if (a == b && b == c) {
        return kSymbols[a].tripleReward;
    }
    if (a == b || b == c || a == c) {
        int pairSymbol = a == b ? a : (b == c ? b : a);
        return kSymbols[pairSymbol].pairReward;
    }

    int detectiveSet = 0;
    detectiveSet += (a == 0 || a == 3 || a == 5) ? 1 : 0;
    detectiveSet += (b == 0 || b == 3 || b == 5) ? 1 : 0;
    detectiveSet += (c == 0 || c == 3 || c == 5) ? 1 : 0;
    if (detectiveSet == 3) {
        return 55;
    }

    int orgSet = 0;
    orgSet += (a == 2 || a == 6 || a == 7) ? 1 : 0;
    orgSet += (b == 2 || b == 6 || b == 7) ? 1 : 0;
    orgSet += (c == 2 || c == 6 || c == 7) ? 1 : 0;
    if (orgSet == 3) {
        return 45;
    }

    return 0;
}

void FinishSpin() {
    gSpinning = false;
    gLastWin = EvaluateWin();
    gBalance += gLastWin;

    if (gLastWin >= 300) {
        gMessage = "Jackpot! Triple character match!";
        AddBurst(520, 185, RGB(255, 215, 0), 80);
    } else if (gLastWin >= 80) {
        gMessage = "Strong combo! Matching pair payout collected!";
        AddBurst(520, 185, RGB(120, 255, 220), 48);
    } else if (gLastWin > 0) {
        gMessage = "Special team bonus unlocked!";
        AddBurst(520, 185, RGB(255, 140, 180), 32);
    } else {
        gMessage = "No reward this time.";
    }

    if (gBalance < kSpinCost) {
        gGameOver = true;
        if (gLastWin == 0) {
            gMessage = "Game over. Credits are below the lever cost.";
        } else {
            gMessage += " Credits are now too low for another pull.";
        }
    }
}

void StartSpin() {
    if (!gConfigured) {
        gMessage = "Choose starting credits and press APPLY first.";
        return;
    }
    if (gGameOver) {
        gMessage = "Game over. Press RESET or change the starting credits.";
        return;
    }
    if (gSpinning) {
        return;
    }
    if (gBalance < kSpinCost) {
        gGameOver = true;
        gMessage = "Game over. Credits are below the lever cost.";
        return;
    }

    gBalance -= kSpinCost;
    gLastWin = 0;
    gSpinning = true;
    gSpinStep = 0;
    gMessage = "Lever pulled. Reels are rolling...";
    DWORD now = GetTickCount();
    gStopTick[0] = now + 1200;
    gStopTick[1] = now + 1850;
    gStopTick[2] = now + 2500;
    for (int i = 0; i < kReelCount; ++i) {
        gReels[i].locked = false;
    }
    SetTimer(gMainWindow, 1, 40, nullptr);
}

void ResetGame() {
    ApplyConfiguredBalance();
    gMessage = "Credits reset to the selected starting value.";
}

void UpdateSpin() {
    if (!gSpinning) {
        UpdateParticles();
        return;
    }

    DWORD now = GetTickCount();
    ++gSpinStep;
    for (int i = 0; i < kReelCount; ++i) {
        if (!gReels[i].locked) {
            gReels[i].symbol = (gReels[i].symbol + 1 + rand() % 3) % kSymbolCount;
            if (now >= gStopTick[i]) {
                gReels[i].locked = true;
                AddBurst(365 + i * 155, 320, kSymbols[gReels[i].symbol].accent, 18);
            }
        }
    }

    UpdateParticles();

    if (gReels[0].locked && gReels[1].locked && gReels[2].locked) {
        KillTimer(gMainWindow, 1);
        FinishSpin();
    }
}

void DrawPortrait(Graphics& graphics, const RECT& rc, int symbol) {
    Image* image = gPortraits[symbol];
    if (!image) {
        return;
    }

    const SymbolInfo& info = kSymbols[symbol];
    REAL srcX = static_cast<REAL>(image->GetWidth() * info.cropX);
    REAL srcY = static_cast<REAL>(image->GetHeight() * info.cropY);
    REAL srcW = static_cast<REAL>(image->GetWidth() * info.cropW);
    REAL srcH = static_cast<REAL>(image->GetHeight() * info.cropH);

    GraphicsPath clip;
    clip.AddEllipse(
        static_cast<INT>(rc.left + 12),
        static_cast<INT>(rc.top + 4),
        static_cast<INT>(rc.right - rc.left - 24),
        static_cast<INT>(rc.bottom - rc.top - 8)
    );
    GraphicsState state = graphics.Save();
    graphics.SetClip(&clip);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(SmoothingModeHighQuality);
    graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
    graphics.Clear(Color(0, 0, 0, 0));
    graphics.DrawImage(
        image,
        Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top),
        srcX, srcY, srcW, srcH, UnitPixel
    );
    graphics.Restore(state);

    Pen ring(Color(240, GetRValue(info.accent), GetGValue(info.accent), GetBValue(info.accent)), 3.0f);
    graphics.DrawEllipse(
        &ring,
        static_cast<INT>(rc.left + 12),
        static_cast<INT>(rc.top + 4),
        static_cast<INT>(rc.right - rc.left - 24),
        static_cast<INT>(rc.bottom - rc.top - 8)
    );
}

void DrawReel(HDC hdc, const RECT& rc, int symbol, bool locked) {
    RECT outer = rc;
    FillVerticalGradient(hdc, outer, RGB(20, 30, 50), RGB(6, 10, 24));
    DrawGlowFrame(hdc, outer, locked ? RGB(255, 215, 0) : RGB(0, 230, 255), 4, 28);

    RECT inner = {rc.left + 14, rc.top + 14, rc.right - 14, rc.bottom - 14};
    FillVerticalGradient(hdc, inner, kSymbols[symbol].secondary, RGB(190, 220, 255));
    DrawGlowFrame(hdc, inner, RGB(255, 255, 255), 1, 20);

    Graphics graphics(hdc);
    RECT portrait = {inner.left + 4, inner.top + 8, inner.right - 4, inner.bottom - 48};
    DrawPortrait(graphics, portrait, symbol);

    RECT nameRect = {inner.left, inner.bottom - 52, inner.right, inner.bottom - 24};
    DrawCenteredText(hdc, nameRect, kSymbols[symbol].name, kSymbols[symbol].accent, 18, FW_HEAVY);

    RECT footer = {inner.left, inner.bottom - 28, inner.right, inner.bottom - 4};
    DrawCenteredText(hdc, footer, locked ? "LOCKED" : "ROLLING", RGB(30, 45, 90), 14, FW_BOLD);
}

string RewardLineOne() {
    return "Triple rewards: Conan 260  Ran 220  Ai 300  Kaito 420  Heiji 240";
}

string RewardLineTwo() {
    return "More triples: Shinichi 320  Gin 520  Vermouth 360  Kogoro 280  Kazuha 260";
}

string RewardLineThree() {
    return "Pair rewards follow each character. Detective team bonus: Conan/Kaito/Shinichi = 55";
}

string RewardLineFour() {
    return "Organization bonus: Ai/Gin/Vermouth = 45. Pull cost = 20 each round";
}

void PaintScene(HDC hdc, const RECT& client) {
    FillVerticalGradient(hdc, client, RGB(5, 8, 24), RGB(24, 8, 42));

    RECT frame = {40, 40, 980, 650};
    DrawGlowFrame(hdc, frame, RGB(255, 70, 150), 5, 40);

    RECT header = {120, 48, 900, 120};
    DrawCenteredText(hdc, header, "NEON SLOT MACHINE", RGB(255, 245, 140), 42, FW_HEAVY);

    RECT sub = {120, 100, 900, 138};
    DrawCenteredText(hdc, sub, "Detective Conan real portrait edition", RGB(140, 230, 255), 18, FW_SEMIBOLD);

    RECT machine = {250, 150, 790, 450};
    FillVerticalGradient(hdc, machine, RGB(35, 40, 70), RGB(12, 14, 28));
    DrawGlowFrame(hdc, machine, RGB(0, 220, 255), 4, 36);

    for (int i = 0; i < 18; ++i) {
        int x = 260 + i * 29;
        int y1 = 162;
        int y2 = 436;
        COLORREF bulbColor = (gSpinStep / 2 + i) % 2 == 0 ? RGB(255, 210, 60) : RGB(255, 90, 140);
        HBRUSH brush = CreateSolidBrush(bulbColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN oldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
        Ellipse(hdc, x, y1, x + 14, y1 + 14);
        Ellipse(hdc, x, y2, x + 14, y2 + 14);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
    }

    for (int i = 0; i < kReelCount; ++i) {
        RECT reel = {305 + i * 155, 215, 425 + i * 155, 395};
        DrawReel(hdc, reel, gReels[i].symbol, gReels[i].locked || !gSpinning);
    }

    RECT balancePanel = {70, 170, 205, 270};
    FillVerticalGradient(hdc, balancePanel, RGB(28, 40, 70), RGB(8, 15, 30));
    DrawGlowFrame(hdc, balancePanel, RGB(120, 255, 220), 3, 24);
    DrawCenteredText(hdc, balancePanel, "CREDITS", RGB(120, 255, 220), 20, FW_BOLD);
    RECT balanceValue = {70, 205, 205, 270};
    std::ostringstream balanceStream;
    balanceStream << gBalance;
    DrawCenteredText(hdc, balanceValue, balanceStream.str(), RGB(255, 245, 140), 30, FW_HEAVY);

    RECT setupPanel = {58, 300, 215, 468};
    FillVerticalGradient(hdc, setupPanel, RGB(32, 28, 66), RGB(10, 10, 28));
    DrawGlowFrame(hdc, setupPanel, RGB(255, 110, 210), 3, 24);
    RECT setupTitle = {58, 309, 215, 340};
    DrawCenteredText(hdc, setupTitle, "START", RGB(255, 160, 220), 20, FW_BOLD);
    RECT setupValue = {58, 338, 215, 385};
    std::ostringstream startStream;
    startStream << gConfiguredStartBalance;
    DrawCenteredText(hdc, setupValue, startStream.str(), RGB(255, 245, 140), 28, FW_HEAVY);
    gMinusButton = {74, 390, 118, 430};
    gPlusButton = {155, 390, 199, 430};
    gApplyButton = {83, 436, 190, 462};
    DrawButton(hdc, gMinusButton, "-", RGB(255, 170, 170), RGB(255, 100, 130), !gSpinning);
    DrawButton(hdc, gPlusButton, "+", RGB(170, 255, 220), RGB(90, 170, 255), !gSpinning);
    DrawButton(hdc, gApplyButton, "APPLY", RGB(255, 235, 140), RGB(255, 150, 90), !gSpinning);

    RECT payoutPanel = {825, 170, 960, 270};
    FillVerticalGradient(hdc, payoutPanel, RGB(55, 28, 70), RGB(18, 8, 25));
    DrawGlowFrame(hdc, payoutPanel, RGB(255, 120, 200), 3, 24);
    DrawCenteredText(hdc, payoutPanel, "WIN", RGB(255, 140, 210), 20, FW_BOLD);
    RECT payoutValue = {825, 205, 960, 270};
    std::ostringstream winStream;
    winStream << gLastWin;
    DrawCenteredText(hdc, payoutValue, winStream.str(), RGB(255, 245, 140), 30, FW_HEAVY);

    RECT infoPanel = {812, 300, 977, 430};
    FillVerticalGradient(hdc, infoPanel, RGB(26, 44, 78), RGB(10, 18, 36));
    DrawGlowFrame(hdc, infoPanel, RGB(110, 220, 255), 3, 22);
    RECT infoTitle = {817, 307, 972, 334};
    DrawCenteredText(hdc, infoTitle, "RULES", RGB(220, 240, 255), 18, FW_BOLD);
    RECT info1 = {820, 336, 968, 360};
    RECT info2 = {820, 360, 968, 384};
    RECT info3 = {820, 384, 968, 408};
    RECT info4 = {820, 408, 968, 430};
    DrawCenteredText(hdc, info1, "Triple = major reward", RGB(255, 240, 170), 14, FW_MEDIUM);
    DrawCenteredText(hdc, info2, "Pair = character reward", RGB(255, 240, 170), 14, FW_MEDIUM);
    DrawCenteredText(hdc, info3, "Credits < 20 => game over", RGB(255, 240, 170), 14, FW_MEDIUM);
    DrawCenteredText(hdc, info4, "APPLY sets the new start money", RGB(255, 240, 170), 13, FW_MEDIUM);

    gSpinButton = {290, 500, 495, 565};
    gResetButton = {545, 500, 750, 565};
    DrawButton(hdc, gSpinButton, gSpinning ? "PULLING..." : "PULL", RGB(255, 220, 100), RGB(255, 90, 100), !gSpinning && !gGameOver);
    DrawButton(hdc, gResetButton, "RESET", RGB(120, 255, 220), RGB(70, 140, 255), true);

    RECT reward1 = {230, 148, 810, 174};
    RECT reward2 = {230, 448, 810, 472};
    RECT reward3 = {150, 585, 890, 608};
    RECT reward4 = {150, 610, 890, 633};
    DrawCenteredText(hdc, reward1, RewardLineOne(), RGB(255, 220, 120), 13, FW_MEDIUM);
    DrawCenteredText(hdc, reward2, RewardLineTwo(), RGB(160, 230, 255), 13, FW_MEDIUM);
    DrawCenteredText(hdc, reward3, RewardLineThree(), RGB(255, 220, 180), 13, FW_MEDIUM);
    DrawCenteredText(hdc, reward4, RewardLineFour(), RGB(255, 220, 180), 13, FW_MEDIUM);

    RECT messagePanel = {145, 650, 895, 680};
    DrawCenteredText(hdc, messagePanel, gMessage, RGB(255, 255, 255), 18, FW_SEMIBOLD);

    if (gGameOver) {
        RECT over = {280, 255, 760, 360};
        FillVerticalGradient(hdc, over, RGB(90, 20, 30), RGB(35, 8, 12));
        DrawGlowFrame(hdc, over, RGB(255, 100, 100), 4, 28);
        DrawCenteredText(hdc, over, "GAME OVER", RGB(255, 235, 160), 36, FW_HEAVY);
    }

    for (size_t i = 0; i < gParticles.size(); ++i) {
        const Particle& p = gParticles[i];
        int size = 4 + p.life / 6;
        HBRUSH brush = CreateSolidBrush(p.color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
        HPEN oldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
        Ellipse(hdc, p.x - size, p.y - size, p.x + size, p.y + size);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(brush);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            srand(static_cast<unsigned int>(time(nullptr)));
            RandomizeReels();
            gBalance = gConfiguredStartBalance;
            if (!LoadPortraits()) {
                MessageBoxA(hwnd, "Failed to load portrait images from slot_assets.", "Asset Error", MB_ICONERROR);
                PostQuitMessage(1);
            }
            return 0;

        case WM_TIMER:
            UpdateSpin();
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;

        case WM_LBUTTONDOWN: {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            if (PtInRect(&gSpinButton, pt)) {
                StartSpin();
            } else if (PtInRect(&gResetButton, pt)) {
                ResetGame();
            } else if (PtInRect(&gMinusButton, pt) && !gSpinning) {
                gConfiguredStartBalance = std::max(20, gConfiguredStartBalance - 20);
                if (!gConfigured) {
                    gBalance = gConfiguredStartBalance;
                }
            } else if (PtInRect(&gPlusButton, pt) && !gSpinning) {
                gConfiguredStartBalance = std::min(2000, gConfiguredStartBalance + 20);
                if (!gConfigured) {
                    gBalance = gConfiguredStartBalance;
                }
            } else if (PtInRect(&gApplyButton, pt) && !gSpinning) {
                ApplyConfiguredBalance();
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }

        case WM_KEYDOWN:
            if (wParam == VK_SPACE) {
                StartSpin();
            } else if (wParam == 'R') {
                ResetGame();
            } else if ((wParam == VK_UP || wParam == VK_RIGHT) && !gSpinning) {
                gConfiguredStartBalance = std::min(2000, gConfiguredStartBalance + 20);
            } else if ((wParam == VK_DOWN || wParam == VK_LEFT) && !gSpinning) {
                gConfiguredStartBalance = std::max(20, gConfiguredStartBalance - 20);
            } else if (wParam == VK_RETURN && !gSpinning) {
                ApplyConfiguredBalance();
            }
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT client;
            GetClientRect(hwnd, &client);

            HDC memoryDc = CreateCompatibleDC(hdc);
            HBITMAP bitmap = CreateCompatibleBitmap(hdc, client.right, client.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memoryDc, bitmap);

            PaintScene(memoryDc, client);
            BitBlt(hdc, 0, 0, client.right, client.bottom, memoryDc, 0, 0, SRCCOPY);

            SelectObject(memoryDc, oldBitmap);
            DeleteObject(bitmap);
            DeleteDC(memoryDc);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            KillTimer(hwnd, 1);
            ReleasePortraits();
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

}  // namespace

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand) {
    GdiplusStartupInput gdiplusStartupInput;
    if (GdiplusStartup(&gGdiToken, &gdiplusStartupInput, nullptr) != Ok) {
        MessageBoxA(nullptr, "Failed to initialize GDI+.", "Error", MB_ICONERROR);
        return 1;
    }

    WNDCLASSEXA wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursor(nullptr, IDC_HAND);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "NeonSlotMachineWindow";

    if (!RegisterClassExA(&wc)) {
        GdiplusShutdown(gGdiToken);
        MessageBoxA(nullptr, "Failed to register window class.", "Error", MB_ICONERROR);
        return 1;
    }

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT desired = {0, 0, kWindowWidth, kWindowHeight};
    AdjustWindowRect(&desired, style, FALSE);

    gMainWindow = CreateWindowExA(
        0,
        wc.lpszClassName,
        "Neon Slot Machine",
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        desired.right - desired.left,
        desired.bottom - desired.top,
        nullptr,
        nullptr,
        instance,
        nullptr
    );

    if (!gMainWindow) {
        GdiplusShutdown(gGdiToken);
        MessageBoxA(nullptr, "Failed to create main window.", "Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(gMainWindow, showCommand);
    UpdateWindow(gMainWindow);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gGdiToken);
    return static_cast<int>(msg.wParam);
}
