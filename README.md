<h1 align="center">Setp</h1>
<h2 align="center">The Windows command-line PATH modification utility</h2>

## Installation

Installation is quite simple. Setp is a portable binary and does not need to be installed, but if you would like it on your path, you can use it to add itself globally.

Simply copy it to the desired location (recommended location is `Program Files\setp` for all users, and `%localappdata%\setp` for the local user only), then use the command `setp add .` in a terminal in the same location to add it to the local user's path, or `setp add -s .` to add it to the system path (Note that this requires admin).

## Commands

There are three commands available to you using setp.

### Add

Syntax: `setp add [OPTIONS] <PATH> [PATH...]`

Adds the specified paths to the environment variable. Make sure to surround the path by quotation marks if it contains spaces.

By default, this will resolve the location of the path first, so if the path does not yet exist, it will raise an error. This also means that `.` resolves to the current directory and `..` resolves to the parent directory. Use `-r` if you want to bypass this automatic resolution.

Additionally, it will only apply changes to the local user's path, as that does not require administrator permissions. If you want it to be added to the system path instead, make sure to run the terminal you are using with administrator privileges and use the `-s` flag to add it globally.

The path can either be added to the beginning or end of the variable. By default, it is appended to the end. If you have multiple paths with conflicting binaries, use `-p` to prepend the path instead, thus giving it a higher priority and allowing it to override any other paths.

If you are worried and want a preview of the changes to be made, run `-w`. This will not actually make any changes to any environment variables, and it does not require administrator privileges when run with `-s`. It will list the resultant set of paths after the modification.

#### Options

| Name        | Shortcut | Description                                                                                      |
| ----------- | -------- | ------------------------------------------------------------------------------------------------ |
| `--prepend` | `-p`     | Prepends paths to the beginning of the variable instead of at the end                            |
| `--system`  | `-s`     | Modifies global/system environment variables instead of the local user's                         |
| `--raw`     | `-r`     | Adds or removes paths in the environment variable raw/absolutely instead of resolving them first |
| `--preview` | `-w`     | Simulates a preview of how the path would be changed instead of modifying it. Useful for testing |
| `--help`    | `-h`     | Prints the help message for this command                                                         |

### Remove

Syntax: `setp remove [OPTIONS] <PATH> [PATH...]`

Removes the specified paths from the environment variable. Make sure to surround the path by quotation marks if it contains spaces.
The command will error if the path cannot be found

By default, this will resolve the location of the path first, so if the path no longer exists, it will raise an error. This also means that `.` resolves to the current directory and `..` resolves to the parent directory. Use `-r` if you want to bypass this automatic resolution.

Additionally, it will only apply changes to the local user's path, as that does not require administrator permissions. If you want it to be added to the system path instead, make sure to run the terminal you are using with administrator privileges and use the `-s` flag to add it globally.

If you are worried and want a preview of the changes to be made, run `-w`. This will not actually make any changes to any environment variables, and it does not require administrator privileges when run with `-s`. It will list the resultant set of paths after the modification.

#### Options

| Name        | Shortcut | Description                                                                                      |
| ----------- | -------- | ------------------------------------------------------------------------------------------------ |
| `--system`  | `-s`     | Modifies global/system environment variables instead of the local user's                         |
| `--raw`     | `-r`     | Adds or removes paths in the environment variable raw/absolutely instead of resolving them first |
| `--preview` | `-w`     | Simulates a preview of how the path would be changed instead of modifying it. Useful for testing |
| `--help`    | `-h`     | Prints the help message for this command                                                         |

### List

Lists all the paths on the environment variable. By default, prints a merged list containing both system and local user paths, system coming first. To only list user paths, use `-l`. To only list system paths, use `-s`. This is a porcelain command by default,
so if you want to use it as part of a script, make sure to add `-q` to remove info messages.

| Name        | Shortcut | Description                                                                                      |
| ----------- | -------- | ------------------------------------------------------------------------------------------------ |
| `--quiet`   | `-q`     | Disables user messages and places each entry on its own line so that the output may be processed
                           by another program.                                                                              |
| `--system`  | `-s`     | Displays global/system environment variables only.                                               |
| `--local`   | `-l`     | Displays local user environment variables only.                                                  |
| `--help`    | `-h`     | Prints the help message for this command                                                         |