	void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){}

	Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){}
	void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){}

	// Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){}


	Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		// printf("ASSSSSSS\n");
		if(this->get_data_type()== double_data_type){
			// Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
			// temp->set_variable_status(true);
			// double lhs_value = lhs->evaluate(eval_env, file_buffer).get_double_value();
			Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
			temp->set_variable_status(true);
			double rhs_value = rhs->evaluate(eval_env, file_buffer).get_double_value();
			temp->set_value(rhs_value);
			lhs->set_value_of_evaluation(eval_env, *temp);
			this->print(file_buffer);
			lhs->print_value(eval_env, file_buffer);
			return lhs->get_value_of_evaluation(eval_env);
		}
		else if(this->get_data_type()== int_data_type){
			Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
			temp->set_variable_status(true);
			int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
			temp->set_value(rhs_value);
			lhs->set_value_of_evaluation(eval_env, *temp);
			this->print(file_buffer);
			lhs->print_value(eval_env, file_buffer);
			return lhs->get_value_of_evaluation(eval_env);
		}
		// double rhs_value = lhs->evaluate(eval_env, file_buffer).get_double_value();
	}

	// Code_For_Ast & Assignment_Ast::compile(){}
	// Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}


	void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
		file_buffer<<"\n";
		if(this->get_data_type() == int_data_type){
			int temp = this->get_value_of_evaluation(eval_env).get_int_value();
			file_buffer<<VAR_SPACE<<variable_symbol_entry->get_variable_name()<<" : "<<temp;
		}
		else if(this->get_data_type() == double_data_type){
			double temp = this->get_value_of_evaluation(eval_env).get_double_value();
			file_buffer<<VAR_SPACE<<variable_symbol_entry->get_variable_name()<<" : "<<temp;
		}
		file_buffer<<"\n\n";
	}
	Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
		if(eval_env.is_variable_defined(this->get_symbol_entry().get_variable_name())){
			return *eval_env.get_variable_value(this->get_symbol_entry().get_variable_name());
		}
		else if(interpreter_global_table.is_variable_defined(this->get_symbol_entry().get_variable_name())){
			return *interpreter_global_table.get_variable_value(this->get_symbol_entry().get_variable_name());
		}
		else{
			printf("cs316: Error : Variable is not declared/defined\n");
			exit(0);
		}
	}
	void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
		if(eval_env.does_variable_exist(this->get_symbol_entry().get_variable_name()))
			eval_env.put_variable_value(result,this->get_symbol_entry().get_variable_name());
		else if(interpreter_global_table.does_variable_exist(this->get_symbol_entry().get_variable_name()))
			interpreter_global_table.put_variable_value(result,this->get_symbol_entry().get_variable_name());
		// eval_env.set_variable_status(>variable_symbol_entry->get_variable_name());
		// eval.
	}
	Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		return this->get_value_of_evaluation(eval_env);
	}

	// Code_For_Ast & Name_Ast::compile(){}
	// Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
	// Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){}

	// template class Number_Ast<int>;
	// template class Number_Ast<double>;

	template <>
	Eval_Result & Number_Ast<int>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
		temp->set_variable_status(true);
		temp->set_value(this->constant);
		return *temp;
	}

	template <>
	Eval_Result & Number_Ast<double>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
		temp->set_variable_status(true);
		temp->set_value(this->constant);
		return *temp;
	}

	// template <class T>
	// Code_For_Ast & Number_Ast<T>::compile(){}
	// template <class T>
	// Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){}	

	
	Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		if(this->get_data_type()== double_data_type){
			Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
			temp->set_variable_status(true);
			double lhs_value = lhs->evaluate(eval_env, file_buffer).get_double_value();
			double rhs_value = rhs->evaluate(eval_env, file_buffer).get_double_value();
			temp->set_value(lhs_value+rhs_value);
			return *temp;
		}
		else if(this->get_data_type()== int_data_type){
			Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
			temp->set_variable_status(true);
			int lhs_value = lhs->evaluate(eval_env, file_buffer).get_int_value();
			int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
			temp->set_value(lhs_value+rhs_value);
			return *temp;
		}
	}

	// Code_For_Ast & Plus_Ast::compile(){}
	// Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

	Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		if(this->get_data_type()== double_data_type){
			Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
			temp->set_variable_status(true);
			double lhs_value = lhs->evaluate(eval_env, file_buffer).get_double_value();
			double rhs_value = rhs->evaluate(eval_env, file_buffer).get_double_value();
			temp->set_value(lhs_value-rhs_value);
			return *temp;
		}
		else if(this->get_data_type()== int_data_type){
			Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
			temp->set_variable_status(true);
			int lhs_value = lhs->evaluate(eval_env, file_buffer).get_int_value();
			int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
			temp->set_value(lhs_value-rhs_value);
			return *temp;
		}
	}

	// Code_For_Ast & Minus_Ast::compile(){}
	// Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

	Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		if(this->get_data_type()== double_data_type){
			Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
			temp->set_variable_status(true);
			double lhs_value = lhs->evaluate(eval_env, file_buffer).get_double_value();
			double rhs_value = rhs->evaluate(eval_env, file_buffer).get_double_value();
			temp->set_value(lhs_value*rhs_value);
			return *temp;
		}
		else if(this->get_data_type()== int_data_type){
			Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
			temp->set_variable_status(true);
			int lhs_value = lhs->evaluate(eval_env, file_buffer).get_int_value();
			int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
			temp->set_value(lhs_value*rhs_value);
			return *temp;
		}
	}

	// Code_For_Ast & Mult_Ast::compile(){}
	// Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}


	Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		if(this->get_data_type()== double_data_type){
			Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
			temp->set_variable_status(true);
			double lhs_value = lhs->evaluate(eval_env, file_buffer).get_double_value();
			double rhs_value = rhs->evaluate(eval_env, file_buffer).get_double_value();
			if(rhs_value == 0){
				printf("cs316: Error : Divide by 0\n");
				exit(0);
			}
			temp->set_value(lhs_value/rhs_value);
			return *temp;
		}
		else if(this->get_data_type()== int_data_type){
			Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
			temp->set_variable_status(true);
			int lhs_value = lhs->evaluate(eval_env, file_buffer).get_int_value();
			int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
			if(rhs_value == 0){
				printf("cs316: Error : Divide by 0\n");
				exit(0);
			}
			temp->set_value(lhs_value/rhs_value);
			return *temp;
		}
	}

	// Code_For_Ast & Divide_Ast::compile(){}
	// Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}


	Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		if(this->get_data_type()== double_data_type){
			Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
			temp->set_variable_status(true);
			double lhs_value = lhs->evaluate(eval_env, file_buffer).get_double_value();
			// double rhs_value = rhs->evaluate(eval_env, file_buffer).get_double_value();
			temp->set_value(-1*lhs_value);
			return *temp;
		}
		else if(this->get_data_type()== int_data_type){
			Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
			temp->set_variable_status(true);
			int lhs_value = lhs->evaluate(eval_env, file_buffer).get_int_value();
			// int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
			temp->set_value(-1*lhs_value);
			return *temp;
		}
	}

	// Code_For_Ast & UMinus_Ast::compile(){}
	// Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

	Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		int cond_value = cond->evaluate(eval_env, file_buffer).get_int_value();
		if(cond_value == 1){
			if(lhs->get_data_type()== int_data_type){
				Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
				temp->set_variable_status(true);
				int lhs_value = lhs->evaluate(eval_env, file_buffer).get_int_value();
				// int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
				temp->set_value(lhs_value);
				return *temp;
			}
			else if(lhs->get_data_type()== double_data_type){
				Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
				temp->set_variable_status(true);
				double lhs_value = lhs->evaluate(eval_env, file_buffer).get_double_value();
				// int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
				temp->set_value(lhs_value);
				return *temp;
			}

		}
		else{
			if(rhs->get_data_type()== int_data_type){
				Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
				temp->set_variable_status(true);
				int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
				// int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
				temp->set_value(rhs_value);
				return *temp;
			}
			else if(rhs->get_data_type()== double_data_type){
				Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
				temp->set_variable_status(true);
				double rhs_value = rhs->evaluate(eval_env, file_buffer).get_double_value();
				// int rhs_value = rhs->evaluate(eval_env, file_buffer).get_int_value();
				temp->set_value(rhs_value);
				return *temp;
			}
		}
	}

	// Code_For_Ast & Conditional_Expression_Ast::compile(){}


	// Code_For_Ast & Return_Ast::compile(){}
	// Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

	// Code_For_Ast & Relational_Expr_Ast::compile(){}

	Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		if(lhs_condition->get_data_type() == int_data_type){
			Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
			temp->set_variable_status(true);
			int lhs_value = lhs_condition->evaluate(eval_env, file_buffer).get_int_value();
			int rhs_value = rhs_condition->evaluate(eval_env, file_buffer).get_int_value();
			bool ans;
			switch(rel_op){
				case less_equalto : ans = lhs_value<=rhs_value; break;
				case less_than : ans = lhs_value<rhs_value; break;
				case greater_than : ans = lhs_value>rhs_value; break;
				case greater_equalto : ans = lhs_value>=rhs_value; break;
				case equalto : ans = lhs_value==rhs_value; break;
				case not_equalto : ans = lhs_value!=rhs_value; break;
			}
			temp->set_value((int)ans);
			return *temp;
		}
		else if(lhs_condition->get_data_type() == double_data_type){
			Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
			temp->set_variable_status(true);
			double lhs_value = lhs_condition->evaluate(eval_env, file_buffer).get_double_value();
			double rhs_value = rhs_condition->evaluate(eval_env, file_buffer).get_double_value();
			bool ans;
			switch(rel_op){
				case less_equalto : ans = lhs_value<=rhs_value; break;
				case less_than : ans = lhs_value<rhs_value; break;
				case greater_than : ans = lhs_value>rhs_value; break;
				case greater_equalto : ans = lhs_value>=rhs_value; break;
				case equalto : ans = lhs_value==rhs_value; break;
				case not_equalto : ans = lhs_value!=rhs_value; break;
			}
			temp->set_value((int)ans);
			return *temp;
		}
	}

	// Code_For_Ast & Logical_Expr_Ast::compile(){}

	Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
		temp->set_variable_status(true);
		int rhs_value = rhs_op->evaluate(eval_env, file_buffer).get_int_value();
		bool ans;
		switch(bool_op){
			case _logical_not : ans = !rhs_value; break;
			case _logical_and : {int lhs_value = lhs_op->evaluate(eval_env, file_buffer).get_int_value();
								ans = lhs_value&&rhs_value; break;}
			case _logical_or : {int lhs_value = lhs_op->evaluate(eval_env, file_buffer).get_int_value();
									ans = lhs_value||rhs_value; break; }
		}
		temp->set_value((int)ans);
		return *temp;
	}

	// Code_For_Ast & Selection_Statement_Ast::compile(){}

	Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		int cond_value = cond->evaluate(eval_env, file_buffer).get_int_value();
			// printf("while%d",cond_value);
		if(cond_value==1){
			return then_part->evaluate(eval_env, file_buffer);
		}
		else{
			if(ast_num_child == ternary_arity){
				return else_part->evaluate(eval_env,file_buffer);
			}
		}

	}

	// Code_For_Ast & Iteration_Statement_Ast::compile(){}

	Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		if(!is_do_form){
			// int cond = cond->evaluate(eval_env, file_buffer);
			while(cond->evaluate(eval_env,file_buffer).get_int_value() == 1){
				body->evaluate(eval_env, file_buffer);
			}
		}
		else{
			do{
				body->evaluate(eval_env, file_buffer);
			}
			while(cond->evaluate(eval_env,file_buffer).get_int_value() == 1);
		}
	}

	// Code_For_Ast & Sequence_Ast::compile(){}

	Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		list<Ast *>::iterator it;
		for(it=statement_list.begin(); it!=statement_list.end();it++){
			(*it)->evaluate(eval_env, file_buffer);
		}
	}

	Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){}
	Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){}