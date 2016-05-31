//  
//!  \file prf.h
//  PRF_Sim
//
//  Created by Giulio Stramondo on 10/05/16.
//  Copyright © 2016 UVA. All rights reserved.
//

#ifndef PRF_H
#define PRF_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ERR -1 /*!< The default error value. */
//#define N 10   /*!< Size of one dimension of the original matrix. */
//#define M 10   /*!< Size of the other dimension of the original matrix. */
extern int N;
extern int M;

//!  Data structure used for representing a linearly accessible register.
/*!  To represent the fact that the register is accessible linearly a list 
	structure has been used. 
*/
typedef struct list {
    int data; /*!< The integer stored by this node. */
    struct list* next; /*!< Pointer to the next node. */
} linearRegister;

//! Enum containing all the available mapping scheme
/*! suitable for writing and reading data from a Polymorphic Register.
*/
typedef enum  {
    RECTANGLE_ONLY,
    RECT_ROW,
    RECT_COL,
    ROW_COL,
    RECT_TRECT,
    UNDEFINED
} scheme;

//!Enum containing all the available access types
typedef enum {
	RECTANGLE, /*!< Access p x q rectangle */
	ROW,       /*!< Access 1 x p*q rows */
	COLUMN,    /*!< Access p*q x 1 columns */
	MAIN_DIAG, /*!< Access elements in the main diagonal */
	SECONDARY_DIAG, /*!< Access elements in the secondary diagonal */
	DEFAULT
} acc_type;

//!  Data structure used for representing a Polymorphic Register.
typedef struct polReg {
    linearRegister** data; /*!< 2D array of linearRegister (lists). */
    int p; /*!< Size of the first dimension of the linearRegister array. */
    int q; /*!< Size of the second dimension of the linearRegister array. */
    scheme s; /*!< Eunum which identifies the mapping scheme used by this register. 
		   \sa scheme
		*/
} PolymorphicRegister;

//! Given two input indices and a PRF mapping scheme, outputs the row of the corrispondent linear register where the data is stored.
/*!
	\param index_i index of the access.
	\param index_j index of the access.
	\param s selected mapping scheme for the access.
	\param p size of the PRF on its first dimension.
	\param q size of the PRF on its second dimension.
	\return Correspondent linear register row.
*/
int m_v(int index_i, int index_j, scheme s, int p, int q);

//! Given two input indices and a PRF mapping scheme, outputs the column of the corrispondent linear register where the data is stored.
/*!
	\param index_i index of the access.
	\param index_j index of the access.
	\param s selected mapping scheme for the access.
	\param p size of the PRF on its first dimension.
	\param q size of the PRF on its second dimension.
	\return Correspondent linear register row.
*/
int m_h(int index_i, int index_j, scheme s, int p, int q);

//! Given two input indices and a PRF mapping scheme, outputs the linear register index where the data is stored.
/*!
	\param index_i index of the access.
	\param index_j index of the access.
	\param p size of the PRF on its first dimension.
	\param q size of the PRF on its second dimension.
	\return Correspondent index in the linear register.
*/
int A_standard(int index_i, int index_j, int p, int q);

//! Allocates memory required for a Polymorphic Register and returns a pointer to it 
/*!
	\param p size of the PRF on its first dimension.
	\param q size of the PRF on its second dimension.
	\param linRegSize size of each linear register in the PRF.
	\return Pointer to the Polymorphic Register
*/
PolymorphicRegister *createPolymorphicRegister(int p, int q, int linRegSize);

//! Stores an integer at the given "logical" index in the Polymorphic Register given as argument 
/*!     
	\param pR Pointer to the Polymorphic Register.
	\param index_i Logical index on the first dimension.
	\param index_j Logical index on the second dimension.
	\param data integer to store
	\return Pointer to the Polymorphic Register
*/
void writeToPR(PolymorphicRegister *pR, int data, int index_i, int index_j);

//! Reads an integer at the given "logical" index in the Polymorphic Register given as argument 
/*!

	\param pR Pointer to the Polymorphic Register.
	\param index_i Logical index on the first dimension.
	\param index_j Logical index on the second dimension.
	\return integer at the given logical position.
*/
int readFromPR(PolymorphicRegister *pR, int index_i, int index_j);

//! Reads an array of integer at the given depth in the Polymorphic Register given as argument 
/*!
	\param pR Pointer to the Polymorphic Register.
	\param z Depth of the 2D array in the Polymorphic Register.
	\return 2D array resulting from the parallel read.
*/
int** parallelReadFromPR(PolymorphicRegister *pR, int z);

//Trying to implement the Inverse mapping function
int** parallelReadRectangleOnly(PolymorphicRegister *pR, int index_i, int index_j);

int** parallelReadRow(PolymorphicRegister *pR, int index_i, int index_j);

#endif
