%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"
%}
number [0-9]+
digit  [0-9]
%%
[/][/].*$	{}
int			{store_token_name("INTEGER",yytext,yylineno); yylval.string_value = new string(yytext);  return INTEGER;}
float		{store_token_name("FLOAT",yytext,yylineno);  yylval.string_value = new string(yytext); return FLOAT;}
void		{store_token_name("VOID",yytext,yylineno);  yylval.string_value = new string(yytext); return VOID;}
main		{store_token_name("NAME",yytext,yylineno);  yylval.string_value = new string(yytext); return NAME;}
if			{store_token_name("IF",yytext,yylineno);  yylval.string_value = new string(yytext); return IF;}
while 		{store_token_name("WHILE",yytext,yylineno);  yylval.string_value = new string(yytext); return WHILE;}
else		{store_token_name("ELSE",yytext,yylineno);  yylval.string_value = new string(yytext); return ELSE;}
do 			{store_token_name("DO",yytext,yylineno);  yylval.string_value = new string(yytext); return DO;}
print 			{store_token_name("PRINT",yytext,yylineno);  yylval.string_value = new string(yytext); return PRINT;}
return 			{store_token_name("RETURN",yytext,yylineno);  yylval.string_value = new string(yytext); return RETURN;}
[,;(){}?:]	{store_token_name("META CHAR",yytext,yylineno); yylval.string_value = new string(yytext); return yytext[0];}
"=="			{store_token_name("EQUAL",yytext,yylineno); yylval.string_value = new string(yytext); return EQUAL;}
"!="			{store_token_name("NOT_EQUAL",yytext,yylineno); yylval.string_value = new string(yytext); return NOTEQUAL;}
">="			{store_token_name("GREATER_THAN_EQUAL",yytext,yylineno); yylval.string_value = new string(yytext); return GREATERTHANEQUAL;}
"<="			{store_token_name("LESS_THAN_EQUAL",yytext,yylineno); yylval.string_value = new string(yytext); return LESSTHANEQUAL;}
">"			{store_token_name("GREATER_THAN",yytext,yylineno); yylval.string_value = new string(yytext); return GREATERTHAN;}
"<"			{store_token_name("LESS_THAN",yytext,yylineno); yylval.string_value = new string(yytext); return LESSTHAN;}
"||"			{store_token_name("OR",yytext,yylineno); yylval.string_value = new string(yytext); return OR;}
"&&"			{store_token_name("AND",yytext,yylineno); yylval.string_value = new string(yytext); return AND;}
"!"			{store_token_name("NOT",yytext,yylineno); yylval.string_value = new string(yytext); return NOT;}
=			{store_token_name("ASSIGN_OP",yytext,yylineno); yylval.string_value = new string(yytext); return ASSIGN;}
[-+*/]		{store_token_name("ARITHOP",yytext,yylineno); yylval.string_value = new string(yytext); return yytext[0];}
{number}					{store_token_name("NUM",yytext,yylineno);  yylval.integer_value = atoi(yytext); return INTEGER_NUMBER;}
({digit}*"."{digit}+|{digit}+".")		  		{store_token_name("FNUM",yytext,yylineno); yylval.double_value = atof(yytext); return DOUBLE_NUMBER;}
[A-Za-z_][A-Za-z0-9_]*  	{store_token_name("NAME",yytext,yylineno);  yylval.string_value = new string(strcat(yytext,"_"));  return NAME;}
[ \t\n]+ 		{}
.			{printf("cs316: Error : --no action found\n"); exit(0);}
%%