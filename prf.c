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

PolymorphicRegister *createPolymorphicRegister(int p, int q, int linRegSize){
	linearRegister **lR = (linearRegister**)malloc(sizeof(linearRegister*)*p);
	for(int i = 0; i< p;i++){
		lR[i]= (linearRegister*)malloc(sizeof(linearRegister)*q);
	}

	for(int i = 0; i<p;i++)
		for(int j = 0; j< q; j++){
			linearRegister currentLinReg = lR[i][j];
			for(int k=0; k<linRegSize;k++){
				currentLinReg.next = (linearRegister*) malloc(sizeof(linearRegister));
				if(k == linRegSize-1){
					currentLinReg.next=NULL;
				}else{
				currentLinReg = *(currentLinReg.next);
				}
			}
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
	linearRegister currentLinReg = pR->data[reg_i][reg_j];

	for(int i =0;i< inModuleIndex;i++){
		currentLinReg = *(currentLinReg.next);
	}
	currentLinReg.data = data;
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
	linearRegister currentLinReg = pR->data[reg_i][reg_j];

	for(int i =0;i< inModuleIndex;i++){
		currentLinReg = *(currentLinReg.next);
	}
	return currentLinReg.data;
}

int main(){

	int data_elements[N][M];
	for(int i=0;i< N;i++)
		for(int j=0;j<M;j++){
			data_elements[i][j]=i+j;
		}

	PolymorphicRegister *pR = createPolymorphicRegister(5,5,2);
	pR->s=RECTANGLE_ONLY;

	for(int i=0;i< N;i++)
		for(int j=0;j<M;j++){
			writeToPR(pR,data_elements[i][j],i,j);
		}

	for(int i=0;i< N;i++)
		for(int j=0;j<M;j++){
			int data = readFromPR(pR,i,j);
			printf("read %d at (%d,%d)\n",data,i,j);
		}
		return 0;
}