#include <windows.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

#include "helper.h"

// Helper funcs

void setp::helper::report_error()
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
void setp::helper::verify_privileges()
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