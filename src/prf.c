//
//  main.c
//  PRF_Sim
//
//  Created by Giulio Stramondo on 10/05/16.
//  Copyright © 2016 UVA. All rights reserved.
//

#include "prf.h"


int m_v(int index_i, int index_j, scheme s, int p, int q){

    switch(s){
	case RECTANGLE_ONLY:
	    return index_i%p;
	case RECT_ROW:
	    return (index_i+ (int)floor( index_j/q ))%p;
	case RECT_COL:
	    return index_i%p;
	case  ROW_COL:
	    return (index_i+ (int)floor( index_j/q ))%p;
	case RECT_TRECT:
	    if (p<q){
		return index_i%p;
	    } else {
		return (index_i+index_j-(index_j%q))%q;
	    }
	case UNDEFINED:
	    return ERR;
    }
    return ERR;
}

int m_h(int index_i, int index_j, scheme s, int p, int q){
    switch(s){
	case RECTANGLE_ONLY:
	    return index_j%q;

	case RECT_ROW:
	    return index_j%q;

	case RECT_COL:
	    return ((int)floor(index_i/p) + index_j)%q;

	case  ROW_COL:
	    return ((int)floor(index_i/p) + index_j)%q;

	case RECT_TRECT:
	    if (p<q){
		return (index_i-index_i%p+index_j)%q;
	    } else {
		return index_j%q;
	    }
	case UNDEFINED:
	    return ERR;
    }
    return ERR;
}

Address2d* AGU(int index_i, int index_j,int p, int q, acc_type type){
    Address2d* res = (Address2d*) malloc(sizeof(Address2d)*(p*q));
    for(int i=0; i<p;i++)
	for(int j=0; j<q; j++){
	    switch(type){
		case RECTANGLE:
		    res[i*q+j].i= index_i+i;
		    res[i*q+j].j= index_j+j;

		    break;
		case ROW:
		    res[i*q+j].i=index_i;
		    res[i*q+j].j=index_j + (i*q+j);
		    break;
		case MAIN_DIAG:
		    res[i*q+j].i = index_i + (i*q+j);
		    res[i*q+j].j = index_j + (i*q+j);	
		    break;
		case SECONDARY_DIAG:
		    res[i*q+j].i = index_i + (i*q+j);
		    res[i*q+j].j = index_j - (i*q+j);
		    break;
		case COLUMN:
		    res[i*q+j].i = index_i + (i*q+j);
		    res[i*q+j].j = index_j;
		    break;
		case TRANSP_RECTANGLE:
		    res[i*q+j].i= index_i+j;
		    res[i*q+j].j= index_j+i;

		    break;
		default:
		    return NULL;
	    }
	}
    return res;
} 

int A_standard(int index_i, int index_j, int p, int q){
    return (int)floor(index_i/p)*(M/q) + (int)floor(index_j/q);
}

int A_custom(PolymorphicRegister *pR,int index_i, int index_j, int alpha, int beta, acc_type type){
    int c_i;
    int c_j;
    int c_j1, c_j2;
    int k;
    int l;

    k = m_v(index_i+alpha,index_j+beta,pR->s,pR->p,pR->q);
    l = m_h(index_i+alpha,index_j+beta,pR->s,pR->p,pR->q);
    int p = pR->p;
    int q = pR->q;

    switch(pR->s){
	case RECTANGLE_ONLY:
	    //c_i = (int)floor(((index_i%pR->p)+alpha)/pR->p);
	    //c_j = (int)floor(((index_j%pR->q)+beta)/pR->q);
	    // Above is equivalent to below
	    if(k<(index_i%p))
		c_i = 1;
	    else 
		c_i = 0;
	    if(l<(index_j%q))
		c_j = 1;
	    else
		c_j = 0;
	    break;
	case RECT_ROW:
	    switch(type){
		case RECTANGLE:
		    if(l<(index_j%q))
			c_j = 1;
		    else
			c_j = 0;
		    c_i = ((index_i%p)+(k-(index_j/q)%p-c_j-(index_i%p))%p)/p; 
		    break;
		case ROW:
		    c_i = 0;
		    if(l<(index_j%q))
			c_j1 = 1;
		    else
			c_j1 = 0;
		    c_j = c_j1 + (k-(index_i%p)-((index_j/q)%p)-c_j1)%p;
		    break;
		case MAIN_DIAG:
		    if(l<(index_j%q))
			c_j1 = 1;
		    else
			c_j1 = 0;
		    int delta_md;
		    delta_md = k-(index_i%p)-((l-(index_j%q))%q)%p - c_j1 - (index_j/q)%p;
		    //c_j2 = ((delta_md%p) w pag 67 of the thesis is undefined.
		    break;
		default:
		    break;
	    }
	    break;
	default:
	    c_i=0;
	    c_j=0;
    }
    int res = ((int)floor(index_i/pR->p)+c_i)*(M/pR->q) + (int)floor(index_j/pR->q) + c_j;
    return res;
}

