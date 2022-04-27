#include <windows.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include "CLI/CLI.hpp"

using std::string;
using std::wstring;

using std::cerr;
using std::cout;
using std::endl;

const WCHAR *PC_ENV_PATH = L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
const WCHAR *USER_ENV_PATH = L"Environment";

const HKEY PC_ENV_HKEY = HKEY_LOCAL_MACHINE;
const HKEY USER_ENV_HKEY = HKEY_CURRENT_USER;

CLI::App app;

// CLI arguments
string o_path;
bool f_user_only = false;
bool f_first = false;
bool f_remove = false;

void report_error();
void verify_privileges();

void add_path(string path, bool first = false, bool user_only = false);
// void remove_path(string path);

int main(int argc, char **argv)
{
    app.add_option("path", o_path, "The path to be added to the PATH environment variable")->required()->type_name("PATH");
    app.add_flag("-u,--user", f_user_only, "Adds the path to the user only, instead of on the Local PC");
    auto first_flag = app.add_flag("-f,--first", f_first, "Adds the path to the beginning of the PATH environment variable");
    app.add_flag("-r,--remove", f_remove, "Removes the specified path from the PATH environment variable")->excludes(first_flag);

    CLI11_PARSE(app, argc, argv);

    if (!f_remove)
        add_path(o_path, f_first, f_user_only);
    // else if (f_user_only)
    //     remove_path(o_path);
    // else
    //     remove_path(o_path);

    return 0;
}

void report_error()
{
    LPSTR message = NULL;
    int error_id = GetLastError();
    if (error_id)
    {
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, error_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&message, 0, NULL);
        cerr << "ERROR: (" << error_id << ") " << message << endl;
        exit(1);
    }
}

// https://stackoverflow.com/a/8196291/16255372
void verify_privileges()
{
    BOOL elevated = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
        {
            elevated = Elevation.TokenIsElevated;
        }
    }
    if (hToken)
    {
        CloseHandle(hToken);
    }

    if (!elevated)
    {
        cout << "Insufficient privileges. Cannot add path to Local PC without Administrator privileges. Use --user to only add path to current user" << endl;
        exit(ERROR_ACCESS_DENIED);
    }
}

void add_path(string path, bool first, bool user_only)
{
    HKEY environment_hkey;
    const WCHAR *environment_path;
    if (!user_only)
    {
        environment_hkey = PC_ENV_HKEY;
        environment_path = PC_ENV_PATH;
    }
    else
    {
        environment_hkey = USER_ENV_HKEY;
        environment_path = USER_ENV_PATH;
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    // Open key
    HKEY environment_key;
    RegOpenKeyEx(environment_hkey, environment_path, 0, KEY_READ | KEY_WRITE | KEY_QUERY_VALUE, &environment_key);

    // Get current path and write to buffer
    DWORD old_path_size;
    WCHAR *old_path;
    RegQueryValueExW(environment_key, L"PATH", NULL, NULL, NULL, &old_path_size);
    old_path = (WCHAR *)new char[old_path_size + 1]; // Make room for null-byte
    RegQueryValueExW(environment_key, L"PATH", NULL, NULL, (LPBYTE)old_path, &old_path_size);

    // Append new path to buffer
    wstring path_w = converter.from_bytes(path);
    wstring new_path = (wstring)old_path + L";" + path_w;
    const WCHAR *new_path_buf = new_path.c_str();

    // Write new path to registry
    RegSetValueExW(environment_key, L"PATH", NULL, REG_EXPAND_SZ, (LPBYTE)new_path_buf, new_path.size() * sizeof(wchar_t) + 1);

    RegCloseKey(environment_key);
}