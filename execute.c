//
//  execute.c
//  COMP3005UnixCommandShell
//
//  Created by Manoj Vaseekharan on 01/11/2014.
//  Copyright (c) 2014 Manoj Vaseekharan. All rights reserved.
//  DESCRIPTION: execute searches for the user's desired program, and executes it with their chosen args in a child process. Then, subsequently cleans up variables, ready for the next input from user.
//  execute also handles commands such as change directory and PATH/HOME assignments.

#include "execute.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "console.h"
#include <unistd.h>
#include "profile.h"
#include <dirent.h>
#include <sys/wait.h>
#include "main.h"

void getFullProgramPath(char *name, char *directory)
{
    //concatenates directory of program and name of program together.
    fullProgramPath = (char*)calloc(strlen(name)+strlen(directory)+1,sizeof(char));
    fullProgramPathMemoryAssigned = true;
    int loopCounter = 0;
    int index = 0;
    for (loopCounter = 0; loopCounter < strlen(directory); loopCounter++)
    {
        fullProgramPath[loopCounter] = directory[loopCounter];
    }
    
    fullProgramPath[loopCounter] = '/';
    loopCounter++;
    
    for (index = 0; index < strlen(name); index++)
    {
        fullProgramPath[loopCounter+index] = name[index];
    }
}

bool searchForProgram()
{
    int loopCounter = 0;
    for(loopCounter = 0; loopCounter < numberOfPaths; loopCounter++)
    {
        DIR *directory;
        directory = opendir(paths[loopCounter]);
        if(directory != NULL)
        {
            struct dirent *entry;
            while(true)
            {
                entry = readdir(directory);
                if(entry != NULL)
                {
                    if(strcmp(programName,entry->d_name) == 0)
                    {
                        getFullProgramPath(entry->d_name,paths[loopCounter]);
                        return true;
                    }
                }
                else
                {
                    closedir(directory);
                    break;
                }
            }
        }
    }
    fputs("WARNING: Chosen program could not be found. Consider altering your PATH variable.\n",stdout);
    return false;
}


void startForkAndExecv()
{
    pid_t childID = fork();
    if(childID == 0)
    {
        int status = execv(fullProgramPath, programArguments);
        exit(status);
    }
    else if (childID < 0 )
    {
        fputs("ERROR: could not fork.\n",stderr);
        exit(EXIT_FAILURE);
    }
    else
    {
        if(waitpid(childID,NULL,0) < 0)
        {
            fputs("ERROR: PID failure.\n",stderr);
            exit(EXIT_FAILURE);
        }
    }
}

void cleanup()
{
    //free up allocated memory and reset variables.
    if(programArgumentsMemoryAllocated == true)
    {
        free(programArguments);
        programArgumentsMemoryAllocated = false;
    }
    if(programNameMemoryAllocated == true)
    {
        free(programName);
        programNameMemoryAllocated = false;
    }
    if(fullProgramPathMemoryAssigned == true)
    {
        free(fullProgramPath);
        fullProgramPathMemoryAssigned = true;
    }
    programName = NULL;
    programArguments = NULL;
    fullProgramPath = NULL;
    numberOfArguments = 0;
    noOfWhiteSpaces = 0;
    programNameSize = 0;
}

int checkIfShellProgram()
{
    //checks to see if the user chose to do a HOME/PATH assignment or a change directory command.
    if(programName[0] == 'c' && programName[1] == 'd')
    {
        return 0;
    }
    else if (programName[0] == '$')
    {
        if(programName[1] == 'H' && programName[2] == 'O' && programName[3] == 'M' && programName[4] == 'E' && programName[5] == '=')
        {
            return 1;
        }
        else if(programName[1] == 'P' && programName[2] == 'A' && programName[3] == 'T' && programName[4] == 'H' && programName[5] == '=')
        {
            return 2;
        }
    }
    else if (programName[0] == 'e' && programName[1] == 'x' && programName[2] == 'i' && programName[3] == 't')
    {
        return 3;
    }
    else
    {
        return -1;
    }
    return -1;
}

void createNewWorkingDirectoryVar()
{
    memset(workingDirectory,0,PATH_MAX);
    char *buffer = calloc(sizeof(char),PATH_MAX);
    getcwd(buffer, PATH_MAX);
    strcpy(workingDirectory, buffer);
}

void changeDirectory()
{
    if(programArguments[1] == NULL || (strcmp(programArguments[1],"~") == 0))
    {
        int resultOfChange = chdir(homeDirectory);
        if(resultOfChange == -1)
        {
            fputs("ERROR: HOME cannot be found. Consider changing HOME variable.\n",stderr);
        }
        else if (resultOfChange == 0)
        {
            createNewWorkingDirectoryVar();
        }
    }
    else
    {
        int resultOfChange = chdir(programArguments[1]);
        if(resultOfChange == -1)
        {
            fputs("ERROR: Directory cannot be found.\n",stderr);
        }
        else if (resultOfChange == 0)
        {
            programArguments += 1;
            //implies a full path was given
            if(*programArguments[0] == '/')
            {
                workingDirectory = programArguments[0];
            }
            else
            {
                //we moved into a subdirectory
                createNewWorkingDirectoryVar();
            }
        }
    }
}

void executeProgram()
{
    fullProgramPathMemoryAssigned = false;
    int resultOfCheck = checkIfShellProgram();
    if(resultOfCheck == -1)
    {
        if(searchForProgram() == true && programName != NULL)
        {
            programArguments[0] = fullProgramPath;
            startForkAndExecv();
            cleanup();
        }
    }
    else
    {
        switch(resultOfCheck)
        {
            case 0:
                changeDirectory();
                break;
            case 1:
                memset(homeDirectory,0,LINE_MAX);
                programName += 6;
                strcpy(homeDirectory, programName);
                break;
            case 2:
                paths = NULL;
                programName += 1;
                paths = parsePath(programName);
                break;
            case 3:
                exit(EXIT_SUCCESS);
        }
    }
}