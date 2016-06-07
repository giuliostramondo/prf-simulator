//  main.c
//  PRF_Sim
//
//  Created by Giulio Stramondo on 10/05/16.
//  Copyright © 2016 UVA. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "prf.h"
#include "utility.h"

int M;
int N;

int main(int argc, char** argv){
    //Setting the default values for N M q and p
    
    Options opt = parseArguments(argc,argv);
    if(opt.error == 1)
	return 0;

    int p=opt.p;
    int q=opt.q;
    N=opt.N;
    M=opt.M;
    scheme s = opt.s;
    if(N%p!=0){
	printf("N%%p!=0\n");
	return 1;
	}

    if(M%q!=0){
	printf("M%%q!=0");
	return 1;
	}
    int z=(N*M)/(p*q);
	
	int ** data_elements1 = (int**)malloc(sizeof(int*)*N);
	for(int i=0;i<N;i++){
		data_elements1[i]=(int*)malloc(sizeof(int)*M);
	}


    for(int i=0;i< N;i++)
        for(int j=0;j<M;j++){
            data_elements1[i][j]=i*M+j;
        }

    

	//printMatrix(data_elements1,N,M);
     
    PolymorphicRegister *pR = createPolymorphicRegister(p,q,z);
    pR->s=s;
    
    for(int i=0;i< N;i++)
        for(int j=0;j<M;j++){
            writeToPR(pR,data_elements1[i][j],i,j);
        }
    

   
    int **PrLayer;
    for(int i=0;i<z;i++){
	printf("\n");
	printf("Layer %d\n",i);
	PrLayer=parallelReadFromPR(pR,i);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	} 
	printf("\n");

	if(pR->s == 0 ){
	printf("Performing custom ReO access B(3,2)\n");
	PrLayer=parallelReadRectangleOnly(pR, 3, 2);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	}

	if(pR->s == 1){
	printf("Performing custiom ReRo access ROW B(0,0)\n");
	PrLayer= parallelReadRow(pR, 0, 0);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);

	printf("Performing custiom ReRo access ROW B(1,0)\n");
	PrLayer= parallelReadRow(pR, 1, 0);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);

	printf("Performing custiom ReRo access ROW B(2,0)\n");
	PrLayer= parallelReadRow(pR, 2, 0);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);

	printf("Performing custiom ReRo access ROW B(3,0)\n");
	PrLayer= parallelReadRow(pR, 3, 0);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);

	printf("Performing custiom ReRo access RECTANGLE B(3,2)\n");
	PrLayer=parallelReadRectangleOnly(pR, 3, 2);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);

	printf("Performing custiom ReRo access RECTANGLE B(4,4)\n");
	PrLayer=parallelReadRectangleOnly(pR, 4, 4);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	}

	printf("Performing standard access RECTANGLE B(4,4)\n");
	PrLayer = readBlock(pR, 4, 4, RECTANGLE);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	printf("Conflicts matrix\n");
	PrLayer = computeConflicts(pR, 4, 4, RECTANGLE);
	printConflicts(PrLayer, pR->p, pR->q);	

	printf("Performing standard access ROW B(3,0)\n");
	PrLayer = readBlock(pR, 3, 0, ROW);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	printf("Conflicts matrix\n");
	PrLayer = computeConflicts(pR, 3, 0, ROW);
	printConflicts(PrLayer, pR->p, pR->q);	

	printf("Performing standard access COLUMN B(3,0)\n");
	PrLayer = readBlock(pR, 3, 0, COLUMN);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	printf("Conflicts matrix\n");
	PrLayer = computeConflicts(pR, 3, 0, COLUMN);
	printConflicts(PrLayer, pR->p, pR->q);

	printf("Performing standard access MAIN_DIAG B(3,0)\n");
	PrLayer = readBlock(pR, 3, 0, MAIN_DIAG);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	printf("Conflicts matrix\n");
	PrLayer = computeConflicts(pR, 3, 0, MAIN_DIAG);
	printConflicts(PrLayer, pR->p, pR->q);	

	printf("Performing standard access MAIN_DIAG B(3,0)\n");
	PrLayer = readBlock(pR, 3, 0, MAIN_DIAG);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	printf("Conflicts matrix\n");
	PrLayer = computeConflicts(pR, 3, 0, MAIN_DIAG);
	printConflicts(PrLayer, pR->p, pR->q);

	printf("Performing standard access SECONDARY_DIAG B(4,7)\n");
	PrLayer = readBlock(pR, 4, 7, SECONDARY_DIAG);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	printf("Conflicts matrix\n");
	PrLayer = computeConflicts(pR, 4, 7, SECONDARY_DIAG);
	printConflicts(PrLayer, pR->p, pR->q);

	printf("Performing standard access TRANSP_RECTANGLE B(4,4)\n");
	PrLayer = readBlock(pR, 4, 4, TRANSP_RECTANGLE);
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\n");
	printMatrix(PrLayer,pR->p,pR->q);
	printf("Conflicts matrix\n");
	PrLayer = computeConflicts(pR, 4, 4, TRANSP_RECTANGLE);
	printConflicts(PrLayer, pR->p, pR->q);
	
	performBlockRead(4,4,RECTANGLE,data_elements1, pR);

	performBlockRead(4,5,RECTANGLE,data_elements1, pR);

	performBlockRead(-1,5,RECTANGLE,data_elements1, pR);
	performBlockRead(4,-1,RECTANGLE,data_elements1, pR);
	performBlockRead(15,4,RECTANGLE,data_elements1, pR);
	
    return 0;
}
