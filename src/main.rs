use clap::{ Parser, Subcommand, Args };

mod util;
mod commands;

#[derive(Parser, Debug)]
#[command(version, about = "The setp utility allows you to add or remove paths from the global PATH environment variable on \
a Windows system. Paths can be added to the end or to the front and can be added either to the system environment variable with the -s \
switch (requiring Administrator privileges) or locally to the user PATH variable instead (default). Local directories can also be added \
with relative mode (-r). You may add the current directory using `setp add .` or the parent directory using `setp add ..` or a local directory in the same \
path using `setp add ./<dir name>.`", long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand, Debug)]
enum Commands {
    #[command(about="Adds paths to the environment variable.", after_help="By default, paths are added in raw mode and to the local user's environment variable. \
    This means that paths are not checked to be valid and will be added as-is. For a safer evaluation, you may use the -r switch which will automatically resolve \
    paths based on the current directory or from an absolute path. To modify the system environment variables, use the -s switch (An error will be thrown if the \
    program is run without Administrator privileges). You may use the -p switch to add paths to the beginning of the variable instead of at the end.")]
    Add{
        #[arg(long="prepend", short='p', help="Prepends paths to the beginning of the variable instead of at the end.")]
        prepend: bool,

        #[arg(long="resolve", short='r', help="Resolves absolute or relative paths based on the current directory. Will throw an error if the path is invalid.")]
        resolve: bool,

        #[command(flatten)]
        args: ModifyArgs,
    },

    #[command(about="Removes paths from the environment variable.", after_help="By default, paths are removed from the local user's environment variable. \
    Use the -s switch to modify system environment variables. (An error will be thrown if the program is run without Administrator privileges).")]
    Remove(ModifyArgs),

    #[command(about="Lists paths from the environment variable", after_help="By default, paths are read from both the system and local user's environment \
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

    #[arg(long="preview", short='v', help="Prints out a peview of how the path would be changed instead of modifying them. Useful for testing.")]
    preview: bool,

    paths: Vec<String>,
}

fn main() {
    let args = Cli::parse();

    match args.command {
        Commands::Add { prepend, resolve, args: ModifyArgs { system, preview, paths } } => commands::add(prepend, resolve, if system { util::Location::System } else { util::Location::Local }, preview, paths),
        Commands::Remove(_) => unimplemented!(),
        Commands::List { quiet, system, local } => commands::list(quiet, if system { commands::ListFrom::System } else if local { commands::ListFrom::Local } else { commands::ListFrom::Merged }),
    }
}
