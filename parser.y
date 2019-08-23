%{
#include <stdio.h>
#include <string.h>
extern "C" void yyerror(char *s);
extern int yylex(void);
extern int yylineno;
Symbol_Table *my_lst = new Symbol_Table();
Symbol_Table *my_gst = new Symbol_Table();
string current_proc;
int counter = 0;
bool main_defined = true;
%}
%token ASSIGN
%token VOID
%token NAME
%token INTEGER
%token FLOAT
%token IF
%token ELSE
%token DO
%token WHILE
%token PRINT
%token AND
%token OR
%token NOT
%token EQUAL
%token NOTEQUAL
%token GREATERTHANEQUAL
%token LESSTHANEQUAL
%token GREATERTHAN
%token LESSTHAN
%token INTEGER_NUMBER
%token DOUBLE_NUMBER
%token RETURN;
%left '?' ':'
%left OR
%left AND
%left EQUAL NOTEQUAL 
%left GREATERTHANEQUAL LESSTHANEQUAL LESSTHAN GREATERTHAN 
%left '+' '-'
%left '*' '/'
%left NOT
%union 
{
		int integer_value;
		double double_value;
		std::string * string_value;
		list<Ast *> * ast_list;
		Ast * ast;
		Symbol_Table * symbol_table;
		Symbol_Table_Entry * symbol_entry;
		Basic_Block * basic_block;
		Procedure * procedure;
		list<Symbol_Table_Entry *> * symbol_list;
}
%type <ast> variable constant identifier arithmetic_expr assignment_statement statement unmatched_stmt matched_stmt iterative_matched_statement 
%type <ast> iterative_unmatched_statement relational_expr logical_expr print_statement call_statement return_statement
%type <ast_list> statement_list optional_parameter_list parameter_list
%type <symbol_table> local_declaration local_variable_declaration local_variable_declaration_list optional_local_variable_declaration_list argument_list optional_argument_list
%type <symbol_table> global_declaration global_variable_declaration argument_type_list optional_argument_type_list
%type <symbol_list> variable_list
%type <string_value> NAME INTEGER FLOAT ASSIGN VOID
%type <procedure> procedure_definition
%type <integer_value> INTEGER_NUMBER
%type <symbol_entry> argument argument_type
%type <double_value> DOUBLE_NUMBER
%%
program 							: program1 program2 {
										if(!main_defined){
											yyerror("cs316: Error : Main should be defined and it's type should be void");
											exit(0);
										}
										}
program1							: program1 global_variable_declaration {
											(*my_gst).set_table_scope(global);
											program_object.set_global_table(*my_gst);
											main_defined = false;
										}
									| program1 procedure_declaration {
											my_lst = new Symbol_Table();
											main_defined = false;
										}
											
									| %empty {program_object.set_procedure_map(*(new map<string, Procedure *>));}
									; 	
program2							:	program2 procedure_definition {
											if($2->get_proc_name()=="main"){
												main_defined = true;
											}
									}
											// program_object.set_proc_to_map($2->get_proc_name(), $2);}

									|	procedure_definition {
										if($1->get_proc_name()=="main"){
												main_defined = true;
											}
										}
											// if(program_object.variable_in_proc_map_check($1->get_proc_name())){
											// 	yyerror("cs316: Error : Procedure has already been defined before");
											// 	exit(0);
											// }
											// program_object.set_proc_to_map($1->get_proc_name(), $1);}
									;


