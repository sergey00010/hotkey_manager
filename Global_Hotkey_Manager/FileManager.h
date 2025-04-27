#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include "KeyManager.h"

class FileManager
{
public:
    // Reads .env line by line and parses each line
    static void LoadEnv(const std::string& filename, std::map<int, HotkeyAction>& hotkeyMap);
};

