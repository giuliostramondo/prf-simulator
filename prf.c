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


int A_standard(int index_i, int index_j, int p, int q){
    return (int)floor(index_i/p)*(M/q) + (int)floor(index_j/q);
}

int A_custom(PolymorphicRegister *pR,int index_i, int index_j, int alpha, int beta, acc_type type){
	int c_i;
	int c_j;
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
					int c_j1;
					if(l<(index_j%q))
						c_j1 = 1;
					else
						c_j1 = 0;
					c_j = c_j1 + (k-(index_i%p)-((index_j/q)%p)-c_j1)%p;
				break;
				case MAIN_DIAG:
					int c_j1;
					int c_j2;
					if(l<(index_j%q))
						c_j1 = 1;
					else
						c_j1 = 0;
					int delta_md;
					delta_md = k-(index_i%p)-((l-(index_j%q))%q)%p - c_j1 - (index_j/q)%p;
					//c_j2 = ((delta_md%p) w pag 67 of the thesis is undefined.
				break;
			}
		break;
		default:
			c_i=0;
			c_j=0;
	}
	return ((int)floor(index_i/pR->p)+c_i)*(M/pR->q) + (int)floor(index_j/pR->q) + c_j;
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
		int inModuleAddress = A_custom(pR,index_i,index_j, i, j,RECTANGLE);
		int reg_i = m_v(index_i+i,index_j+j,pR->s,pR->p,pR->q);
    		int reg_j = m_h(index_i+i,index_j+j,pR->s,pR->p,pR->q);
		linearRegister *currentLinReg = &(pR->data[reg_i][reg_j]);
		for(int k = 0;k<inModuleAddress;k++)
			currentLinReg = currentLinReg->next;
		res[i][j]=currentLinReg -> data;
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
		int inModuleAddress = A_custom(pR,index_i,index_j, i, j,ROW);
		int reg_i = m_v(index_i+i,index_j+j,pR->s,pR->p,pR->q);
    		int reg_j = m_h(index_i+i,index_j+j,pR->s,pR->p,pR->q);
		linearRegister *currentLinReg = &(pR->data[reg_i][reg_j]);
		for(int k = 0;k<inModuleAddress;k++)
			currentLinReg = currentLinReg->next;
		res[i][j]=currentLinReg -> data;
		}
	}
	
	return res;
}

