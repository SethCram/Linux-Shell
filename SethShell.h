/* Seth Cram
   9/4/2021
   hw2: Linux Shell Framework
   File: SethShell.h
   Notes: 
    -included within SethShell_Builtins.c, SethShell.c

    Description:
    -header file w/ prototypes and macros
*/

//libraries needed:
#include <stdio.h> //for printf()
#include <stdlib.h> //for getchar(), strtok(), malloc()
#include <string.h> //for strtok(), strcmp()
#include <stdbool.h> //for bool
#include <unistd.h> //for system calls fork() and exec()
#include <sys/wait.h> //for waitpid()

//prototypes:
char *ReadCmdLine();

char **ParseLine();

int RunProc( char **argList );

int ShellBuiltins( int i, char **argList );

int Cd_Funct( char **argList );

int Exit_Funct( char **argList );

int ExecuteCmd( char **argList );

int NumBuiltinCmds( char **builtins );

//macros used for readable consts:
#define CMD_LINE_MAX_CHARS 2093 //found w/ 'env | wc -c' on cs server




