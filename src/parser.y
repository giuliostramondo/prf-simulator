%{
#include <stdio.h>    
#include <stdlib.h>
#include <string.h>
#include <malloc/malloc.h>
#include <stdarg.h>
#include "prf.h"
#include "utility.h"
#include "collection.h"

#define DEBUG 0

//Defining the PRF Access modes
#define STANDARD 0
#define CUSTOM 1

int line_num;
int N;
int M;
int z;
int **data_elements1;
PolymorphicRegister *pR;
int mode;
Options opt;

void printDebug(const char * restrict format, ... ){

if(DEBUG){
	    va_list args;
	va_start(args, format);
		vprintf(format,args);
		va_end(args);
	fflush(stdin);
	}
}
int yylex();
int yyerror(char* msg); 
%}


//------------------------------
//			Record Semantici
//______________________________
%union {
	char *svalue;
	int intval;
	acc_type access_type;
	scheme mapping_scheme;
	t_list *list;
	void *generic;
	Address2d *access;
}

//------------------------------
//			Tokens
//______________________________

%start instructions

%token <svalue> IDENTIFIER
%token <intval> NUMBER
%token <access_type> ACC_TYPE
%token <mapping_scheme> SCHEME
%token LSQUARE
%token RSQUARE
%token LPAR
%token RPAR
%token SEMI
%token COMMA
%token ALL
%token A
%token SHOW
%token SET
%token CHECK

//%type <list> id_list
//%type <declaration> eval_item
//%type <list> eval_item_list
//%type <list> rel_item_list
//%type <couple> rel_item
//%type <list> exp
%type <list> arg_list
%type <list> access_list
%type <generic> argument
%type <access> access

//------------------------
//      Precedenze
//------------------------

//%left IFF
//%left IMPL
//%left OR
//%left AND
//%right NOT NEC POS
//%left L_PAR R_PAR
%%

instructions : instructions instruction |
	     /*empty*/
		;

instruction : 
	    
		set_instruction |
		show_instruction| 
		function_call   |
		access_generic 	|
		error { 
		printf("> ");
			}   
	 /*|
	     check_instruction */
		;




access_generic: access_all   |
      		access_block |
		sparse_access;

access_all : A ALL SEMI{
	   printDebug("Parsed access_all\n"); 

performBlockRead(0,0,RECTANGLE,data_elements1,pR,mode);
		performBlockRead(0,0,ROW,data_elements1,pR,mode);
		performBlockRead(0,0,COLUMN,data_elements1,pR,mode);
		performBlockRead(0,0,MAIN_DIAG,data_elements1,pR,mode);
		performBlockRead(0,0,TRANSP_RECTANGLE,data_elements1,pR,mode);
		performBlockRead(0,M-1,SECONDARY_DIAG,data_elements1,pR,mode);
		printf("> ");
	};