PolymorphicRegister *createPolymorphicRegister(int p, int q, int linRegSize){
    linearRegister **lR = (linearRegister**)malloc(sizeof(linearRegister*)*p);
    for(int i = 0; i< p;i++){
	lR[i]= (linearRegister*)malloc(sizeof(linearRegister)*q);
    }

    for(int i = 0; i<p;i++)
	for(int j = 0; j< q; j++){
	    linearRegister* currentLinReg = &(lR[i][j]);
	    for(int k=0; k<linRegSize-1;k++){
		currentLinReg->next = (linearRegister*) malloc(sizeof(linearRegister));
		currentLinReg = currentLinReg->next;
	    }
	    currentLinReg->next=NULL;
	}
    PolymorphicRegister *pR = (PolymorphicRegister*)malloc(sizeof(PolymorphicRegister));
    pR-> data = lR;
    pR-> p=p;
    pR-> q=q;
    pR->s = UNDEFINED;
    return pR;
}

void writeToPR(PolymorphicRegister *pR, int data, int index_i, int index_j){
    if(pR->s == UNDEFINED){
	printf("No scheme associated with the given PolymorphicRegister\n");
	return;
    }
    //int m_h(int index_i, int index_j, scheme s, int p, int q)
    int reg_i = m_v(index_i,index_j,pR->s,pR->p,pR->q);
    int reg_j = m_h(index_i,index_j,pR->s,pR->p,pR->q);

    int inModuleIndex = A_standard(index_i,index_j,pR->p,pR->q);
    linearRegister* currentLinReg = &(pR->data[reg_i][reg_j]);

    for(int i =0;i< inModuleIndex;i++){
	currentLinReg = currentLinReg->next;
    }
    currentLinReg->data = data;
    return;
}


int readFromPR(PolymorphicRegister *pR, int index_i, int index_j){
    if(pR->s == UNDEFINED){
	printf("No scheme associated with the given PolymorphicRegister\n");
	return -1;
    }
    int reg_i = m_v(index_i,index_j,pR->s,pR->p,pR->q);
    int reg_j = m_h(index_i,index_j,pR->s,pR->p,pR->q);
    int inModuleIndex = A_standard(index_i,index_j,pR->p,pR->q);
    linearRegister *currentLinReg = &(pR->data[reg_i][reg_j]);

    for(int i =0;i< inModuleIndex;i++){
	currentLinReg = currentLinReg->next;
    }
    return currentLinReg->data;
}

int** parallelReadFromPR(PolymorphicRegister *pR, int z){
    int **res = (int **)malloc(sizeof(int*)* pR->p);
    for(int i = 0; i< pR->p;i++){
	res[i] = (int*) malloc(sizeof(int)*pR->q);
    }

    for(int i =0;i<pR->p;i++){
	for(int j=0;j<pR->q;j++){
	    linearRegister *currentLinReg = &(pR->data[i][j]);
	    for(int k=0;k<z;k++){
		currentLinReg = currentLinReg->next;
	    }
	    res[i][j]=currentLinReg->data;
	}
    }
    return res;
}


