//
//  execute.h
//  COMP3005UnixCommandShell
//
//  Created by Manoj Vaseekharan on 01/11/2014.
//  Copyright (c) 2014 Manoj Vaseekharan. All rights reserved.
//

#ifndef __COMP3005UnixCommandShell__execute__
#define __COMP3005UnixCommandShell__execute__

#include <stdio.h>
#include <stdbool.h>

char* fullProgramPath;
bool fullProgramPathMemoryAssigned;

void getFullProgramPath(char *name, char *directory);
bool searchForProgram();
void startForkAndExecv();
void cleanup();
int checkIfShellProgram();
void createNewWorkingDirectoryVar();
void changeDirectory();
void executeProgram();



#endif /* defined(__COMP3005UnixCommandShell__execute__) */
