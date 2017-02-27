# Svsh

A simple shell written in c.

by Rodger Friesen, Jun Huang, David Tooley, Andrew Ward

Commands:

    # execute a program
    run command arg1 arg2

    # execute a program in the background
    run command arg1 arg2 <bg>

	# can execute things without "run" as well
    command arg1 arg2
    command arg1 arg2 <bg>

    # list the currently running jobs
    listjobs

    # change the prompt displayed
    defprompt text

    # change the current working directory
    cd path/to/directory

    # assign environment variables
    $varname = value

    # execute command and pipe stdout to the variable
    assignto $varname command arg1 arg2

    # exit the shell
    bye
    exit

