%{
#include <cstdlib>  // needed for getenv
#include <iostream>
#include <string>
#include <stack>

#include "syntax/FlexBYACC.h"
#include "nodes/all.h"
#include "semantics/ExpressionType.h"

#define LINE scanner()->lineno()

// The rules below will be included in the function yyparse, the main
// parsing function. The macro below will cause the function to become
// a method instead. This is very fragile, but works with byacc.
// The class is FlexBYACC, a cdk::Parser subclass for the Flex+BYACC
// toolset.
#define yyparse pipoca::syntax::FlexBYACC::yyparse

typedef cdk::node::expression::Expression* exprPtr;
typedef cdk::node::Node* nodePtr;

%}

%union {
  int                               i;           /* integer value */
  double                            f;           /* real value */
  std::string                       *s;          /* symbol name or string literal */
  cdk::node::Node                   *node;       /* node pointer */
  cdk::node::Sequence               *sequence;
  cdk::node::expression::Expression *expression; /* expression nodes */
  ExpressionType					*EType;		 /* Expression types */	
  cdk::node::expression::Identifier *id;
  pipoca::node::expression::Index   *index;
};

%token <i> PP_INT
%token <s> PP_IDENTIFIER PP_STRING
%token <f> PP_REAL


%token PP_IF PP_THEN PP_WHILE PP_DO PP_UNTIL PP_BREAK PP_CONTINUE PP_RETURN PP_GOTO 
%token PP_T_INT PP_T_REAL PP_T_STRING PP_CONST PP_VAR PP_INIT PP_EXTERN  PP_END PP_ENDIF PP_DONE PP_ELSE PP_BLOCKBEGIN PP_BLOCKEND PP_BR
 
%nonassoc PP_FDECL
%nonassoc PP_BR 
	
%nonassoc    '!' PP_PLN
%right       PP_ATR
%left        PP_OR
%left        PP_AND
%left        '=' PP_NE
%nonassoc    '<' '>' PP_LE PP_GE
%left        '+' '-'
%left        '*' '/' '%'
%right       PP_POW
%nonassoc    PP_NOT
%nonassoc    PP_DEC PP_INC
%nonassoc    '?'
%nonassoc    '@'
%left        '[' ']'
%nonassoc    '(' ')'

%nonassoc    UMINUS

/* State naming:
*	'_o'  optional
*	'_s'  sequence
*	'_so' optional sequence
*	'Ti'  type int
*	'Tr'  type real
*	'Ts'  type string
*	'Tv'  type vector
*	'Ta'  type int|real|string (atomic)
*/

/* I-File & Declarations */
%start file
%type <node> decl function var varext
%type <sequence> decl_s

/* II-Block & Lines */	
%type <node> linevarTa block
%type <sequence> linevarTa_s linevarTa_so

/* III-Type & Args & Identifiers */
%type <EType> type_o type
%type <id> identifier 
%type <index> index  id_index
%type <expression> arg
%type <sequence> arg_s arg_so identifier_s id_index_s

/* IV-Literals */
%type <sequence> literal_s literalTa_s 
%type <expression> literal literalTi_o literalTi literalTr literalTs literalTv literalTa

/* V-Instructions*/
%type <sequence> instr_s
%type <node> instrln instrs instr instrelse_o instrfinal 
%type <expression> condition

/* VI-Expressions */
%type <sequence> expr_s expr_so
%type <expression> expr


%%



/* BOF I-File & Declarations */	
file		: decl_s 				{_compiler->syntax(new pipoca::node::ProgramNode(LINE, $1));};

decl_s		: decl					{ $$ = new  cdk::node::Sequence(LINE, $1);}
			| decl_s decl			{ $$ = new  cdk::node::Sequence(LINE, $2, $1);}
			;

decl		: var PP_BR													{ $$ = $1;}
			| function													{ $$ = $1;}
			| PP_EXTERN varext PP_BR									{ $$ = new pipoca::node::VarExtNode(LINE,$2);} 				//5 Declarações exteriores
			;

