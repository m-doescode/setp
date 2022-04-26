#include <windows.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include "CLI/CLI.hpp"


using std::string;
using std::wstring;

using std::cout;
using std::cerr;
using std::endl;

CLI::App app;

// CLI arguments
string o_path;
bool f_user_only = false;
bool f_first = false;
bool f_remove = false;

void report_error();
void verify_privileges();

void add_path_user(string path, bool first = false);
void add_path_pc(string path, bool first = false);
void remove_path_user(string path);
void remove_path_pc(string path);

int main(int argc, char **argv)
{
    app.add_option("path", o_path, "The path to be added to the PATH environment variable")->required()->type_name("PATH");
    app.add_flag("-u,--user", f_user_only, "Adds the path to the user only, instead of on the Local PC");
    auto first_flag = app.add_flag("-f,--first", f_first, "Adds the path to the beginning of the PATH environment variable");
    app.add_flag("-r,--remove", f_remove, "Removes the specified path from the PATH environment variable")->excludes(first_flag);

    CLI11_PARSE(app, argc, argv);

    if (!f_remove)
        if (f_user_only)
            add_path_user(o_path, f_first);
        else
            add_path_pc(o_path, f_first);
    else
        if (f_user_only)
            remove_path_user(o_path);
        else
            remove_path_pc(o_path);

    return 0;
}

void report_error()
{
    LPSTR message = NULL;
    int error_id = GetLastError();
    if (!error_id)
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
    if( OpenProcessToken( GetCurrentProcess( ),TOKEN_QUERY,&hToken ) ) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof( TOKEN_ELEVATION );
        if( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) ) {
            elevated = Elevation.TokenIsElevated;
        }
    }
    if( hToken ) {
        CloseHandle( hToken );
    }
    
    if (!elevated)
    {
        cout << "Insufficient privileges. Cannot add path to Local PC without Administrator privileges. Use --user to only add path to current user" << endl;
        exit(ERROR_ACCESS_DENIED);
    }
}