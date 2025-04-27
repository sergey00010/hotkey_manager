#include "FileManager.h"

// Reads .env line by line and parses each line
void FileManager::LoadEnv(const std::string& filename, std::map<int, HotkeyAction>& hotkeyMap) {
    std::ifstream file(filename);
    std::string line;
    int id = 1;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);

        size_t pipe = value.find('|');
        if (pipe == std::string::npos) continue;

        std::string hotkeyStr = value.substr(0, pipe);
        std::string action = value.substr(pipe + 1);

        size_t lastPlus = hotkeyStr.find_last_of('+');
        std::string modStr = hotkeyStr.substr(0, lastPlus);
        std::string keyStr = hotkeyStr.substr(lastPlus + 1);

        HotkeyAction ha;
        ha.modifiers = KeyManager::ParseModifiers(modStr);
        ha.key = KeyManager::ParseKey(keyStr);
        ha.action = std::wstring(action.begin(), action.end());

        if (ha.modifiers != 0 && ha.key != 0) {
            hotkeyMap[id] = ha;
            id++;
        }
    }
}