procedure_declaration				:	VOID NAME '(' optional_argument_type_list ')' ';' {
		    	     //       						if(program_object.variable_in_proc_map_check(*$2)){
												// 	yyerror("cs316: Error : Procedure has already been defined before");
												// 	exit(0);
												// }
												if(my_gst->variable_in_symbol_list_check(*$2)){
													yyerror("cs316: Error : Procedure name cannot be same as global variable");
													exit(0);
												}
		    	           						 Procedure *p = new Procedure(void_data_type, *$2, yylineno);
												 program_object.set_proc_to_map(*$2, p);
		    	           			  			(*p).set_formal_param_list(*$4);
										}

									|	INTEGER NAME '(' optional_argument_type_list ')' ';' {
		    	     //       						if(program_object.variable_in_proc_map_check(*$2)){
												// 	yyerror("cs316: Error : Procedure has already been defined before");
												// 	exit(0);
												// }
												if(my_gst->variable_in_symbol_list_check(*$2)){
													yyerror("cs316: Error : Procedure name cannot be same as global variable");
													exit(0);
												}
		    	           						Procedure *p = new Procedure(int_data_type, *$2, yylineno);
												program_object.set_proc_to_map(*$2, p);
		    	           			  			(*p).set_formal_param_list(*$4);
										}

									| 	FLOAT NAME '(' optional_argument_type_list ')' ';' {
		    	     //       						if(program_object.variable_in_proc_map_check(*$2)){
												// 	yyerror("cs316: Error : Procedure has already been defined before");
												// 	exit(0);
												// }
												if(my_gst->variable_in_symbol_list_check(*$2)){
													yyerror("cs316: Error : Procedure name cannot be same as global variable");
													exit(0);
												}
		    	           						Procedure *p = new Procedure(double_data_type, *$2, yylineno);
												program_object.set_proc_to_map(*$2, p);
		    	           			  			(*p).set_formal_param_list(*$4);
										}

									| VOID NAME '(' optional_argument_list ')' ';' {
		    	     //       						if(program_object.variable_in_proc_map_check(*$2)){
												// 	yyerror("cs316: Error : Procedure has already been defined before");
												// 	exit(0);
												// }
		    	           						 Procedure *p = new Procedure(void_data_type, *$2, yylineno);
												 program_object.set_proc_to_map(*$2, p);
		    	           			  			(*p).set_formal_param_list(*$4);
										}

									|	INTEGER NAME '(' optional_argument_list ')' ';' {
	    	     //       						if(program_object.variable_in_proc_map_check(*$2)){
												// 	yyerror("cs316: Error : Procedure has already been defined before");
												// 	exit(0);
												// }
		    	           						 Procedure *p = new Procedure(int_data_type, *$2, yylineno);
												 program_object.set_proc_to_map(*$2, p);
		    	           			  			(*p).set_formal_param_list(*$4);
										}

									| 	FLOAT NAME '(' optional_argument_list ')' ';' {
		    	     //       						if(program_object.variable_in_proc_map_check(*$2)){
												// 	yyerror("cs316: Error : Procedure has already been defined before");
												// 	exit(0);
												// }
		    	           						 Procedure *p = new Procedure(double_data_type, *$2, yylineno);
												 program_object.set_proc_to_map(*$2, p);
		    	           			  			(*p).set_formal_param_list(*$4);
										}
									;




