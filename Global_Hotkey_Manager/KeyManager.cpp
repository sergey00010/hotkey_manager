#include "KeyManager.h"

UINT KeyManager::ParseModifiers(const std::string& modStr) {
    UINT mods = 0;
    if (modStr.find("CTRL") != std::string::npos)  mods |= MOD_CONTROL;
    if (modStr.find("ALT") != std::string::npos)   mods |= MOD_ALT;
    if (modStr.find("SHIFT") != std::string::npos) mods |= MOD_SHIFT;
    if (modStr.find("WIN") != std::string::npos)   mods |= MOD_WIN;
    return mods;
}

UINT KeyManager::ParseKey(const std::string& keyStr) {
    if (keyStr.size() == 1 && isalpha(keyStr[0])) {
        return toupper(keyStr[0]); // A-Z
    }
    return 0;
}