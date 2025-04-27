#include "WindowsManager.h"

LRESULT WindowsManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_HOTKEY: {
        int id = (int)wParam;
        auto it = keyMng->hotkeyMap.find(id);
        if (it != keyMng->hotkeyMap.end()) {
            const std::wstring& act = it->second.action;

            if (act == L"exit") {
                PostQuitMessage(0);
            }
            else if (act.find(L".exe") != std::wstring::npos) {
                ShellExecute(NULL, L"open", act.c_str(), NULL, NULL, SW_SHOWNORMAL);
            }
            else if (act == L"minimize") {
                HWND hwndActive = GetForegroundWindow();
                ShowWindow(hwndActive, SW_MINIMIZE);
            }
            else {
                for (wchar_t ch : act) {
                    INPUT ip[2] = {};
                    ip[0].type = INPUT_KEYBOARD;
                    ip[0].ki.wVk = 0;
                    ip[0].ki.wScan = ch;
                    ip[0].ki.dwFlags = KEYEVENTF_UNICODE;
                    SendInput(1, &ip[0], sizeof(INPUT));

                    ip[1] = ip[0];
                    ip[1].ki.dwFlags |= KEYEVENTF_KEYUP;
                    SendInput(1, &ip[1], sizeof(INPUT));
                } 
            }
        }
        break;
    }
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WindowsManager::LoadKeys(const std::string& filename)
{
    FileManager::LoadEnv(filename, keyMng->hotkeyMap);
}