procedure_definition				:	VOID NAME '(' optional_argument_list ')' 
			    	           						{ current_proc = *$2;
			    	           						if(program_object.is_procedure_exists(*$2) && program_object.get_procedure_prototype(*$2)->is_proc_defined()){
														yyerror("cs316: Error : Procedure has already been defined before");
														exit(0);
													}
													Procedure *p;
													if(program_object.is_procedure_exists(*$2)){
														p = program_object.get_procedure_prototype(*$2);
														if(p->get_return_type()!=void_data_type){
															yyerror("cs316: Error : return type in declaration and definition not matching");
															exit(0);
														}
				    	           			  			if(!$4->is_empty())
				    	           			  				(*p).set_formal_param_list(*$4);
													}
													else{
			    	           							p = new Procedure(void_data_type, *$2, yylineno);
														program_object.set_proc_to_map(*$2, p);
			    	           			  				(*p).set_formal_param_list(*$4);

													}
			    	           			  			p->set_proc_is_defined();
													}
			              	   		'{' 
			              	   				optional_local_variable_declaration_list
			                        		statement_list
			    	           		'}'  { 
			    	           				Procedure * p = program_object.get_procedure_prototype(*$2);
			    	           			  (*$8).set_table_scope(local);
			    	           			  (*p).set_local_list(*$8);
			    	           			 (*p).set_ast_list(*$9); 
			    	           			  my_lst = new Symbol_Table();
			    	           			  $$ = p;
			    	           			 }


			    	           		|	INTEGER NAME '(' optional_argument_list ')' 
			    	           						{ current_proc = *$2;
			    	           						if(program_object.is_procedure_exists(*$2) && program_object.get_procedure_prototype(*$2)->is_proc_defined()){
														yyerror("cs316: Error : Procedure has already been defined before");
														exit(0);
													}
													if(*$2=="main"){
														yyerror("cs316: Error : Main should be defined and it's type should be void");
														exit(0);
													}
													Procedure *p;
													if(program_object.is_procedure_exists(*$2)){
														p = program_object.get_procedure_prototype(*$2);
														if(p->get_return_type()!=int_data_type){
															yyerror("cs316: Error : return type in declaration and definition not matching");
															exit(0);
														}
				    	           			  			if(!$4->is_empty())
				    	           			  				(*p).set_formal_param_list(*$4);
													}
													else{
			    	           							p = new Procedure(int_data_type, *$2, yylineno);
														program_object.set_proc_to_map(*$2, p);
			    	           			  				(*p).set_formal_param_list(*$4);

													}
			    	           			  			p->set_proc_is_defined();
													}
			              	   		'{' 
			              	   				optional_local_variable_declaration_list
			                        		statement_list
			    	           		'}'  { 
			    	           				Procedure * p = program_object.get_procedure_prototype(*$2);
			    	           			  (*$8).set_table_scope(local);
			    	           			  (*p).set_local_list(*$8);
			    	           			 (*p).set_ast_list(*$9); 
			    	           			  my_lst = new Symbol_Table();
			    	           			  $$ = p;
			    	           			 }

			    	           		|	FLOAT NAME '(' optional_argument_list ')' 
			    	           						{ current_proc = *$2;
			    	           						if(program_object.is_procedure_exists(*$2) && program_object.get_procedure_prototype(*$2)->is_proc_defined()){
														yyerror("cs316: Error : Procedure has already been defined before");
														exit(0);
													}
													if(*$2=="main"){
														yyerror("cs316: Error : Main should be defined and it's type should be void");
														exit(0);
													}
													Procedure *p;
													if(program_object.is_procedure_exists(*$2)){
														p = program_object.get_procedure_prototype(*$2);
														if(p->get_return_type()!=double_data_type){
															yyerror("cs316: Error : return type in declaration and definition not matching");
															exit(0);
														}
				    	           			  			if(!$4->is_empty())
				    	           			  				(*p).set_formal_param_list(*$4);
													}
													else{
			    	           							p = new Procedure(double_data_type, *$2, yylineno);
														program_object.set_proc_to_map(*$2, p);
			    	           			  				(*p).set_formal_param_list(*$4);

													}
			    	           			  			p->set_proc_is_defined();
													}
			              	   		'{' 
			              	   				optional_local_variable_declaration_list
			                        		statement_list
			    	           		'}'  { 
			    	           				Procedure * p = program_object.get_procedure_prototype(*$2);
			    	           			  (*$8).set_table_scope(local);
			    	           			  (*p).set_local_list(*$8);
			    	           			 (*p).set_ast_list(*$9); 
			    	           			  my_lst = new Symbol_Table();
			    	           			  $$ = p;
			    	           			 }
			                     	;

optional_argument_type_list			:	argument_type_list {$$ = $1; counter = 0;}
									;

optional_argument_list 				:	%empty {Symbol_Table *st = new Symbol_Table(); $$=st;}
									|	argument_list {$$ = $1;}
									;

optional_parameter_list				:	%empty {list<Ast *>* temp = new list<Ast *>; $$=temp;}
									|	parameter_list {$$ = $1;}
									;


optional_local_variable_declaration_list	: %empty	{Symbol_Table *st = new Symbol_Table(); $$=st;}
									|			local_variable_declaration_list {$$ = $1;}
									;

// optional_global_variable_declaration_list	: %empty	{Symbol_Table *st = new Symbol_Table(); $$=st;}
// 									|			global_variable_declaration_list {$$ = $1;}
									;

argument_list 						:	argument_list ',' argument {
											$1->push_symbol($3);
											$$ = $1;
										}
									|	argument { Symbol_Table* st = new Symbol_Table();
													st->push_symbol($1);
													$$ = st;}
									;

argument_type_list 					:	argument_type_list ','  argument_type {
											$1->push_symbol($3);
											$$ = $1;
										}
									|	argument_type { Symbol_Table* st = new Symbol_Table();
													st->push_symbol($1);
													$$ = st;}
									;


