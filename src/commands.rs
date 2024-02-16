use crate::util::{ load_path_key, save_path_key, Location };

use dunce;

pub enum ListFrom {
    Merged,
    System,
    Local,
}

pub fn list(quiet: bool, location: ListFrom) {
    let mut paths: Vec<String> = vec![];

    match location {
        ListFrom::System => {
            paths.extend(load_path_key(&Location::System));
        }
        ListFrom::Local => {
            paths.extend(load_path_key(&Location::Local));
        }
        ListFrom::Merged => {
            paths.extend(load_path_key(&Location::System));
            paths.extend(load_path_key(&Location::Local));
        }
    }

    // Print info message
    if !quiet {
        match location {
            ListFrom::System => println!("Showing PATH entries for System:\n"),
            ListFrom::Local => println!("Showing PATH entries for {}:\n", whoami::username()),
            ListFrom::Merged => println!("Showing combined path entries for {}:\n", whoami::username())
        }
    }

    for entry in paths {
        println!("{}", entry);
    }
}

pub fn add(prepend: bool, resolve: bool, location: Location, preview: bool, paths: Vec<String>) {
    let current_paths: Vec<String> = load_path_key(&location);

    let mut new_paths: Vec<String> = vec![];

    let resolved_paths = if !resolve { paths } else {
        paths.into_iter().map(|path| dunce::canonicalize(path).unwrap().to_str().unwrap().to_owned()).collect()
    };

    if !prepend {
        new_paths.extend(current_paths);
        new_paths.extend(resolved_paths);
    } else {
        new_paths.extend(resolved_paths);
        new_paths.extend(current_paths);
    }

    if preview {
        for entry in new_paths {
            println!("{}", entry);
        }
    } else {
        save_path_key(&location, &new_paths);
    }
}