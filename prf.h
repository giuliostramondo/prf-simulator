//
//  prf.h
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

#define ERR -1
#define N 10
#define M 10

typedef struct list {
    int data;
    struct list* next;
} linearRegister;

typedef enum  {
    RECTANGLE_ONLY,
    RECT_ROW,
    RECT_COL,
    ROW_COL,
    RECT_TRECT,
    UNDEFINED
} scheme;

typedef struct polReg {
    linearRegister** data;
    int p;
    int q;
    scheme s;
} PolymorphicRegister;

int m_v(int index_i, int index_j, scheme s, int p, int q);

int m_h(int index_i, int index_j, scheme s, int p, int q);

int A_standard(int index_i, int index_j, int p, int q);

PolymorphicRegister *createPolymorphicRegister(int p, int q, int linRegSize);

void writeToPR(PolymorphicRegister *pR, int data, int index_i, int index_j);

int readFromPR(PolymorphicRegister *pR, int index_i, int index_j);

int** parallelReadFromPR(PolymorphicRegister *pR, int z);


#endif