int** parallelReadRectangleOnly(PolymorphicRegister *pR, int index_i, int index_j){
    int **res = (int **)malloc(sizeof(int*)* pR->p);
    for(int i = 0; i< pR->p;i++){
	res[i] = (int*) malloc(sizeof(int)*pR->q);
    }

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    res[i][j]=-1;
	}
    }

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    int inModuleAddress = A_custom(pR,index_i,index_j, i, j,RECTANGLE);
	    inModuleAddress =  A_standard(index_i+i, index_j+j, pR->p, pR->q);

	    int reg_i = m_v(index_i+i,index_j+j,pR->s,pR->p,pR->q);
	    int reg_j = m_h(index_i+i,index_j+j,pR->s,pR->p,pR->q);
	    linearRegister *currentLinReg = &(pR->data[reg_i][reg_j]);
	    for(int k = 0;k<inModuleAddress;k++)
		currentLinReg = currentLinReg->next;
	    res[reg_i][reg_j]=currentLinReg -> data;
	}
    }

    return res;
}



int** parallelReadRow(PolymorphicRegister *pR, int index_i, int index_j){
    int **res = (int **)malloc(sizeof(int*)* pR->p);
    for(int i = 0; i< pR->p;i++){
	res[i] = (int*) malloc(sizeof(int)*pR->q);
    }

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    res[i][j]=-1;
	}
    }

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    int inModuleAddress = A_custom(pR,index_i,index_j, i, j,ROW);
	    inModuleAddress =  A_standard(index_i, index_j+(i*pR->q)+j, pR->p, pR->q);

	    int reg_i = m_v(index_i+i,index_j+j,pR->s,pR->p,pR->q);
	    int reg_j = m_h(index_i+i,index_j+j,pR->s,pR->p,pR->q);
	    linearRegister *currentLinReg = &(pR->data[reg_i][reg_j]);
	    for(int k = 0;k<inModuleAddress;k++)
		currentLinReg = currentLinReg->next;
	    res[reg_i][reg_j]=currentLinReg -> data;
	}
    }

    return res;
}

int** readBlock(PolymorphicRegister *pR, int index_i, int index_j, acc_type type){
    int **res = (int **)malloc(sizeof(int*)* pR->p);
    for(int i = 0; i< pR->p;i++){
	res[i] = (int*) malloc(sizeof(int)*pR->q);
    }

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    res[i][j]=-1;
	}
    }

    Address2d * generated_accesses = AGU(index_i,index_j,pR->p,pR->q,type);
    //the address have to be reordered
    //the reordering should allow to perform the accesses following the placement
    //of the phisical memory block

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    Address2d current_access = generated_accesses[i*pR->q+j];
	    //Checks that generated address are within the input matrix bound
	    if(current_access.i < 0 || current_access.j < 0 ||
		    current_access.i >= N || current_access.j >= M)
		return NULL;

	    int inModuleAddress;
	    inModuleAddress =  A_standard(current_access.i, current_access.j, pR->p, pR->q);

	    //This part represents the full crossbar switch in hw
	    //Redirecting each access to the right memory cell
	    int mm_i = m_v(current_access.i,current_access.j,pR->s,pR->p,pR->q);
	    int mm_j = m_h(current_access.i,current_access.j,pR->s,pR->p,pR->q);
	    linearRegister *currentLinReg = &(pR->data[mm_i][mm_j]);

	    for(int k = 0;k<inModuleAddress;k++)
		currentLinReg = currentLinReg->next;

	    //The value is placed again in the original position within the pxq
	    //parallel access. This is implemented again in hw with a full crossbar
	    res[i][j]=currentLinReg -> data;
	}
    }

    return res;
}

int** readBlockCustom(PolymorphicRegister *pR, int index_i, int index_j, acc_type type){
    int **res = (int **)malloc(sizeof(int*)* pR->p);
    for(int i = 0; i< pR->p;i++){
	res[i] = (int*) malloc(sizeof(int)*pR->q);
    }

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    res[i][j]=-1;
	}
    }

    Address2d * generated_accesses = AGU(index_i,index_j,pR->p,pR->q,type);
    //the address have to be reordered
    //the reordering should allow to perform the accesses following the placement
    //of the phisical memory block

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    Address2d current_access = generated_accesses[i*pR->q+j];
	    //Checks that generated address are within the input matrix bound
	    if(current_access.i < 0 || current_access.j < 0 ||
		    current_access.i >= N || current_access.j >= M)
		return NULL;

	    int inModuleAddress;
	    int alpha = current_access.i-index_i;
	    int beta = current_access.j-index_j;
	    inModuleAddress =  A_custom(pR,index_i,index_j,alpha, beta, type);

	    //This part represents the full crossbar switch in hw
	    //Redirecting each access to the right memory cell
	    int mm_i = m_v(current_access.i,current_access.j,pR->s,pR->p,pR->q);
	    int mm_j = m_h(current_access.i,current_access.j,pR->s,pR->p,pR->q);
	    linearRegister *currentLinReg = &(pR->data[mm_i][mm_j]);

	    for(int k = 0;k<inModuleAddress;k++)
		currentLinReg = currentLinReg->next;

	    //The value is placed again in the original position within the pxq
	    //parallel access. This is implemented again in hw with a full crossbar
	    res[i][j]=currentLinReg -> data;
	}
    }

    return res;
}

