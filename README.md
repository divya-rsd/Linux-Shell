# Description
First tokenized with respect to ; then & later with pipes followed by i/o redirections.

# Assumptions

1.While printing the full path of directory in specification 3,the absolute path of directory with respect to laptop is printed.
2.In specification 4, the flag "-a-l" and "-l-a" is considered as invalid (no space between the flags).
3.Colour of files and directories being printed white for regular, green for execuatbles, blue for directories, zip files are not given in red.
4.Pastevents file is created in the home directory of pc (/home/user/pastevents.txt).
5.I/O redirections are having more priority than pipes.
6.In specification 8, flags except -e, -f , -d ,-ef or -fe,-ed or -de,-e -f or -f -e, -d -e or -e -d are taken as invalid flags.
7.sed will not work if there are spaces in arguments.
8.ctrl-c and ctrl-z do not work on user defined functions as well as neonate.
9.If some command is having an error then entire command will not be stored(error command is not given to run in background).
10.When there are pipes, ctrl z will not work and also the stopped process will be the current fg process and not the entire command given.(with ; and other tokens).