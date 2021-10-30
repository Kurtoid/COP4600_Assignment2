# mysh Shell program
This shell program is for the second assignment in COP4600, Fall 2021 edition.  
This program was written by Kurt Wilson and Joseph Terribile

## Building and running

1. make a build directory, and cd into it
    - `mkdir build`
    - `cd build`
2. configure and make the project
    - `cmake ..`
    - `make`
3. run the project
   - `./assignment2`

## Extra Credit 
We implement both extra credit commands: `repeat` and `dalekall`
 - `repeat n command [args]`
    - repeats a command in the background n times, passing an optional set of arguments to each task
    - prints a list of the PIDs for the new processes
    - processes run until they are killed (either by `dalek`, `dalekall`, or by another program) or end on their own
 - `dalekall`
    - kills all spawned child processes that were started with `background` or `repeat`.
    - prints a list of the PIDs of the child processes that were stopped
 - example: 
```
# background /bin/sleep 100
Started with pid: 10207
# repeat 3 /bin/sleep 20
PIDs: 10212, 10213, 10214
# dalekall
Exterminating 4 processes: 10207 10212 10213 10214
# 
```
