use std::thread::panicking;

use winreg::RegKey;
use winreg::enums::{HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, KEY_WRITE};
use is_elevated::is_elevated;

pub enum Location {
    System,
    Local,
}

pub fn load_path_key(location: &Location) -> Vec<String> {
    let environment_key = match location {
        Location::Local => RegKey::predef(HKEY_CURRENT_USER).open_subkey(r"Environment").expect("Unable to open local user key."),
        Location::System => RegKey::predef(HKEY_LOCAL_MACHINE).open_subkey(r"SYSTEM\CurrentControlSet\Control\Session Manager\Environment").expect("Unable to open system environment key."),
    };

    let path_string: String = environment_key.get_value("Path").expect("Unable to read Path value.");

    return path_string.split_terminator(';').map(|s| s.to_owned()).collect();
}

pub fn save_path_key(location: &Location, paths: &Vec<String>) {
    if match location { Location::System => true, _ => false } && !is_elevated() {
        panic!("Cannot update system variables without elevation.");
    }

    let environment_key = match location {
        Location::Local => RegKey::predef(HKEY_CURRENT_USER).open_subkey_with_flags(r"Environment", KEY_WRITE).expect("Unable to open local user key."),
        Location::System => RegKey::predef(HKEY_LOCAL_MACHINE).open_subkey_with_flags(r"SYSTEM\CurrentControlSet\Control\Session Manager\Environment", KEY_WRITE).expect("Unable to open system environment key."),
    };

    let path_string = paths.join(";");

    environment_key.set_value("Path", &path_string).expect("Unable to save path string.");
}