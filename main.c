//
//  main.c
//  COMP3005UnixCommandShell
//
//  Created by Manoj Vaseekharan on 21/10/2014.
//  Copyright (c) 2014 Manoj Vaseekharan. All rights reserved.
//  DESCRIPTION: main gets current working directory and makes high level calls to functions that handle shell operations.
//  Holds global variable associated with the current working directory.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "main.h"
#include "execute.h"
#include "console.h"
#include "profile.h"


bool workingDirectoryIsValid(char* workingDirectory)
{
    if(&workingDirectory == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void handleWorkingDirectory()
{
    workingDirectory = (char*)calloc(PATH_MAX,sizeof(char));
    getwd(workingDirectory);
    if(!workingDirectoryIsValid(workingDirectory))
    {
        fputs("ERROR: Cannot obtain current working directory.\n", stderr);
        free(workingDirectory);
        exit(EXIT_FAILURE);
    }
}

int main()
{
    handleWorkingDirectory();
    handleProfile();
    while(true)
    {
        startIO();
        if(programName != NULL)
        {
            executeProgram();
        }
        else
        {
            fputs("Please enter a program name.\n",stderr);
        }
    }
    
}
