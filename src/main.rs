use clap::{ Parser, Subcommand, Args };
use commands::{ ListFrom, Modification };
use util::Location;

mod util;
mod commands;

#[derive(Parser, Debug)]
#[command(version, about = "The setp utility allows you to add or remove paths from the global PATH environment variable on \
a Windows system. Use `setp add` followed by a relative or absolute path to append it to the path. If you want to remove a path, \
use `setp remove` instead. To list all installed paths use `setp list` and to add a path to all users use `-s` (Note: Requires admin).", long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand, Debug)]
enum Commands {
    #[command(about="Adds paths to the environment variable.")]
    Add{
        #[arg(long="prepend", short='p', help="Prepends paths to the beginning of the variable instead of at the end.")]
        prepend: bool,

        #[command(flatten)]
        args: ModifyArgs,
    },

    #[command(about="Removes paths from the environment variable.")]
    Remove(ModifyArgs),

    #[command(about="Lists paths in the environment variable", after_help="By default, paths are read from both the system and local user's environment \
    variable and are merged together, system coming first, mimicking normal path resolution behavior. If you would like to only list system or user paths, \
    use the -s switch or -l switch for system or local user paths respectively. A message will be printed with this information for clarity's sake, so if \
    you want to process this information, use the -q switch to print it in raw mode to be processed by another program.")]
    List {
        #[arg(long="quiet", short='q', help="Disables user messages and places each entry on its own line so that the output may be processed by another program.")]
        quiet: bool,

        #[arg(long="system", short='s', help="Displays global/system environment variables only.", group="namespace")]
        system: bool,

        #[arg(long="local", short='l', help="Displays local user environment variables only.", group="namespace")]
        local: bool,
    }
}

#[derive(Args, Debug)]
struct ModifyArgs {
    #[arg(long="system", short='s', help="Modifies global/system environment variables instead of the local user's.")]
    system: bool,

    #[arg(long="raw", short='r', help="Adds or removes paths in the environment variable raw/absolutely instead of resolving them first.")]
    raw: bool,

    #[arg(long="preview", short='w', help="Simulates a preview of how the path would be changed instead of modifying it. Useful for testing.")]
    preview: bool,

    paths: Vec<String>,
}

fn main() {
    let args = Cli::parse(); 

    match args.command {
        Commands::Add { prepend, args: ModifyArgs { system, raw, preview, paths } } => commands::modify(Modification::Add { prepend }, raw, Location::from_system(system), preview, paths),
        Commands::Remove(ModifyArgs { system, raw, preview, paths }) => commands::modify(Modification::Remove, raw, Location::from_system(system), preview, paths),
        Commands::List { quiet, system, local } => commands::list(quiet, ListFrom::from_flags(system, local)),
    }
}