function	: type_o identifier '(' arg_so ')' PP_BR %prec PP_FDECL { $$ = new pipoca::node::FunctionDeclNode(LINE,$1,$2,$4); }
			| type_o identifier  '(' arg_so ')' PP_BR block PP_END PP_BR{ $$ = new pipoca::node::FunctionDefNode(LINE,$1,$2,$4,$7); }
			;

var			: PP_CONST identifier_s PP_ATR literal_s	{ $$ = new pipoca::node::VarConstNode(LINE,$2,$4);}
			| PP_INIT id_index PP_ATR expr  			{ $$ = new pipoca::node::VarInitNode(LINE,$2,$4);}
			| PP_VAR  type_o id_index_s					{ $$ = new pipoca::node::VarNode(LINE,$2,$3);}
			;
varext		: PP_CONST identifier_s 					{ $$ = new pipoca::node::VarConstNode(LINE,$2,NULL);}
			| PP_INIT id_index	 			  			{ $$ = new pipoca::node::VarInitNode(LINE,$2,NULL);}
			| PP_VAR  type_o id_index_s					{ $$ = new pipoca::node::VarNode(LINE,$2,$3);}
			;			
/* EOF I-File & Declarations */


/* BOF II-Block & Lines */	
block		: PP_BLOCKBEGIN linevarTa_so instrs PP_BLOCKEND	{ $$ = new pipoca::node::BlockNode(LINE, $2, $3); };		

			
linevarTa 	: var   PP_BR 			{ $$ = $1;};			

linevarTa_s	: linevarTa				{ $$ = new cdk::node::Sequence(LINE, $1);}
			| linevarTa_s linevarTa	{ $$ = new cdk::node::Sequence(LINE, $2, $1);}
			;
linevarTa_so: linevarTa_s			{ $$ = $1;}
			|						{ $$ = NULL;}
			;
/* EOF II-Block & Lines */


/* BOF III-Type & Args & Identifiers */
type_o		: type							{ $$ = $1;}
			|								{ $$ = new ExpressionType(TYPE_INT);}
			;
type		: PP_T_INT						{ $$ = new ExpressionType(TYPE_INT);}
			| PP_T_REAL						{ $$ = new ExpressionType(TYPE_REAL);}
			| PP_T_STRING					{ $$ = new ExpressionType(TYPE_STRING);}
			;
arg			: type_o id_index				{ $$ = new pipoca::node::expression::Argument(LINE,$1,$2);};

arg_s		: arg							{ $$ = new cdk::node::Sequence(LINE,$1);}
			| arg_s ',' arg					{ $$ = new cdk::node::Sequence(LINE, $3, $1);}
			;

arg_so		: arg_s							{ $$ = $1;}
			| 								{ $$ = NULL;}
			;
	
identifier  : PP_IDENTIFIER					{ $$ = new cdk::node::expression::Identifier(LINE, $1);};
identifier_s: identifier					{ $$ = new cdk::node::Sequence(LINE,$1 );}
			| identifier_s ',' identifier	{ $$ = new cdk::node::Sequence(LINE, $3, $1);}
			;
id_index_s	: id_index						{ $$ = new cdk::node::Sequence(LINE,$1 );}
			| id_index_s ',' id_index		{ $$ = new cdk::node::Sequence(LINE, $3, $1);}
			;
index		: identifier '[' expr ']' 		{ $$ = new pipoca::node::expression::Index(LINE, $1, $3);}
			;
id_index	: index							{ $$ = $1;}			
			| identifier  					{ $$ = new pipoca::node::expression::Index(LINE, $1, new cdk::node::expression::Integer(LINE, 0));}
			| identifier  '['']'			{ $$ = new pipoca::node::expression::Index(LINE, $1, NULL);}
			;
			
/* EOF III-Type & Args & Identifiers */		


/* BOF IV-Literals */				 
literalTi	: PP_INT					{ $$ = new cdk::node::expression::Integer(LINE, $1);};
literalTi_o : literalTi					{ $$ = $1;}
			|							{ $$ = new cdk::node::expression::Integer(LINE, 1);;}
			;
