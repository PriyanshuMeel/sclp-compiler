

Code_For_Ast::Code_For_Ast(){}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg){
	this->ics_list = ic_l;
	this->result_register  = reg;
}
void Code_For_Ast::append_ics(Icode_Stmt & ics){
	this->ics_list.push_back(&ics);
}
list<Icode_Stmt *> & Code_For_Ast::get_icode_list(){
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg(){
	return result_register;
}
void Code_For_Ast::set_reg(Register_Descriptor * reg){
	result_register  = reg;
}

Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){
	this->ics_list = rhs.ics_list;
	this->result_register = rhs.result_register;
	return *this;
}



Register_Descriptor * Ics_Opd::get_reg(){};

Symbol_Table_Entry * symbol_entry;
Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
	symbol_entry = &se;
}
	
void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer<<symbol_entry->get_variable_name();
}
void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){
	// file_buffer<<"Mem_Addr_Opd_asm";
    if(symbol_entry->get_symbol_scope() == global){
    	file_buffer<<symbol_entry->get_variable_name();
    }
    else{
    	file_buffer<<symbol_entry->get_start_offset()<<"($fp)";
    }
}

Mem_Addr_Opd & Mem_Addr_Opd::operator= (const Mem_Addr_Opd & rhs){
	this->symbol_entry = rhs.symbol_entry;
	return *this;
}

Register_Descriptor * register_description;


Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
	register_description = rd;
}

Register_Descriptor * Register_Addr_Opd::get_reg(){
	return register_description;
}
void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer<<register_description->get_name();
}
void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){
	file_buffer<<"$"<<register_description->get_name();
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs){
	this->register_description = rhs.register_description;
	return *this;
}

template class Const_Opd<int>;
template class Const_Opd<double>;

template <class T>
Const_Opd<T>::Const_Opd (T num){
	this->num = num;
}
template <class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer){
	file_buffer<<num;
}
template <class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer){
	file_buffer<<num;
}
template <class T>
Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd & rhs){
	this->num = rhs.num;
	return *this;
}

///////////////////////////////Ic_stmt/////////////////////




	// Instruction_Descriptor op_desc;

Instruction_Descriptor & Icode_Stmt::get_op(){}
Ics_Opd * Icode_Stmt::get_opd1(){return NULL;}
Ics_Opd * Icode_Stmt::get_opd2(){return NULL;}
Ics_Opd * Icode_Stmt::get_result(){return NULL;}

void Icode_Stmt::set_opd1(Ics_Opd * io){}
void Icode_Stmt::set_opd2(Ics_Opd * io){}
void Icode_Stmt::set_result(Ics_Opd * io){}





Print_IC_Stmt::Print_IC_Stmt(){
	this->op_desc = *(machine_desc_object.spim_instruction_table[print]);
}
Print_IC_Stmt::~Print_IC_Stmt(){}
void Print_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer<<"\t"<<op_desc.get_name()<<endl;
}
void Print_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer<<"\t"<<op_desc.get_mnemonic()<<endl;
}



Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result){
	this->opd1 = opd1;
	this->result = result;
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
}
Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs){
	this->op_desc = rhs.op_desc;
	this->opd1 = rhs.opd1;
	this->result = rhs.result;
	return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1(){
	return this->opd1;
}
void Move_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result(){
	return result;
}
void Move_IC_Stmt::set_result(Ics_Opd * io){
	result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer){

	file_buffer << "\t"<< op_desc.get_name()<<":    \t";
    result->print_ics_opd(file_buffer);
    file_buffer<< " <- ";
    opd1->print_ics_opd(file_buffer);
    file_buffer<<endl;
	// file_buffer<<"Move_IC_Stmt_icode"<<endl;
}
void Move_IC_Stmt::print_assembly(ostream & file_buffer){
	// file_buffer<<"Move_IC_Stmt_asssembly"<<endl;
	if(op_desc.get_assembly_format() == a_op_r_o1){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		result->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<endl;
	}
	else if(op_desc.get_assembly_format() == a_op_o1_r){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<", ";
		result->print_asm_opd(file_buffer);
		file_buffer<<endl;
	}
}


Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, Ics_Opd * result){
	this->opd1 = opd1;
	this->opd2 = opd2;
	this->result = result;
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
} 
	
Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs){
	this->op_desc = rhs.op_desc;
	this->opd1 = rhs.opd1;
	this->opd2 = rhs.opd2;
	this->result = rhs.result;
	return *this;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}

