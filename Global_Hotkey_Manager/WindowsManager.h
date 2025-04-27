#pragma once

#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT 1002
#define ID_TRAY_SETTINGS 1003


#include <Windows.h>
#include <string>
#include <memory>
#include <tchar.h>

#include "resource.h"
#include "KeyManager.h"
#include "FileManager.h"

class WindowsManager
{
public:
	std::unique_ptr<KeyManager> keyMng = std::make_unique<KeyManager>();
	NOTIFYICONDATA nid = {0};
	HMENU hMenu = nullptr; 

public:
	static WindowsManager& getInstance() {
		static WindowsManager instance;
		return instance;
	}
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void LoadKeys(const std::string& filename);

	void AddTrayIcon(HWND hwnd);
	void ShowContexMenu(HWND hwnd);

private:
	WindowsManager() {}                            
	~WindowsManager() {}                           
	WindowsManager(const WindowsManager&) = delete;           
	WindowsManager& operator=(const WindowsManager&) = delete; 
};