parameter_list						:	parameter_list ',' arithmetic_expr {
											$1->push_back($3);
											$$ = $1;
										}
									|	arithmetic_expr { list<Ast *>* temp = new list<Ast *>;
													temp->push_back($1);
													$$ = temp;}
									;

argument 							:	INTEGER NAME { 	Symbol_Table_Entry  *ste = new Symbol_Table_Entry(*$2, int_data_type, yylineno);
														if(my_lst->variable_in_symbol_list_check(ste->get_variable_name())){
															yyerror("cs316: Error : Variable is declared twice");
															exit(0);
														}
														ste->set_symbol_scope(formal);
														(*my_lst).push_symbol(ste);
														$$ = ste;}
									|	FLOAT NAME { 	Symbol_Table_Entry  *ste = new Symbol_Table_Entry(*$2, double_data_type, yylineno);
														if(my_lst->variable_in_symbol_list_check(ste->get_variable_name())){
															yyerror("cs316: Error : Variable is declared twice");
															exit(0);
														}
														ste->set_symbol_scope(formal);
														(*my_lst).push_symbol(ste);
														$$ = ste;}
									;

argument_type						:	INTEGER  { 	string a = "dummy"+to_string(++counter);  Symbol_Table_Entry  *ste = new Symbol_Table_Entry(a, int_data_type, yylineno);
														
														ste->set_symbol_scope(formal);
														(*my_lst).push_symbol(ste);
														$$ = ste;}
									|	FLOAT  { string a = "dummy"+ to_string(++counter); Symbol_Table_Entry  *ste = new Symbol_Table_Entry(a, double_data_type, yylineno);
														
														ste->set_symbol_scope(formal);
														(*my_lst).push_symbol(ste);
														$$ = ste;}
									;


local_variable_declaration_list			:	local_variable_declaration {$$ = $1;}
									|	local_variable_declaration_list 
										local_variable_declaration {(*$1).append_list(*$2, yylineno); $$=$1;}
									;

// global_variable_declaration_list			:	global_variable_declaration {$$ = $1;}
// 									|	global_variable_declaration_list 
// 										global_variable_declaration {(*$1).append_list(*$2, yylineno); $$=$1;}
									;

local_variable_declaration				:	local_declaration {$$ = $1;}
										;

global_variable_declaration				:	global_declaration {$$ = $1;}
										;

local_declaration							:	INTEGER variable_list ';' { Symbol_Table *st = new Symbol_Table();
																for (list<Symbol_Table_Entry *>::iterator it = (*$2).begin(); it != (*$2).end(); ++it){
																	(*it)->set_data_type(int_data_type);
																	(*it)->set_symbol_scope(local);
																	if(my_lst->variable_in_symbol_list_check((*it)->get_variable_name()) && !my_lst->variable_in_formal_list_check((*it)->get_variable_name())){
																	yyerror("cs316: Error : Variable is declared twice");
																	exit(0);
																	}
																    if (my_lst->variable_in_formal_list_check((*it)->get_variable_name())){
																    	Symbol_Table_Entry* ste = & my_lst->get_symbol_table_entry((*it)->get_variable_name());
																    	ste->set_symbol_scope(local);
																    	ste->set_data_type(int_data_type);
																    	(*st).push_symbol(ste);
																    }
																    else{
																    	(*st).push_symbol(*it);
																    	(*my_lst).push_symbol(*it);
																    }
																}
																$$ = st;}
									|	FLOAT variable_list ';' { Symbol_Table *st = new Symbol_Table();
																for (list<Symbol_Table_Entry *>::iterator it = (*$2).begin(); it != (*$2).end(); ++it){
																	(*it)->set_data_type(double_data_type);
																	(*it)->set_symbol_scope(local);
																	if(my_lst->variable_in_symbol_list_check((*it)->get_variable_name()) && !my_lst->variable_in_formal_list_check((*it)->get_variable_name())){
																	yyerror("cs316: Error : Variable is declared twice");
																	exit(0);
																	}
																    if (my_lst->variable_in_formal_list_check((*it)->get_variable_name())){
																    	Symbol_Table_Entry* ste = & my_lst->get_symbol_table_entry((*it)->get_variable_name());
																    	ste->set_symbol_scope(local);
																    	ste->set_data_type(double_data_type);
																    	(*st).push_symbol(ste);
																    }
																    else{
																    	(*st).push_symbol(*it);
																    	(*my_lst).push_symbol(*it);
																    }
																}
																$$ = st;}
                                    ;

