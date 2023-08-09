%{
	#pragma once
    #include <bits/stdc++.h>
    #include "SymbolTable.h"
	#include"helper.h"
	using namespace std;

	bool gs = true ; bool arr_idx = false ;
	map< string, string> label ;
	string wl ,wle ; //whileloop and whileloop end


	extern string allCapital(string str) ;
	int id = 0; 

	SymbolTable st ;

    extern FILE *yyin;
    extern int yylineno ;
    int yyparse(void);
    int yylex(void);

    ofstream logFile , error, parsefile ;
	extern ofstream codeSegOut ;
    int err_cnt = 0 ;
	//int var = 0 ;


    //vector<SymbolInfo*>* func_param = nullptr ;
	vector<SymbolInfo*> dec_list ; 
    int pline ;

	int yylex_destroy(void) ;
	int countr = 0 ;

	void DFS(SymbolInfo* StartState, string space)
	{    
		space=space+" ";
		
		if(StartState->leaf) {   
			parsefile << space << StartState->getType() << " : " << StartState->getName() <<"\t"<<"<Line: "<< StartState->start_line << ">"<<endl;
			delete StartState ;
			countr++ ;
			
			return ;
		}
		parsefile << space<<StartState->getType() << " \t" << "<Line: " << StartState->start_line<<"-" << StartState->end_line << ">" <<endl;
		vector<SymbolInfo*>temp = StartState->child_list;
		for(SymbolInfo *next : temp) {
			countr++ ;
			DFS(next,space);
			//delete next ;
		}

		delete StartState ;
			
	}

	void dfs_delete(SymbolInfo* StartState){    
		//
			
	}


	void yyerror(const char* s){
		error <<"Line# "<<yylineno<<": "<<s<<"\n"<<endl;
		err_cnt++;
	}

	void delete_vector(vector<SymbolInfo*>& list) {
		for(SymbolInfo* info: list){
			delete info ;
		}
	}

    void func_declare(string name , string ret, vector<SymbolInfo*>& plist , int line = yylineno ) {
		
		if(ret == "void") ret = "VOID" ;
		if(ret == "int") ret = "INT" ;
		if(ret == "float") ret = "FLOAT" ;
		if(ret == "double") ret = "DOUBLE" ;


		bool yes = st.insertSymbol(name , "ID", logFile) ;
		SymbolInfo* info = st.lookup(name) ;
		name="'"+name+"'" ;
		if(yes){
			info->info_type = FUNCTION_DECLARATION ;
			info->data_type = ret ;

			if(plist.size()) {
				for(SymbolInfo* p : plist){
					info->add(p->getName() , p->data_type,p->info_type) ;
				}
            }
		}
        
        else{
			if(info->info_type == FUNCTION_DECLARATION){
				error<<"Line# "<<line<<": Redeclaration of "+name<<endl;
				err_cnt++;
				plist.clear() ;
				return;
			}
		}
		plist.clear() ;
	}

	void func_def(string name, string ret, vector<SymbolInfo*>& plist, int line = yylineno) {
		
		if(ret == "void") ret = "VOID" ;
		if(ret == "int") ret = "INT" ;
		if(ret == "float") ret = "FLOAT" ;
		if(ret == "double") ret = "DOUBLE" ;
		SymbolInfo* info = st.lookup(name);
		if(info==nullptr) { 
			st.insertSymbol(name , "ID",logFile);
			info = st.lookup(name);
			name = "'"+name+"'" ;
		}
        else{
			name = "'"+name+"'" ;
			if(info->info_type == FUNCTION_DECLARATION){
				if(info->data_type != ret){
				    error<<"Line# "<<line<<": Conflicting types for "+name<<endl;
					err_cnt++;
					return;
				}
				vector<SymbolInfo*> params = info->_parameter_list ;
				

				int x ;

				if(plist.size() == 0) {
					x = 0 ;
				}
				else x = plist.size() ;
				if(params.size() != x){
					error << "Line# " << line << ": Conflicting types for "+name << endl ;
					err_cnt++ ;
					return ;
				}
				if(plist.size()) {
					vector<SymbolInfo*> plist2 = plist;
					for(int i = 0 ; i < params.size() ; i++){
						if(params[i]->data_type != plist2[i]->data_type){
							error<<"Line# "<<line<<": Conflicting types for "+name<<endl;
							err_cnt++;
							return;
						}
					}
				}
			}
            else {
				if(info->info_type == FUNCTION_DEFINITION) {
					error<<"Line# "<<line<<": Multiple declaration of "+name<<endl;
					err_cnt++;
					return;
				}
				else {
					error<<"Line# "<<line<<": "<<name << " redeclared as different kind of symbol"<<endl;
					err_cnt++;
					return;
				}
			}
		}
		if(info->info_type == FUNCTION_DEFINITION){
			error<<"Line# "<<line<<": redefinition of "+name<<endl;
			err_cnt++;
			return;
		}
		info->info_type = FUNCTION_DEFINITION ;
		info->data_type = ret ;
		info->_parameter_list = vector<SymbolInfo*>() ;

		if(plist.size()){
			for(SymbolInfo* p: plist){
				info->add(p->getName(),p->data_type,p->info_type) ;
			}
		}

		return ;

	}

	void call_func(SymbolInfo*& funcSym, vector<SymbolInfo*>& args){
		string name = funcSym->getName();
		SymbolInfo* info = st.lookup(name);
		name = "'"+name+"'" ;
		if(info == nullptr){
			error << "Line# " <<yylineno << ": Undeclared function " <<name <<endl;
			err_cnt++ ;
			args.clear() ;
			return;
		}
		//if(!info->isFunction()){ // a function call cannot be made with non-function type identifier.
			
			//return;
		//}
		funcSym->data_type = info->data_type ;
		if(info->info_type != FUNCTION_DEFINITION){
			error << "Line# " <<yylineno << ": Function "  << name <<" not defined" << endl ;
			err_cnt++ ;
			args.clear() ;
			return;
		}
		vector<SymbolInfo*> params = info->_parameter_list ;
		int paramCnt = args.size() ;
		//cout << "total argsssss "<<paramCnt << endl;
		// Check whether a function is called with appropriate number of parameters
		if(params.size() != paramCnt){
			if(paramCnt < params.size()) {
				error <<"Line# "<<yylineno << ": Too few arguments to function " << name <<endl ;
			}
			else {
				error << "Line# " << yylineno << ": Too many arguments to function " << name << endl ; 
			}
			err_cnt++ ;
			args.clear() ;
			return;
		}
		if(args.size()){
			vector<SymbolInfo*> argList = args;
			for(int i=0; i<params.size(); i++){ 
				//error << params[i]->var <<" " << argList[i]->var << endl;
				if((params[i]->data_type != argList[i]->data_type) || (params[i]->data_type == argList[i]->data_type && params[i]->var != argList[i]->var)){
					error << "Line# " << yylineno <<": Type mismatch for argument " << to_string(i+1) <<" of " << name<< endl ;
					err_cnt++ ;
					//return;
				}
			}
		}
		args.clear() ;
	}



    bool dec_func_param(string dt , string name , int line = yylineno,int offset = 0) {
		if(dt == "int") dt = "INT" ;
		if(dt == "float") dt = "FLOAT" ;
		if(dt == "double") dt = "DOUBLE" ;
		if (dt == "void") dt = "VOID" ;
        if (dt == "VOID"){
            error <<"Line# "<<line << ": Function parameter cannot be void" << endl;
			err_cnt++ ;
			return false ;
        }

        if(st.insertSymbol(name, "ID", logFile) == true) {
            SymbolInfo* info = st.lookup(name) ;
            info->data_type = dt ;
			info->asm_name = "[BP + " + to_string(offset) + "]" ;
			info->global = false ;
			info->arr_strt = 0 ;

            return true ;
        }
		name="'"+name+"'" ;
		error << "Line# " << line <<": Redefinition of parameter "+name << endl;
		err_cnt++ ;

		return false ;
    }

    void dec_func_param_list(vector<SymbolInfo*> &list , int line = yylineno) {
        if(list.size() == 0) return ;
		int cnt = 0 ;
		int n = list.size() ;
		for(SymbolInfo* info: list){
			int offset = 4 + (n - cnt) * 2 ;
			cnt++ ;
			if(!dec_func_param(info->data_type, info->getName(), line,offset)){
				break ;
			};
		}
		list.clear() ;

		return ;
    }

    void dec_var(string dt ,vector<SymbolInfo*>& list) {
		
        for(SymbolInfo* info : list){
			if(dt == "void" || dt =="VOID"){
				error << "Line# "<<yylineno << ": " << "Variable or field '" << info->getName() << "' declared void" << endl;
				err_cnt++ ;
				continue;
			}
			////cout << info->getName() << endl;
			bool yes = st.insertSymbol(info->getName(), info->getType(),logFile);
			SymbolInfo* info2 = st.lookup(info->getName()) ;
			if(!yes){
				if(info->info_type == FUNCTION_DECLARATION) {
					error << "Line# "<<yylineno << ": '" << info->getName() <<"' redeclared as different kind of symbol"  << endl ;
					err_cnt++ ;
				}
				else if(info->info_type == FUNCTION_DEFINITION) {
					error << "Line# "<<yylineno << ": '" << info->getName() <<"' redeclared as different kind of symbol"  << endl ;
					err_cnt++ ;
				}
				else if(info->data_type == dt && info->info_type != info2->info_type) {
					error << "Line# "<<yylineno << ": '" << info->getName() <<"' redeclared as different kind of symbol"  << endl ;
					err_cnt++ ;	
				}
				else if(info->data_type == dt) {
					error << "Line# "<<yylineno << ": '" << info->getName() <<"' redeclared variable"  << endl ;
					err_cnt++ ;	
				}
				else if(info->data_type != dt) {
					error << "Line# "<<yylineno << ": "<<"Conflicting types for'" <<info->getName()+"'" << endl;
					err_cnt++ ;
					
				}
				else {

				}
			}
            else{
				
				if(dt == "int") dt = "INT" ;
				if(dt == "float") dt = "FLOAT" ;
				if(dt == "double") dt = "DOUBLE" ;

				SymbolInfo* nvar = st.lookup(info->getName()) ;
				nvar->data_type = dt ; 
	
				if(info->is_array()){
					nvar->array_size = info->array_size ;
					nvar->info_type = info->info_type ;

				}
				nvar->array_size = info->array_size ;
				nvar->arr_strt = info->arr_strt ;
				nvar->global = info->global ;
				cout <<"AM I GLOBAL ?" <<info->asm_name << endl ;
				nvar->asm_name = info->asm_name ;
				cout <<"arr name is " << info->getName() << " Arr Size is" <<" : " << info->array_size << endl;
			}
		}

		list.clear() ;

		return ;
    }

%}

