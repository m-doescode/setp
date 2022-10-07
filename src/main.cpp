#include <iostream>
#include <locale>
#include <codecvt>
#include "CLI/CLI.hpp"

#include "path_operations.h"

using std::string;
using std::wstring;

using std::cerr;
using std::cout;
using std::endl;

using namespace setp::path_operations;

CLI::App app;

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

// CLI arguments
string o_path;
path_location_t f_location = path_location_t::COMPUTER;
path_position_t f_position = path_position_t::LAST;
bool f_remove = false;
bool f_selfinstall = false;

int main(int argc, char **argv)
{
    app.description("Add a path to the PATH environment variable. Requires administrator privileges. If you do not have administrator access, you may use the -u flag to add it to the user environment instead.");
    app.add_option("path", o_path, "The path to be added to the PATH environment variable. Use quotation marks for paths with spaces")
        ->type_name("PATH");
    app.add_flag("-u,--user", f_location, "Adds the path to the user only, instead of on the Local PC");
    app.add_flag("-f,--first", f_position, "Adds the path to the beginning of the PATH");
    app.add_flag("-r,--remove", f_remove, "Removes the specified path from the PATH");
    app.add_flag("-i,--install", f_selfinstall, "Installs the location of the setp binary and adds it to PATH");

    CLI11_PARSE(app, argc, argv);

    if (!f_selfinstall && o_path.empty())
    {
        cout << "path is a required argument." << endl;
        return -1;
    }

    wstring wpath = converter.from_bytes(o_path);

    if (f_selfinstall)
        setp::path_operations::install_self();
    else if (f_remove)
        setp::path_operations::remove_path(wpath, f_location);
    else
        setp::path_operations::add_path(wpath, f_location, f_position);


    return 0;
}