global_declaration							:	INTEGER variable_list ';' { Symbol_Table *st = new Symbol_Table();
																for (list<Symbol_Table_Entry *>::iterator it = (*$2).begin(); it != (*$2).end(); ++it){
																	(*it)->set_data_type(int_data_type);
																	(*it)->set_symbol_scope(global);
																	if(my_gst->variable_in_symbol_list_check((*it)->get_variable_name())){
																	yyerror("cs316: Error : Variable is declared twice");
																	exit(0);
																	}
																    (*st).push_symbol(*it);
																    (*my_gst).push_symbol(*it);
																}
																$$ = st;}
									|	FLOAT variable_list ';' { Symbol_Table *st = new Symbol_Table();
																for (list<Symbol_Table_Entry *>::iterator it = (*$2).begin(); it != (*$2).end(); ++it){
																	(*it)->set_data_type(double_data_type);
																	(*it)->set_symbol_scope(global);
																	if(my_gst->variable_in_symbol_list_check((*it)->get_variable_name())){
																	yyerror("cs316: Error : Variable is declared twice");
																	exit(0);
																	}
																    (*st).push_symbol(*it);
																    (*my_gst).push_symbol(*it);

																}
																$$ = st;}
                                    ;


variable_list                       :   NAME {$$ = new list<Symbol_Table_Entry *>;;
											  Symbol_Table_Entry  *ste = new Symbol_Table_Entry(*$1, void_data_type, yylineno);
											  (*$$).push_back(ste);}
                                    |   variable_list ',' NAME { Symbol_Table_Entry  *ste = new Symbol_Table_Entry(*$3, void_data_type, yylineno);
											  					 (*$1).push_back(ste);
											  					 $$ = $1;}
									;

statement_list	        			:	%empty {$$ = new list<Ast *>;}
									|	statement_list
										statement {$1->push_back($2); $$=$1;}
									;

assignment_statement				:	variable ASSIGN arithmetic_expr ';' {Assignment_Ast* temp = new Assignment_Ast($1, $3,yylineno);
												if($1->get_data_type() != $3->get_data_type()){
													yyerror("cs316: Error : Assignment statement data type not compatible");
													exit(0);
												}
												temp->set_data_type($1->get_data_type());
																  $$ = temp;}
									|	variable ASSIGN call_statement ';' {Assignment_Ast* temp = new Assignment_Ast($1, $3,yylineno);
												if($1->get_data_type() != $3->get_data_type()){
													yyerror("cs316: Error : Assignment statement data type not compatible");
													exit(0);
												}
												temp->set_data_type($1->get_data_type());
																  $$ = temp;}	
									;

arithmetic_expr 					:	logical_expr '?' arithmetic_expr ':' arithmetic_expr {
										Conditional_Expression_Ast* temp = new Conditional_Expression_Ast($1,$3,$5,yylineno);
										if($3->get_data_type() != $5->get_data_type()){
										yyerror("cs316: Error : Arithmetic statement data type not compatible");
										exit(0);
										} 
										temp->set_data_type($3->get_data_type());
										$$ = temp;
									}
									|	arithmetic_expr '+' arithmetic_expr {Plus_Ast *temp = new Plus_Ast($1, $3, yylineno);
											if($1->get_data_type() != $3->get_data_type()){
											yyerror("cs316: Error : Arithmetic statement data type not compatible");
											exit(0);
														}
													   temp->set_data_type($1->get_data_type());
													   $$ = temp;}
									|	arithmetic_expr '-' arithmetic_expr {Minus_Ast *temp = new Minus_Ast($1, $3, yylineno);
									 if($1->get_data_type() != $3->get_data_type()){
										yyerror("cs316: Error : Arithmetic statement data type not compatible");
										exit(0); 
									 }
													   temp->set_data_type($1->get_data_type());
													   $$ = temp;}
									|	arithmetic_expr '*' arithmetic_expr {Mult_Ast* temp = new Mult_Ast($1 , $3, yylineno);
															 temp->set_data_type($1->get_data_type());
														if($1->get_data_type() != $3->get_data_type()){
															 yyerror("cs316: Error : Arithmetic statement data type not compatible");
															exit(0);
														}
															 $$ = temp;}
									|	arithmetic_expr '/' arithmetic_expr	{Divide_Ast* temp = new Divide_Ast($1 , $3, yylineno);
															 temp->set_data_type($1->get_data_type());
															 if($1->get_data_type() != $3->get_data_type()){
															 yyerror("cs316: Error : Arithmetic statement data type not compatible");
															 exit(0);
															 }
															 $$ = temp;}
									| 	identifier {$$ = $1;}
									;

