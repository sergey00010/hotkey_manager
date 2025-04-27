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
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_TRAY_EXIT:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            DestroyWindow(hwnd);
            break;

        case ID_TRAY_SETTINGS:
            MessageBox(hwnd, L"open settings", L"settings", MB_OK);
            break;
        }
        break;
    }
    case WM_DESTROY: {
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        break;
    }
    default: {
        if (uMsg == WM_APP + 1) { // Обработка кликов по иконке в трее
            switch (lParam) {
            case WM_RBUTTONUP:
                ShowContexMenu(hwnd);
                break;
            case WM_LBUTTONUP:
                break;
            }
        }
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WindowsManager::LoadKeys(const std::string& filename)
{
    FileManager::LoadEnv(filename, keyMng->hotkeyMap);
}

void WindowsManager::AddTrayIcon(HWND hwnd)
{
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY_APP_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_APP + 1; // Пользовательское сообщение
    nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APP_ICON));
    wcscpy_s(nid.szTip, L"Global Hotkey Manager"); // Описание при наведении

    Shell_NotifyIcon(NIM_ADD, &nid);
}

void WindowsManager::ShowContexMenu(HWND hwnd)
{
    POINT pt;
    GetCursorPos(&pt);

    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, ID_TRAY_SETTINGS, L"Settings");
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, L"Exit");

    SetForegroundWindow(hwnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
    PostMessage(hwnd, WM_NULL, 0, 0);
    DestroyMenu(hMenu);
}
