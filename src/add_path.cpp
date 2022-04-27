#include <windows.h>
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

#include "common.h"

#define PC_ENV_PATH L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"
#define USER_ENV_PATH L"Environment"

#define PC_ENV_HKEY HKEY_LOCAL_MACHINE
#define USER_ENV_HKEY HKEY_CURRENT_USER

using std::string;
using std::wstring;
using std::wstring_convert;
using std::codecvt_utf8_utf16;
using std::cout;
using std::endl;

void report_error();
void verify_privileges();

void add_path(string path, bool first, bool user_only)
{
    HKEY environment_hkey;
    const WCHAR *environment_path;
    if (!user_only)
    {
        environment_hkey = PC_ENV_HKEY;
        environment_path = PC_ENV_PATH;
        verify_privileges();
    }
    else
    {
        environment_hkey = USER_ENV_HKEY;
        environment_path = USER_ENV_PATH;
    }

    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    // Open key
    HKEY environment_key;
    RegOpenKeyEx(environment_hkey, environment_path, 0, KEY_READ | KEY_WRITE | KEY_QUERY_VALUE, &environment_key);
    report_error();

    // Get current path and write to buffer
    DWORD old_path_size;
    WCHAR *old_path;
    RegQueryValueExW(environment_key, L"PATH", NULL, NULL, NULL, &old_path_size);
    old_path = (WCHAR *)new char[old_path_size + 1]; // Make room for null-byte
    RegQueryValueExW(environment_key, L"PATH", NULL, NULL, (LPBYTE)old_path, &old_path_size);
    report_error();

    // Append new path to buffer
    wstring path_w = converter.from_bytes(path);
    wstring new_path = first ? path_w + L";" + old_path : (wstring)old_path + L";" + path_w;
    const WCHAR *new_path_buf = new_path.c_str();

    // Write new path to registry
    RegSetValueExW(environment_key, L"PATH", NULL, REG_EXPAND_SZ, (LPBYTE)new_path_buf, new_path.size() * sizeof(wchar_t) + 1);
    report_error();

    RegCloseKey(environment_key);

    cout << "Successfully added \"" << path << "\" to PATH of " << (user_only ? "Current User" : "Local PC") << (first ? " at the beginning" : "") << endl;
}