literalTr	: PP_REAL					{ $$ = new cdk::node::expression::Double(LINE, $1); };
literalTs	: PP_STRING					{ $$ = new cdk::node::expression::String(LINE, $1); };
literalTv	: '[' literalTa_s ']'		{ $$ = new pipoca::node::expression::Vector(LINE, $2); };
literalTa	: literalTi					{ $$ = $1;}
			| literalTr					{ $$ = $1;}
			| literalTs					{ $$ = $1;}
			;
literal		: literalTa					{ $$ = $1;}
			| literalTv					{ $$ = $1;}
			;
literal_s	: literal					{ $$ = new cdk::node::Sequence(LINE,$1);}
			| literal_s ',' literal		{ $$ = new cdk::node::Sequence(LINE, $3, $1);}
			;
literalTa_s	: literalTa					{ $$ = new cdk::node::Sequence(LINE,$1);}
			| literalTa_s ',' literalTa	{ $$ = new cdk::node::Sequence(LINE, $3, $1);}
			;
/* EOF IV-Literals */		
			 

/* BOF V-Instructions */

instrln		: instr	PP_BR				{ $$ = new cdk::node::Sequence(LINE, $1);}
			| br_o block 				{ $$ = new cdk::node::Sequence(LINE, $2);}					//7.2 Blocos
			;
			
instr_s		: instrln					{ $$ = new cdk::node::Sequence(LINE, $1);}
			| instr_s instrln			{ $$ = new cdk::node::Sequence(LINE, $2, $1);}
			;

instrs		: instr_s instrfinal PP_BR 	{ $$ = new cdk::node::Sequence(LINE, $2, $1);}
			| instr_s					{ $$ = $1;}	
			| instr						{ $$ = $1;}	
			| instrfinal br_o			{ $$ = $1;}	
        	;			
	
instrfinal	: PP_CONTINUE	literalTi_o	{ $$ = new pipoca::node::ContinueNode(LINE, $2);} 								//7.6 Instrução de continuação
			| PP_BREAK		literalTi_o { $$ = new pipoca::node::BreakNode(LINE, $2);} 									//7.7 Instrução de terminação
			| PP_RETURN					{ $$ = new pipoca::node::ReturnNode(LINE);} 									//7.5 Instrução de retorno	
			;
			
instr		: expr											 	  { $$ = new pipoca::node::InstructionNode(LINE,$1);}			//7.1 Expressão como instrução
        	| PP_IF condition PP_THEN instrs instrelse_o PP_ENDIF { $$ = new pipoca::node::IfElseNode(LINE, $2, $4, $5);}		//7.3 Instrução conditional
			| PP_WHILE condition PP_DO instrs instrelse_o PP_DONE { $$ = new pipoca::node::WhileElseNode(LINE, $2, $4, $5); }	//7.4 Instruções de iteração
			| PP_DO instrs PP_UNTIL condition instrelse_o PP_DONE { $$ = new pipoca::node::DoElseNode(LINE, $4, $2, $5); }		//7.4 Instruções de iteração			
			| '[' identifier ']' PP_ATR expr 				 	  { $$ = new pipoca::node::MemoryAllocationNode(LINE, $2, $5);}	//7.8 Reserva de memória
        	;
instrelse_o	: PP_ELSE instrs								 { $$ = $2;}
			|												 { $$ = NULL;}
			;
condition	: expr											 { $$ = new pipoca::node::expression::Condition(LINE,$1);}
			;
			
/* EOF V-Instructions */


/* BOF VI-Expressions */
expr_s		: expr					{ $$ = new cdk::node::Sequence(LINE, $1); }
       		| expr_s ',' expr		{ $$ = new cdk::node::Sequence(LINE, $3, $1); } 
        	;
        
expr_so		: expr_s				{ $$ = $1; }
        	|						{ $$ = NULL; }
       		;