function_call: IDENTIFIER LPAR arg_list RPAR SEMI{
	     int length =getLength($3);
		printDebug("length: %d",length);
		printDebug("Function Call\nFunction name:%s\nCalled with %d arguments",$1,length);
		for(int i = 0 ; i<getLength($3); i++){
			t_list *raw_elem = getElementAt($3,i);
			int elem = *((int*)(raw_elem->data));
			printDebug("Arg %d: %d\n",i,elem);
		}
		if(strcmp($1,"m")==0){
			if(length != 2){
				printf("Wrong number of arguments, function m takes 2 arguments\n> ");
			}else{
				t_list *raw_elem = getElementAt($3,0);
				int index_i = *((int*)(raw_elem->data));
				raw_elem = getElementAt($3,1);
				int index_j = *((int*)(raw_elem->data));
				int mv= m_v(index_i,index_j,pR->s,pR->p,pR->q);
				int mh= m_h(index_i,index_j,pR->s,pR->p,pR->q);
				printf("(%d,%d)\n> ",mv,mh);
			}
		}else if(strcmp($1,"AGU")==0){
			if(length != 3){
				printf("Wrong number of arguments, function takes 3 arguments\n> ");
			}else{
				t_list *raw_elem = getElementAt($3,0);
				int index_i = *((int*)(raw_elem->data));
				raw_elem = getElementAt($3,1);
				int index_j = *((int*)(raw_elem->data));
				raw_elem = getElementAt($3,2);
				acc_type type = *((acc_type*)(raw_elem->data));
				Address2d* addresses = AGU(index_i,index_j,pR->p,pR->q,type);
				for(int i = 0 ; i<((pR->p)*(pR->q)); i++){
					printf("(%d, %d) ",addresses[i].i, addresses[i].j);
				}
				printf("\n> ");

}
		}else if(strcmp($1,"A_custom")==0){
			if(length != 5){
				printf("Wrong number of arguments, function takes 5 arguments\n> ");
			}else{
				t_list *raw_elem = getElementAt($3,0);
				int index_i = *((int*)(raw_elem->data));
				raw_elem = getElementAt($3,1);
				int index_j = *((int*)(raw_elem->data));
				raw_elem = getElementAt($3,2);
				int alpha = *((int*)(raw_elem->data));
				raw_elem = getElementAt($3,3);
				int beta = *((int*)(raw_elem->data));
				raw_elem = getElementAt($3,4);
				acc_type type = *((acc_type*)(raw_elem->data));

int res = A_custom(pR,index_i,index_j,alpha,beta,type);	
				printf("%d\n> ",res);

}
		}else if(strcmp($1,"A_standard")==0){
			if(length != 2){
				printf("Wrong number of arguments, function takes 2 arguments\n> ");
			}else{
				t_list *raw_elem = getElementAt($3,0);
				int index_i = *((int*)(raw_elem->data));
				raw_elem = getElementAt($3,1);
				int index_j = *((int*)(raw_elem->data));

int res = A_standard(index_i,index_j,pR->p,pR->q);	
				printf("%d\n> ",res);

}
		}else{
			printf("Function %s doesn't exist\n> ",$1);
		}
	}

arg_list: arg_list COMMA argument{
	printDebug("Adding element %d to the given list\n",*((int*)$3));
		$$ = addElement($1,$3,-1);
		int length = getLength($$);
		printDebug("List length: %d\n",length);
		for(int i =0; i<length;i++){
			t_list *raw_elem = getElementAt($$,i);
			int elem = *((int*)(raw_elem->data));
			printDebug("Element %d: %d\n",i,elem);
		}
	}
	| argument
	{	printDebug("Creating a new arg_list and adding %d\n",*((int*)$1));
		$$ = addElement(NULL,$1,-1);	
		t_list *retrived = getElementAt($$,0);
		int retrived_int = *((int*)(retrived->data));
		printDebug("Retrived element : %d\n",retrived_int);
	}
;

argument: NUMBER {
	printDebug("Argument is a NUMBER");
		int *elem = (int*)malloc(sizeof(int));
		*elem =$1;
		$$ = elem;
	} |
	ACC_TYPE {
		printDebug("Argument is an ACC_TYPE");
		acc_type *elem = (acc_type*)malloc(sizeof(acc_type));
		*elem = $1;
		$$ = elem;
	}
;
set_instruction: SET IDENTIFIER NUMBER SEMI{
	       printDebug("parsed set_instruction with number\n");
		if(strcmp($2,"s")==0){
			//ADD FREE old pR 
			pR->s = $3;
			for(int i=0;i< N;i++)
				for(int j=0;j<M;j++){
					writeToPR(pR,data_elements1[i][j],i,j);
				}
			printf("s=%d\n",pR->s);	
		}
		else{
			printf("Can't set %s\n",$2);
		}

printf("> ");
	}
	|
	SET IDENTIFIER IDENTIFIER SEMI{
		printDebug("parsed set_instruction with identifier\n");
		if(strcmp($2,"mode")==0){
			if(strcmp($3,"custom")==0){
				printf("PRF in custom mode\n");
				mode = CUSTOM;
			}else if(strcmp($3,"standard")==0){
				printf("PRF in standard mode\n");
				mode = STANDARD;
			}else{
				printf("No such mode\n");
			}
		}

printf("> ");
	}
	;

