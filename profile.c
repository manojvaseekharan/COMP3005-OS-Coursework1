//
//  profile.c
//  COMP3005UnixCommandShell
//
//  Created by Manoj Vaseekharan on 25/10/2014.
//  Copyright (c) 2014 Manoj Vaseekharan. All rights reserved.
//  DESCRIPTION: profile handles the reading and interpreting of .profile.
//  Also holds global variables relating to PATH and HOME variables.

#include "profile.h"
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "main.h"


int getSizeOfList(char* line)
{
    int counter = 0;
    int colonCounter = 0;
    while(line[counter] != '\0')
    {
        if (line[counter] == ':')
        {
            colonCounter++;
        }
        counter++;
    }
    return colonCounter + 1;
}

char** getMemoryAssignedArray()
{
    //returns pointer to char pointers, memory assigned, ready to go.
    char **array = (char**)calloc(numberOfPaths,sizeof(char*));
    int loopCounter = 0;
    for(loopCounter = 0; loopCounter<numberOfPaths; loopCounter++)
    {
        *(array+loopCounter) = (char*)calloc(LINE_MAX,sizeof(char));
    }
    return array;
}

char** parsePath(char *line)
{
    //parses path variables and returns an array of paths.
    char *pathName;
    numberOfPaths = getSizeOfList(line);
    line+=5;
    //if nothing has been written, then exit.
    if(*line == '\0')
    {
        fputs("ERROR: No paths assigned.\n",stderr);
        exit(EXIT_FAILURE);
    }
    char **array = getMemoryAssignedArray();
    int charCounter = 0;
    int loopCounter = 0;
    for (loopCounter = 0; loopCounter < numberOfPaths; loopCounter ++)
    {
        pathName = (char*)calloc(1024,sizeof(char));
        while(true)
        {
            if (*line == ':' || *line == '\n' || *line == '\0')
            {
                charCounter = 0;
                line++;
                array[loopCounter] = pathName;
                break;
            }
            else if(*line != ':' || *line == '\n')
            {
                pathName[charCounter] = *line;
                charCounter++;
                line++;
            }
        }
    }
    return array;
}

bool checkVariableName(char *line, char* nameToCheck)
{
    //checks if the first four characters in line match either the word PATH or HOME.
    int loopCounter = 0;
    for(loopCounter = 0; loopCounter < 4; loopCounter++)
    {
        if(line[loopCounter] != nameToCheck[loopCounter])
        {
            return false;
        }
    }
    return true;
}


void checkContentsOfLine(char *line, bool *pathMarker, bool *homeMarker)
{
    if(checkVariableName(line,"PATH") && *pathMarker != true)
    {
        *pathMarker = true;
        paths = parsePath(line);
    }
    else if (checkVariableName(line, "HOME") && *homeMarker != true)
    {
        *homeMarker = true;
        homeDirectory = (char*)calloc(LINE_MAX,sizeof(char));
        line += 5;
        int index = 0;
        while(line[index] != '\n')
        {
            homeDirectory[index] = line[index];
            index++;
        }
    }
    else
    {
        fputs("ERROR: Structure of profile is incorrect.\n",stderr);
        exit(EXIT_FAILURE);
    }
}

void readProfile(FILE *profileFile)
{
    //reads each line from profile, extracts PATH and HOME if they exist, exits program if profile is malformed.
    bool pathVariableDetected = false;
    bool homeVariableDetected = false;
    
    char lineFromFile[LINE_MAX];
    while(fgets (lineFromFile, LINE_MAX, profileFile) != NULL)
    {
        checkContentsOfLine(lineFromFile, &pathVariableDetected, &homeVariableDetected);
    }
}

char *getProfilePath()
{
    //concatenates the working directory and '/profile'.
    const char profileSuffix[] = "/profile";
    char *profile = (char*)calloc(PATH_MAX,sizeof(char));
    int loopCounter = 0;
    for(loopCounter = 0; loopCounter < PATH_MAX; loopCounter++)
    {
        if(workingDirectory[loopCounter] != '\0')
        {
            profile[loopCounter] = workingDirectory[loopCounter];
        }
        else
        {
            int innerLoopCounter = 0;
            for(innerLoopCounter = 0; innerLoopCounter < 9; innerLoopCounter++)
            {
                profile[innerLoopCounter + loopCounter] = profileSuffix[innerLoopCounter];
            }
            return profile;
        }
    }
    //we will never reach here. but in any case...
    return profile;
}


void handleProfile()
{
    char *profilePath = getProfilePath();
    FILE *profileFile = fopen(profilePath,"r");
    if(profileFile == NULL)
    {
        fputs("ERROR: profile is missing.\n",stderr);
        free(profilePath);
        free(workingDirectory);
        exit(EXIT_FAILURE);
    }
    else
    {
        readProfile(profileFile);
    }
    free(profilePath);
}