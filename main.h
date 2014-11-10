//
//  main.h
//  COMP3005UnixCommandShell
//
//  Created by Manoj Vaseekharan on 26/10/2014.
//  Copyright (c) 2014 Manoj Vaseekharan. All rights reserved.
//

#ifndef COMP3005UnixCommandShell_main_h
#define COMP3005UnixCommandShell_main_h

#include <stdbool.h>
#include <limits.h>
#include "console.h"
#include "profile.h"
#include "execute.h"

char *workingDirectory;

void handleWorkingDirectory();
bool workingDirectoryIsValid(char* workingDirectory);


#endif
