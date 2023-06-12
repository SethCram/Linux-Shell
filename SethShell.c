/*
  Seth Cram
  9/4/2021
  hw2: Linux Shell Framework
  File: SethShell.c

  Notes:
   execvp() = 1st arg is filename, 2nd is 'vector' (bc 'v') to an array
            - rets -1 if fails
	    - w/ 'p' included, OS searches directories for provided filename
   fork() = caller is parent proc
          - rets PID of child proc to parent proc
	  - rets PID of 0 to child proc
	  - rets -1 if failure to make child proc
   waitpid() = suspends calling proc till child proc ends/stops
             - 1st arg = child PID waiting for
	     - 2nd arg = mem loc waitpid() stores status value
	     - 3rd arg = option macros (def'd in sys/wait.h)
	               - WUNTRACED = reports on stopped proc's too
	     - rets -1 if fails
   perror() = prints a descriptive error message after string arg

   exit() = kills curr proc w/ arg as exit val ret'd to cmd line 

   Status:
    - 0=success
    - 1=termination level error
    - (-1)=error
 
   Description:
    The implementation file for the m shell. 
    It contains most functions and main().  
*/

#include "SethShell.h" //header file w/ prototypes, macros, globals

//funct pnter approach: (possible later path)
// int (*builtins_funct[])(char **) = { &Cd_Funct, &Exit_Funct };

int main( int argc, char **argv )
{
  char *line;
  char **argList;
  int runStatus;

  printf("********************************************************\n");
  printf("Welcome to msh!\n");
  printf("Current limitations include, but are not limited to: \n");
  printf("1. tabs and special separation characters cant be used\n");
  printf("2. the only built-in shell command accessible is 'exit'\n");
  printf("********************************************************\n");

  do 
    {
      printf(">: "); //shell prompt

      //READ cmd line into 'line':      
      line = ReadCmdLine();

      //check: printf("Line: %s\n", line);

      //PARSE line into args:
      argList = ParseLine( line );

      //check:      printf("1st argList entry: %s\n", argList[0]);

      //EXECUTE cmd:
      runStatus = ExecuteCmd( argList );

      //check: printf("Current run status: %d\n", runStatus);

    }while( runStatus != 1 ); //run while not a termination lvl error

  return runStatus; //return termination lvl error
}

//find number of build-in cmds:
int NumBuiltinCmds( char **builtins )
{
  int i; //for-loop cnting var
  int numOfEntries = 0; //cnts builtins

  for( i = 0; builtins[i] != NULL ; i++)
    {
      numOfEntries++;
    }

  //check:  printf("Number of Builtin cmds: %d\n", numOfEntries );

  return numOfEntries;
}

//determines whether passed in cmd is built-in, present, or neither:
int ExecuteCmd( char **argList )
{
  //hardcoded, built-in shell cmds:
  char *builtins[] = { "cd",  "exit", NULL }; //NULL to find num of entries

  bool builtinCmd = false; //to signal if curr run cmd is a builtin one
  int runStatus, i;
  int totalBuiltins; //holds number of builtin cmds 

  totalBuiltins = NumBuiltinCmds( builtins ); //calc numer of builtins

  if( argList[0] == NULL ) //if no cmd entered
    {
      return -1;//error
    } 

  for( i = 0; i < totalBuiltins; i++ ) //loop thru builtin cmds
    {
      if( strcmp( builtins[i], argList[0] ) == 0 )
	//if found builtin cmd
	{
	  runStatus = ShellBuiltins( i, argList );

	  builtinCmd = true;
	}
    }

  if( !builtinCmd ) //only run cmd if not built in
    {
      runStatus = RunProc( argList ); //store ret status of cmd + run it
    }

  return runStatus;
}

//store cmd line in ret'd char *:
char *ReadCmdLine()
{
  //int numOfChars = 100; //assumes cmd lines is less than 100 chars
  int getCharInt;
  char *line = (char *)malloc( CMD_LINE_MAX_CHARS * sizeof(char) ); //dont free bc ret pntr
  int i; //for-loop cnting var

  while( 1 ) //loop forever unless ret'd from
    {
      getCharInt = getchar(); //get nxt char

      for( i = 0; getCharInt != EOF && getCharInt != '\n'; i++ )
	{
	  //check: putchar( getCharInt ); //put read in char out to std out
	  
	  line[i] = getCharInt; //put read in char into line
	                        // implicit typecast done
	  
	  //check: printf( "%c", line[i] ); //check copy in
	  
	  getCharInt = getchar(); //get nxt char
	}
      
      line[i] = '\0'; //add null term char to end of line
      
      return line;
    }
}

//parse line into args and ret pntr to array of pntr to chars w/ args:
char **ParseLine( char *line)
{
  char **argList = (char **)malloc( CMD_LINE_MAX_CHARS * sizeof(char*) );
  int i; //for-loop cnting var

  argList[0] = strtok( line, " " ); //read 1st arg, for for-loop conditon

  for( i = 0; argList[i] != NULL; i++ )
    {
      //check:  printf("ArgList[%d]: %s\n", i, argList[i]); //print curr arg

      argList[i+1] = strtok(NULL, " " ); //i+1 so this is checked in
                                         // for-loop condition
    }

  return argList;
}

//runs parsed argList cmd as a new proc:
int RunProc( char **argList )
{
  pid_t forkPID, waitPID; //type of PID is 'pid_t'
  int childStatus;
  char *errorMsg = "msh";

  forkPID = fork(); //save PID (type pid_t)
                   
  if( forkPID == -1 ) //couldnt fork proc
    {
      //testing: printf("Error forking.\n");

      perror( errorMsg ); 

      printf("msh: %s: fork() failed.\n", argList[0] );

      return -1; //failure
    }

  if( forkPID == 0 ) //if child proc
    {
      if( execvp( argList[0] , argList ) == -1) //if execvp fails
	{
	  perror( errorMsg );

	  printf("msh: %s: Execvp() failed.\n", argList[0] );
	  
	  //filed error outputting:
	  //	  strcat( errorMsg, argList[0] ); //append cmd to error msg
	  //	  strcat( errorMsg, ":" ); //add for formatting
	  //	  perror( errorMsg );

	  exit( -1 ); //failure (make sure to del child shell if not using)
	}

    }
  else //parent proc
    {
      //wait till child proc exited or killed to prompt for input:
      do
	{
	  waitPID = waitpid( forkPID, &childStatus, WUNTRACED );
	}while( waitPID != -1 ); //not sure why works
    }

  return 0; //success
}