identifier							:	variable {$$ = $1;}
									|	constant {$$ = $1;}
									|	'-' variable {UMinus_Ast* temp = new UMinus_Ast($2, NULL, yylineno); 
													  temp->set_data_type($2->get_data_type());
													  $$ = temp;}
									|	'-' constant {UMinus_Ast* temp = new UMinus_Ast($2, NULL, yylineno); 
													  temp->set_data_type($2->get_data_type());
													  $$ = temp;}
									;

variable							:	NAME { 
										Symbol_Table_Entry * st;
										if(my_lst->variable_in_symbol_list_check(*$1)){
											st = & my_lst->get_symbol_table_entry(*$1);
										}
										else if(my_gst->variable_in_symbol_list_check(*$1)){
											st = & my_gst->get_symbol_table_entry(*$1);
										}
										else{
											yyerror("cs316: Error : Variable has not been declared");
											exit(0);
											st= new Symbol_Table_Entry(*$1,int_data_type, yylineno);  
										}
										Name_Ast* temp =  new Name_Ast(*$1, *st, yylineno);
										$$ = temp;}

									|	'(' arithmetic_expr ')' {$$ = $2;}
									;

constant							:	INTEGER_NUMBER {
										Number_Ast<int>* temp = new Number_Ast<int>($1, int_data_type, yylineno);
										$$ = temp;}
									|	DOUBLE_NUMBER  { 
										Number_Ast<double>* temp = new Number_Ast<double>($1, double_data_type, yylineno);
										$$ = temp;}
									;

