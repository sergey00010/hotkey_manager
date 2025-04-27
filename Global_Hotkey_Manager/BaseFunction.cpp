#include "BaseFunction.h"

void BaseFunction::closeApp()
{
    PostQuitMessage(0);
}

void BaseFunction::openApplication(const std::wstring& act)
{
    ShellExecute(NULL, L"open", act.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void BaseFunction::minimizeApp(HWND& hwnd)
{
    ShowWindow(hwnd, SW_MINIMIZE);
}

void BaseFunction::closeApp(HWND& hwnd)
{
    if (hwnd) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }
}

void BaseFunction::enterBindText(const std::wstring& act)
{
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

void BaseFunction::reloadApplication(HWND& hwnd)
{
    if (hwnd) {
        // get size and position window
        RECT rect;
        if (GetWindowRect(hwnd, &rect)) {
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            int x = rect.left;
            int y = rect.top;

            // get path to bin file 
            DWORD pid;
            GetWindowThreadProcessId(hwnd, &pid);

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
            if (hProcess) {
                wchar_t exePath[MAX_PATH];
                if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH)) {
                    // close app
                    PostMessage(hwnd, WM_CLOSE, 0, 0);

                    // open app
                    ShellExecute(NULL, L"open", exePath, NULL, NULL, SW_SHOWNORMAL);
                }
                CloseHandle(hProcess);
            }
        }
    }
}

void BaseFunction::toggleTopMost(HWND& hwnd)
{
    //get current styles
    HWND hWndInsertAfter = (GetWindowLongPtr(hwnd, GWL_EXSTYLE) & WS_EX_TOPMOST)
        ? HWND_NOTOPMOST : HWND_TOPMOST;

    // set window like top-most and normal
    SetWindowPos(
        hwnd,
        hWndInsertAfter,       
        0, 0, 0, 0,            
        SWP_NOMOVE | SWP_NOSIZE
    );
}

void BaseFunction::enterSleepMode(bool forceHibernate)
{
    if (forceHibernate)
        setPrivilege(SE_SHUTDOWN_NAME, true);
    SetSuspendState(forceHibernate, FALSE, FALSE);
}

bool BaseFunction::setPrivilege(LPCTSTR privilege, bool enable)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return false;

    if (!LookupPrivilegeValue(NULL, privilege, &luid))
        return false;

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
    CloseHandle(hToken);

    return GetLastError() == ERROR_SUCCESS;
}

