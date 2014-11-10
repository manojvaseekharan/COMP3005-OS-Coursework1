//
//  console.h
//  COMP3005UnixCommandShell
//
//  Created by Manoj Vaseekharan on 26/10/2014.
//  Copyright (c) 2014 Manoj Vaseekharan. All rights reserved.
//

#ifndef __COMP3005UnixCommandShell__console__
#define __COMP3005UnixCommandShell__console__

#include <stdio.h>
#include <stdbool.h>

char *programName;
char **programArguments;

int numberOfArguments;
int programNameSize;
int noOfWhiteSpaces;

bool programNameMemoryAllocated;
bool programArgumentsMemoryAllocated;
char** getProgramArguments(char *input);
void extractArgument(char **input_p, int index, char *programArgument);
char* getProgramName(char *input);
void parseInput();
void getInput();
int getNumberOfArguments();
void printWorkingPath();
void startIO();
char** assignMemoryToProgramArguments(int numberArgs);


#endif /* defined(__COMP3005UnixCommandShell__console__) */
