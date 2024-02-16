use crate::util::{ Location, load_path_key };

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