show_instruction: SHOW IDENTIFIER SEMI{
		printDebug("parsed show_instruction\n");
			if(strcmp($2,"s")==0){
				printf("s=%d\n",pR->s);
				}

if(strcmp($2,"matrix")==0){
				printMatrix(data_elements1,N,M);	
				}
			if(strcmp($2,"PRF")==0){
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

}

if(strcmp($2,"mode")==0){
				switch(mode){
					case STANDARD:
							printf("PRF in standard mode\n");
							break;
					case CUSTOM:
							printf("PRF in custom mode \n");
							break;
					default:
							printf("PRF in unknown mode\n");
				}
			}
			printf("> ");

};

access_block : access_list COMMA ACC_TYPE SEMI{
	     printDebug("Parsed access_block\n");
		Address2d *access =(Address2d*) ($1->data);
		printf("A[%d][%d],%d\n",access->i,access->j,$3);
		performBlockRead(access->i,access->j,$3,data_elements1,pR,mode);
		printf("> ");

};

sparse_access: access_list SEMI{
	     printDebug("Parsed an access\n");
		if(getLength($1)==1){
			printDebug("It is a single access \n");
			void *tmp = ($1)->data;
			Address2d *access =(Address2d*)tmp;
			int res = readFromPR(pR,access->i,access->j);
			printf("A[%d][%d]=%d\n",access->i,access->j,res);
			printf("> ");
			}
		else{
		
			printDebug("It is a parallel access \n");
		t_list *block_accesses = parallelAccessCoverage(pR,$1);
		for(int i=0; i<getLength(block_accesses);i++){
				//TODO finish to print the list of accesses
				t_list *elem = getElementAt(block_accesses,i);
				BlockAccess *elem_block=(BlockAccess*)elem->data;
				Address2d start_index = elem_block->start_index;
				acc_type type = elem_block->type;
				printf("A[%d][%d],%s -> [ ",start_index.i,start_index.j,accessStringFromAccessType(type));
				//print covered elements
				Address2d* addresses = AGU(start_index.i,start_index.j,pR->p,pR->q,type);
				printf("(%d, %d) ",addresses[0].i, addresses[0].j);
				for(int i = 1 ; i<((pR->p)*(pR->q)); i++){
					printf(",(%d, %d) ",addresses[i].i, addresses[i].j);
				}
				printf(" ]\n");

			}
				printf(">");
		}
		};

access_list: access_list COMMA access{
	   
	   $$ = addElement($1,$3,-1);			
					}
					|
	     access {
		printDebug("putting it into a list\n");
		t_list *access_list = addElement(NULL,$1,-1);
		$$ = access_list;
					};

access: A LSQUARE NUMBER RSQUARE LSQUARE NUMBER RSQUARE{
      printDebug("Found access A[%d][%d]\n",$3,$6);
      Address2d *current_access= (Address2d*) malloc(sizeof(Address2d));
		current_access->i=$3;
		current_access->j=$6;
		$$ = current_access;
	};
%%

int yyerror(char* msg) {
	printf("%s\n", msg);
	return 0;
}

int main (int argc, char* argv[]){
	opt = parseArguments(argc,argv);
	if(opt.error == 1)
		return 0;
    int p=opt.p;
    int q=opt.q;
    N=opt.N;
    M=opt.M;
	opt.s=0;
    scheme s = opt.s;
    if(N%p!=0){
	printf("N%%p!=0\n");
	return 1;
	}

if(M%q!=0){
	printf("M%%q!=0");
	return 1;
	}
    z=(N*M)/(p*q);

data_elements1 = (int**)malloc(sizeof(int*)*N);
	for(int i=0;i<N;i++){
		data_elements1[i]=(int*)malloc(sizeof(int)*M);
	}

for(int i=0;i< N;i++)
	for(int j=0;j<M;j++){
	    data_elements1[i][j]=i*M+j;
	}

pR = createPolymorphicRegister(p,q,z);
	pR->s=s;

for(int i=0;i< N;i++)
	for(int j=0;j<M;j++){
	    writeToPR(pR,data_elements1[i][j],i,j);
	}

printDebug("start parsing\n");
    printf("> ");
	return yyparse();
}
