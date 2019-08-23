list<int> fort;

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){}
Code_For_Ast & Assignment_Ast::compile(){
    Code_For_Ast rhs_compile = rhs->compile();
    list<Icode_Stmt *> rhs_stmt = rhs_compile.get_icode_list();

    rhs_compile.get_reg()->set_use_for_expr_result();
    list<Icode_Stmt *> lhs_stmt = lhs->create_store_stmt(rhs_compile.get_reg()).get_icode_list();
    rhs_compile.get_reg()->reset_use_for_expr_result();

    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    for (list<Icode_Stmt *>::iterator it = rhs_stmt.begin(); it!=rhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    for (list<Icode_Stmt *>::iterator it = lhs_stmt.begin(); it!=lhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }

    Code_For_Ast *result = new Code_For_Ast(*ics_list, rhs_compile.get_reg());
    return *result;
}
Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}


template class Number_Ast<int>;
template class Number_Ast<double>;


Code_For_Ast & Name_Ast::compile(){
	Code_For_Ast* result = new Code_For_Ast();
	if(this->get_data_type() == int_data_type){
		result->set_reg(machine_desc_object.get_new_register<int_reg>());
		Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(load, new Mem_Addr_Opd(this->get_symbol_entry()), new Register_Addr_Opd(result->get_reg()));
		result->append_ics(*ic_stmt1);
	}
	else{
		result->set_reg(machine_desc_object.get_new_register<float_reg>());
		Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(load_d, new Mem_Addr_Opd(this->get_symbol_entry()), new Register_Addr_Opd(result->get_reg()));
		result->append_ics(*ic_stmt1);
	}
	return *result;
}
Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}
Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){
    Code_For_Ast * result = new Code_For_Ast;
    if (this->get_data_type() == int_data_type){
    	Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(store, new Register_Addr_Opd(store_register), new Mem_Addr_Opd(this->get_symbol_entry()));
    	result->append_ics(*ic_stmt1);
    }
    else{
    	Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(store_d, new Register_Addr_Opd(store_register), new Mem_Addr_Opd(this->get_symbol_entry()));
    	result->append_ics(*ic_stmt1);
    }

    return *result;
}


template <class T>
Code_For_Ast & Number_Ast<T>::compile(){
	Const_Opd<T>*  number_opd = new Const_Opd<T>(this->constant);
	Code_For_Ast * result = new Code_For_Ast;

	if(this->get_data_type()==int_data_type){
		result->set_reg(machine_desc_object.get_new_register<int_reg>());
		Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(imm_load,number_opd, new Register_Addr_Opd(result->get_reg()));
		result->append_ics(*ic_stmt1);
	}
	else{
		result->set_reg(machine_desc_object.get_new_register<float_reg>());
		Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(imm_load_d,number_opd, new Register_Addr_Opd(result->get_reg()));
		result->append_ics(*ic_stmt1);
	}
	return *result;
}
template <class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){}


