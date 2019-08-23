	Local_Environment::Local_Environment(){}
	Local_Environment::~Local_Environment(){}

	void Local_Environment::print(ostream & file_buffer){
		map<string, Eval_Result *>::iterator it;
		for(it=this->variable_table.begin(); it!=this->variable_table.end(); it++){
			if(it->second->is_variable_defined()){
				if(it->second->get_result_enum()==int_result)
					file_buffer<<AST_SPACE<<it->first<<" : "<<it->second->get_int_value();
				else if(it->second->get_result_enum()==double_result)
					file_buffer<<AST_SPACE<<it->first<<" : "<<it->second->get_double_value();
			}
			else{
				file_buffer<<AST_SPACE<<it->first<<" : undefined";
			}
			file_buffer<<"\n";
		}
		// file_buffer<<"Local_Environment print\n";
	}
	bool Local_Environment::is_variable_defined(string name){
		map<string, Eval_Result *>::iterator it;
		for(it=this->variable_table.begin(); it!=this->variable_table.end(); it++){
			if(it->first == name && it->second->is_variable_defined())
				return true;
		}
		return false;
	}
	Eval_Result * Local_Environment::get_variable_value(string name){
		map<string, Eval_Result *>::iterator it;
		for(it=this->variable_table.begin(); it!=this->variable_table.end(); it++){
			if(it->first == name)
				return it->second;
		}
		return NULL;
	}
	void Local_Environment::put_variable_value(Eval_Result & value, string name){
		variable_table[name] = &value;
	}
	bool Local_Environment::does_variable_exist(string name){
		map<string, Eval_Result *>::iterator it;
		for(it=this->variable_table.begin(); it!=this->variable_table.end(); it++){
			if(it->first == name)
				return true;
		}
		return false;
	}

	void Symbol_Table::create(Local_Environment & local_global_variables_table){
		list<Symbol_Table_Entry *>::iterator it;
		for(it=this->variable_table.begin(); it!=this->variable_table.end(); it++){
			if((*it)->get_data_type()==int_data_type){
				Eval_Result_Value_Int* temp = new Eval_Result_Value_Int;
				temp->set_variable_status(false);
				// printf("%s\n", );
				if(this->get_table_scope()==global){
					temp->set_variable_status(true);
					temp->set_value(0);
				}
				local_global_variables_table.put_variable_value(*temp, (*it)->get_variable_name());
			}
			else if((*it)->get_data_type()==double_data_type){
				Eval_Result_Value_Double* temp = new Eval_Result_Value_Double;
				temp->set_variable_status(false);	
				if(this->get_table_scope()==global){
					temp->set_variable_status(true);
					temp->set_value(0);
				}
				local_global_variables_table.put_variable_value(*temp, (*it)->get_variable_name());
			}

		}
	}

	int Eval_Result::get_int_value(){}
	void Eval_Result::set_value(int value){}
	double Eval_Result::get_double_value(){}
	void Eval_Result::set_value(double value){}

	bool Eval_Result::is_variable_defined(){}
	void Eval_Result::set_variable_status(bool def){}

	void Eval_Result_Value::set_value(int number){}
	void Eval_Result_Value::Eval_Result_Value::set_value(double number){}
	int Eval_Result_Value::get_int_value(){}
	double Eval_Result_Value::get_double_value(){}

	Eval_Result_Value_Int::Eval_Result_Value_Int(){
		result_type = int_result;
	}
	Eval_Result_Value_Int::~Eval_Result_Value_Int(){}

	void Eval_Result_Value_Int::set_value(int number){
		this->value = number;
	}
	void Eval_Result_Value_Int::set_value(double number){
		this->value = (int) number;
	}
	int Eval_Result_Value_Int::get_int_value(){
		return this->value;
	}

	void Eval_Result_Value_Int::set_variable_status(bool def){
		this->defined = def;
	}
	bool Eval_Result_Value_Int::is_variable_defined(){
		if(this->defined)
			return true;
		return false;
	}

	void Eval_Result_Value_Int::set_result_enum(Result_Enum res){
		this->result_type = res;
	}
	Result_Enum Eval_Result_Value_Int::get_result_enum(){
		return this->result_type;
	}

	Eval_Result_Value_Double::Eval_Result_Value_Double(){
		result_type = double_result;
	}
	Eval_Result_Value_Double::~Eval_Result_Value_Double(){}

	void Eval_Result_Value_Double::set_value(int number){
		this->value = (double) number;
	}
	void Eval_Result_Value_Double::set_value(double number){
		this->value = number;
	}
	double Eval_Result_Value_Double::get_double_value(){
		return this->value;
	}

	void Eval_Result_Value_Double::set_variable_status(bool def){
		this->defined = def;
	}
	bool Eval_Result_Value_Double::is_variable_defined(){
		if(this->defined)
			return true;
		return false;
	}

	void Eval_Result_Value_Double::set_result_enum(Result_Enum res){
		this->result_type = res;
	}
	Result_Enum Eval_Result_Value_Double::get_result_enum(){
		return this->result_type;
	}