relational_expr						:	arithmetic_expr EQUAL arithmetic_expr {
										Relational_Expr_Ast* temp = new Relational_Expr_Ast($1, equalto, $3, yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	arithmetic_expr NOTEQUAL arithmetic_expr {
										Relational_Expr_Ast* temp = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	arithmetic_expr	GREATERTHANEQUAL arithmetic_expr {
										Relational_Expr_Ast* temp = new Relational_Expr_Ast($1,greater_equalto, $3, yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	arithmetic_expr LESSTHANEQUAL arithmetic_expr {
										Relational_Expr_Ast* temp = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	arithmetic_expr GREATERTHAN arithmetic_expr {
										Relational_Expr_Ast* temp = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	arithmetic_expr LESSTHAN arithmetic_expr {
										Relational_Expr_Ast* temp = new Relational_Expr_Ast($1, less_than, $3, yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									;

logical_expr						:	relational_expr {
										$$ = $1;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	'(' logical_expr  ')' {
										$$ = $2;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	logical_expr AND logical_expr {
										Logical_Expr_Ast* temp = new Logical_Expr_Ast($1, _logical_and , $3, yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	logical_expr OR logical_expr {
										Logical_Expr_Ast* temp = new Logical_Expr_Ast($1, _logical_or , $3, yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									|	NOT logical_expr {
										Logical_Expr_Ast* temp = new Logical_Expr_Ast(NULL, _logical_not , $2 , yylineno);
										$$ = temp;
										if(!$$->check_ast()){
											yyerror("cs316: Error : Relational statement data type not compatible");
											exit(0);
										}
										}
									;

statement 							: matched_stmt	{ $$ = $1;} 
									| unmatched_stmt { $$ = $1;}
									;

matched_stmt 						: 	IF logical_expr matched_stmt ELSE matched_stmt {
										Selection_Statement_Ast* temp = new Selection_Statement_Ast($2,$3,$5,yylineno);
										$$ = temp;
									}			
									| 	assignment_statement {$$ = $1;}
									|	iterative_matched_statement {$$ = $1;}
									|	print_statement {$$ = $1;}
									|	return_statement {$$ = $1;}
									|	call_statement ';' {$$ = $1;}
									| 	'{' statement_list '}' {
											Sequence_Ast* temp = new Sequence_Ast(yylineno);
											bool list_empty = true;
											for(list<Ast *>::iterator it=$2->begin();it!=$2->end();it++){
												temp->ast_push_back(*it);
												list_empty = false;
											}
											if(list_empty){
												yyerror("cs316: Error :  Block of statements cannot be empty");
												exit(0);
											}


											$$ = temp;
											}
									;

unmatched_stmt 						: 	IF logical_expr statement {
										Selection_Statement_Ast* temp = new Selection_Statement_Ast($2,$3,NULL, yylineno);
										$$ = temp;
									}
									| 	IF logical_expr matched_stmt ELSE unmatched_stmt{
										Selection_Statement_Ast* temp = new Selection_Statement_Ast($2,$3,$5,yylineno);
										$$ = temp; 
									}
									|	iterative_unmatched_statement {$$ = $1;}	
									;
iterative_matched_statement			: 	WHILE logical_expr matched_stmt {
										Iteration_Statement_Ast* temp = new Iteration_Statement_Ast($2,$3,yylineno,false);
										$$ = temp;
									}
									| 	DO matched_stmt WHILE logical_expr ';' {
										Iteration_Statement_Ast* temp = new Iteration_Statement_Ast($4,$2,yylineno,true);
										$$ = temp;
									}
									;

iterative_unmatched_statement		: 	WHILE logical_expr unmatched_stmt {
										Iteration_Statement_Ast* temp = new Iteration_Statement_Ast($2,$3,yylineno,false);
										$$ = temp;
									}
									| 	DO unmatched_stmt WHILE logical_expr ';' {
										Iteration_Statement_Ast* temp = new Iteration_Statement_Ast($4,$2,yylineno,true);
										$$ = temp;
									}
									;

print_statement						: PRINT NAME ';' {
									 	Symbol_Table_Entry * st;
										if(my_lst->variable_in_symbol_list_check(*$2)){
											st = & my_lst->get_symbol_table_entry(*$2);
										}
										else if(my_gst->variable_in_symbol_list_check(*$2)){
											st = & my_gst->get_symbol_table_entry(*$2);
										}
										else{
											yyerror("cs316: Error : Variable has not been declared");
											exit(0);  
										}
										Name_Ast* temp1 =  new Name_Ast(*$2, *st, yylineno);
										
										Print_Ast* temp = new Print_Ast(temp1, yylineno);
										$$ = temp;
									}
									;

return_statement 					: RETURN arithmetic_expr ';' {
										// Symbol_Table_Entry * st;
										// if(my_lst->variable_in_symbol_list_check(*$2)){
										// 	st = & my_lst->get_symbol_table_entry(*$2);
										// }
										// else if(my_gst->variable_in_symbol_list_check(*$2)){
										// 	st = & my_gst->get_symbol_table_entry(*$2);
										// }
										// else{
										// 	yyerror("cs316: Error : Variable has not been declared");
										// 	exit(0);  
										// }
										// Name_Ast* temp1 =  new Name_Ast(*$2, *st, yylineno);
										if(program_object.get_procedure_prototype(current_proc)->get_return_type()!=$2->get_data_type()){
											yyerror("cs316: Error : Return type not matching");
											exit(0);
										}
										Return_Ast* temp = new Return_Ast($2, current_proc, yylineno);
										$$ = temp;
									}
									| RETURN ';' {
										if(program_object.get_procedure_prototype(current_proc)->get_return_type()!=void_data_type){
											yyerror("cs316: Error : Return type not matching");
											exit(0);
										}
										Return_Ast* temp = new Return_Ast(NULL, current_proc, yylineno);
										$$ = temp;
									}
									;

call_statement 						:	NAME '(' optional_parameter_list ')' {
										if(! program_object.variable_in_proc_map_check(*$1)){
											yyerror("cs316: Error : Function has not been declared");
											exit(0);  
										}
										Call_Ast * temp = new Call_Ast(*$1, yylineno);
										temp->set_actual_param_list(*$3);
										
										Symbol_Table st = program_object.get_procedure_prototype(*$1)->get_formal_param_list();
										temp->check_actual_formal_param(st);
										$$ = temp;
										}
									;

%%