expr		: identifier			{ $$ = $1;} 												//8.1.1 Identificadores
			| literal				{ $$ = $1;} 												//8.1.2 Literias
		 	| '(' expr ')'			{ $$ = new pipoca::node::expression::Brackets(LINE, $2);} 	//8.1.3 Parênteses curvos
			| index					{ $$ = $1;}													//8.1.4 Indexação
			| identifier '(' expr_so ')' { $$ = new pipoca::node::expression::FunctionCall(LINE, $1, $3); }	//8.1.5 Invocação de funções
			| '@' identifier		{ $$ = new pipoca::node::expression::Address(LINE, $2);}	//8.2.1 Expressão de indicação de posição
			| identifier  '?'		{ $$ = new pipoca::node::expression::Read(LINE, $1);}		//8.2.2 Leitura
			| '+' expr %prec UMINUS	{ $$ = $2;} 												//8.2.3 Identidade
			| '-' expr %prec UMINUS	{ $$ = new cdk::node::expression::NEG(LINE, $2);}			//8.2.4 Simétrico
			| PP_INC identifier 	{ $$ = new pipoca::node::expression::INC(LINE, $2, false);}	//8.2.5 Incremento (prefixo)
			| identifier PP_INC		{ $$ = new pipoca::node::expression::INC(LINE, $1, true);}	//8.2.5 Incremento (sufixo)
			| PP_DEC identifier 	{ $$ = new pipoca::node::expression::DEC(LINE, $2, false);}	//8.2.6 Decremento (prefixo)
			| identifier PP_DEC		{ $$ = new pipoca::node::expression::DEC(LINE, $1, true);}	//8.2.6 Decremento (sufixo)
			| expr PP_POW expr		{ $$ = new pipoca::node::expression::POW(LINE, $1, $3);}	//8.3   Expressão potência	
			| expr '*' expr			{ $$ = new cdk::node::expression::MUL(LINE, $1, $3);} 		//8.4   Expressões multiplicativas
			| expr '/' expr			{ $$ = new cdk::node::expression::DIV(LINE, $1, $3);} 		//8.4   Expressões multiplicativas	 	
		 	| expr '%' expr			{ $$ = new cdk::node::expression::MOD(LINE, $1, $3);} 		//8.4.2 Expressões multiplicativas
			| expr '+' expr			{ $$ = new cdk::node::expression::ADD(LINE, $1, $3);} 		//8.5   Expressões aditivas
		 	| expr '-' expr			{ $$ = new cdk::node::expression::SUB(LINE, $1, $3);} 		//8.5   Expressões aditivas
			| expr '<' expr			{ $$ = new cdk::node::expression::LT(LINE, $1, $3);} 		//8.6   Expressões de comparação
		 	| expr '>' expr			{ $$ = new cdk::node::expression::GT(LINE, $1, $3);} 		//8.6   Expressões de comparação
		 	| expr PP_GE expr       { $$ = new cdk::node::expression::GE(LINE, $1, $3);} 		//8.6   Expressões de comparação
		 	| expr PP_LE expr       { $$ = new cdk::node::expression::LE(LINE, $1, $3);} 		//8.6   Expressões de comparação
		 	| expr '=' expr			{ $$ = new cdk::node::expression::EQ(LINE, $1, $3);} 		//8.7   Expressões de igualdade
		 	| expr PP_NE expr       { $$ = new cdk::node::expression::NE(LINE, $1, $3);} 		//8.7   Expressões de igualdade
			| PP_NOT expr			{ $$ = new pipoca::node::expression::NOT(LINE, $2);} 		//8.8   Expressões lógicas
			| expr PP_AND expr		{ $$ = new pipoca::node::expression::AND(LINE, $1, $3);}   	//8.8   Expressões lógicas
			| expr PP_OR expr		{ $$ = new pipoca::node::expression::OR(LINE, $1, $3);} 	//8.8   Expressões lógicas
			| identifier PP_ATR expr			  { $$ = new pipoca::node::expression::Attribution(LINE, $1, $3);} 				//8.9   Expressão de atribuição
			| identifier '[' expr ']' PP_ATR expr { /*$$ = new pipoca::node::expression::AttributionIndex(LINE, $1, $2, $3);*/} //8.9   Expressão de atribuição
        	| expr '!'				{ $$ = new pipoca::node::expression::Print(LINE, $1, false);} 		//8.10 Expressão escrita
        	| expr PP_PLN			{ $$ = new pipoca::node::expression::Print(LINE, $1, true);} 	//8.10 Expressão escrita
		 	;

			
/* EOF VI-Expressions */

br_o		: PP_BR {}
			| 		{}
			;
		 
%%