//This function is going to store the number of times each memory module is accessed
//in order to perform the requested address, given the current memory scheme.
//In the case in which there are no conflicts each memory block should have only one access.
//The number of read operation required to obtain the required data given the scheme and the
//access shape will be equal to the maximum number of accesses done.
int** computeConflicts(PolymorphicRegister *pR, int index_i, int index_j, acc_type type){
    int **res = (int **)malloc(sizeof(int*)* pR->p);
    for(int i = 0; i< pR->p;i++){
	res[i] = (int*) malloc(sizeof(int)*pR->q);
    }

    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    res[i][j]=0;
	}
    }

    Address2d * generated_accesses = AGU(index_i,index_j,pR->p,pR->q,type);
    for(int i=0; i<pR->p; i++){
	for(int j=0;j<pR->q; j++){
	    Address2d current_access = generated_accesses[i*pR->q+j];
	    int mm_i = m_v(current_access.i,current_access.j,pR->s,pR->p,pR->q);
	    int mm_j = m_h(current_access.i,current_access.j,pR->s,pR->p,pR->q);
	    res[mm_i][mm_j]++;
	}
    }

    return res;
}

t_list* getAvaliableAccessType(scheme s){
    t_list* listOfAccessTypes= NULL;
    acc_type *types;
    switch(s){
	case RECTANGLE_ONLY:
	    types= (acc_type*) malloc(sizeof(acc_type));
	    types[0] = RECTANGLE;
	    listOfAccessTypes= addElement(listOfAccessTypes,types,-1);
	    return listOfAccessTypes;
	case RECT_ROW:
	    types= (acc_type*) malloc(sizeof(acc_type)*4);
	    types[0] = RECTANGLE;
	    types[1] = ROW;
	    types[2] = MAIN_DIAG;
	    types[3] = SECONDARY_DIAG;
	    for(int i=0;i<4;i++)
		listOfAccessTypes= addElement(listOfAccessTypes,types+i,-1);
	    return listOfAccessTypes;
	case RECT_COL:
	    types= (acc_type*) malloc(sizeof(acc_type)*4);
	    types[0] = RECTANGLE;
	    types[1] = COLUMN;
	    types[2] = MAIN_DIAG;
	    types[3] = SECONDARY_DIAG;
	    for(int i=0;i<4;i++)
		listOfAccessTypes= addElement(listOfAccessTypes,types+i,-1);
	    return listOfAccessTypes;
	case ROW_COL:
	    types= (acc_type*) malloc(sizeof(acc_type)*3);
	    types[0] = RECTANGLE;
	    types[1] = ROW;
	    types[2] = COLUMN;
	    for(int i=0;i<3;i++)
		listOfAccessTypes= addElement(listOfAccessTypes,types+i,-1);
	    return listOfAccessTypes;
	case RECT_TRECT:
	    types= (acc_type*) malloc(sizeof(acc_type)*2);
	    types[0] = RECTANGLE;
	    types[1] = TRANSP_RECTANGLE;
	    for(int i=0;i<2;i++)
		listOfAccessTypes= addElement(listOfAccessTypes,types+i,-1);
	    return listOfAccessTypes;
	default:
	    return NULL;
    }
}

int computeCoverageScore(PolymorphicRegister *pR, Address2d* solution,t_list *parallel_accesses){
    int score = 0;
    for(int i = 0; i <(pR->p)*(pR->q); i++){
	t_list* elem = CustomfindElement(parallel_accesses, (void*)(solution+i),compareAddress);

	if(elem!=NULL)
	    score++;
    }
    return score;
} 

