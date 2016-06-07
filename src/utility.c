#include "utility.h"


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

void printConflicts(int **inputMatrix, int dim1, int dim2){

    for(int i=0;i<dim1;i++){
	for(int j=0;j<dim2;j++){
	    if(inputMatrix[i][j]<10){
		if(inputMatrix[i][j]==1)
		    printf("  %d ",inputMatrix[i][j]);
		else	
		    printf("\033[1;31m  %d\033[0m ",inputMatrix[i][j]);
	    }
	    else if(inputMatrix[i][j]<100){
		if(inputMatrix[i][j]==1)
		    printf(" %d ",inputMatrix[i][j]);
		else	
		    printf("\033[1;31m %d\033[0m ",inputMatrix[i][j]);
	    }
	    else{
		if(inputMatrix[i][j]==1)
		    printf("%d ",inputMatrix[i][j]);
		else	
		    printf("\033[1;31m%d\033[0m ",inputMatrix[i][j]);
	    }
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



Options parseArguments(int argc, char** argv){
    Options res;
    res.p=2;
    res.q=4;
    res.N=14;
    res.M=8;

    scheme s = RECTANGLE_ONLY;

    for(int i=1; i<argc; i++){
	if(!strcmp(argv[i],"-p") && argc>i+1){
	    res.p = atoi(argv[++i]);
	} else if(!strcmp(argv[i],"-q") && argc>i+1){
	    res.q = atoi(argv[++i]);
	} else if(!strcmp(argv[i],"-s") && argc>i+1){
	    //s = atoi(argv[++i]);
	    switch(atoi(argv[++i])){

		case 0:
		    res.s = RECTANGLE_ONLY;
		    break;
		case 1:
		    res.s=RECT_ROW;
		    break;
		case 2:
		    res.s=RECT_COL;
		    break;
		case 3:
		    res.s=ROW_COL;
		    break;
		case 4:
		    res.s=RECT_TRECT;
		    break;
		default:
		    printUsage(argv[0]);
		    res.error=1;
	    }
	}  else if(!strcmp(argv[i],"-M") && argc>i+1){
	    res.M = atoi(argv[++i]);
	} else if(!strcmp(argv[i],"-N") && argc>i+1){
	    res.N = atoi(argv[++i]);
	} else {
	    printUsage(argv[0]);
	    res.error = 1;
	}
    }
    return res;
}
char *accessStringFromAccessType(acc_type type){
	switch(type){
		case ROW:
		    return "ROW";
		case COLUMN:
		    return "COLUMN";
		case MAIN_DIAG:
		    return "MAIN_DIAG";
		case SECONDARY_DIAG:
		    return "SECONDARY_DIAG";
		case RECTANGLE:
		    return "RECTANGLE";
		case TRANSP_RECTANGLE:
		    return "TRANSP_RECTANGLE";
	}
	return "";
}
void performBlockRead(int index_i, int index_j, acc_type type,int ** data_elements1, PolymorphicRegister* pR){

	printf("Performing standard access %s A(%d,%d)\n",accessStringFromAccessType(type),index_i,index_j);
	int **PrLayer = readBlock(pR, index_i, index_j, type);
	if(PrLayer == NULL){
	    printf("Access out of bound\n");
	    return;
	}
	printMatrixHighlight(data_elements1,N,M,PrLayer, pR->p, pR->q);
	printf("\nPRF output\n");
	printMatrix(PrLayer,pR->p,pR->q);
	printf("Conflicts matrix\n");
	PrLayer = computeConflicts(pR, index_i, index_j, type);
	printConflicts(PrLayer, pR->p, pR->q);
	printf("\n");
}