%union{
	SymbolInfo* symbol_info; 
	std::string* str_info;
}

%token <symbol_info> IF ELSE FOR WHILE DO BREAK INT CHAR FLOAT DOUBLE VOID RETURN SWITCH CASE DEFAULT CONTINUE PRINTLN

%token <symbol_info> ADDOP MULOP RELOP LOGICOP
%token <symbol_info> INCOP DECOP ASSIGNOP NOT

%token <symbol_info> LPAREN RPAREN LCURL RCURL LSQUARE RSQUARE COMMA SEMICOLON

%token <symbol_info> CONST_INT CONST_FLOAT CONST_CHAR ID

%type <symbol_info> variable factor term unary_expression simple_expression rel_expression logic_expression expression declaration_list parameter_list argument_list arguments
%type <symbol_info> expression_statement statement statements compound_statement if_same
%type <symbol_info> type_specifier var_declaration func_declaration func_definition unit program start

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%%
start : {
		codeAsmOut << ".MODEL SMALL\n.STACK 100h\n.DATA\n\n";
		codeSegOut << ".CODE\n";
	}
	program { 
		$$ = new SymbolInfo("","start : program") ;
		logFile << "start : program" << endl;
		$$->add_child($2) ;
		$$->start_line = $2->start_line ;
		$$->end_line = $2->end_line ;
		DFS($$,"") ;
		
		st.printAllScope(logFile); 
        st.exitScope();	
		cout << countr << endl;
		cout << "Total Lines: " << yylineno  << endl;
		cout << "Total Errors: " << err_cnt << endl;
		genFunctionForPrintln();
	}
	;