Ics_Opd * Compute_IC_Stmt::get_opd1(){
	return opd1;
}
void Compute_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2(){
	return opd2;
}
void Compute_IC_Stmt::set_opd2(Ics_Opd * io){
	opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result(){
	return result;
}
void Compute_IC_Stmt::set_result(Ics_Opd * io){
	result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
	if(op_desc.get_ic_format() == i_r_o1_op_o2){
		file_buffer << "\t" << op_desc.get_name()<<":    \t";
	    result->print_ics_opd(file_buffer);
	    file_buffer<< " <- ";
	    opd1->print_ics_opd(file_buffer);
	    file_buffer<<" , ";
	    opd2->print_ics_opd(file_buffer);
	    file_buffer<<endl;
	}
	else if(op_desc.get_ic_format() == i_r_op_o1){
		file_buffer << "\t" << op_desc.get_name()<<":    \t";
	    result->print_ics_opd(file_buffer);
	    file_buffer<< " <- ";
	    opd1->print_ics_opd(file_buffer);
	    // file_buffer<<" , ";
	    // opd2->print_ics_opd(file_buffer);
	    file_buffer<<endl;
	}
	else if(op_desc.get_ic_format() == i_o1_op_o2){
		file_buffer << "\t" << op_desc.get_name()<<":    \t";
	    file_buffer<< " c1 <- ";
	    opd1->print_ics_opd(file_buffer);
	    file_buffer<<" , ";
	    opd2->print_ics_opd(file_buffer);
	    file_buffer<<endl;
		// printf("cs316: Error : sclp internal error float comparisons not allowed\n ");
		// exit(0);
	}
}
void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
	if(op_desc.get_assembly_format() == a_op_r_o1_o2){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		result->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer<<endl;
	}
	else if(op_desc.get_assembly_format() == a_op_r_o1){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		result->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd1->print_asm_opd(file_buffer);
		// file_buffer<<", ";
		// opd2->print_asm_opd(file_buffer);
		file_buffer<<endl;
	}
	else if(op_desc.get_assembly_format() == a_op_o1_o2){
		file_buffer << "\t" << op_desc.get_mnemonic()<<" ";
	    // result->print_ics_opd(file_buffer);
	    // file_buffer<< " <- ";
	    opd1->print_asm_opd(file_buffer);
	    file_buffer<<", ";
	    opd2->print_asm_opd(file_buffer);
	    file_buffer<<endl;
	}
	else{
	file_buffer<<"Compute_IC_Stmt_asssembly"<<endl;

	}
}





Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, string label, int size){
	this->opd1 = o1;
	this->opd2 = o2;
	this->op_desc = *(machine_desc_object.spim_instruction_table[op]);
	this->offset = label;
	this->actual_param_size = size;
}
Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs){
	this->opd1 = rhs.opd1;
	this->opd2 = rhs.opd2;
	this->op_desc = rhs.op_desc;
	this->actual_param_size = rhs.actual_param_size;
	this->offset = rhs.offset;
	return *this;	
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){
	return opd1;
}
void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2(){
	return opd2;
}
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io){
	opd2 = io;
}

