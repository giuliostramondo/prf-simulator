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

int M;
int N;

void printMatrix(int **inputMatrix, int dim1, int dim2){

	for(int i=0;i<dim1;i++){
		for(int j=0;j<dim2;j++){
			if(inputMatrix[i][j]<10)
				printf("  %d ",inputMatrix[i][j]);
			else if(inputMatrix[i][j]<100)
				printf(" %d ",inputMatrix[i][j]);
			else
				printf("%d ",inputMatrix[i][j]);

			}
		printf("\n");
	}
}

void printMatrixHighlight(int **inputMatrix,int dim1,int dim2, int **highlightMatrix, int dimH1, int dimH2){
	int highlight;
	for(int i=0;i<dim1;i++){
		for(int j=0;j<dim2;j++){
			highlight = 0;
			for(int k = 0; k< dimH1;k++){
				for(int w=0;w<dimH2;w++){
					if(highlightMatrix[k][w] == inputMatrix[i][j]){
						highlight=1;
						break;	
						}
					}
					//if(highlight)
					//	break;
				}
			if(highlight){
				if(inputMatrix[i][j]<10)
					printf("  \033[1;31m%d\033[0m ",inputMatrix[i][j]);
				else if(inputMatrix[i][j]<100)
					printf(" \033[1;31m%d\033[0m ",inputMatrix[i][j]);
				else 
					printf("\033[1;31m%d\033[0m ",inputMatrix[i][j]);
				}	
			else{
				if(inputMatrix[i][j]<10)
					printf("  %d ",inputMatrix[i][j]);
				else if(inputMatrix[i][j]<100) 
					printf(" %d ",inputMatrix[i][j]);
				else 
					printf("%d ",inputMatrix[i][j]);
				}	
		}	
		printf("\n");
	}


}
void printUsage(char *programName){
    printf("Usage: %s [Options]\n\n",programName);
    printf("-N <num>\t Change the horizontal size of the input matrix (default 9)\n");
    printf("-M <num>\t Change the vertical size of the input matrix (default 9)\n");
    printf("-p <num>\t Change the horizontal size of the PRF (default 3)\n");
    printf("-q <num>\t Change the horizontal size of the PRF (default 3)\n");
    printf("-s <num>\t Change the schema used by the PRF (default 0 -> RECTANGLE_ONLY)\n");
    printf("        \t  other schemes 1->RECT_ROW, 2->RECT_COL, 3->ROW_COL, 4->RECT_TRECT\n");

}
int main(int argc, char** argv){
    //Setting the default values for N M q and p
    int p=3;
    int q=3;
    N=9;
    M=9;

    scheme s = RECTANGLE_ONLY;

    for(int i=1; i<argc; i++){
	if(!strcmp(argv[i],"-p") && argc>i+1){
		p = atoi(argv[++i]);
	} else if(!strcmp(argv[i],"-q") && argc>i+1){
		q = atoi(argv[++i]);
	} else if(!strcmp(argv[i],"-s") && argc>i+1){
		//s = atoi(argv[++i]);
		switch(atoi(argv[++i])){
		
		case 0:
			s = RECTANGLE_ONLY;
			break;
		case 1:
			s=RECT_ROW;
			break;
		case 2:
			s=RECT_COL;
			break;
		case 3:
			s=ROW_COL;
			break;
		case 4:
			s=RECT_TRECT;
			break;
		default:
			printUsage(argv[0]);
			return 1;
		}
	}  else if(!strcmp(argv[i],"-M") && argc>i+1){
		M = atoi(argv[++i]);
	} else if(!strcmp(argv[i],"-N") && argc>i+1){
		N = atoi(argv[++i]);
	} else {
        printUsage(argv[0]);
	return 0;
    }
    }
    printf("scheme: %d\n",s);

    if(N%p!=0){
	printf("N%%p!=0\n");
	return 1;
	}

    if(M%q!=0){
	printf("M%%q!=0");
	return 1;
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

    

	//printMatrix(data_elements1,N,M);
     
    PolymorphicRegister *pR = createPolymorphicRegister(p,q,z);
    pR->s=s;
    
    for(int i=0;i< N;i++)
        for(int j=0;j<M;j++){
            writeToPR(pR,data_elements[i][j],i,j);
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

    return 0;
}
