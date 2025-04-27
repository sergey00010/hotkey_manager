#pragma once

#include <string>
#include <Windows.h>
#include <psapi.h>
#include <powrprof.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "PowrProf.lib")

class BaseFunction
{
public:
	static void closeApp();
	static void openApplication(const std::wstring& act);
	static void minimizeApp(HWND& hwnd);
	static void closeApp(HWND& hwnd);
	static void enterBindText(const std::wstring& act);
	static void reloadApplication(HWND& hwnd);
	static void toggleTopMost(HWND& hwnd);

	static void enterSleepMode(bool forceHibernate = false);
	static bool setPrivilege(LPCTSTR privilege, bool enable);
};

