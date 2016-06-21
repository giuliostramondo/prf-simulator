//  
//!  \file prf.h
//  PRF_Sim
//  
//!  \file utility.h
//  Utility
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

//!  Data structure used to store the user's given arguments.
typedef struct options{
	scheme s;/*!< The mapping scheme used by the PRF */
	int p;/*!< Horizontal size of the PRF */
	int q;/*!< Vertical size of the PRF */
	int N;/*!< Horizontal size of the input matrix */
	int M;/*!< Vertical size of the input matrix */
	int error;/*!< Variable used when the user arguments generate errors */
} Options;

//! Parses the user arguments.
/*!
	\param argc number of user arguments.
	\param argv list of user's arguments.
	\return Options struct containing the settings defined by the user.
	\sa Options
*/
Options parseArguments(int argc, char** argv);

//! Prints in the console a formatted 2D matrix.
/*!
	\param inputMatrix the 2D array containing the data.
	\param dim1 the horizontal dimension of the given matrix.
	\param dim2 the vertical dimension of the given matrix.
*/
void printMatrix(int **inputMatrix, int dim1, int dim2);

//! Prints in the console a conflict matrix, highlighting the conflicts.
/*!
	\param inputMatrix the 2D array containing the confict's data.
	\param dim1 the horizontal dimension of the given matrix.
	\param dim2 the vertical dimension of the given matrix.
*/
void printConflicts(int **inputMatrix, int dim1, int dim2);

//! Prints in the console a matrix, highlighing certain elements.
/*!
	\param inputMatrix the 2D array containing all the matrix data.
	\param dim1 the horizontal dimension of the given matrix.
	\param dim2 the vertical dimension of the given matrix.
	\param highlightMatrix the 2D array containing the elements of the inputMatrix to highlight.
	\param dimH1 the horizontal dimension of the given highlightMatrix.
	\param dimH2 the vertical dimension of the given highlightMatrix.

*/
void printMatrixHighlight(int **inputMatrix,int dim1,int dim2, int **highlightMatrix, int dimH1, int dimH2);

//! Prints in the console the usage informations.
/*!
	\param programName name of the executable

*/
void printUsage(char *programName);

//! Prints the report information of a block read.
/*!
	\param index_i the horizontal index of the top-left element of the block read.
	\param index_j the vertical index of the top-left element of the block read.
	\param type access type defining the block access shape.
	\param data_elements1 the 2D array containing original input matrix.
	\param pR pointer to the PolymorphicRegister used for the block read.
*/
void performBlockRead(int index_i, int index_j, acc_type type,int ** data_elements1, PolymorphicRegister* pR);

#endif
