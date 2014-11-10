//
//  console.c
//  COMP3005UnixCommandShell
//
//  Created by Manoj Vaseekharan on 26/10/2014.
//  Copyright (c) 2014 Manoj Vaseekharan. All rights reserved.
//  DESCRIPTION: console takes input from the user and parses it accordingly.

#include "console.h"
#include "profile.h"
#include "main.h"

int getNoOfArguments(char *input)
{
    int argsCounter = 0;
    while(*input != '\n')
    {
        if (*input == ' ')
        {
            while(true)
            {
                input++;
                //if argument is enclosed in quotes, count whole thing as one argument.
                if(*input == '"')
                {
                    while(true)
                    {
                        input++;
                        if(*input == '"')
                        {
                            break;
                        }
                    }
                }
                if(*input == '\n')
                {
                    break;
                }
                if(*input != ' ')
                {
                    argsCounter++;
                    break;
                }
            }
        }
        else
        {
            while(true)
            {
                input++;
                if(*input == '\n')
                {
                    break;
                }
                //someone enters an escape character.
                else if (*input == '\\')
                {
                    input++;
                }
                else if(*input == ' ')
                {
                    break;
                }
            }
        }
    }
    return argsCounter;
}

char** assignMemoryToProgramArguments(int numberOfArgs)
{
    //execv takes in two parameters, a char[] containing location of program and an array of char* consisting of the program arguments.
    //arguments needs to be in a specific format. first entry is the program location (by convention).
    //last entry is NULL.
    //entries in between are the arguments entered by the user.
    //so with this in mind, allocate memory for the number of arguments entered + 2.
    char **programArguments = (char**)calloc(numberOfArgs+2,sizeof(char*));
    int k = 0;
    for(k = 0; k < numberOfArgs+2; k ++)
    {
        if(k == numberOfArgs+1)
        {
            programArguments[k] = NULL;
        }
        else
        {
            programArguments[k]=(char*)calloc(MAX_INPUT,sizeof(char));
        }
    }
    
    return programArguments;
}


void extractArgument(char **input_p, int index, char *programArgument)
{
    while(*(*input_p) != ' ')
    {
        //if our argument is enclosed in quotation marks, then treat everything inside as a single argument.
        if(*(*input_p) == '"')
        {
            (*input_p)++;
            while(true)
            {
                if(*(*input_p) != '"')
                {
                    programArgument[index] = *(*input_p);
                    index++;
                    (*input_p)++;
                }
                else
                {
                    (*input_p)++;
                    break;
                }
            }
            break;
        }
        //if the current character pointed at is a '\', this indicates an escape character.
        //skip to next index and automatically accept the character.
        else if(*(*input_p) == '\\')
        {
            (*input_p)++;
        }
        //if we reach the end of the user's input, break.
        else if(*(*input_p) == '\n')
        {
            break;
        }
        programArgument[index] = *(*input_p);
        index++;
        (*input_p)++;
    }
}

char** getProgramArguments(char *input)
{
    //advance the pointer beyond the program name...
    input += programNameSize;
    
    //find out how many arguments we are trying to find
    numberOfArguments = getNoOfArguments(input);
    
    //assign memory to our array of strings...
    char **programArguments = assignMemoryToProgramArguments(numberOfArguments);
    programArgumentsMemoryAllocated = true;
    
    //we will leave the first space blank, will be assigned later with program location.
    programArguments[0] = NULL;
    
    //now extract each argument.
    int loopCounter = 0;
    int index = 0;
    if (numberOfArguments > 0)
    {
        for(loopCounter = 0; loopCounter < numberOfArguments; loopCounter++)
        {
            char *programArgument = (char*)calloc(MAX_INPUT,sizeof(char));
            while(*input !='\n')
            {
                if(*input == ' ')
                {
                    input++;
                }
                //if input is NOT whitespace...let's retrieve our argument.
                else if (*input != ' ')
                {
                    extractArgument(&input, index, programArgument);
                    break;
                }
            }
            index = 0;
            programArguments[loopCounter+1] = programArgument;
        }
    }
    else
    {
        fullProgramPathMemoryAssigned = false;
    }
    return programArguments;
}

char* getProgramName(char *input)
{
    char *programName = (char*)calloc(MAX_INPUT,sizeof(char));
    programNameMemoryAllocated = true;
    int counter = 0;
    while(true)
    {
        if(*input == ' ' || *input == '\n')
        {
            break;
        }
        if(*input != '\0')
        {
            programName[counter] = *input;
            counter++;
            input++;
        }
    }
    programNameSize = counter;
    return programName;
}



void parseInput(char *input)
{
    //we first advance past any preceding whitespace, then start retrieving program name and arguments.
    int loopCounter = 0;
    for (loopCounter = 0; loopCounter < MAX_INPUT; loopCounter ++)
    {
        //moving past whitespace...
        if (*input == ' ')
        {
            input++;
        }
        else
        {
            programName = getProgramName(input);
            programArguments = getProgramArguments(input);
            break;
        }
    }
}

void getInput()
{
    char buf[MAX_INPUT];
    fgets(buf,MAX_INPUT,stdin);
    if(buf[0] == '\n')
    {
        return;
    }
    else
    {
        parseInput(buf);
    }
    
}

void printWorkingPath()
{
    printf("%s>",workingDirectory);
}

void startIO()
{
    programArgumentsMemoryAllocated = false;
    programNameMemoryAllocated = false;
    printWorkingPath();
    getInput();
    
}

