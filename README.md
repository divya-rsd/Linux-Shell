## Operating Systems and Networks

## Linux Shell

A custom command-line interpreter built entirely in C, emulating the functionality of Bash.
<br>

## Instructions to Run
To compile and run the code:
```sh
make
./a.out
```
<br>

### Features

#### 1. Command Prompt

- Command prompt is similar to the one in a Bash shell, displayed in the form of `<username@system_name:curr_dir>`.
- Colored output enhances user experience.
- Starts from the root directory where the shell is launched.

#### 2. Input Handling

- Supports `;` and `&` separators for command lists.
- Handles random spaces and tabs in user input seamlessly.
- `;` allows executing multiple commands sequentially.
- `&` runs the preceding command in the background, displaying the process ID.


#### 3. I/O Redirection

The shell also provides I/O redirection.

```sh
wc < newfile.txt > a.txt
```

- **Output Redirection (`>`, `>>`):**
  - Redirect command output to files.
  - Create or append to files as needed.
  - Handle errors gracefully if files don't exist.

- **Input Redirection (`<`):**
  - Read command input from files.
  - Prompt users with meaningful error messages for missing input files.

Both input and output redirections can be used simultaneously too. 

#### 4. Pipes

The shell supports piping with commands

```sh
echo "Lorem Ipsum" | wc | sed 's/ //g'
```

- **Multiple Pipes (`|`):**
  - Pass output from one command as input to the next.
  - Supports any number of sequential pipes.
  - Ensures correct order of execution from left to right.

#### 5. I/O Redirection with Pipes

Seamlessly integrate I/O redirection with pipe operations.

```sh
cat < a.txt | wc | sed 's/ //g' | cat > b.txt
```

<br>

### Commands 


#### 1. warp command

Changes the directory that the shell is currently in and prints the full path of working directory after changing, displays an error if the specified directory does not exist.
`.`, `..`, `~`, and `-` are all valid directory names.

```sh
warp <path_to_destination_directory>
```

 > Implemented in spec3.c



#### 2. peek command 

Lists all the files and directories in the specified directories in lexicographic order (default peek does not show hidden files).

***Flags Supported:***

- `-l` : displays extra information
- `-a` : displays all files, including hidden files

Similar to warp,`.`, `..`, `~`, and `-` are all valid symbols.

Supports both relative and absolute paths.
If no argument is given, peeks at the current working directory.

```sh
peek <flag> <path/name>
```
Combination of flags can also be used.

- green for executables
- white for files 
- blue for directories



#### 3. pastevents command

Command is similar to the actual history command in bash. The 15 most recent command statements given as input are stored.
Pastevents is persistent over different shell runs.

the commands pastevents and pastevents purge are not stored.
```sh
pastevents purge
```
Clears all the pastevents currently stored. Do not store this command in the pastevents.

```sh
pastevents 
```
dispays the list of events stores ordered from oldest to recent

```sh
pastevents execute <index>
```
Execute the command at position in pastevents (ordered from most recent to oldest). 



#### 4. proclore command

This command is used to obtain information regarding a process. If an argument is missing, prints the information of your shell.

Information  that print :

pid
Process Status (R/R+/S/S+/Z)
Process group
Virtual Memory
Executable path of process

```sh
procolore <pid>
```

Process states :

R/R+ : Running
S/S+ : Sleeping in an interruptible wait
Z : Zombie

“+” if it is a foreground process.



#### 5. seek command:

Command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories (files in green and directories in blue) separated with a newline character.

Flags :

- `-d` : Only look for directories (ignore files even if name matches)
- `-f` : Only look for files (ignore directories even if name matches)
- `-e` : This flag is effective only when a single file or a single directory with the name is found.
  -   If only one file (and no directories) is found, then prints it’s output. If only one directory (and no files) is found, then current working directory is changed to it.


- `-e` flag works with `-d` and `-f` flags. If `-e` flag is enabled but the directory does not have access permission (execute) or file does not have read permission, then outputs **Missing permissions for task!**


```sh
seek <flags> <search> <target_directory>
```

Argument 1 :

- The target that the user is looking for. A name without whitespace characters will be given here. You have to look for a file/folder with the exact name as this.

Argument 2 :

- The path to target directory where the search will be performed (this path can have symbols like . and ~ as explained in the peek command). If this argument is missing, target directory is the current working directory. The target directory’s tree must be searched (and not just the directory).

- Prints **No match found!** in case no matching files/directories is found. 
- `-d` and `-f` flag can’t be used at the same time and returns an error message **Invalid flags!**.



#### 6. neonate command

```sh
neonate -n [time_arg]
```

The command prints the Process-ID of the most recently created process on the system , this pid will be printed every [time_arg] seconds until the key ‘x’ is pressed.



#### 7. activities command

This command prints the list of all the processes currently running that were spanned by the shell in lexicographic order. The list conatins the following information about all processes:

Command Name
pid
state : running or stopped

Format of an entry:

```sh
[pid] : [command name] - [State]
```


#### 8. fg

```sh
fg <pid>
```
Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal. 
Prints “No such process found”, if no process with given pid exists.



#### 9. bg

```sh
bg <pid>
```
Changes the state of a stopped background process to running (in the background). If a process with given pid does not exist, “No such process found” is printed to the terminal.



#### 10. System Commands

The shell can execute the other system commands present in Bash  like emacs, gedit etc. This is possible in both foreground and background processes.

<br>

### Signal Handling

#### ping command :

Command used to send signals to processes. It takes the pid of a process and sends a signal to it which corresponds to the signal number (which is provided as an argument). Print error “No such process found”, if process with given pid does not exist. You should take signal number’s modulo with 32 before checking which signal it belongs to (assuming x86/ARM machine). Check man page for signal for an exhaustive list of all signals present.

```sh
ping <pid> <signal_number>
```

Following 3 commands are direct keyboard input where Ctrl is Control key on keyboard (or it’s equivalent).


#### 1. Ctrl-C

Interrupt any currently running foreground process by sending it the **SIGINT** signal. It has no effect if no foreground process is currently running.

#### 2. Ctrl-D

Log the user out of the shell(after killing all processes), without having any effect on the actual terminal.

#### 3. Ctrl-Z

Pushes any currently running foreground process into the background and changes its state from “Running” to “Stopped” by sending the **SIGTSTP**. It has no effect on the shell if there is no foreground process running.

<br>

### Processes


#### 1. Foreground Processes

Executing a command in foreground means the shell will wait for that process to complete and regain control afterwards. Control of terminal is handed over to this process for the time being while it is running.

Time taken by the foreground process and the name of the process will be printed in the next prompt if process takes > 2 seconds to run rounded to nearest integer.

To run processes in the foreground, just enter the command and the process will start up.
```sh
<command>
```

 

#### 2. Background Processes

Any command invoked with “&” is treated as a background command. The shell will spawn that process but doesn’t hand the control of terminal to it. Shell will keep taking other user commands. Whenever a new background process is started,the PID of the newly created background process is printed.After the background process exits, the shell displays the exit status on the terminal.


```sh
<command> &
``` 
or 
```
<command>&
```

Multiple background processes cn be executed.

<br>

### Networking 

iMan fetches man pages from the internet using sockets and outputs it to the terminal (stdout). In this case, the webisted used to get the man pages: http://man.he.net/ .

```sh
iMan <command_name>
```
<command_name> is the name of the man page to fetch.

Prints an error statement if the page does not exist.

<br>


### Tokenization priority order

First tokenized with respect to ; then & later with pipes followed by i/o redirections.


