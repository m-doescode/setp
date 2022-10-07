#include <string>

namespace setp::path_operations {
    enum class path_location_t {
        COMPUTER,
        LOCAL_USER
    };

    enum class path_position_t {
        LAST,
        FIRST
    };

    void add_path(std::wstring path, path_location_t location, path_position_t position = path_position_t::LAST);
    void remove_path(std::wstring path, path_location_t location);
    void install_self();
}