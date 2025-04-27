#pragma once

#include <Windows.h>
#include <string>
#include <memory>

#include "KeyManager.h"
#include "FileManager.h"

class WindowsManager
{
public:
	std::unique_ptr<KeyManager> keyMng = std::make_unique<KeyManager>();

public:
	static WindowsManager& getInstance() {
		static WindowsManager instance;
		return instance;
	}
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void LoadKeys(const std::string& filename);

private:
	WindowsManager() {}                            
	~WindowsManager() {}                           
	WindowsManager(const WindowsManager&) = delete;           
	WindowsManager& operator=(const WindowsManager&) = delete; 
};