program : program unit { 
		$$ = new SymbolInfo("","program : program unit") ;
		logFile << "program : program unit"<< endl ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
	}
	| unit {
		$$ = new SymbolInfo("","program : unit") ;
		logFile << "program : unit" << endl ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
	;
unit : var_declaration {
		$$ = new SymbolInfo("", "unit : var_declaration") ;
		logFile << "unit : var_declaration" << endl ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
    | func_declaration {
		$$ = new SymbolInfo("", "unit : func_declaration") ;
		logFile << "unit : func_declaration" << endl ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
    | func_definition {
		$$ = new SymbolInfo("", "unit : func_definition") ;
		logFile << "unit : func_definition" << endl ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
    ;
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {
		$$ = new SymbolInfo("", "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON") ;
		logFile << "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON" << endl ;
		func_declare($2->getName(), $1->data_type, dec_list);
		$2->info_type = FUNCTION_DECLARATION ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->add_child($6) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $6->end_line ;
	}
	| type_specifier ID LPAREN RPAREN SEMICOLON {
		$$ = new SymbolInfo("","func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON") ;
		logFile << "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON" << endl ;
		func_declare($2->getName(), $1->data_type,dec_list);
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $5->end_line ;
	}
	;
func_definition : type_specifier ID LPAREN parameter_list RPAREN {
		gs = false ;
		codeSegOut << "\t" << $2->getName() << " PROC\n";
		if ($2->getName() == "main") {
			main_terminal_label = newlabel();
			codeSegOut << "\t\tMOV AX, @DATA\n\t\tmov DS, AX\n";
			_main = true;
		}
		else{
			_main = false;
			codeSegOut << "\t\tPUSH BP" << endl ;
		}
		codeSegOut << "\t\tMOV BP, SP" << endl ;
		func_def($2->getName(), $1->data_type, dec_list,yylineno);
	} compound_statement {
		if ($2->getName() == "main") {
			codeSegOut << "\n\t\t" << main_terminal_label << ":" << endl;
			codeSegOut << "\t\tMOV AH, 4CH" << endl;
			codeSegOut << "\t\tINT 21H" << endl;
		}
		else{
			codeSegOut <<"\t\tPOP BP" << endl ;
			codeSegOut << "\t\tRET" << endl ;
			_main = true;
		}
		codeSegOut << "\t" << $2->getName() << " ENDP\n\n" ;
		$$ = new SymbolInfo("", "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement") ;	
		logFile << "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement" <<endl ;
		$$->add_child($1);
		$$->add_child($2);
		$$->add_child($3);
		$$->add_child($4);
		$$->add_child($5) ;
		$$->add_child($7) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $7->end_line ;
	}
	| type_specifier ID LPAREN RPAREN 
	{
		gs = false ;
		codeSegOut << "\t" << $2->getName() << " PROC\n";
		if ($2->getName() == "main") {
			main_terminal_label = newlabel();
			codeSegOut << "\t\tMOV AX, @DATA\n\t\tmov DS, AX\n";
			_main = true;
		}
		else{
			_main = false;
			codeSegOut << "\t\tPUSH BP" << endl ;
		}
		codeSegOut << "\t\tMOV BP, SP" << endl ;

		func_def($2->getName(), $1->data_type,dec_list,yylineno );
	} compound_statement {
		if ($2->getName() == "main") {
			codeSegOut << "\n\t\t" << main_terminal_label << ":" << endl;
			codeSegOut << "\t\tMOV AH, 4CH" << endl;
			codeSegOut << "\t\tINT 21H" << endl;
		}
		else{
			codeSegOut << "\t\tPOP BP" << endl ;
			codeSegOut << "\t\tRET" << endl ;
			_main = true;
		}
		codeSegOut << "\t" << $2->getName() << " ENDP\n\n" ;
		$$ = new SymbolInfo("","func_definition : type_specifier ID LPAREN RPAREN compound_statement") ;
		logFile << "func_definition : type_specifier ID LPAREN RPAREN compound_statement" << endl ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($6) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $6->end_line ;

	}
	;
parameter_list  : parameter_list COMMA type_specifier ID {
		$$ = new SymbolInfo("" ,"parameter_list : parameter_list COMMA type_specifier ID") ;
		logFile << "parameter_list : parameter_list COMMA type_specifier ID" <<endl ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $4->end_line ;

		SymbolInfo* info = new SymbolInfo($4->getName(),"ID") ;
		info->data_type = $3->data_type ;
		info->info_type = $4->info_type ;
		info->arr_strt = $4->arr_strt ;
		info->asm_name = $4->asm_name ;
		info->global = $4->global ;
		dec_list.push_back(info) ;
		pline = yylineno;
	}
	| parameter_list COMMA type_specifier {
		$$ = new SymbolInfo("","parameter_list : parameter_list COMMA type_specifier") ;
		logFile << "parameter_list : parameter_list COMMA type_specifier" <<endl ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		SymbolInfo* symbol = new SymbolInfo($3->getType(),"ID") ;
        symbol->data_type = $3->data_type ;
		pline = yylineno;
		dec_list.push_back(symbol) ;
	}
	| type_specifier ID {
		$$ = new SymbolInfo("","parameter_list : type_specifier ID") ;
		logFile <<"parameter_list : type_specifier ID" <<endl ;
        $$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;

		SymbolInfo* symbol = new SymbolInfo($2->getName(),"ID") ;
        symbol->data_type = $1->data_type ;
		symbol->info_type = $2->info_type ;
		symbol->arr_strt = $2->arr_strt ;
		symbol->global = $2->global ;
		symbol->asm_name = $2->asm_name ;
		dec_list.push_back(symbol) ;
		//$$->_parameter_list = dec_list ;
		pline = yylineno;
	}
	| type_specifier {
		$$ = new SymbolInfo("", "parameter_list : type_specifier") ;
		logFile << "parameter_list : type_specifier" << endl ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;

		string s = "" ;
        SymbolInfo* symbol = new SymbolInfo(s,$1->getType()) ;
        symbol->data_type = $1->data_type ;
		dec_list.push_back(symbol) ;
	}
	;
compound_statement : LCURL {st.enterScope(++id , 11);dec_func_param_list(dec_list, pline);} statements RCURL {
		logFile << "compound_statement : LCURL statements RCURL" <<endl ;
		$$ = new SymbolInfo("","compound_statement : LCURL statements RCURL");
		$$->add_child($1) ;
		$$->add_child($3) ;
		$$->add_child($4) ;

		$$->start_line = $1->start_line ;
		$$->end_line = $4->end_line ;
		st.printAllScope(logFile);
        st.exitScope();
	}
	| LCURL {st.enterScope(++id,11);} RCURL {
		logFile << "compound_statement : LCURL RCURL" <<endl ;
		$$ = new SymbolInfo("","compound_statement : LCURL RCURL") ;
		$$->add_child($1) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		st.printAllScope(logFile) ;
		st.exitScope();
	}
	;
var_declaration : type_specifier declaration_list SEMICOLON {
		logFile << "var_declaration : type_specifier declaration_list SEMICOLON" <<endl ;
		$$ = new SymbolInfo("", "var_declaration : type_specifier declaration_list SEMICOLON") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		$$->_parameter_list = dec_list ;
        dec_var($1->data_type,dec_list) ;
        dec_list.clear() ;
	}
	;
type_specifier : INT {
		logFile << "type_specifier : INT" << endl ;
		$$ = new SymbolInfo("","type_specifier : INT") ;
		$$->data_type = "INT" ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
	| FLOAT {
		logFile << "type_specifier : FLOAT" << endl ;
		$$ = new SymbolInfo("","type_specifier : FLOAT") ;
		$$->data_type = "FLOAT" ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		
	}
	| VOID {
		logFile << "type_specifier : VOID" <<endl ;
		$$ = new SymbolInfo("","type_specifier : VOID") ;
		$$->data_type = "VOID" ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
	;
declaration_list : declaration_list COMMA ID {
		logFile << "declaration_list : declaration_list COMMA ID" << endl ;
		$$ = new SymbolInfo("","declaration_list : declaration_list COMMA ID");
		gen_var_dec_code_asmbly($3,gs) ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		dec_list.push_back($3) ;
		$$->_parameter_list = dec_list ;
	}
	| declaration_list COMMA ID LSQUARE CONST_INT RSQUARE {
		$$ = new SymbolInfo("", "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE") ;
		string code = $3->getName()+"["+$5->getName()+"]";
		$3->array_size = $5->getName() ;
		$3->info_type = ARRAY ;
		dec_list.push_back($3) ;
		logFile << "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE" << endl ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->add_child($6) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $6->end_line ;

		$$->_parameter_list = dec_list ;
		gen_var_dec_code_asmbly($3,gs) ;
	}
	| ID {
		logFile << "declaration_list : ID" << endl ;
		$$ = new SymbolInfo("", "declaration_list : ID") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		dec_list.push_back($1) ;
		$$->_parameter_list = dec_list ;
		gen_var_dec_code_asmbly($1,gs) ;
	}
	| ID LSQUARE CONST_INT RSQUARE {
		$$ = new SymbolInfo("","declaration_list : ID LSQUARE CONST_INT RSQUARE") ;
		logFile << "declaration_list : ID LSQUARE CONST_INT RSQUARE" << endl ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $4->end_line ;
		$1->array_size = $3->getName();
		$1->info_type = ARRAY ;
		dec_list.push_back($1) ;
		$$->_parameter_list = dec_list ;
		gen_var_dec_code_asmbly($1,gs) ;
	}
	;
statements : statement {
		logFile << "statements : statement" << endl ;
		$$ = new SymbolInfo("","statements : statement") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
	| statements statement {
		logFile << "statements : statements statement" << endl ;
		$$ = new SymbolInfo("","statements : statements statement") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
	}
	;
statement : var_declaration {
		//codeSegOut <<"\t\t"+newlabel()+":" << endl;
		logFile << "statement : var_declaration" << endl ;
		$$ = new SymbolInfo("","statement : var_declaration") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}	
	| expression_statement {
		//codeSegOut <<"\t\t"+newlabel()+":" << endl;
		logFile << "statement : expression_statement" << endl ;
		$$ = new SymbolInfo("","statement : expression_statement") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		codeSegOut << "\t\tPOP AX" << endl ;
	}
	| compound_statement {
		logFile << "statement : compound_statement" <<endl ;
		$$ = new SymbolInfo("", "statement : compound_statement") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
	| FOR LPAREN {
		label.clear() ;
	} expression_statement {
		label["forCond"] = newlabel();
		label["forStmt"] = newlabel();
		label["forUpdate"] = newlabel();
		label["endFor"] = newlabel();
		codeSegOut << "\t\tPOP AX" << endl ;
		codeSegOut << "\t\t"+label["forCond"]+":" << endl ;
	} expression_statement {
		codeSegOut << "\t\tPOP AX" << endl ; 
		codeSegOut << "\t\tCMP AX, 0" << endl ;
		codeSegOut << "\t\tJE "+label["endFor"] << endl ; 
		codeSegOut << "\t\tJMP "+label["forStmt"] << endl ;
		codeSegOut << "\t\t"+label["forUpdate"]+":" << endl ;
		//codeSegOut << "More Jaitesi" << endl;
	} expression {
		codeSegOut << "\t\tPOP AX" << endl ;
		codeSegOut << "\t\tJMP "+label["forCond"] << endl ;
		codeSegOut << "\t\t"+label["forStmt"]+":" << endl ;
		//codeSegOut << "More Jaitesi" << endl;
	} RPAREN statement {
		logFile << "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement" << endl ;
		$$ = new SymbolInfo("", "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($4) ;
		$$->add_child($6) ;
		$$->add_child($8) ;
		$$->add_child($10) ;
		$$->add_child($11) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $11->end_line ;
		codeSegOut << "\t\tJMP "+label["forUpdate"] << endl ;
		codeSegOut << "\t\t"+label["endFor"]+":" << endl ;
	}
	| if_same %prec LOWER_THAN_ELSE {
		logFile << "statement : IF LPAREN expression RPAREN statement" <<endl ;
		$$ = new SymbolInfo("", "statement : IF LPAREN expression RPAREN statement") ;
		$$->add_child($1) ;
		//$$->add_child($2) ;
		//$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		codeSegOut << "\t\t"+$1->label+":" << endl ;
	}
	| if_same ELSE {
		string els_end = newlabel() ;
		codeSegOut << "\t\tJMP "+els_end << endl ;
		codeSegOut << "\t\t"+$1->label+":" << endl ;
		$1->label = els_end ; 
	} statement {
		logFile << "statement : IF LPAREN expression RPAREN statement ELSE statement" << endl ;
		$$ = new SymbolInfo("","statement : IF LPAREN expression RPAREN statement ELSE statement") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($4) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $4->end_line ;
		codeSegOut << "\t\t"+$1->label+":" << endl ;
	}
	| WHILE LPAREN {
		wl = newlabel() ;
		codeSegOut << "\t\t"+wl+":" << endl ;
	} expression RPAREN {
		wle = newlabel() ;
		codeSegOut << "\t\tPOP AX" << endl ;
		codeSegOut << "\t\tCMP AX , 0" << endl ;
		codeSegOut << "\t\tJE "+ wle << endl ;
		$4->label = wl+" "+wle ;
	} statement {
		logFile << "statement : WHILE LPAREN expression RPAREN statement" << endl ;
		$$ = new SymbolInfo("","statement : WHILE LPAREN expression RPAREN statement");
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->add_child($7) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $7->end_line ;
		stringstream ss($4->label);
		string wl, wle ;
		ss >> wl >> wle;


		codeSegOut << "\t\tJMP "+wl << endl ;
		codeSegOut << "\t\t"+ wle +":" << endl;  
	}
	| PRINTLN LPAREN variable RPAREN SEMICOLON {
		logFile << "statement : PRINTLN LPAREN ID RPAREN SEMICOLON" << endl ;
		$$ = new SymbolInfo("","statement : PRINTLN LPAREN ID RPAREN SEMICOLON") ;
		if(!st.lookup($3->getName())){
			error <<"Line# "<<yylineno << ": Undeclared variable  '"+$3->getName()+"'" << endl ;
			err_cnt++ ;
		}
		else {
			SymbolInfo* info = st.lookup($3->getName()) ;
			codeSegOut << "\t\tMOV BX, "+ $3->asm_name << endl ;
			codeSegOut << "\t\tCALL PRINT_NUMBER" << endl ;
		}
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $5->end_line ;

	}
	| RETURN expression SEMICOLON {
		logFile << "statement : RETURN expression SEMICOLON" <<endl ;
		$$ = new SymbolInfo("","statement : RETURN expression SEMICOLON") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		if(_main == false) {
			codeSegOut << "\t\tPOP AX" << endl ;
			codeSegOut << "\t\tMOV [BP+4], AX" << endl ;
			int _add = 0 ;
			if(st.getCurrScope()) {
				_add = st.getCurrScope()->var_cnt ;
			}
			codeSegOut << "\t\tADD SP, "+to_string(_add * 2) << endl ;
			codeSegOut << "\t\tPOP BP" << endl ;
			codeSegOut << "\t\tRET" << endl; 
		}
	}
	| RETURN SEMICOLON {
		logFile << "statement : RETURN expression SEMICOLON" <<endl ;
		$$ = new SymbolInfo("","statement : RETURN expression SEMICOLON") ;
		//$$->add_child($1) ;
		//$$->add_child($2) ;
		//$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
	}
	;

if_same : IF LPAREN expression RPAREN {
		string end_if = newlabel() ;
		codeSegOut << "\t\tPOP AX" << endl ;
		codeSegOut << "\t\tCMP AX, 0" << endl ;
		codeSegOut << "\t\tJE "+end_if << endl ;
		$3->label = end_if ;
	} statement {
		$$ = new SymbolInfo("","if_same : IF LPAREN expression RPAREN statement") ;
		$$->label = $3->label ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($6) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $6->end_line ;
		$$->label = $3->label ;
	}
	;

expression_statement : SEMICOLON {
		logFile << "expression_statement : SEMICOLON" << endl ;
		$$ = new SymbolInfo("","expression_statement : SEMICOLON");
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		codeSegOut << "\t\tPUSH 1" << endl ;
	}			
	| expression SEMICOLON {
		logFile << "expression_statement : expression SEMICOLON" << endl ;
		$$ = new SymbolInfo("","expression_statement : expression SEMICOLON");
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
	}
	;
variable : ID { 
		logFile << "variable : ID" << endl ;
		SymbolInfo *info = st.lookup($1->getName());
		$$ = new SymbolInfo("","variable : ID",true) ;
		if(info){
			if(info->is_array()){
				//error << "Line# " << yylineno <<": Type mismatch, "+info->getName()+" is an array" << endl;
				//err_cnt++ ;
			}
			$$->setName(info->getName()) ;
			$$->info_type = info->info_type ;
			$$->data_type = info->data_type ;
			$$->array_size = info->array_size ;
			$$->position = info->position ;
			$$->_parameter_list = info->_parameter_list ;
			$$->var = info->info_type ;
			$$->asm_name = get_var_address(info) ;
			cout << $$->getName() <<$1->asm_name <<"MAKE AMERICA GREAT AGAIN" << endl ;
		}else{
			error << "Line# " <<yylineno << ": Undeclared variable '"+$1->getName()+"'" << endl;
			err_cnt++ ;
			$$->setName($1->getName()) ;
			$$->setType("variable : ID") ;
			$$->data_type = $1->data_type ;
			$$->var = VARIABLE ;
			
		}
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
	| ID LSQUARE {
		arr_idx = true ;
	} expression RSQUARE {
		arr_idx = false ;
		logFile << "variable : ID LSQUARE expression RSQUARE" << endl ;
		SymbolInfo *info = st.lookup($1->getName());
		if(info != nullptr){
			$1->data_type = info->data_type ;
			if(!info->is_array()){
				error << "Line# "<<yylineno <<": '" <<$1->getName() <<"' is not an array" << endl ;
				err_cnt++ ;
			}
			// Generate an error message if the index of an array is not an integer
			if($4->data_type !="INT"){
				error << "Line# " <<yylineno <<": Array subscript is not an integer"<<endl;
				err_cnt++ ;
			}
			else if($4->data_type =="") {
				error << "Line# " <<yylineno <<": Array subscript cannot be empty"<<endl;
				err_cnt++ ;
			}
			$1 = new SymbolInfo(info) ;

			codeSegOut << "\t\tPOP AX" << endl ;
			codeSegOut << "\t\tSHL AX, 1" << endl ;
			codeSegOut << "\t\tLEA BX, "+info->asm_name << endl;
			codeSegOut << "\t\tSUB BX, AX" << endl ;
			codeSegOut << "\t\tPUSH BX" << endl ;
			$1->asm_name = "[BX]" ;
			$1->global = false ;
			$1->arr_strt = 0 ;
		}else{
			error <<"Line# " << yylineno << ": Undeclared variable '"+$1->getName()+"'" << endl ;
			err_cnt++ ;
		}
		$$ = new SymbolInfo("","variable : ID LSQUARE expression RSQUARE") ;

		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $5->end_line ;
		$$->info_type = ARRAY ;
		$$->setName($1->getName()) ;
		$$->setType("variable : ID LSQUARE expression RSQUARE") ;
		$$->rule = true ;
		$$->data_type = $1->data_type ;
		$$->var = VARIABLE ; 
		$$->array_size = $1->array_size ;
		$$->global = $1->global ;
		$$->arr_strt = $1->arr_strt ;
		$$->asm_name = $1->asm_name ;
		string code = $1->getName()+"["+$4->array_size+"]";
		cout << code << " " <<code << endl;
	}
	;
expression : logic_expression {
		logFile << "expression : logic_expression" << endl ;
		$$ = new SymbolInfo("","expression : logic_expression") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;

		$$->setName($1->getName()) ;
		$$->rule = true ;
		$$->setType("expression : logic_expression") ;
		$$->data_type = $1->data_type ;
		$$->var = $1->var ;
		$$->array_size = $1->array_size ;
		$$->label = $1->label ;
	}
	| variable ASSIGNOP logic_expression {
		logFile << "expression : variable ASSIGNOP logic_expression" << endl ;
		SymbolInfo *info = st.lookup($1->getName());
		if(info!=nullptr){
			if(allCapital(info->data_type) == "INT" && allCapital($3->data_type)=="FLOAT"){
				error << "Line# " <<yylineno<<": Warning: possible loss of data in assignment of FLOAT to INT" << endl ;
				err_cnt++ ;
			}
		}
		if(allCapital($3->data_type)=="VOID"){
				error << "Line# "<<yylineno <<": Void cannot be used in expression" << endl ;
				err_cnt++ ;
		}
		$$ = new SymbolInfo("", "expression : variable ASSIGNOP logic_expression");
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;

		$$->data_type = $1->data_type ;
		$$->var = $1->var ;
		codeSegOut << "\t\tPOP AX" << endl ;

		codeSegOut << "\t\tMOV "+get_var_address($1,true) + ", AX" << endl ;
		codeSegOut << "\t\tPUSH AX" << endl ;
	}
	;
logic_expression : rel_expression { 
		logFile << "logic_expression : rel_expression" << endl ;
		$$ = new SymbolInfo("","logic_expression : rel_expression") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->data_type = $1->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;
		$$->array_size = $1->array_size ;
		$$->label = $1->label ;
		//cout <<"rel expression data_type : " << $1->data_type << endl;
	}	
	| rel_expression LOGICOP {
		codeSegOut << "\t\tPOP AX" << endl ;
		string val ;
		if($2->getName() == "&&") {
			val = "1" ;
		}
		else val = "0" ;
		codeSegOut << "\t\tCMP AX, " + val << endl ;
		string jmplbl = newlabel() ;
		codeSegOut << "\t\tJNE "+jmplbl << endl ;
		$1->label = jmplbl ;
	} rel_expression {
		logFile << "logic_expression : rel_expression LOGICOP rel_expression" <<endl ;
		$$ = new SymbolInfo("","logic_expression : rel_expression LOGICOP rel_expression") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($4) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $4->end_line ;
		$$->data_type = "INT" ;
		$$->var = $1->var ;
		
		
		codeSegOut << "\t\tPOP AX" << endl ;
		string val ;
		if($2->getName() == "&&") {
			val = "1" ;
		}
		else val = "0" ;
		codeSegOut << "\t\tCMP AX, "+val << endl ;
		codeSegOut << "\t\tJNE "+$1->label << endl ;
		codeSegOut << "\t\t\tPUSH "+val << endl ;
		string le = newlabel();
		codeSegOut << "\t\t\tJMP "+ le << endl ;
		codeSegOut << "\t\t"+$1->label+":" << endl ;
		if($2->getName() == "&&") {
			val = "0" ;
		}
		else val = "1" ;
		codeSegOut << "\t\t\tPUSH " +val << endl ;
		codeSegOut << "\t\t"+le+":" << endl ;
	}	
	;
rel_expression : simple_expression {
		logFile << "rel_expression : simple_expression"<< endl ;
		$$ = new SymbolInfo("","rel_expression : simple_expression") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->data_type = $1->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;
		$$->array_size = $1->array_size ;
		$$->label = $1->label ;
	}
	| simple_expression RELOP simple_expression	{
		logFile << "rel_expression : simple_expression RELOP simple_expression" <<endl ;
		$$ = new SymbolInfo("","rel_expression : simple_expression RELOP simple_expression") ;
		
		if($1->data_type == "INT" && $3->data_type == "FLOAT"){
			$1->data_type = "FLOAT" ;
		}
		else if($1->data_type == "FLOAT" && $3->data_type == "INT"){
			$3->data_type = "FLOAT" ;
		}
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		$$->data_type = "INT" ;

		string true_ = newlabel();
		string false_ = newlabel();
		string op = get_relop_jmp_asmbly($2->getName());


		codeSegOut << "\t\tPOP BX" << endl ;
		codeSegOut << "\t\tPOP AX" << endl ;
		codeSegOut << "\t\tCMP AX, BX" << endl ;
		codeSegOut << "\t\t"+op+" "+true_ << endl ;
		codeSegOut << "\t\t\tPUSH 0" << endl;
		codeSegOut << "\t\t\tJMP "+false_ << endl ;
		codeSegOut << "\t\t"+true_+":" << endl ;
		codeSegOut << "\t\t\tPUSH 1" << endl ;
		codeSegOut << "\t\t"+false_+":" << endl ;
	}
	;
simple_expression : term {
		logFile << "simple_expression : term" << endl ;
		$$ = new SymbolInfo("","simple_expression : term") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->data_type = $1->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;
		$$->array_size = $1->array_size ;
		$$->label = $1->label ;
	}	
	| simple_expression ADDOP term {
		logFile << "simple_expression : simple_expression ADDOP term" << endl ;
		$$ = new SymbolInfo("","simple_expression : simple_expression ADDOP term") ;
		if(allCapital($1->data_type) == "VOID" || allCapital($3->data_type) == "VOID") {
			error << "Line# " << yylineno << ": Void cannot be used in expression" <<endl ;
			err_cnt++ ;
		}
		string TYPE ="" ;
		if($1->data_type == $3->data_type) {
			TYPE = $1->data_type ;
		}
		else if($1->data_type == "INT" && $3->data_type == "FLOAT"){
			$1->data_type = "FLOAT" ;
			TYPE = "FLOAT" ;
		}
		else if($1->data_type == "FLOAT" && $3->data_type == "INT"){
			$3->data_type = "FLOAT" ;
			TYPE = "FLOAT" ;
		}
		else if($1->data_type != "VOID") {
			TYPE = $1->data_type ;
		}
		else {
			TYPE = $3->data_type ;
		}
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;

		$$->data_type = TYPE ;
		
		
		codeSegOut << "\t\tPOP BX" << endl ;
		codeSegOut << "\t\tPOP AX" << endl ;
		string op ;
		if($2->getName() == "+") {
			op = "ADD" ;
		}
		else op = "SUB" ;
		
		codeSegOut << "\t\t"+op+" AX, BX" << endl ;
		codeSegOut << "\t\tPUSH AX" << endl ;
	} 
	;
term :	unary_expression {
		logFile << "term : unary_expression" << endl ; 
		$$ = new SymbolInfo("","term : unary_expression") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->data_type = $1->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;
		$$->array_size = $1->array_size ;
		$$->label = $1->label ;
	}
    |  term MULOP unary_expression {
		
		logFile <<"term : term MULOP unary_expression" <<endl ;
		$$ = new SymbolInfo("","term : term MULOP unary_expression") ;
		if(allCapital($1->data_type) == "VOID" || allCapital($3->data_type) == "VOID") {
			error << "Line# " << yylineno << ": Void cannot be used in expression" <<endl ;
			err_cnt++ ;
		}
		if($2->getName() == "%"){
			if($3->getName() == "0") {
				err_cnt++ ;
				error << "Line# " << yylineno << ": Warning: division by zero i=0f=1Const=0" << endl;
			}
			//Type Checking: Both the operands of the modulus operator should be integers.
			if(allCapital($1->data_type) != "INT" || allCapital($3->data_type) != "INT"){
				error << "Line# " << yylineno <<": Operands of modulus must be integers " << endl ;
				err_cnt++ ;
			}
			$1->data_type = "INT" ;
			$3->data_type ="INT" ;
		}
		string TYPE ="" ;
		if($1->data_type == $3->data_type) {
			TYPE = $1->data_type ;
		}
		else if($1->data_type == "INT" && $3->data_type == "FLOAT"){
			$1->data_type = "FLOAT" ;
			TYPE = "FLOAT" ;
		}
		else if($1->data_type == "FLOAT" && $3->data_type == "INT"){
			$3->data_type = "FLOAT" ;
			TYPE = "FLOAT" ;
		}
		else if($1->data_type != "VOID") {
			TYPE = $1->data_type ;
		}
		else {
			TYPE = $3->data_type ;
		}

		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;

		$$->data_type = TYPE ;
		
		codeSegOut << "\t\tPOP BX" << endl ;
		codeSegOut << "\t\tPOP AX" << endl ;
		codeSegOut << "\t\tXOR DX, DX" << endl ;
		string op ;
		if($2->getName() == "*") {
			op = "IMUL" ;
		}
		else op = "IDIV" ;
		codeSegOut << "\t\t"+op+" BX" << endl ;
		string result ;
		if($2->getName() == "%") 
			result =  "DX" ;
		else result = "AX" ;
		codeSegOut << "\t\tPUSH "+result << endl ;
	}
    ;
unary_expression : ADDOP unary_expression {
		logFile << "unary_expression : ADDOP unary_expression" ;
		$$ = new SymbolInfo("","unary_expression : ADDOP unary_expression") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
		$$->data_type = $2->data_type ;


		if($1->getName() == "-"){
			codeSegOut << "\t\tPOP AX" << endl ;	codeSegOut << "\t\tNEG AX" << endl ;	codeSegOut << "\t\tPUSH AX" << endl ;
		}
	}  
	| NOT unary_expression {
		logFile << "unary_expression : NOT unary_expression" << endl ;
		$$ = new SymbolInfo("", "unary_expression : NOT unary_expression") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
		$$->data_type = $2->data_type ;

		string label1 , label2 ;
		label1 = newlabel() ; label2 = newlabel() ;
		codeSegOut << "\t\tPOP AX" << endl ;
		codeSegOut << "\t\tCMP AX, 0" << endl ;
		codeSegOut << "\t\tJE "+label1 << endl ;
		codeSegOut << "\t\t\tPUSH 0" << endl ;
		codeSegOut << "\t\t\tJMP "+label2 << endl ;
		codeSegOut << "\t\t"+label1+":" <<endl ;
		codeSegOut << "\t\tPUSH 1" << endl ;
		codeSegOut << "\t\t"+label2+":" << endl ;
	} 
	| factor {
		logFile << "unary_expression : factor" << endl ;
		$$ = new SymbolInfo("","unary_expression : factor") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->setType("unary_expression : factor") ;
		$$->setName($1->getName()) ;
		$$->rule = true ;
		$$->data_type = $1->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;
		$$->array_size = $1->array_size ;
		$$->label = $1->label ;
	} 
	;
factor	: variable {
		logFile << "factor : variable" << endl ;
		$$ = new SymbolInfo("","factor : variable") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->setName($1->getName()) ;
		$$->rule = true ;
		$$->setType("factor : variable") ;
		$$->data_type = $1->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;
		$$->label = $1->label ;
		codeSegOut << "\t\tPUSH " +get_var_address($1,true) << endl ;
	}
	| ID LPAREN argument_list RPAREN {
		logFile << "factor : ID LPAREN argument_list RPAREN" << endl ;
		
		$$ = new SymbolInfo("","factor : ID LPAREN argument_list RPAREN") ;
		int x = dec_list.size() ;
		call_func($1,dec_list) ;

		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $4->end_line ;

		$$->data_type = $1->data_type ;
		codeSegOut << "\t\tPUSH 0" << endl ;
		codeSegOut << "\t\tCALL "+$1->getName() << endl ;
		codeSegOut << "\t\tPOP AX" << endl ;
		codeSegOut << "\t\tADD SP,	"+to_string(x*2) << endl ;
		if($1->data_type != "void"){
			codeSegOut << "\t\tPUSH AX" << endl ;
		}else{
			codeSegOut << "\t\tPUSH 0" << endl ;
		}
	}
	| LPAREN expression RPAREN {
		logFile << "factor : LPAREN expression RPAREN" <<endl ;
		$$ = new SymbolInfo("","factor : LPAREN expression RPAREN") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		$$->data_type = $2->data_type ;
		//$$->var = $1->var ;
	}
	| CONST_INT { 
		logFile << "factor : CONST_INT" << endl ;
		$$ = new SymbolInfo($1->getName(),"factor : CONST_INT",true) ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->data_type = "INT" ;
		codeSegOut << "\t\tPUSH "+$1->getName() << endl ;
		$$->array_size = $1->getName() ;
		cout << $$->array_size << endl;
		$$->label = $1->label ;

	}
	| CONST_FLOAT { 
		logFile << "factor : CONST_FLOAT" << endl ;
		$$ = new SymbolInfo($1->getName(), "factor : CONST_FLOAT",true) ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->data_type = "FLOAT" ;
		codeSegOut << "\t\tPUSH "+$1->getName() << endl ;
	}
	| variable INCOP {
		logFile << "factor : variable INCOP" << endl ;
		$$ = new SymbolInfo("","factor : variable INCOP") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
		$$->data_type = $1->data_type ;
		string address = get_var_address($1,true) ;
		codeSegOut << "\t\tPUSH " << address << endl;
		codeSegOut << "\t\tINC " << address << endl ;
	}
	| variable DECOP {
		logFile << "factor : variable DECOP" << endl ;
		$$ = new SymbolInfo("", "factor : variable DECOP",true) ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
		$$->data_type = $1->data_type ;
		string address = get_var_address($1,true) ;
		codeSegOut << "\t\tPUSH " << address << endl;
		codeSegOut << "\t\tDEC "<< address << endl ;
	}
	;
	
//vector<SymbolInfo*>*
argument_list : arguments {
		logFile <<"argument_list : arguments" << endl ;
		$$ = new SymbolInfo("", "argument_list : arguments") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->_parameter_list = dec_list ;
	}
	|
	{
		logFile  << "argument_list" << endl ;
		$$ = new SymbolInfo("","argument_list") ;

	}
	;
	
//vector<SymbolInfo*>*
arguments : arguments COMMA logic_expression {
		//string code = toSymbolNameListStr($1) + "," + $3->getName();
		logFile << "arguments : arguments COMMA logic_expression" << endl ;
		$$ = new SymbolInfo("","arguments : arguments COMMA logic_expression") ;

		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		$$->data_type = $3->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;
		dec_list.push_back($3) ;
	}
	| logic_expression {
		logFile << "arguments : logic_expression" << endl ;
		$$ = new SymbolInfo("","arguments : logic_expression") ;

		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->data_type = $1->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;

		dec_list.push_back($1) ;

	}
	;
%%

int main(int argc,char *argv[]) {
	
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}

	codeSegOut.open("code2.txt") ;
	//logFile.open("log.txt");
	//error.open("error.txt") ;
	//parsefile.open("parser.txt") ;
	st.enterScope(++id,11) ;

	yyin= fin;
	yylineno = 1 ;
	yyparse() ;
	fclose(yyin);
	yylex_destroy() ;

	ifstream codeSegmentIn("code2.txt");
    string line;
    codeAsmOut << endl;
    while (getline(codeSegmentIn, line)) {
        codeAsmOut << line << endl;
    }

    codeAsmOut << endl << "END MAIN" << endl;

    codeSegmentIn.close();
    codeAsmOut.close();

	ifstream codeSegmentIn1("code.asm") ;
    ofstream optOut("opt_code.asm", ios::out);
    vector<string> lines;
    while (getline(codeSegmentIn1, line)) {
        lines.push_back(line);
    }

    for (int i = 0; i < lines.size(); i++) {
        if (i + 1 >= lines.size() || lines[i].size() < 4 || lines[i + 1].size() < 4) {
			//do nothing
        }
        else if (lines[i].substr(2, 5) == "MOV" && lines[i + 1].substr(2, 5) == "MOV") {
            string line1 = lines[i].substr(4) ;
            string line2 = lines[i + 1].substr(4) ;

            int Index1 = line1.find(",") ; int Index2 = line2.find(",") ;

            if (line1.substr(1, Index1 - 1) == line2.substr(Index2 + 2)){
                if (line1.substr(Index1 + 2) == line2.substr(1, Index2 - 1)) {
                    i++;
                    continue;
                }
			}
        }
		else if(lines[i].substr(2,4) == "PUSH" && lines[i+1].substr(2,3) == "POP") {
			string x = lines[i].substr(7,2) ;
			string y = lines[i+1].substr(6,2) ;
			//cout <<"1st here" << endl;
			if(x == y && x=="AX" || x == "BX") {
				i = i+1 ;
				cout <<"here" << endl;
				continue ;
			}
		}

        optOut << lines[i] << endl;
    }

    optOut.close();
    codeSegmentIn1.close();
	remove("code2.txt") ;



	return 0;
}

