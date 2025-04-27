#include <windows.h>

#include "WindowsManager.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WindowsManager& wndManager = WindowsManager::getInstance();
    return wndManager.WindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    WindowsManager& wndManager = WindowsManager::getInstance();
    wndManager.LoadKeys(".env");

    const wchar_t CLASS_NAME[] = L"HotkeyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"HotkeyManagerApp", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);
    if (!hwnd) return 1;

    // register keys
    for (const auto& [id, hk] : wndManager.keyMng->hotkeyMap) {
        if (!RegisterHotKey(hwnd, id, hk.modifiers, hk.key)) {
            MessageBox(NULL, L"error register key", L"Error", MB_OK | MB_ICONERROR);
        }
    }

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    for (const auto& [id, _] : wndManager.keyMng->hotkeyMap) {
        UnregisterHotKey(hwnd, id);
    }

    return 0;
}
