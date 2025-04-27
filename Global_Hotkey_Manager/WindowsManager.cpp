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
            

            HWND hwndActive = GetForegroundWindow();

            if (act == L"exit") {
                BaseFunction::closeApp();
            }
            else if (act.find(L".exe") != std::wstring::npos) {
                BaseFunction::openApplication(act);
            }
            else if (act == L"minimize") {
                BaseFunction::minimizeApp(hwndActive);
            }
            else if (act == L"close") {  
                BaseFunction::closeApp(hwndActive);
            }
            else if (act == L"reload") {
                BaseFunction::reloadApplication(hwndActive);
            }
            else if (act == L"toggleTopMost") {
                BaseFunction::toggleTopMost(hwndActive);
            }
            else if (act == L"sleep") {
                BaseFunction::enterSleepMode(false);
            }
            else {
				BaseFunction::enterBindText(act);
            }
        }
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_TRAY_EXIT:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            DestroyWindow(hwnd);
            PostQuitMessage(0);
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