BlockAccess *findLocalBestAccess(PolymorphicRegister *pR,t_list *parallel_accesses, acc_type type, int* score){
    int bestScore = 0;
    Address2d bestCoverage;
    int i_MAX = 0;
    int j_MAX = 0;
    int i_MIN = 0;
    int j_MIN = 0;

    switch(type){
	case RECTANGLE:
	    i_MAX= (N-(pR->p));
	    j_MAX= (M-(pR->q));
	    break;
	case ROW:
	    i_MAX = N-1;
	    j_MAX = M-((pR->p)*(pR->q));
	    break;
	case COLUMN:
	    i_MAX = N -((pR->p)*(pR->q));
	    j_MAX = M-1;
	    break;
	case TRANSP_RECTANGLE:
	    i_MAX=N-(pR->q);
	    j_MAX=M-(pR->p);
	    break;
	case MAIN_DIAG:
	    i_MAX = N -((pR->p)*(pR->q));
	    j_MAX = M-((pR->p)*(pR->q));
	    break;
	case SECONDARY_DIAG:
	    j_MIN = ((pR->p)*(pR->q));
	    i_MAX = N-((pR->p)*(pR->q));
	    j_MAX = M-1;
	    break;
	default:
	   j_MAX = N-1;
	   i_MAX = M-1;
	   break;

    }

    for(int i = i_MIN; i<= i_MAX; i++)
	for(int j =j_MIN ; j<= j_MAX ;j++){
	    Address2d* currentBlockAccesses = AGU(i,j,pR->p,pR->q,type);
	    int current_score = computeCoverageScore(pR,currentBlockAccesses,parallel_accesses);
	    if(current_score > bestScore){
		bestScore = current_score;
		bestCoverage.i=i;
		bestCoverage.j=j;
	    }
	}

    BlockAccess* localBest = (BlockAccess*) malloc(sizeof(BlockAccess));
    (localBest->start_index).i=bestCoverage.i;
    (localBest->start_index).j=bestCoverage.j;
    localBest->type= type;
    *score = bestScore;
    return localBest;
}

t_list* parallelAccessCoverage(PolymorphicRegister *pR, t_list *parallel_accesses){
    t_list *toCover = cloneList(parallel_accesses);
    t_list *blockAccesses = NULL;
    t_list *access_type = getAvaliableAccessType(pR->s);	
    while(getLength(toCover)!=0){
	int available_access_number = getLength(access_type);
	BlockAccess *currentBlockAccesses[available_access_number];
	int coverage_score[available_access_number];
	int tmp_score;
	for(int i = 0; i<available_access_number; i++){
	    t_list *type_el = getElementAt(access_type,i);
	    acc_type current_type =*((acc_type*)(type_el->data));
	    currentBlockAccesses[i] = findLocalBestAccess(pR,toCover,current_type,&tmp_score);
	    coverage_score[i]=tmp_score;
	}
	tmp_score = 0;
	BlockAccess *currentSolution = NULL;
	for(int i = 0; i<available_access_number;i++){
	    if(coverage_score[i] > tmp_score){
		tmp_score=coverage_score[i];
		currentSolution = currentBlockAccesses[i];
	    }
	}
	blockAccesses = addElement(blockAccesses,currentSolution,-1);
	Address2d start_index = currentSolution->start_index;
	Address2d *covered_elements = AGU(start_index.i, start_index.j, pR->p, pR->q, currentSolution->type);
	for(int i = 0; i<(pR->p*pR->q); i++){
	    t_list* covered = CustomfindElement(toCover, (void*)(covered_elements+i),compareAddress);
	    if(covered!=NULL){
		toCover = removeElement(toCover,covered->data);
	    }
	}
    }
    return blockAccesses;
}

int compareAddress(void *a, void *b){
    Address2d *varA;
    Address2d *varB;
    if (a == NULL)
    {
	if (b == NULL)
	    return 1;
	return 0;
    }

    if (b == NULL)
	return 0;
    varA=(Address2d*)a;
    varB=(Address2d*)b;

    return ((varA->i == varB->i)&&(varA->j == varB->j));

}



