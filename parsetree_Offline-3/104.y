%{
	#pragma once
    #include <bits/stdc++.h>
    #include "SymbolTable.h"
    
    
    using namespace std;

	extern string allCapital(string str) ;
	int id = 0; 

	SymbolTable st ;

    extern FILE *yyin;
    extern int yylineno ;
    int yyparse(void);
    int yylex(void);

    ofstream logFile , error, parsefile ;
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



    bool dec_func_param(string dt , string name , int line = yylineno) {
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

            return true ;
        }
		name="'"+name+"'" ;
		error << "Line# " << line <<": Redefinition of parameter "+name << endl;
		err_cnt++ ;

		return false ;
    }

    void dec_func_param_list(vector<SymbolInfo*> &list , int line = yylineno) {
        if(list.size() == 0) return ;
		for(SymbolInfo* info: list){
			if(!dec_func_param(info->data_type, info->getName(), line)){
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
%type <symbol_info> expression_statement statement statements compound_statement
%type <symbol_info> type_specifier var_declaration func_declaration func_definition unit program start

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%%
start : program { 
		$$ = new SymbolInfo("","start : program") ;
		logFile << "start : program" << endl;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		DFS($$,"") ;
		//delete $$ ;
		
		st.printAllScope(logFile); 
        st.exitScope();

		//dfs_delete($$) ;	
		cout << countr << endl;
		cout << "Total Lines: " << yylineno  << endl;
		cout << "Total Errors: " << err_cnt << endl;
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
		//delete $1; 
        //delete $2;
	}
	;
func_definition : type_specifier ID LPAREN parameter_list RPAREN {func_def($2->getName(), $1->data_type, dec_list,yylineno);} compound_statement {
		$$ = new SymbolInfo("", "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement") ;	
		logFile << "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement" <<endl ;
        $$->add_child($1);
        $$->add_child($2);
		$$->add_child($3);
		$$->add_child($4);
		$$->add_child($5) ;
		$$->add_child($7) ;
		//cout << $7 << endl;
		$$->start_line = $1->start_line ;
		$$->end_line = $7->end_line ;
	}
	| type_specifier ID LPAREN RPAREN {func_def($2->getName(), $1->data_type,dec_list,yylineno );} compound_statement {
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
		dec_list.push_back(info) ;
		//$$->_parameter_list = dec_list ; 
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
		//$$->_parameter_list = dec_list ; 
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
		//$$->_parameter_list = dec_list ;
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
	}
	| ID {
		logFile << "declaration_list : ID" << endl ;
		$$ = new SymbolInfo("", "declaration_list : ID") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		dec_list.push_back($1) ;
		$$->_parameter_list = dec_list ;
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
		logFile << "statement : var_declaration" << endl ;
		$$ = new SymbolInfo("","statement : var_declaration") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}	
	| expression_statement {
		logFile << "statement : expression_statement" << endl ;
		$$ = new SymbolInfo("","statement : expression_statement") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
	| compound_statement {
		logFile << "statement : compound_statement" <<endl ;
		$$ = new SymbolInfo("", "statement : compound_statement") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
	}
	| FOR LPAREN expression_statement expression_statement expression RPAREN statement {
		logFile << "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement" << endl ;
		$$ = new SymbolInfo("", "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->add_child($6) ;
		$$->add_child($7) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $7->end_line ;
	}
	| IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE {
		logFile << "statement : IF LPAREN expression RPAREN statement" <<endl ;
		$$ = new SymbolInfo("", "statement : IF LPAREN expression RPAREN statement") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $5->end_line ;
	}
	| IF LPAREN expression RPAREN statement ELSE statement {
		logFile << "statement : IF LPAREN expression RPAREN statement ELSE statement" << endl ;
		$$ = new SymbolInfo("","statement : IF LPAREN expression RPAREN statement ELSE statement") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->add_child($6) ;
		$$->add_child($7) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $7->end_line ;
	}
	| WHILE LPAREN expression RPAREN statement {
		logFile << "statement : WHILE LPAREN expression RPAREN statement" << endl ;
		$$ = new SymbolInfo("","statement : WHILE LPAREN expression RPAREN statement");
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->add_child($5) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $5->end_line ;
	}
	| PRINTLN LPAREN ID RPAREN SEMICOLON {
		logFile << "statement : PRINTLN LPAREN ID RPAREN SEMICOLON" << endl ;
		$$ = new SymbolInfo("","statement : PRINTLN LPAREN ID RPAREN SEMICOLON") ;
		if(!st.lookup($3->getName())){
			error <<"Line# "<<yylineno << ": Undeclared variable  '"+$3->getName()+"'" << endl ;
			err_cnt++ ;
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
	}
	;

expression_statement : SEMICOLON {
		logFile << "expression_statement : SEMICOLON" << endl ;
		$$ = new SymbolInfo("","expression_statement : SEMICOLON");
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
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

			////cout << $1->getName() << endl;
			//delete $1; // free ID SymbolInfo*
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
	| ID LSQUARE expression RSQUARE {
		//string code = $1->getName()+"["+$3->getName()+"]";
		logFile << "variable : ID LSQUARE expression RSQUARE" << endl ;
		SymbolInfo *info = st.lookup($1->getName());
		if(info != nullptr){
			$1->data_type = info->data_type ;
			if(!info->is_array()){
				error << "Line# "<<yylineno <<": '" <<$1->getName() <<"' is not an array" << endl ;
				err_cnt++ ;
			}
			// Generate an error message if the index of an array is not an integer
			if($3->data_type !="INT"){
				error << "Line# " <<yylineno <<": Array subscript is not an integer"<<endl;
				err_cnt++ ;
			}
			else if($3->data_type =="") {
				error << "Line# " <<yylineno <<": Array subscript cannot be empty"<<endl;
				err_cnt++ ;
			}
		}else{
			error <<"Line# " << yylineno << ": Undeclared variable '"+$1->getName()+"'" << endl ;
			err_cnt++ ;
		}
		$$ = new SymbolInfo("","variable : ID LSQUARE expression RSQUARE") ;

		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $4->end_line ;
		$$->info_type = ARRAY ;
		$$->setName($1->getName()) ;
		$$->setType("variable : ID LSQUARE expression RSQUARE") ;
		$$->rule = true ;
		$$->data_type = $3->data_type ;
		$$->var = VARIABLE ; 
		//cout << $$->data_type <<"HERE AT LINE ---------->792" << endl ;
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
		//$$->var = $1->var ;
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
		//cout <<"rel expression data_type : " << $1->data_type << endl;
	}	
	| rel_expression LOGICOP rel_expression {
		logFile << "logic_expression : rel_expression LOGICOP rel_expression" <<endl ;
		$$ = new SymbolInfo("","logic_expression : rel_expression LOGICOP rel_expression") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $3->end_line ;
		$$->data_type = "INT" ;
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
		//$$->var = $3->var ;
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
		//$$->var = $3->var ;
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
		//$$->var = $2->var ;
	}  
	| NOT unary_expression {
		logFile << "unary_expression : NOT unary_expression" << endl ;
		$$ = new SymbolInfo("", "unary_expression : NOT unary_expression") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
		$$->data_type = $2->data_type ;
		//$$->var = $2->var ;
	} 
	| factor {
		logFile << "unary_expression : factor" << endl ;
		$$ = new SymbolInfo("","unary_expression : factor") ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->setType("unary_expression : factor") ;
		//if($1->getName()!="")	//cout << $1->getName() <<"783568734587" << endl;
		$$->setName($1->getName()) ;
		$$->rule = true ;
		$$->data_type = $1->data_type ;
		$$->info_type = $1->info_type ;
		$$->var = $1->var ;
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
	}
	| ID LPAREN argument_list RPAREN {
		//string code = $1->getName() + "(" + toSymbolNameListStr($3) + ")" ;
		logFile << "factor : ID LPAREN argument_list RPAREN" << endl ;
		
		$$ = new SymbolInfo("","factor : ID LPAREN argument_list RPAREN") ;
		call_func($1,dec_list) ;

		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->add_child($3) ;
		$$->add_child($4) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $4->end_line ;

		$$->data_type = $1->data_type ;
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
	}
	| CONST_FLOAT { 
		logFile << "factor : CONST_FLOAT" << endl ;
		$$ = new SymbolInfo($1->getName(), "factor : CONST_FLOAT",true) ;
		$$->add_child($1) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $1->end_line ;
		$$->data_type = "FLOAT" ;
	}
	| variable INCOP {
		logFile << "factor : variable INCOP" << endl ;
		$$ = new SymbolInfo("","factor : variable INCOP") ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
		$$->data_type = $1->data_type ;
	}
	| variable DECOP {
		logFile << "factor : variable DECOP" << endl ;
		$$ = new SymbolInfo("", "factor : variable DECOP",true) ;
		$$->add_child($1) ;
		$$->add_child($2) ;
		$$->start_line = $1->start_line ;
		$$->end_line = $2->end_line ;
		$$->data_type = $1->data_type ;
		//$$->var = $1->var ;
	}
	| {
		$$ = new SymbolInfo("","factor : empty") ;
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
		//$$ = new vector<SymbolInfo*>();
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
		//cout <<$3->data_type <<"(&*^7825678243" << endl;

		//$$->push_back($3);
		dec_list.push_back($3) ;
		//$$->_parameter_list = dec_list ;
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
		//cout <<$1->data_type <<" efihsdfv" << endl;

		//$$ = new vector<SymbolInfo*>(); // init list
		//$$->push_back($1);
		dec_list.push_back($1) ;
		//$$->_parameter_list = dec_list ;
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

	logFile.open("log.txt");
	error.open("error.txt") ;
	parsefile.open("parser.txt") ;
	st.enterScope(++id,11) ;

	yyin= fin;
	yylineno = 1 ;
	yyparse() ;
	fclose(yyin);
	yylex_destroy() ;
	logFile.close();
	error.close() ;
	parsefile.close() ;



	return 0;
}

