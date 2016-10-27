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
#include "collection.h"

#define ERR -1 /*!< The default error value. */
//#define N 10   /*!< Size of one dimension of the original matrix. */
//#define M 10   /*!< Size of the other dimension of the original matrix. */
extern int N;/*!< Size of the horizontal dimension of the original matrix. */
extern int M;/*!< Size of the vertical dimension of the original matrix. */


//!  Data structure used for representing a linearly accessible register.
/*!  To represent the fact that the register is accessible linearly a list 
	structure has been used. 
*/
typedef struct list {
    int data; /*!< The integer stored by this node. */
    struct list* next; /*!< Pointer to the next node. */
} linearRegister;

//!  Data structure used for representing a 2D address. 
typedef struct address2d {
	int i;
	int j;
}Address2d;


//! Enum containing all the available mapping scheme
/*! suitable for mapping input matrix logical addresses into physical Polymorphic Register addresses;
*/
typedef enum  {
    RECTANGLE_ONLY,/*!< This access scheme allows only conflict free rectangular block accesses */
    RECT_ROW,/*!< This access scheme allows only conflict free rectangular, row, main diagonal and secondary diagonal block accesses */

    RECT_COL,/*!< This access scheme allows only conflict free rectangular, column, main diagonal and secondary diagonal block accesses */

    ROW_COL,/*!< This access scheme allows only conflict free rectangular, column, and row block accesses */

    RECT_TRECT,/*!< This access scheme allows only conflict free rectangular, and transposed rectangular block accesses */

    UNDEFINED
} scheme;

//!Enum containing all the available access types
typedef enum {
	RECTANGLE, /*!< Access p x q rectangle */
	TRANSP_RECTANGLE, /*!< Access qxp rectangle */
	ROW,       /*!< Access 1 x p*q rows */
	COLUMN,    /*!< Access p*q x 1 columns */
	MAIN_DIAG, /*!< Access elements in the main diagonal */
	SECONDARY_DIAG, /*!< Access elements in the secondary diagonal */
	DEFAULT
} acc_type;

typedef struct block_access{
	Address2d start_index;
	acc_type type;
}BlockAccess;

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
	\return Correspondent linear register column.
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

int A_custom(PolymorphicRegister *pR,int index_i, int index_j, int alpha, int beta, acc_type type);
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
//int** parallelReadRectangleOnly(PolymorphicRegister *pR, int index_i, int index_j);

//int** parallelReadRow(PolymorphicRegister *pR, int index_i, int index_j);

//! Performs a block read on the PolymorphicRegister.
/*!
	\param pR Pointer to the Polymorphic Register.
	\param index_i Logical index on the horizontal dimension of the top-left element in the accessed block.
	\param index_j Logical index on the vertical dimension of the top-left element in the accessed block.
	\param type Access Type defining the shape of the block access.

	\return 2D array resulting from the block read.
*/
int** readBlock(PolymorphicRegister *pR, int index_i, int index_j, acc_type type);

int** readBlockCustom(PolymorphicRegister *pR, int index_i, int index_j, acc_type type);
//! Computes the conflict matrix relative to a block read on the PolymorphicRegister.
/*!
	\param pR Pointer to the Polymorphic Register.
	\param index_i Logical index on the horizontal dimension of the top-left element in the accessed block.
	\param index_j Logical index on the vertical dimension of the top-left element in the accessed block.
	\param type Access Type defining the shape of the block access.

	\return 2D array containing the conflict matrix.
*/
int** computeConflicts(PolymorphicRegister *pR, int index_i, int index_j, acc_type type);

//! Generates all the 2D logical addresses of the elements read in a block read.
/*!
	\param index_i index of the top-left element in the block read.
	\param index_j index of the top-left element in the block read.
	\param p size of the PRF on its first dimension.
	\param q size of the PRF on its second dimension.
	\param type access type specifying the shape of the block access.
	\return list of the 2D addresses of the block read.
*/
Address2d* AGU(int index_i, int index_j,int p, int q, acc_type type);

int compareAddress(void *a, void *b);

t_list* parallelAccessCoverage(PolymorphicRegister *pR, t_list *parallel_accesses);
#endif
