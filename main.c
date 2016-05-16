//
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

void printMatrix(int **inputMatrix, int dim1, int dim2){

	for(int i=0;i<dim1;i++){
		for(int j=0;j<dim2;j++){
			printf("%d ",inputMatrix[i][j]);
			}
		printf("\n");
	}
}

void printUsage(char *programName){
    printf("Usage: %s",programName);
}
int main(int argc, char** argv){
    
    int p=5;
    int q=5;

    scheme s = RECTANGLE_ONLY;

    for(int i=1; i<argc; i++){
	if(!strcmp(argv[i],"-p") && argc>i+1){
		p = atoi(argv[++i]);
	} else if(!strcmp(argv[i],"-q") && argc>i+1){
		q = atoi(argv[++i]);
	} else if(!strcmp(argv[i],"-s") && argc>i+1){
		s = atoi(argv[++i]);
	} else {
        printUsage(argv[0]);
    }
    }
        
    int z=(N*M)/(p*q);
    int data_elements[N][M];
	
	int ** data_elements1 = (int**)malloc(sizeof(int*)*N);
	for(int i=0;i<N;i++){
		data_elements1[i]=(int*)malloc(sizeof(int)*M);
	}

    for(int i=0;i< N;i++)
        for(int j=0;j<M;j++){
            data_elements[i][j]=i*M+j;
        }

    for(int i=0;i< N;i++)
        for(int j=0;j<M;j++){
            data_elements1[i][j]=i*M+j;
        }

    

	printMatrix(data_elements1,N,M);
     
    PolymorphicRegister *pR = createPolymorphicRegister(p,q,z);
    pR->s=s;
    
    for(int i=0;i< N;i++)
        for(int j=0;j<M;j++){
            writeToPR(pR,data_elements[i][j],i,j);
        }
    
    for(int i=0;i< N;i++)
        for(int j=0;j<M;j++){
            int data = readFromPR(pR,i,j);
            printf("read %d at (%d,%d)\n",data,i,j);
        }
    
    int **PrLayer =parallelReadFromPR(pR,0);
    printf("\n");
    
    for(int i = 0; i< pR->p; i++){
        for(int j=0; j< pR->q;j++){
            printf("%d ",PrLayer[i][j]);
        }
        printf("\n");
    }
    
    PrLayer =parallelReadFromPR(pR,1);
    printf("\n");
    
    for(int i = 0; i< pR->p; i++){
        for(int j=0; j< pR->q;j++){
            printf("%d ",PrLayer[i][j]);
        }
        printf("\n");
    }
    
    PrLayer =parallelReadFromPR(pR,2);
    printf("\n");
    
    for(int i = 0; i< pR->p; i++){
        for(int j=0; j< pR->q;j++){
            printf("%d ",PrLayer[i][j]);
        }
        printf("\n");
    }
    PrLayer =parallelReadFromPR(pR,3);
    printf("\n");
    
    for(int i = 0; i< pR->p; i++){
        for(int j=0; j< pR->q;j++){
            printf("%d ",PrLayer[i][j]);
        }
        printf("\n");
    }

    return 0;
}