string Control_Flow_IC_Stmt::get_Offset(){
	return offset;
}
void Control_Flow_IC_Stmt::set_Offset(string label){
	offset = label;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer){
	if(op_desc.get_ic_format() == i_op_o1_o2_st) {
            file_buffer<<"\t"<<op_desc.get_name()<<":    \t";
            opd1->print_ics_opd(file_buffer);
            file_buffer<<" , ";
            opd2->print_ics_opd(file_buffer);
            // opd1->print_ics_opd(file_buffer);
            file_buffer << " : goto " <<offset<<endl;
      }
    else if(op_desc.get_name() == "jal"){
		file_buffer<<"\t"<<"call "<<offset<<endl;
	}
	else if(op_desc.get_name() == "jump" || op_desc.get_name() == "bc1f" || op_desc.get_name() == "bc1t"){
		file_buffer<<"\t"<<"goto "<<offset<<endl;
	}
    else if(op_desc.get_ic_format() == i_op_st){
        file_buffer<<"\t";
        file_buffer<<op_desc.get_name();
        file_buffer<<" "<<offset<<endl;
     }
     else if(op_desc.get_ic_format() == i_op){
		file_buffer<<"\t"<<op_desc.get_name()<<endl;
	}
	else if(op_desc.get_ic_format() == i_r_op_o1){
		file_buffer << "\t"<< op_desc.get_name()<<":    \t";
    	opd2->print_ics_opd(file_buffer);
    	file_buffer<< " <- ";
    	opd1->print_ics_opd(file_buffer);
    	file_buffer<<endl;
	}
	// file_buffer<<"Control_Flow_IC_Stmt_icode"<<endl;
}
void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer){
	// file_buffer<<"Control_Flow_IC_Stmt_assembly"<<endl;
	if(op_desc.get_assembly_format()==a_op_o1_o2_st){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd2->print_asm_opd(file_buffer);
		file_buffer<<", "<<offset<<" "<<endl;
	}
	else if(op_desc.get_assembly_format() == a_op_st){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" "<<offset<<endl;
	}
	else if(op_desc.get_assembly_format() == a_op){
		file_buffer<<"\tj "<<offset<<endl;
	}
	else if(op_desc.get_assembly_format() == a_op_o1_r){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<", ";
		file_buffer<<actual_param_size<<"($sp)"<<endl;
	}
	else if(op_desc.get_assembly_format() == a_op_r_o1_o2){
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
		file_buffer<<"$sp, $sp, "<<actual_param_size<<endl;
	}
}



Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label){
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
	this->label = label;
}
Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs){
	this->op_desc = rhs.op_desc;
	this->label = rhs.label;
	return *this;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

string Label_IC_Stmt::get_label(){
	return label;
}
void Label_IC_Stmt::set_label(string label){
	this->label = label;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer<<endl<<label<<":    \t"<<endl;
	// file_buffer<<"Label_IC_Stmt_icode"<<endl;
}
void Label_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer<<endl<<label<<":    \t"<<endl;
	// file_buffer<<"Label_IC_Stmt_assembly"<<endl;
}


Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af){
	this->inst_op = op;
	this->name = name;
	this->mnemonic = mnn;
	this->ic_symbol = ics;
	this->ic_format = icf;
	this->assem_format = af;
}
Instruction_Descriptor::Instruction_Descriptor(){
	inst_op = nop;
    name = "";
    mnemonic = "";
    ic_symbol = "";
    ic_format = i_nsy;
    assem_format = a_nsy;
}

Tgt_Op Instruction_Descriptor::get_op(){return inst_op;}
string Instruction_Descriptor::get_name(){return name;}
string Instruction_Descriptor::get_mnemonic(){return mnemonic;}
string Instruction_Descriptor::get_ic_symbol(){return ic_symbol;}
Icode_Format Instruction_Descriptor::get_ic_format(){return ic_format;}
Assembly_Format Instruction_Descriptor::get_assembly_format(){return assem_format;}
 
void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){
	file_buffer<<"Instruction_Descriptor_print"<<endl;
}
