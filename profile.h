//
//  profile.h
//  COMP3005UnixCommandShell
//
//  Created by Manoj Vaseekharan on 25/10/2014.
//  Copyright (c) 2014 Manoj Vaseekharan. All rights reserved.
//

#ifndef __COMP3005UnixCommandShell__profile__
#define __COMP3005UnixCommandShell__profile__

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>

char *homeDirectory;
int numberOfPaths;
char **paths;

int getSizeOfList(char *line);
char** getMemoryAssignedArray();
char** parsePath(char *line);
bool checkVariableName(char *line, char* nameToCheck);
void checkContentsOfLine(char *line, bool *pathMarker, bool *homeMarker);
void readProfile(FILE *profileFile);
char *getProfilePath();
void handleProfile();


#endif /* defined(__COMP3005UnixCommandShell__profile__) */