Code_For_Ast & Plus_Ast::compile(){
	Code_For_Ast lhs_compile = lhs->compile();
	Code_For_Ast rhs_compile = rhs->compile();
	list<Icode_Stmt *> rhs_stmt = rhs_compile.get_icode_list();

    list<Icode_Stmt *> lhs_stmt = lhs_compile.get_icode_list();

    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    for (list<Icode_Stmt *>::iterator it = lhs_stmt.begin(); it!=lhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    for (list<Icode_Stmt *>::iterator it = rhs_stmt.begin(); it!=rhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    Code_For_Ast * result;

    if(lhs->get_data_type()==int_data_type){
    	Register_Descriptor* result_reg = machine_desc_object.get_new_register<int_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(add, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }
    else{
		Register_Descriptor* result_reg = machine_desc_object.get_new_register<float_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(add_d, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }

    lhs_compile.get_reg()->reset_use_for_expr_result();
    rhs_compile.get_reg()->reset_use_for_expr_result();
    // Code_For_Ast *result = new Code_For_Ast(*ics_list, rhs->compile().get_reg());
    return *result;
}
Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & Minus_Ast::compile(){
	Code_For_Ast lhs_compile = lhs->compile();
	Code_For_Ast rhs_compile = rhs->compile();
	list<Icode_Stmt *> rhs_stmt = rhs_compile.get_icode_list();

    list<Icode_Stmt *> lhs_stmt = lhs_compile.get_icode_list();

    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    for (list<Icode_Stmt *>::iterator it = lhs_stmt.begin(); it!=lhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    for (list<Icode_Stmt *>::iterator it = rhs_stmt.begin(); it!=rhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    Code_For_Ast * result;

    if(lhs->get_data_type()==int_data_type){
    	Register_Descriptor* result_reg = machine_desc_object.get_new_register<int_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(sub, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }
    else{
		Register_Descriptor* result_reg = machine_desc_object.get_new_register<float_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(sub_d, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }

    lhs_compile.get_reg()->reset_use_for_expr_result();
    rhs_compile.get_reg()->reset_use_for_expr_result();
    // Code_For_Ast *result = new Code_For_Ast(*ics_list, rhs->compile().get_reg());
    return *result;
}
Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & Divide_Ast::compile(){
	Code_For_Ast lhs_compile = lhs->compile();
	Code_For_Ast rhs_compile = rhs->compile();
	list<Icode_Stmt *> rhs_stmt = rhs_compile.get_icode_list();

    list<Icode_Stmt *> lhs_stmt = lhs_compile.get_icode_list();

    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    for (list<Icode_Stmt *>::iterator it = lhs_stmt.begin(); it!=lhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    for (list<Icode_Stmt *>::iterator it = rhs_stmt.begin(); it!=rhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    Code_For_Ast * result;

    if(lhs->get_data_type()==int_data_type){
    	Register_Descriptor* result_reg = machine_desc_object.get_new_register<int_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(divd, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }
    else{
		Register_Descriptor* result_reg = machine_desc_object.get_new_register<float_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(div_d, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }

    lhs_compile.get_reg()->reset_use_for_expr_result();
    rhs_compile.get_reg()->reset_use_for_expr_result();
    // Code_For_Ast *result = new Code_For_Ast(*ics_list, rhs->compile().get_reg());
    return *result;
}
Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & Mult_Ast::compile(){
	Code_For_Ast lhs_compile = lhs->compile();
	Code_For_Ast rhs_compile = rhs->compile();
	list<Icode_Stmt *> rhs_stmt = rhs_compile.get_icode_list();

    list<Icode_Stmt *> lhs_stmt = lhs_compile.get_icode_list();

    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    for (list<Icode_Stmt *>::iterator it = lhs_stmt.begin(); it!=lhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    for (list<Icode_Stmt *>::iterator it = rhs_stmt.begin(); it!=rhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    Code_For_Ast * result;

    if(lhs->get_data_type()==int_data_type){
    	Register_Descriptor* result_reg = machine_desc_object.get_new_register<int_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(mult, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }
    else{
		Register_Descriptor* result_reg = machine_desc_object.get_new_register<float_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(mult_d, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }

    lhs_compile.get_reg()->reset_use_for_expr_result();
    rhs_compile.get_reg()->reset_use_for_expr_result();
    // Code_For_Ast *result = new Code_For_Ast(*ics_list, rhs->compile().get_reg());
    return *result;
}
Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & UMinus_Ast::compile(){
	Code_For_Ast lhs_compile = lhs->compile();
	// Code_For_Ast rhs_compile = rhs->compile();
	// list<Icode_Stmt *> rhs_stmt = rhs_compile.get_icode_list();

    list<Icode_Stmt *> lhs_stmt = lhs_compile.get_icode_list();

    // list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    // for (list<Icode_Stmt *>::iterator it = lhs_stmt.begin(); it!=lhs_stmt.end(); it++){
    // 	ics_list->push_back(*it);
    // }
    list<Icode_Stmt *>* ics_list = &lhs_stmt;
    // for (list<Icode_Stmt *>::iterator it = rhs_stmt.begin(); it!=rhs_stmt.end(); it++){
    	// ics_list->push_back(*it);
    // }
    Code_For_Ast * result;
    if(lhs->get_data_type()==int_data_type){
    	Register_Descriptor* result_reg = machine_desc_object.get_new_register<int_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(uminus, new Register_Addr_Opd(lhs_compile.get_reg()), NULL,  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }
    else{
		Register_Descriptor* result_reg = machine_desc_object.get_new_register<float_reg>();
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(uminus_d, new Register_Addr_Opd(lhs_compile.get_reg()), NULL,  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }

    lhs_compile.get_reg()->reset_use_for_expr_result();
    // rhs_compile.get_reg()->reset_use_for_expr_result();
    // Code_For_Ast *result = new Code_For_Ast(*ics_list, rhs->compile().get_reg());
    return *result;
}
Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & Conditional_Expression_Ast::compile(){
	Code_For_Ast cond_compile = cond->compile();
	Code_For_Ast then_part_compile = lhs->compile();
	string label1 = this->get_new_label();
    Code_For_Ast else_part_compile = rhs->compile();
	string label2 = this->get_new_label();
	list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
	Register_Descriptor * result_reg;
	if(lhs->get_data_type()==int_data_type)
		result_reg = machine_desc_object.get_new_register<int_reg>();
	else
		result_reg = machine_desc_object.get_new_register<float_reg>();
	// list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
	ics_list->splice(ics_list->end(),cond_compile.get_icode_list());
	if(cond_compile.get_reg()!=NULL){
        ics_list->push_back(new Control_Flow_IC_Stmt(beq, new Register_Addr_Opd(cond_compile.get_reg()), new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]), label1, 2));
	    cond_compile.get_reg()->reset_use_for_expr_result();
    }
    else{
        if(fort.back() == 1){
	        Control_Flow_IC_Stmt *ic_stmt1 = new Control_Flow_IC_Stmt(bc1t, NULL, NULL, label1);
	        ics_list->push_back(ic_stmt1);
	    }
	    else{
	    	Control_Flow_IC_Stmt *ic_stmt1 = new Control_Flow_IC_Stmt(bc1f, NULL, NULL, label1);
	        ics_list->push_back(ic_stmt1);
	    }
	    fort.pop_back();
    }
	ics_list->splice(ics_list->end(),then_part_compile.get_icode_list());
    if(cond_compile.get_reg()!=NULL)
	   ics_list->push_back(new Compute_IC_Stmt(or_t, new Register_Addr_Opd(then_part_compile.get_reg()), new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]) ,new Register_Addr_Opd(result_reg)));
	else
        ics_list->push_back(new Move_IC_Stmt(move_d, new Register_Addr_Opd(then_part_compile.get_reg()), new Register_Addr_Opd(result_reg)));
    ics_list->push_back(new Control_Flow_IC_Stmt(j, NULL, NULL, label2));
   	ics_list->push_back(new Label_IC_Stmt(label, label1));
	ics_list->splice(ics_list->end(),else_part_compile.get_icode_list());
    if(cond_compile.get_reg()!=NULL)
        ics_list->push_back(new Compute_IC_Stmt(or_t, new Register_Addr_Opd(else_part_compile.get_reg()),new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]), new Register_Addr_Opd(result_reg)));
	else
        ics_list->push_back(new Move_IC_Stmt(move_d, new Register_Addr_Opd(else_part_compile.get_reg()), new Register_Addr_Opd(result_reg)));
    ics_list->push_back(new Label_IC_Stmt(label, label2));
    then_part_compile.get_reg()->reset_use_for_expr_result();
    else_part_compile.get_reg()->reset_use_for_expr_result();
    // cond_compile.get_reg()->reset_use_for_expr_result();
    Code_For_Ast * result = new Code_For_Ast(*ics_list, result_reg);
    return * result;	
}
// Code_For_Ast & Conditional_Expression_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}


Code_For_Ast & Sequence_Ast::compile(){
	list<Ast *>::iterator it;
	for(it=statement_list.begin(); it!=statement_list.end();it++){
		Code_For_Ast compiled = (*it)->compile();
		for (list<Icode_Stmt *>::iterator it1 = compiled.get_icode_list().begin(); it1!=compiled.get_icode_list().end(); it1++){
    		sa_icode_list.push_back(*it1);
    	}
	}
	return * new Code_For_Ast(this->sa_icode_list,NULL);

}
Code_For_Ast & Return_Ast::compile(){
    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    if(return_value == NULL){
        ics_list->push_back(new Move_IC_Stmt(ret_inst, NULL, NULL));
        Code_For_Ast *result = new Code_For_Ast(*ics_list, NULL);
        return *result;
    }
    Code_For_Ast return_value_compile = return_value->compile();
    list<Icode_Stmt *> return_stmt = return_value_compile.get_icode_list();

    for (list<Icode_Stmt *>::iterator it = return_stmt.begin(); it!=return_stmt.end(); it++){
        ics_list->push_back(*it);
    }
    if(this->get_data_type()==int_data_type)
        ics_list->push_back(new Move_IC_Stmt(mov, new Register_Addr_Opd(return_value_compile.get_reg()), new Register_Addr_Opd(machine_desc_object.spim_register_table[v1])));
    else
        ics_list->push_back(new Move_IC_Stmt(move_d, new Register_Addr_Opd(return_value_compile.get_reg()), new Register_Addr_Opd(machine_desc_object.spim_register_table[f0])));
    ics_list->push_back(new Control_Flow_IC_Stmt(ret_inst, NULL, NULL, "epilogue_"+proc_name));
    return_value_compile.get_reg()->reset_use_for_expr_result();
    Code_For_Ast *result = new Code_For_Ast(*ics_list, NULL);
    return *result;
}
Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & Iteration_Statement_Ast::compile(){
	Code_For_Ast cond_compile = cond->compile();
	string label1 = this->get_new_label();
	string label2 = this->get_new_label();
    Code_For_Ast body_compile = body->compile();
	list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;

	if(!is_do_form)
		ics_list->push_back(new Control_Flow_IC_Stmt(j, NULL, NULL, label2));

	ics_list->push_back(new Label_IC_Stmt(label, label1));
	ics_list->splice(ics_list->end(), body_compile.get_icode_list());
	ics_list->push_back(new Label_IC_Stmt(label, label2));
	ics_list->splice(ics_list->end(), cond_compile.get_icode_list());
    if(cond_compile.get_reg()!=NULL){
        ics_list->push_back(new Control_Flow_IC_Stmt(bne, new Register_Addr_Opd(cond_compile.get_reg()), new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]), label1, 2));
	    cond_compile.get_reg()->reset_use_for_expr_result();
    }
    else{
        if(fort.back() == 1){
	        Control_Flow_IC_Stmt *ic_stmt1 = new Control_Flow_IC_Stmt(bc1f, NULL , NULL, label1);
	        ics_list->push_back(ic_stmt1);
	    }
	    else{
	    	Control_Flow_IC_Stmt *ic_stmt1 = new Control_Flow_IC_Stmt(bc1t, NULL , NULL, label1);
	        ics_list->push_back(ic_stmt1);
	    }
	    fort.pop_back();
    }
	Code_For_Ast * result = new Code_For_Ast(*ics_list,NULL);
	return *result;
}
// Code_For_Ast & Iteration_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & Selection_Statement_Ast::compile(){
	Code_For_Ast cond_compile = cond->compile();
    Code_For_Ast then_part_compile = then_part->compile();
	string label1 = this->get_new_label();
    Code_For_Ast else_part_compile;
    string label2;
    if(else_part!=NULL)
        label2 = this->get_new_label();
    if(else_part!=NULL)
        else_part_compile = else_part->compile();
	list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
	// list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
	ics_list->splice(ics_list->end(),cond_compile.get_icode_list());
    if(cond_compile.get_reg()!=NULL){
    	Control_Flow_IC_Stmt *ic_stmt1 = new Control_Flow_IC_Stmt(beq, new Register_Addr_Opd(cond_compile.get_reg()), new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]), label1, 2);
    	ics_list->push_back(ic_stmt1);
        cond_compile.get_reg()->reset_use_for_expr_result();
    }
    else{
    	if(fort.back() == 1){
	        Control_Flow_IC_Stmt *ic_stmt1 = new Control_Flow_IC_Stmt(bc1t, NULL , NULL, label1);
	        ics_list->push_back(ic_stmt1);
	    }
	    else{
	    	Control_Flow_IC_Stmt *ic_stmt1 = new Control_Flow_IC_Stmt(bc1f, NULL , NULL, label1);
	        ics_list->push_back(ic_stmt1);
	    }
	    fort.pop_back();
    }
    ics_list->splice(ics_list->end(),then_part_compile.get_icode_list());
    if(else_part!=NULL){
        ics_list->push_back(new Control_Flow_IC_Stmt(j, NULL, NULL, label2));
        ics_list->push_back(new Label_IC_Stmt(label, label1));
        ics_list->splice(ics_list->end(),else_part_compile.get_icode_list());
        ics_list->push_back(new Label_IC_Stmt(label, label2));
    }
    else{
        ics_list->push_back(new Label_IC_Stmt(label, label1));

    }
    Code_For_Ast * result = new Code_For_Ast(*ics_list, NULL);
    return * result;	
}
// Code_For_Ast & Selection_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast & Logical_Expr_Ast::compile(){
	list<Icode_Stmt *> lhs_stmt;
	Code_For_Ast lhs_compile;
	if(lhs_op!=NULL){
		lhs_compile = lhs_op->compile();
	}
    else{
        lhs_compile = (new Number_Ast<int>(1,int_data_type,0))->compile();
    }
        lhs_stmt = lhs_compile.get_icode_list();

    Code_For_Ast rhs_compile = rhs_op->compile();
    list<Icode_Stmt *> rhs_stmt = rhs_compile.get_icode_list();

    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    for (list<Icode_Stmt *>::iterator it = lhs_stmt.begin(); it!=lhs_stmt.end(); it++){
        ics_list->push_back(*it);
    }

    for (list<Icode_Stmt *>::iterator it = rhs_stmt.begin(); it!=rhs_stmt.end(); it++){
        ics_list->push_back(*it);
    }

    Code_For_Ast * result;

	Register_Descriptor* result_reg = machine_desc_object.get_new_register<int_reg>();
	Tgt_Op op;
	switch(bool_op){
		case _logical_or 	: 	op = or_t; break;
		case _logical_and 	: 	op = and_t; break;
		case _logical_not 	: 	op = not_t; break;

	}

	Compute_IC_Stmt* ic_stmt1;
    if(lhs_op!=NULL)
	   ic_stmt1 = new Compute_IC_Stmt(op, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
	else
       ic_stmt1 = new Compute_IC_Stmt(op, new Register_Addr_Opd(rhs_compile.get_reg()), new Register_Addr_Opd(lhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
	ics_list->push_back(ic_stmt1);
	// result.set_ics_list = ics_list;
   	result = new Code_For_Ast(*ics_list,result_reg);

    lhs_compile.get_reg()->reset_use_for_expr_result();
    rhs_compile.get_reg()->reset_use_for_expr_result();
    // Code_For_Ast *result = new Code_For_Ast(*ics_list, rhs->compile().get_reg());
    return *result;
}
// Code_For_Ast & Logical_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

Code_For_Ast &  Relational_Expr_Ast::compile(){
	Code_For_Ast lhs_compile = lhs_condition->compile();
	Code_For_Ast rhs_compile = rhs_condition->compile();

    list<Icode_Stmt *> lhs_stmt = lhs_compile.get_icode_list();
    list<Icode_Stmt *> rhs_stmt = rhs_compile.get_icode_list();

    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    for (list<Icode_Stmt *>::iterator it = lhs_stmt.begin(); it!=lhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    for (list<Icode_Stmt *>::iterator it = rhs_stmt.begin(); it!=rhs_stmt.end(); it++){
    	ics_list->push_back(*it);
    }
    Code_For_Ast * result;

    if(lhs_condition->get_data_type()==int_data_type){
    	Register_Descriptor* result_reg = machine_desc_object.get_new_register<int_reg>();
    	Tgt_Op op;
    	switch(rel_op){
    		case less_equalto : 	op = sle; break;
			case less_than : 		op = slt; break;
			case greater_than : 	op = sgt; break;
			case greater_equalto : 	op = sge; break;
			case equalto :			op = seq; break;
			case not_equalto : 		op = sne; break;
    	}
    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(op, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()),  new Register_Addr_Opd(result_reg));
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,result_reg);
    }
    else{
		Register_Descriptor* result_reg = machine_desc_object.get_new_register<int_reg>();
		Tgt_Op op;
    	switch(rel_op){
    		case less_equalto : 	op = sle_d; fort.push_back(0); break;
			case less_than : 		op = slt_d; fort.push_back(0); break;
			case greater_than : 	op = sgt_d; fort.push_back(1); break;
			case greater_equalto : 	op = sge_d; fort.push_back(1); break;
			case equalto :			op = seq_d; fort.push_back(0); break;
			case not_equalto : 		op = sne_d; fort.push_back(1); break;
    	}

    	Compute_IC_Stmt* ic_stmt1 = new Compute_IC_Stmt(op, new Register_Addr_Opd(lhs_compile.get_reg()), new Register_Addr_Opd(rhs_compile.get_reg()), NULL);
    	ics_list->push_back(ic_stmt1);
    	// result.set_ics_list = ics_list;
    	result = new Code_For_Ast(*ics_list,NULL);
    }

    lhs_compile.get_reg()->reset_use_for_expr_result();
    rhs_compile.get_reg()->reset_use_for_expr_result();
    // Code_For_Ast *result = new Code_For_Ast(*ics_list, rhs->compile().get_reg());
    return *result;
}

Code_For_Ast & Print_Ast::compile(){
    Code_For_Ast* result;
    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    if(var->get_data_type() == int_data_type){
        // Register_Descriptor *v0 = machine_desc_object.get_new_register<int_reg>();
        Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(imm_load, new Const_Opd<int>(1), new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]));
        Move_IC_Stmt* ic_stmt2 = new Move_IC_Stmt(load, new Mem_Addr_Opd(var->get_symbol_entry()), new Register_Addr_Opd(machine_desc_object.spim_register_table[a0]));
        Print_IC_Stmt* ic_stmt3 = new Print_IC_Stmt(); 
        ics_list->push_back(ic_stmt1);
        ics_list->push_back(ic_stmt2);
        ics_list->push_back(ic_stmt3);
        // v0->reset_use_for_expr_result();
        result = new Code_For_Ast(*ics_list,NULL);
    }
    else{
        // Register_Descriptor *v0 = machine_desc_object.get_new_register<int_reg>();
        Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(imm_load, new Const_Opd<int>(3), new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]));
        Move_IC_Stmt* ic_stmt2 = new Move_IC_Stmt(load_d, new Mem_Addr_Opd(var->get_symbol_entry()), new Register_Addr_Opd(machine_desc_object.spim_register_table[f12]));
        Print_IC_Stmt* ic_stmt3 = new Print_IC_Stmt(); 
        ics_list->push_back(ic_stmt1);
        ics_list->push_back(ic_stmt2);
        ics_list->push_back(ic_stmt3);
        // v0->reset_use_for_expr_result();
        result = new Code_For_Ast(*ics_list,NULL);
    }
    return *result;
}

Code_For_Ast & Call_Ast::compile(){
    list<Icode_Stmt *>* ics_list = new list<Icode_Stmt *>;
    list<Symbol_Table_Entry *> formal_param_list = program_object.get_procedure_prototype(procedure_name)->get_formal_param_list().get_table();
    list<Symbol_Table_Entry *>::reverse_iterator it1=formal_param_list.rbegin();
    int offset = 0;
    for(list<Ast *>::reverse_iterator it=actual_param_list.rbegin(); it!=actual_param_list.rend();it++){
        Code_For_Ast res = (*it)->compile();
        ics_list->splice(ics_list->end(),res.get_icode_list());
        if((*it)->get_data_type() == int_data_type){
            ics_list->push_back(new Control_Flow_IC_Stmt(store, new Register_Addr_Opd(res.get_reg()), new Mem_Addr_Opd(*(*it1)), "", offset));
            offset-=4;
        }
        else{
            ics_list->push_back(new Control_Flow_IC_Stmt(store_d, new Register_Addr_Opd(res.get_reg()), new Mem_Addr_Opd(*(*it1)), "", offset));
            offset-=8;
        }
        res.get_reg()->reset_use_for_expr_result();
        it1++;
    }
    if(offset!=0)
        ics_list->push_back(new Control_Flow_IC_Stmt(sub, NULL, NULL, procedure_name, -1*offset));
    ics_list->push_back(new Control_Flow_IC_Stmt(jal, NULL, NULL, procedure_name));
    if(offset!=0)
        ics_list->push_back(new Control_Flow_IC_Stmt(add, NULL, NULL, procedure_name, -1*offset));

    Code_For_Ast * result = new Code_For_Ast(*ics_list, NULL);
    if(this->get_data_type()==int_data_type){
        result->set_reg(machine_desc_object.get_new_register<int_reg>());
        Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(mov, new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]), new Register_Addr_Opd(result->get_reg()));
        result->append_ics(*ic_stmt1);
    }
    else if(this->get_data_type()==double_data_type){
        result->set_reg(machine_desc_object.get_new_register<float_reg>());
        Move_IC_Stmt* ic_stmt1 = new Move_IC_Stmt(move_d, new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]), new Register_Addr_Opd(result->get_reg()));
        result->append_ics(*ic_stmt1);   
    }
    return * result;

}
Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}