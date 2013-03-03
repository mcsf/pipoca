%option c++ yyclass="PipocaScanner"  outfile="PipocaScanner.cpp"
%option  stack noyywrap yylineno 8bit

%{ 
	
	#define TAB_INDENT 8
	
     #include <string>
     #include <sstream>
     #include <cdk/nodes/Node.h>
     #include <cdk/nodes/Sequence.h>
     #include <cdk/nodes/expressions/Expression.h>
     #include "PipocaScanner.h"
     
	 #include <stdio.h>
     #include <stdlib.h>
     #include <errno.h>
	 #include <stack>
    
    // ensure that relevant  includes are made before including the following
    #include "PipocaParser.tab.h"

	//  this is terrible, but Flex's but C++ lexers are crap...
    #define yyerror LexerError

	std::stringstream err;
    std::string* str; 
    
	bool joinLine = false;
	bool endBlocks = false;
	int counter; 
	std::stack<int> stackInd; 

    std::string strOverflow = "Overflow  error!";
    
%}


DIG		[0-9]
DIG8	[0-7]
DIGF	[0-9a-fA-F]


OCT		0{DIG8}*
DEC		[1-9]{DIG}*
HEX		0x{DIGF}+

INT 	({DEC}|{HEX}|{OCT})
REAL 	(({DIG}*\.{DIG}+)|({DIG}+\.{DIG}*))([eE][+-]?{DIG}+)?

SPACE 	" "
TAB   	\t
WHITE	{SPACE}|{TAB}

BR		\r?\n|\r
ALL		.|\n	 

%x  X_COMMENT_O X_STRING X_INDENT X_COMMENT_E

%%


^{WHITE}*{BR}		  ;					//ignore white lines

<INITIAL>^.*		  	{ 
							if(!joinLine) {yy_push_state(X_INDENT);counter=0;} 
							yyless(0);
						}
<X_INDENT>{SPACE}       { counter++; }
<X_INDENT>{TAB}         { counter = counter - counter%TAB_INDENT + TAB_INDENT;}
<X_INDENT>{BR}         	{ yy_pop_state(); /* ignore white line*/ }
<X_INDENT>"#"         	{ yy_pop_state();yy_push_state(X_COMMENT_E); }
<X_INDENT>"{"         	{ yy_push_state(X_COMMENT_O); }
<X_INDENT>.             { 	
							yyless(0);
							if(!endBlocks && (stackInd.empty() || stackInd.top()<counter)) {
									if(counter > 0){
										stackInd.push(counter);
										return PP_BLOCKBEGIN;
									}else{
										yy_pop_state(); //out of blocks
									}
							} else {
								if(!stackInd.empty() && stackInd.top()>counter) { 
									stackInd.pop();
									endBlocks = true;
									return PP_BLOCKEND;
								}
								if(!stackInd.empty() && stackInd.top()!= counter) {
									err<<"line:"<<yylineno<<" Returning to inexistent indentation.\n"; yyerror(err.str().c_str());
								} else {
									endBlocks = false;
									yy_pop_state();
								}
							}							
						}			

"if"              return PP_IF;
"then"            return PP_THEN;
"else"            return PP_ELSE;
"endif"           return PP_ENDIF;
    
"while"           return PP_WHILE;
"do"              return PP_DO;
"done"            return PP_DONE;
"until"           return PP_UNTIL;
    
"break"           return PP_BREAK;
"continue"        return PP_CONTINUE;
    
"return"          return PP_RETURN;
    
"goto"            return PP_GOTO;
    
"real"            return PP_T_REAL;
"integer"         return PP_T_INT;
"string"          return PP_T_STRING;
"const"           return PP_CONST;
"var"             return PP_VAR;
"init"            return PP_INIT;
"extern"          return PP_EXTERN;

"end"             return PP_END;
    
"not"             return PP_NOT;
"and"             return PP_AND;
"or"              return PP_OR;

"--"              return PP_DEC;
"++"              return PP_INC;
"**"              return PP_POW;
":="              return PP_ATR;

">="              return PP_GE;
"<="              return PP_LE;
"<>"              return PP_NE;

"!!"			  return PP_PLN;

\\{BR}			  { joinLine =true;}//junção de linhas
{BR}              { joinLine =false;return PP_BR;}


[-+*/?!@%=<>()\[\],]    { return *yytext; /* Operadores de um só carácter */ }




[A-Za-z_][A-Za-z0-9_]*  { yylval.s = new std::string(yytext); return PP_IDENTIFIER; }

{OCT}                   { yylval.i = strtol(yytext+1, NULL,  8); if(errno == ERANGE)  yyerror(strOverflow.c_str()); return PP_INT; }
{DEC}                   { yylval.i = strtol(yytext  , NULL, 10); if(errno == ERANGE)  yyerror(strOverflow.c_str()); return PP_INT; }
{HEX}                   { yylval.i = strtol(yytext+2, NULL, 16); if(errno == ERANGE)  yyerror(strOverflow.c_str()); return PP_INT; }
{REAL}                  { yylval.f = strtod(yytext,NULL);        if(errno == ERANGE)  yyerror(strOverflow.c_str()); return PP_REAL; }


 /* Explanatory comments */
<INITIAL>"#"   						{ yy_push_state(X_COMMENT_O); }
<X_COMMENT_E>{BR}       			{ yy_pop_state(); }
<X_COMMENT_E>.          			;

 /* Operational comments */

<INITIAL,X_COMMENT_O>"{"   			{  yy_push_state(X_COMMENT_O); }
<X_COMMENT_O>"}"                    {  yy_pop_state(); }
<X_COMMENT_O>{ALL}                  ;


 /* Strings */

<INITIAL>'                         	{ yy_push_state(X_STRING); str =   new std::string(""); }
<X_STRING>\\'                      	{ str->append("'"); }
<X_STRING>'                        	{ yy_pop_state(); yylval.s = str; return PP_STRING; }
<X_STRING>\\$                      	;
<X_STRING>\\\\                      { str->append("\\"); }
<X_STRING>\\n                     	{ str->append("\n"); }
<X_STRING>\\t                       { str->append("\t"); }
<X_STRING>\\r                       { str->append("\r"); }
<X_STRING>\\b                       { str->append("\b"); }
<X_STRING>\\{DIG8}{3}              	{  int i = strtol(yytext+1, NULL,  8); str->append(1,char(i)); if(i>255) yyerror("Octal char must be lesser than 377(oct)\n");   }
<X_STRING>\\[xX]{DIGF}{2}           { int i = strtol(yytext+2, NULL, 16); str->append(1,char(i)); }
<X_STRING>\\.                       { str->append(yytext);}
<X_STRING>.                         { str->append(yytext);}

{WHITE}								;


.  { err<<"Unknown character:'"<<yytext<<"' ascii:"<<(int)yytext[0]<<" line:"<<yylineno<<'\n'; yyerror(err.str().c_str()); }

%%

/**
 * Very, very dirty hack: flex is a mess generating C++.
 */
 
int PipocaScanner::yywrap() { return 1; }
