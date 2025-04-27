#pragma once

#include <Windows.h>
#include <string>
#include <map>

struct HotkeyAction {
    UINT modifiers; // modificators CTRL, ALT, SHIFT, WIN
    UINT key; // main key A-Z
    std::wstring action;
};

class KeyManager
{
public:
    std::map<int, HotkeyAction> hotkeyMap;
public:
    // Parses the string CTRL+ALT, CTRL+SHIFT, etc. into a WinAPI bitmask (MOD_CONTROL, MOD_ALT, etc.)
    static UINT ParseModifiers(const std::string& modStr);

    // Converts a letter (A, B, Z) into a virtual key code.
    static UINT ParseKey(const std::string& keyStr);

};

