use crate::util::{ load_path_key, save_path_key, Location };

pub enum ListFrom {
    Merged,
    System,
    Local,
}

impl ListFrom {
    pub fn from_flags(system: bool, local: bool) -> Self {
        if system && !local {
            Self::System
        } else if local && !system {
            Self::Local
        } else {
            Self::Merged
        }
    }
}

pub enum Modification {
    Add {
        prepend: bool
    },
    Remove
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

pub fn modify(modification: Modification, raw: bool, location: Location, preview: bool, paths: Vec<String>) {
    let current_paths: Vec<String> = load_path_key(&location);

    // Resolve the paths if requested
    let paths = if raw { paths } else {
        paths.into_iter().map(|path| dunce::canonicalize(path).expect("Path could not be found.").to_str().unwrap().to_owned()).collect()
    };

    // Process paths
    let new_paths: Vec<String> = match modification {
        Modification::Add { prepend } => {
            if !prepend {
                current_paths.iter().chain(paths.iter()).cloned().collect()
            } else {
                paths.iter().chain(current_paths.iter()).cloned().collect()
            }
        },
        Modification::Remove => {
            current_paths.iter().filter(|x| {
                if let Result::Ok(x_canon) = dunce::canonicalize(x) {
                    return !paths.contains(&x_canon.to_str().unwrap().to_owned())
                }

                true
            }).cloned().collect()
        }
    };

    if preview {
        for entry in new_paths {
            println!("{}", entry);
        }
    } else {
        save_path_key(&location, &new_paths);

        let num_updated = usize::abs_diff(paths.len(), new_paths.len());

        match modification {
            Modification::Add { .. } => {
                println!("Added {} entries", num_updated);
            },
            Modification::Remove => {
                println!("Removed {} entries", num_updated);
            }
        }
    }
}