# myshell

## phase 2
Building my own shell
Now, we can use pipe ( | )

## How to compile
Just write down "make"

## flow
1. Read command
2. Check the command and add it to the .history.txt
3. Check if there is a pipe (|) in the command.

	3-1. If there is no pipe
	
		3-1-1. Split command by space
		
		3-1-2. Execute
		
			3-1-2-1. If it is a built-in command, run right away
			
			3-1-2-2. Otherwise, run after fork()
			
	3-2. Otherwise
	
		3-2-1. Split command by pipe
		
		3-2-1. Split command by space
		
		3-2-2. Execute
		
			3-2-2-1. If it is a built-in command, run right away
			
			3-2-2-2. Otherwise, run after fork()
