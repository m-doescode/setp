#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <iterator>
#include <numeric>

#include "helper.h"

#include "path_operations.h"

using std::cout;
using std::wcout;
using std::cerr;
using std::endl;

using std::string;
using std::wstring;

using std::vector;

using std::wregex;
using std::wsregex_iterator;
using std::wsmatch;

using std::accumulate;
using std::next;

using namespace setp::helper;
using namespace setp::path_operations;

#define PC_ENV_PATH L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"
#define USER_ENV_PATH L"Environment"

#define PC_ENV_HKEY HKEY_LOCAL_MACHINE
#define USER_ENV_HKEY HKEY_CURRENT_USER

//

wstring loc_to_str(path_location_t loc)
{
    switch (loc)
    {
    case path_location_t::COMPUTER:
        return L"Computer";
    case path_location_t::LOCAL_USER:
        return L"Current User";
    }
}

wstring pos_to_str(path_position_t pos)
{
    switch (pos)
    {
    case path_position_t::FIRST:
        return L"at the beginning";
    default:
        return L"";
    }
}

vector<wstring> path_to_vector(wstring raw_path)
{
    vector<wstring> path_vector;

    wregex path_regex(L"[^;]+");
    auto path_begin = wsregex_iterator(raw_path.begin(), raw_path.end(), path_regex);
    auto path_end = wsregex_iterator();

    for (wsregex_iterator i = path_begin; i != path_end; ++i) {
        wsmatch match = *i;
        path_vector.push_back(match.str());
    }

    return path_vector;
}

wstring vector_to_path(vector<wstring> path_vector)
{
    return accumulate(
        next(path_vector.begin()), 
        path_vector.end(), 
        path_vector[0], 
        [](wstring a, wstring b) {
            return a + L";" + b;
        }
    ) + L";";
}

//

// Impl

void setp::path_operations::add_path(std::wstring path, path_location_t location, path_position_t position)
{
    HKEY env_root_key;
    wstring env_path;

    switch (location)
    {
    case path_location_t::LOCAL_USER:
        env_path = USER_ENV_PATH;
        env_root_key = USER_ENV_HKEY;
        break;
    case path_location_t::COMPUTER:
        env_path = PC_ENV_PATH;
        env_root_key = PC_ENV_HKEY;
        break;
    }

    // Open key
    HKEY env_key;
    RegOpenKeyEx(env_root_key, env_path.c_str(), 0, KEY_READ | KEY_WRITE | KEY_QUERY_VALUE, &env_key);
    report_error();

    // Get current path and write to buffer
    DWORD old_path_size;
    WCHAR *old_path;
    RegQueryValueExW(env_key, L"PATH", NULL, NULL, NULL, &old_path_size);
    old_path = (WCHAR *)new char[old_path_size + 1]; // Make room for null-byte
    RegQueryValueExW(env_key, L"PATH", NULL, NULL, (LPBYTE)old_path, &old_path_size);
    report_error();

    // Convert to vector
    vector<wstring> path_vector = path_to_vector(old_path);

    // Add path
    switch (position) {
    case path_position_t::FIRST:
        path_vector.insert(path_vector.begin(), path);
        break;
    case path_position_t::LAST:
        path_vector.push_back(path);
        break;
    }

    // Convert to wstring
    wstring final_path = vector_to_path(path_vector);

    // Write path
    wcout << L"TEST MODE: " << final_path << endl;

    // Close key
    RegCloseKey(env_key);

    // Report success
    wcout << L"Successfully added \"" << path << L"\" to PATH of " << loc_to_str(location) << " " << pos_to_str(position) << endl;
}

void setp::path_operations::remove_path(std::wstring path, path_location_t location)
{

}

void setp::path_operations::install_self() {

}