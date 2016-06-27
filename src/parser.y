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
int line_num;
int N;
int M;
int z;
//Here due to parsing error handling, figure out how to take it away
int **data_elements1;
PolymorphicRegister *pR;
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
	    	access_single   |
	    	access_block    |
		set_instruction |
		show_instruction| 
		access_all	|	     
		function_call   |
		error { 
		printf("> ");
			}   
		/*|
	     access_multiple   |
	     check_instruction */
		;
	     
	     
access_single : A LSQUARE NUMBER RSQUARE LSQUARE NUMBER RSQUARE SEMI{
		printDebug("Parsed access_single\n");
		int res = readFromPR(pR,$3,$6);
		printf("A[%d][%d]=%d\n",$3,$6,res);
		printf("> ");

		};

access_block : A LSQUARE NUMBER RSQUARE LSQUARE NUMBER RSQUARE COMMA ACC_TYPE SEMI{
		printDebug("Parsed access_block\n");
		printf("A[%d][%d],%d\n",$3,$6,$9);
	 	performBlockRead($3,$6,$9,data_elements1,pR);
		printf("> ");

	};

access_all : A ALL SEMI{
		printDebug("Parsed access_all\n"); 
			 
	 	performBlockRead(0,0,RECTANGLE,data_elements1,pR);
	 	performBlockRead(0,0,ROW,data_elements1,pR);
	 	performBlockRead(0,0,COLUMN,data_elements1,pR);
	 	performBlockRead(0,0,MAIN_DIAG,data_elements1,pR);
	 	performBlockRead(0,0,TRANSP_RECTANGLE,data_elements1,pR);
	 	performBlockRead(0,M-1,SECONDARY_DIAG,data_elements1,pR);
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
				raw_elem = getElementAt($3,0);
				int index_j = *((int*)(raw_elem->data));
				int mv= m_v(index_i,index_j,pR->s,pR->p,pR->q);
				int mh= m_h(index_i,index_j,pR->s,pR->p,pR->q);
				printf("(%d,%d)\n> ",mv,mh);
			}
		}else{
			printf("Function %s doesn't exist\n> ",$1);
		}
	}

arg_list: arg_list COMMA NUMBER{
		printDebug("Adding element %d to the given list\n",$3);
		int *elem = (int*)malloc(sizeof(int));
		*elem =$3;
		$$ = addElement($1,elem,-1);
		int length = getLength($$);
		printDebug("List length: %d\n",length);
		for(int i =0; i<length;i++){
			t_list *raw_elem = getElementAt($$,i);
			int elem = *((int*)(raw_elem->data));
			printDebug("Element %d: %d\n",i,elem);
		}
	}
	| NUMBER
	{	printDebug("Creating a new arg_list and adding %d\n",$1);
		int *elem = (int*)malloc(sizeof(int));
		*elem =$1;
		$$ = addElement(NULL,elem,-1);	
		t_list *retrived = getElementAt($$,0);
		int retrived_int = *((int*)(retrived->data));
		printDebug("Retrived element : %d\n",retrived_int);
	}
;

set_instruction: SET IDENTIFIER NUMBER SEMI{
		printDebug("parsed set_instruction\n");
		if(strcmp($2,"s")==0){
			//ADD FREE old pR 
			pR->s = $3;
			for(int i=0;i< N;i++)
        			for(int j=0;j<M;j++){
            				writeToPR(pR,data_elements1[i][j],i,j);
        			}
			printf("s=%d\n",pR->s);	
		}	
		printf("> ");
	};

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
			printf("> ");

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
