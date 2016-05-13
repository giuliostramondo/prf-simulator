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
#include "prf.h"

#define ERR -1
#define N 10
#define M 10


void printUsage(char *programName){
    printf("Usage: %s",programName);
}
int main(int argc, char** argv){
    
    int p=5;
    int q=5;
    if(argc != 1){
        
    }
        
    int data_elements[N][M];
    for(int i=0;i< N;i++)
        for(int j=0;j<M;j++){
            data_elements[i][j]=i*M+j;
        }
    
    
    PolymorphicRegister *pR = createPolymorphicRegister(p,q,3);
    pR->s=RECT_ROW;
    
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
