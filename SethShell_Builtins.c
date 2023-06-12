/*
  Seth Cram
  9/4/2021
  hw2: Linux Shell Framework
  File: SethShell_Builtins.c

  Notes:
   -containts built-in shell command functions
*/

#include "SethShell.h" //header file w/ prototypes, macros, globals

//run funct based on what shell built-in we found:
int ShellBuiltins( int i, char **argList )
{
  int FunctState; //to determine completion or failure

  //find what built in cmd it is and run its funct

  //check:  printf("Found builtin cmd.\n" );

  switch( i )
    {
    case 0: //cd
      FunctState = Cd_Funct( argList );

      break;

    case 1: //exit
      FunctState = Exit_Funct( argList );

      break;

    default: //fail-catch
      printf("Builtin command not found.\n");

      FunctState = -1; //failure

      break;
    }

  return FunctState;
}

/*
//find number of build-in cmds:
int NumBuiltinCmds()
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
*/

//built-in change directory funct:
int Cd_Funct( char **argList )
{
  //use chdir() to change directories

  printf("msh: cd: Not yet implemented\n");

  return 0;
}

//built-in exit funct:
int Exit_Funct( char **argList )
{
  return 1; //ret termination lvl failure
}

