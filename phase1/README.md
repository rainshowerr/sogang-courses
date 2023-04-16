# myshell

## phase 1
Building my own shell

## How to compile
Just write down "make"

## flow
1. Read command
2. Check the command and add it to the .history.txt
3. Split command by space
4. Execute
	4-1. If it is a built-in command, run right away
	4-2. Otherwise, run after fork()