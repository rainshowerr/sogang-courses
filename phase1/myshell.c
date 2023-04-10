#include "myshell.h"

int main(void) {
    do{
        // Shell Prompt: print your prompt print “CSE4100-MP-P1>”
        // Reading: Read the command from standard input. input = myshell readinput ();
        // Parsing: transform the input string into command line arguments. args = myshell parseinput (input);
        // Executing: Execute the command by forking a child process and return to parent process.
        myshell execute(args); } while (true);
}