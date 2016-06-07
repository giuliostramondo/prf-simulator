//  
//!  \file prf.h
//  PRF_Sim
//
//  Created by Giulio Stramondo on 10/05/16.
//  Copyright © 2016 UVA. All rights reserved.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "prf.h"

typedef struct options{
	scheme s;
	int p;
	int q;
	int N;
	int M;
	int error;
} Options;

Options parseArguments(int argc, char** argv);

void printMatrix(int **inputMatrix, int dim1, int dim2);

void printConflicts(int **inputMatrix, int dim1, int dim2);

void printMatrixHighlight(int **inputMatrix,int dim1,int dim2, int **highlightMatrix, int dimH1, int dimH2);

void printUsage(char *programName);

void performBlockRead(int index_i, int index_j, acc_type type,int ** data_elements1, PolymorphicRegister* pR);

#endif
