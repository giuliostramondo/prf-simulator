#ifndef INTERACTIVE_INTERFACE_H
#define INTERACTIVE_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prf.h"
#include "utility.h"


void executeCommand(char* command,PolymorphicRegister* pR, int** data_elements1);

#endif
