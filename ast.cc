// #include "ast.hh"


	Ast::Ast(){}
	Ast::~Ast(){}
	bool Ast::is_value_zero(){}
	bool Ast::check_ast(){}
	Data_Type Ast::get_data_type(){ return node_data_type;}
	void Ast::set_data_type(Data_Type dt){ node_data_type = dt; return;}
	Symbol_Table_Entry & Ast::get_symbol_entry(){}

	int Ast::labelCounter=0;


	Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
		lhs = temp_lhs;
		rhs = temp_rhs;
		lineno = line;
		ast_num_child = binary_arity;
	}
	Assignment_Ast::~Assignment_Ast(){
		delete lhs;
		delete rhs;
	}

	bool Assignment_Ast::check_ast(){
		return lhs->get_data_type() == rhs->get_data_type();
	}


	void Assignment_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_SPACE<<"Asgn:\n";
		file_buffer<<AST_NODE_SPACE<<"LHS (";
		lhs->print(file_buffer);
		file_buffer<<")\n";
		file_buffer<<AST_NODE_SPACE<<"RHS (";
		rhs->print(file_buffer);
		file_buffer<<")";
	}


	Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
		variable_symbol_entry = & var_entry;
		lineno = line;	
		ast_num_child = zero_arity;
		// node_data_type = var_entry.get_data_type();
	}
	Name_Ast::~Name_Ast(){}

	Data_Type Name_Ast::get_data_type(){
		return variable_symbol_entry->get_data_type();
	}
	Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
		return *variable_symbol_entry;
	}
	void Name_Ast::set_data_type(Data_Type dt){
		node_data_type = dt;
		return;
	}

	void Name_Ast::print(ostream & file_buffer){
		file_buffer<<"Name : "<<variable_symbol_entry->get_variable_name();
		return;
	}


	template class Number_Ast<int>;
	template class Number_Ast<double>;

	template <class T>
	Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
		constant = number;
		node_data_type = constant_data_type;
		lineno = line;
		ast_num_child = zero_arity;

	}


	template <class T>
	Data_Type Number_Ast<T>::get_data_type(){
		return node_data_type;
	}
	template <class T>
	void Number_Ast<T>::set_data_type(Data_Type dt){
		node_data_type = dt;
		return;
	}
	template <class T>
	bool Number_Ast<T>::is_value_zero(){
		return constant==0;
	}

	template <class T>
	void Number_Ast<T>::print(ostream & file_buffer){
		file_buffer<<"Num : "<<constant;
		return;
	}




	Data_Type Arithmetic_Expr_Ast::get_data_type(){
		return node_data_type;
	}
	void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
		node_data_type = dt;
		return;
	}
	bool Arithmetic_Expr_Ast::check_ast(){
		if (ast_num_child == binary_arity){
			return lhs->get_data_type() == rhs->get_data_type();
		}
		return true;
	}


	Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
		lhs = l;
		rhs = r;
		lineno = line;
		ast_num_child = binary_arity;

	}

	void Plus_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: PLUS\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
		lhs->print(file_buffer);
		file_buffer<<")\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
		rhs->print(file_buffer);
		file_buffer<<")";
	}



	Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
		lhs = l;
		rhs = r;
		lineno = line;
		ast_num_child = binary_arity;

	}

	void Minus_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: MINUS\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
		lhs->print(file_buffer);
		file_buffer<<")\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
		rhs->print(file_buffer);
		file_buffer<<")";
	}



	Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
		lhs = l;
		rhs = r;
		lineno = line;
		ast_num_child = binary_arity;

	}

	void Divide_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: DIV\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
		lhs->print(file_buffer);
		file_buffer<<")\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
		rhs->print(file_buffer);
		file_buffer<<")";
	}


	Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
		lhs = l;
		rhs = r;
		lineno = line;
		ast_num_child = binary_arity;

	}

	void Mult_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: MULT\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
		lhs->print(file_buffer);
		file_buffer<<")\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
		rhs->print(file_buffer);
		file_buffer<<")";
	}


	UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
		lhs = l;
		rhs = r;
		lineno = line;
		ast_num_child = unary_arity;

	}

	void UMinus_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_NODE_SPACE<<"Arith: UMINUS\n";
		file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
		lhs->print(file_buffer);
		file_buffer<<")";
	}



	Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line){
		this->cond = cond;
		this->lhs = l;
		this->rhs = r;
		lineno = line;
		this->get_new_label();
	}
	Conditional_Expression_Ast::~Conditional_Expression_Ast(){}

	void Conditional_Expression_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_SPACE<<"Cond:\n"<<AST_NODE_SPACE<<"IF_ELSE";
		cond->print(file_buffer);
		file_buffer<<"\n"<<AST_NODE_SPACE<<"LHS (";
		lhs->print(file_buffer);
		file_buffer<<")\n"<<AST_NODE_SPACE<<"RHS (";
		rhs->print(file_buffer);
		file_buffer<<")";
	}

	
	Return_Ast::Return_Ast(Ast * ret_val, string name, int line){
		return_value = ret_val;
		proc_name = name;
		lineno = line;
		if(ret_val==NULL)
			node_data_type == void_data_type;
		else
			node_data_type = ret_val->get_data_type();
	}
	Return_Ast::~Return_Ast(){}
	Data_Type Return_Ast::get_data_type(){
		return node_data_type;
	}

	void Return_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_SPACE<<"RETURN ";
		if(return_value == NULL){
			file_buffer<<"<NOTHING>";
			return;
		}
		return_value->print(file_buffer);
		file_buffer<<endl;
	}


	Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line){
		lhs_condition = lhs;
		rhs_condition = rhs;
		lineno = line;
		rel_op = rop;
		node_data_type = int_data_type;
	}
	Relational_Expr_Ast::~Relational_Expr_Ast(){}

	Data_Type Relational_Expr_Ast::get_data_type(){
		return node_data_type;
	}
	void Relational_Expr_Ast::set_data_type(Data_Type dt){
		node_data_type = dt;
	}

	bool Relational_Expr_Ast::check_ast(){
		if(lhs_condition->get_data_type()==rhs_condition->get_data_type()){
			return true;
		}
		return false;
	}

	void Relational_Expr_Ast::print(ostream & file_buffer){
		// file_buffer<<"RELOP\n";
		file_buffer<<"\n"<<AST_NODE_SPACE<<"Condition: ";
		switch(rel_op){
			case less_equalto : file_buffer<<"LE"; break;
			case less_than : file_buffer<<"LT"; break;
			case greater_than : file_buffer<<"GT"; break;
			case greater_equalto : file_buffer<<"GE"; break;
			case equalto : file_buffer<<"EQ"; break;
			case not_equalto : file_buffer<<"NE"; break;
		}
		file_buffer<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";
		lhs_condition->print(file_buffer);
		file_buffer<<")\n"<<AST_SUB_NODE_SPACE<<"RHS (";
		rhs_condition->print(file_buffer);
		file_buffer<<")";
	}


	Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
		lhs_op = lhs;
		rhs_op = rhs;
		bool_op = bop;
		node_data_type = int_data_type;

		if(bop == _logical_not)
			ast_num_child = unary_arity;
		else
			ast_num_child = binary_arity;
		lineno = line;

	}
	Logical_Expr_Ast::~Logical_Expr_Ast(){}

	Data_Type Logical_Expr_Ast::get_data_type(){
		return node_data_type;
	}
	void Logical_Expr_Ast::set_data_type(Data_Type dt){
		node_data_type = dt;
	}

	bool Logical_Expr_Ast::check_ast(){
		if(ast_num_child == binary_arity)
			return rhs_op!=NULL && lhs_op!=NULL;
		else
			return rhs_op!=NULL && lhs_op==NULL;
	}

	void Logical_Expr_Ast::print(ostream & file_buffer){
		// file_buffer<<"LOGICALEXPR\n";
		file_buffer<<"\n"<<AST_NODE_SPACE<<"Condition: ";
		switch(bool_op){
			case _logical_not : file_buffer<<"NOT"; break;
			case _logical_and : file_buffer<<"AND"; break;
			case _logical_or : file_buffer<<"OR"; break;
		}
		if(ast_num_child==binary_arity){
		file_buffer<<"\n"<<AST_SUB_NODE_SPACE<<"LHS (";
		lhs_op->print(file_buffer);
		file_buffer<<")";
		}
		file_buffer<<"\n"<<AST_SUB_NODE_SPACE<<"RHS (";
		rhs_op->print(file_buffer);
		file_buffer<<")";
	}


	Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line){
		this->cond = cond;
		this->then_part = then_part;
		this->else_part = else_part;
		node_data_type = void_data_type;

		if(else_part == NULL)
			ast_num_child = binary_arity;
		else
			ast_num_child = ternary_arity;
		lineno = line;
		// this->get_new_label();
		
	}
	Selection_Statement_Ast::~Selection_Statement_Ast(){}

	Data_Type Selection_Statement_Ast::get_data_type(){
		return node_data_type;
	}
	void Selection_Statement_Ast::set_data_type(Data_Type dt){
		node_data_type = dt;
	}

	bool Selection_Statement_Ast::check_ast(){
		return cond!=NULL && then_part!=NULL;
	}

	void Selection_Statement_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_SPACE<<"IF : \n"<<AST_SPACE<<"CONDITION (";
		cond->print(file_buffer);
		file_buffer<<")\n"<<AST_SPACE<<"THEN (";
		then_part->print(file_buffer);
		file_buffer<<")";
		if(this->ast_num_child == ternary_arity){
			file_buffer<<"\n"<<AST_SPACE<<"ELSE (";
			else_part->print(file_buffer);
			file_buffer<<")";
		}
		// file_buffer<<"Selection_Statement_Ast\n";
	}



	Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form){
		this->cond = cond;
		this->body = body;
		this->is_do_form = do_form;
		node_data_type = void_data_type;
		lineno = line;
		ast_num_child = binary_arity;
	}
	Iteration_Statement_Ast::~Iteration_Statement_Ast(){}

	Data_Type Iteration_Statement_Ast::get_data_type(){
		return node_data_type;
	}
	void Iteration_Statement_Ast::set_data_type(Data_Type dt){
		node_data_type = dt;
	}

	bool Iteration_Statement_Ast::check_ast(){
		return cond!=NULL && body!=NULL;
	}

	void Iteration_Statement_Ast::print(ostream & file_buffer){
		// file_buffer<<"ITERATION\n";
		if(is_do_form){
			file_buffer<<"\n"<<AST_SPACE<<"DO (";
			body->print(file_buffer);
			file_buffer<<")\n"<<AST_SPACE<<"WHILE CONDITION (";
			cond->print(file_buffer);
			file_buffer<<")";
		}
		else{
			file_buffer<<"\n"<<AST_SPACE<<"WHILE : \n"<<AST_SPACE<<"CONDITION (";
			cond->print(file_buffer);
			file_buffer<<")\n"<<AST_SPACE<<"BODY (";
			body->print(file_buffer);
			file_buffer<<")";
		}
	}



	Sequence_Ast::Sequence_Ast(int line){
		lineno = line;
		node_data_type = void_data_type;
		ast_num_child = zero_arity;
		this->get_new_label();
	}
	Sequence_Ast::~Sequence_Ast(){}
	void Sequence_Ast::ast_push_back(Ast * ast){
		statement_list.push_back(ast);
	}
	void Sequence_Ast::print(ostream & file_buffer){
		list<Ast *>::iterator it;
		for(it=statement_list.begin(); it!=statement_list.end();it++){
			file_buffer<<"\n"<<AST_NODE_SPACE;
			(*it)->print(file_buffer);
		}
	}

	Print_Ast::Print_Ast(Ast *v, int line){
		var = v;
		lineno = line;
		node_data_type = v->get_data_type();
		ast_num_child = unary_arity;
	}
    Print_Ast::~Print_Ast(){}

    void Print_Ast::print(ostream & file_buffer){
    	file_buffer<<"\n"<<AST_SPACE<<"Print :";
    	file_buffer<<"\n"<<AST_SUB_NODE_SPACE<<"(";
    	var->print(file_buffer);
    	file_buffer<<")";
    }
	

    Call_Ast::Call_Ast(string name, int line){
    	procedure_name = name;
    	lineno = line;
		node_data_type = program_object.get_procedure_prototype(procedure_name)->get_return_type();
    }
	Call_Ast::~Call_Ast(){}

	Data_Type Call_Ast::get_data_type(){
		return node_data_type;
	}
	void Call_Ast::set_register(Register_Descriptor * reg){
		return_value_reg = reg;
	}

	void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list){
		list<Ast *>::iterator it;
		int i=0;
		list<Symbol_Table_Entry *> formal_param = formal_param_list.get_table();
		if(formal_param.size() != actual_param_list.size()){
			printf("cs316: Error : lists don't match size \n");
			exit(0);
		}
		for(it=actual_param_list.begin(); it!=actual_param_list.end();it++){
			if((*it)->get_data_type() != formal_param_list.get_symbol_table_entry_by_index(i+1).get_data_type()){
				printf("cs316: Error : lists don't match actually \n");
				exit(0);
			}
			i++;
		}

	}
	void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
		actual_param_list = param_list;
	}

	void Call_Ast::print(ostream & file_buffer){
		file_buffer<<"\n"<<AST_SPACE<<"FN CALL: "<<procedure_name<<"(";
		for(list<Ast *>::iterator it = actual_param_list.begin(); it!=actual_param_list.end();it++){
			file_buffer<<"\n"<<AST_NODE_SPACE;
			(*it)->print(file_buffer);
		}
		file_buffer<<")";
	}