/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "104.y"

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


#line 399 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    FOR = 260,                     /* FOR  */
    WHILE = 261,                   /* WHILE  */
    DO = 262,                      /* DO  */
    BREAK = 263,                   /* BREAK  */
    INT = 264,                     /* INT  */
    CHAR = 265,                    /* CHAR  */
    FLOAT = 266,                   /* FLOAT  */
    DOUBLE = 267,                  /* DOUBLE  */
    VOID = 268,                    /* VOID  */
    RETURN = 269,                  /* RETURN  */
    SWITCH = 270,                  /* SWITCH  */
    CASE = 271,                    /* CASE  */
    DEFAULT = 272,                 /* DEFAULT  */
    CONTINUE = 273,                /* CONTINUE  */
    PRINTLN = 274,                 /* PRINTLN  */
    ADDOP = 275,                   /* ADDOP  */
    MULOP = 276,                   /* MULOP  */
    RELOP = 277,                   /* RELOP  */
    LOGICOP = 278,                 /* LOGICOP  */
    INCOP = 279,                   /* INCOP  */
    DECOP = 280,                   /* DECOP  */
    ASSIGNOP = 281,                /* ASSIGNOP  */
    NOT = 282,                     /* NOT  */
    LPAREN = 283,                  /* LPAREN  */
    RPAREN = 284,                  /* RPAREN  */
    LCURL = 285,                   /* LCURL  */
    RCURL = 286,                   /* RCURL  */
    LSQUARE = 287,                 /* LSQUARE  */
    RSQUARE = 288,                 /* RSQUARE  */
    COMMA = 289,                   /* COMMA  */
    SEMICOLON = 290,               /* SEMICOLON  */
    CONST_INT = 291,               /* CONST_INT  */
    CONST_FLOAT = 292,             /* CONST_FLOAT  */
    CONST_CHAR = 293,              /* CONST_CHAR  */
    ID = 294,                      /* ID  */
    LOWER_THAN_ELSE = 295          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define FOR 260
#define WHILE 261
#define DO 262
#define BREAK 263
#define INT 264
#define CHAR 265
#define FLOAT 266
#define DOUBLE 267
#define VOID 268
#define RETURN 269
#define SWITCH 270
#define CASE 271
#define DEFAULT 272
#define CONTINUE 273
#define PRINTLN 274
#define ADDOP 275
#define MULOP 276
#define RELOP 277
#define LOGICOP 278
#define INCOP 279
#define DECOP 280
#define ASSIGNOP 281
#define NOT 282
#define LPAREN 283
#define RPAREN 284
#define LCURL 285
#define RCURL 286
#define LSQUARE 287
#define RSQUARE 288
#define COMMA 289
#define SEMICOLON 290
#define CONST_INT 291
#define CONST_FLOAT 292
#define CONST_CHAR 293
#define ID 294
#define LOWER_THAN_ELSE 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 329 "104.y"

	SymbolInfo* symbol_info; 
	std::string* str_info;

#line 537 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_FOR = 5,                        /* FOR  */
  YYSYMBOL_WHILE = 6,                      /* WHILE  */
  YYSYMBOL_DO = 7,                         /* DO  */
  YYSYMBOL_BREAK = 8,                      /* BREAK  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_CHAR = 10,                      /* CHAR  */
  YYSYMBOL_FLOAT = 11,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 12,                    /* DOUBLE  */
  YYSYMBOL_VOID = 13,                      /* VOID  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_SWITCH = 15,                    /* SWITCH  */
  YYSYMBOL_CASE = 16,                      /* CASE  */
  YYSYMBOL_DEFAULT = 17,                   /* DEFAULT  */
  YYSYMBOL_CONTINUE = 18,                  /* CONTINUE  */
  YYSYMBOL_PRINTLN = 19,                   /* PRINTLN  */
  YYSYMBOL_ADDOP = 20,                     /* ADDOP  */
  YYSYMBOL_MULOP = 21,                     /* MULOP  */
  YYSYMBOL_RELOP = 22,                     /* RELOP  */
  YYSYMBOL_LOGICOP = 23,                   /* LOGICOP  */
  YYSYMBOL_INCOP = 24,                     /* INCOP  */
  YYSYMBOL_DECOP = 25,                     /* DECOP  */
  YYSYMBOL_ASSIGNOP = 26,                  /* ASSIGNOP  */
  YYSYMBOL_NOT = 27,                       /* NOT  */
  YYSYMBOL_LPAREN = 28,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 29,                    /* RPAREN  */
  YYSYMBOL_LCURL = 30,                     /* LCURL  */
  YYSYMBOL_RCURL = 31,                     /* RCURL  */
  YYSYMBOL_LSQUARE = 32,                   /* LSQUARE  */
  YYSYMBOL_RSQUARE = 33,                   /* RSQUARE  */
  YYSYMBOL_COMMA = 34,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 35,                 /* SEMICOLON  */
  YYSYMBOL_CONST_INT = 36,                 /* CONST_INT  */
  YYSYMBOL_CONST_FLOAT = 37,               /* CONST_FLOAT  */
  YYSYMBOL_CONST_CHAR = 38,                /* CONST_CHAR  */
  YYSYMBOL_ID = 39,                        /* ID  */
  YYSYMBOL_LOWER_THAN_ELSE = 40,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_start = 42,                     /* start  */
  YYSYMBOL_program = 43,                   /* program  */
  YYSYMBOL_unit = 44,                      /* unit  */
  YYSYMBOL_func_declaration = 45,          /* func_declaration  */
  YYSYMBOL_func_definition = 46,           /* func_definition  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_48_2 = 48,                      /* $@2  */
  YYSYMBOL_parameter_list = 49,            /* parameter_list  */
  YYSYMBOL_compound_statement = 50,        /* compound_statement  */
  YYSYMBOL_51_3 = 51,                      /* $@3  */
  YYSYMBOL_52_4 = 52,                      /* $@4  */
  YYSYMBOL_var_declaration = 53,           /* var_declaration  */
  YYSYMBOL_type_specifier = 54,            /* type_specifier  */
  YYSYMBOL_declaration_list = 55,          /* declaration_list  */
  YYSYMBOL_statements = 56,                /* statements  */
  YYSYMBOL_statement = 57,                 /* statement  */
  YYSYMBOL_expression_statement = 58,      /* expression_statement  */
  YYSYMBOL_variable = 59,                  /* variable  */
  YYSYMBOL_expression = 60,                /* expression  */
  YYSYMBOL_logic_expression = 61,          /* logic_expression  */
  YYSYMBOL_rel_expression = 62,            /* rel_expression  */
  YYSYMBOL_simple_expression = 63,         /* simple_expression  */
  YYSYMBOL_term = 64,                      /* term  */
  YYSYMBOL_unary_expression = 65,          /* unary_expression  */
  YYSYMBOL_factor = 66,                    /* factor  */
  YYSYMBOL_argument_list = 67,             /* argument_list  */
  YYSYMBOL_arguments = 68                  /* arguments  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   145

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  122

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   352,   352,   370,   378,   386,   393,   400,   408,   422,
     437,   437,   450,   450,   463,   480,   494,   509,   523,   523,
     535,   535,   546,   559,   567,   576,   585,   596,   614,   623,
     638,   645,   654,   661,   668,   675,   688,   699,   712,   723,
     738,   749,   756,   765,   797,   837,   850,   874,   885,   896,
     906,   924,   934,   969,   979,  1029,  1039,  1049,  1064,  1077,
    1093,  1104,  1112,  1120,  1129,  1139,  1145,  1154,  1163,  1182
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE", "FOR",
  "WHILE", "DO", "BREAK", "INT", "CHAR", "FLOAT", "DOUBLE", "VOID",
  "RETURN", "SWITCH", "CASE", "DEFAULT", "CONTINUE", "PRINTLN", "ADDOP",
  "MULOP", "RELOP", "LOGICOP", "INCOP", "DECOP", "ASSIGNOP", "NOT",
  "LPAREN", "RPAREN", "LCURL", "RCURL", "LSQUARE", "RSQUARE", "COMMA",
  "SEMICOLON", "CONST_INT", "CONST_FLOAT", "CONST_CHAR", "ID",
  "LOWER_THAN_ELSE", "$accept", "start", "program", "unit",
  "func_declaration", "func_definition", "$@1", "$@2", "parameter_list",
  "compound_statement", "$@3", "$@4", "var_declaration", "type_specifier",
  "declaration_list", "statements", "statement", "expression_statement",
  "variable", "expression", "logic_expression", "rel_expression",
  "simple_expression", "term", "unary_expression", "factor",
  "argument_list", "arguments", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
};
#endif

#define YYPACT_NINF (-73)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-21)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      36,   -73,   -73,   -73,     4,    36,   -73,   -73,   -73,   -73,
     -24,   -73,   -73,    20,    24,     1,   -17,    -3,   -73,   -10,
     -13,     7,    22,    28,   -73,    33,    15,    36,   -73,   -73,
      39,    38,   -73,   -73,    33,    34,    44,   100,    41,   -73,
     -73,   -73,    52,    53,    54,    71,    55,    71,    71,    71,
     -73,   -73,   -73,    25,   -73,   -73,    47,    65,   -73,   -73,
       9,    59,   -73,    64,     6,    67,   -73,   -73,   -73,    71,
     105,    71,    62,    50,    37,   -73,   -73,    61,    71,    71,
      80,   -73,   -73,   -73,   -73,    71,   -73,    71,    71,    71,
      71,    86,   105,    87,   -73,    88,   -73,   -73,    89,    90,
      93,   -73,   -73,    67,   101,   -73,   100,    71,   100,    94,
     -73,    71,   -73,   118,   102,   -73,   -73,   -73,   100,   100,
     -73,   -73
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    23,    24,    25,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    28,     0,     0,     0,     0,    22,    12,
       0,    17,     0,    26,     9,     0,    10,     0,    16,    29,
       0,    18,    13,     8,     0,    15,     0,    65,     0,    11,
      14,    27,     0,     0,     0,    65,     0,    65,    65,    65,
      41,    61,    62,    43,    34,    32,     0,    65,    30,    33,
      58,     0,    45,    47,    49,    51,    53,    57,    21,    65,
      65,    65,     0,     0,    58,    55,    56,     0,    65,    65,
      28,    19,    31,    63,    64,    65,    42,    65,    65,    65,
      65,     0,    65,     0,    40,     0,    60,    69,     0,    66,
       0,    46,    48,    52,    50,    54,    65,    65,    65,     0,
      59,    65,    44,    36,     0,    38,    39,    68,    65,    65,
      37,    35
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -73,   -73,   -73,   129,   -73,   -73,   -73,   -73,   -73,   -14,
     -73,   -73,    18,    17,   -73,   -73,   -52,   -68,   -47,   -42,
     -72,    51,    56,    35,   -39,   -73,   -73,   -73
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,     7,     8,    34,    25,    20,    54,
      37,    38,    55,    56,    14,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    98,    99
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      74,    74,    92,    72,    11,    82,    97,    77,    75,    76,
       1,    32,     2,   101,     3,    13,    26,    10,     9,    22,
      39,    27,    10,     9,   107,    24,    88,    91,    89,    93,
      19,    74,    21,    83,    84,    85,    23,   100,    74,   117,
      74,    74,    74,    74,    35,     1,    28,     2,    15,     3,
      33,   105,    16,    78,   113,    29,   115,    79,    17,    18,
      30,    83,    84,    31,    74,   114,   120,   121,    42,   -20,
      43,    44,    68,    40,     1,    36,     2,    41,     3,    45,
      69,    70,    71,    73,    46,    47,    80,    87,    90,    95,
      96,    47,    48,    49,    86,    31,    81,    94,    48,    49,
      50,    51,    52,    42,    53,    43,    44,    51,    52,     1,
      53,     2,    16,     3,    45,   106,   108,   109,   110,    46,
      47,    88,   118,   103,   111,    47,   112,    48,    49,   116,
      31,   119,    48,    49,    12,    50,    51,    52,   102,    53,
      50,    51,    52,     0,    53,   104
};

static const yytype_int8 yycheck[] =
{
      47,    48,    70,    45,     0,    57,    78,    49,    47,    48,
       9,    25,    11,    85,    13,    39,    29,     0,     0,    36,
      34,    34,     5,     5,    92,    35,    20,    69,    22,    71,
      29,    78,    15,    24,    25,    26,    39,    79,    85,   111,
      87,    88,    89,    90,    27,     9,    39,    11,    28,    13,
      35,    90,    32,    28,   106,    33,   108,    32,    34,    35,
      32,    24,    25,    30,   111,   107,   118,   119,     3,    31,
       5,     6,    31,    39,     9,    36,    11,    33,    13,    14,
      28,    28,    28,    28,    19,    20,    39,    23,    21,    39,
      29,    20,    27,    28,    35,    30,    31,    35,    27,    28,
      35,    36,    37,     3,    39,     5,     6,    36,    37,     9,
      39,    11,    32,    13,    14,    29,    29,    29,    29,    19,
      20,    20,     4,    88,    34,    20,    33,    27,    28,    35,
      30,    29,    27,    28,     5,    35,    36,    37,    87,    39,
      35,    36,    37,    -1,    39,    89
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    11,    13,    42,    43,    44,    45,    46,    53,
      54,     0,    44,    39,    55,    28,    32,    34,    35,    29,
      49,    54,    36,    39,    35,    48,    29,    34,    39,    33,
      32,    30,    50,    35,    47,    54,    36,    51,    52,    50,
      39,    33,     3,     5,     6,    14,    19,    20,    27,    28,
      35,    36,    37,    39,    50,    53,    54,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    31,    28,
      28,    28,    60,    28,    59,    65,    65,    60,    28,    32,
      39,    31,    57,    24,    25,    26,    35,    23,    20,    22,
      21,    60,    58,    60,    35,    39,    29,    61,    67,    68,
      60,    61,    62,    64,    63,    65,    29,    58,    29,    29,
      29,    34,    33,    57,    60,    57,    35,    61,     4,    29,
      57,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    44,    45,    45,
      47,    46,    48,    46,    49,    49,    49,    49,    51,    50,
      52,    50,    53,    54,    54,    54,    55,    55,    55,    55,
      56,    56,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    64,    64,    65,    65,    65,    66,    66,
      66,    66,    66,    66,    66,    66,    67,    67,    68,    68
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       0,     7,     0,     6,     4,     3,     2,     1,     0,     4,
       0,     3,     3,     1,     1,     1,     3,     6,     1,     4,
       1,     2,     1,     1,     1,     7,     5,     7,     5,     5,
       3,     1,     2,     1,     4,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     2,     2,     1,     1,     4,
       3,     1,     1,     2,     2,     0,     1,     0,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: program  */
#line 352 "104.y"
                { 
		(yyval.symbol_info) = new SymbolInfo("","start : program") ;
		logFile << "start : program" << endl;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		DFS((yyval.symbol_info),"") ;
		//delete $$ ;
		
		st.printAllScope(logFile); 
        st.exitScope();

		//dfs_delete($$) ;	
		cout << countr << endl;
		cout << "Total Lines: " << yylineno  << endl;
		cout << "Total Errors: " << err_cnt << endl;
	}
#line 1696 "y.tab.c"
    break;

  case 3: /* program: program unit  */
#line 370 "104.y"
                       { 
		(yyval.symbol_info) = new SymbolInfo("","program : program unit") ;
		logFile << "program : program unit"<< endl ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-1].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 1709 "y.tab.c"
    break;

  case 4: /* program: unit  */
#line 378 "104.y"
               {
		(yyval.symbol_info) = new SymbolInfo("","program : unit") ;
		logFile << "program : unit" << endl ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 1721 "y.tab.c"
    break;

  case 5: /* unit: var_declaration  */
#line 386 "104.y"
                       {
		(yyval.symbol_info) = new SymbolInfo("", "unit : var_declaration") ;
		logFile << "unit : var_declaration" << endl ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 1733 "y.tab.c"
    break;

  case 6: /* unit: func_declaration  */
#line 393 "104.y"
                       {
		(yyval.symbol_info) = new SymbolInfo("", "unit : func_declaration") ;
		logFile << "unit : func_declaration" << endl ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 1745 "y.tab.c"
    break;

  case 7: /* unit: func_definition  */
#line 400 "104.y"
                      {
		(yyval.symbol_info) = new SymbolInfo("", "unit : func_definition") ;
		logFile << "unit : func_definition" << endl ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 1757 "y.tab.c"
    break;

  case 8: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON  */
#line 408 "104.y"
                                                                            {
		(yyval.symbol_info) = new SymbolInfo("", "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON") ;
		logFile << "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON" << endl ;
		func_declare((yyvsp[-4].symbol_info)->getName(), (yyvsp[-5].symbol_info)->data_type, dec_list);
		(yyvsp[-4].symbol_info)->info_type = FUNCTION_DECLARATION ;
		(yyval.symbol_info)->add_child((yyvsp[-5].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-5].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 1776 "y.tab.c"
    break;

  case 9: /* func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON  */
#line 422 "104.y"
                                                    {
		(yyval.symbol_info) = new SymbolInfo("","func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON") ;
		logFile << "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON" << endl ;
		func_declare((yyvsp[-3].symbol_info)->getName(), (yyvsp[-4].symbol_info)->data_type,dec_list);
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-4].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		//delete $1; 
        //delete $2;
	}
#line 1795 "y.tab.c"
    break;

  case 10: /* $@1: %empty  */
#line 437 "104.y"
                                                                 {func_def((yyvsp[-3].symbol_info)->getName(), (yyvsp[-4].symbol_info)->data_type, dec_list,yylineno);}
#line 1801 "y.tab.c"
    break;

  case 11: /* func_definition: type_specifier ID LPAREN parameter_list RPAREN $@1 compound_statement  */
#line 437 "104.y"
                                                                                                                                                 {
		(yyval.symbol_info) = new SymbolInfo("", "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement") ;	
		logFile << "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement" <<endl ;
        (yyval.symbol_info)->add_child((yyvsp[-6].symbol_info));
        (yyval.symbol_info)->add_child((yyvsp[-5].symbol_info));
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info));
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info));
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		//cout << $7 << endl;
		(yyval.symbol_info)->start_line = (yyvsp[-6].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 1819 "y.tab.c"
    break;

  case 12: /* $@2: %empty  */
#line 450 "104.y"
                                          {func_def((yyvsp[-2].symbol_info)->getName(), (yyvsp[-3].symbol_info)->data_type,dec_list,yylineno );}
#line 1825 "y.tab.c"
    break;

  case 13: /* func_definition: type_specifier ID LPAREN RPAREN $@2 compound_statement  */
#line 450 "104.y"
                                                                                                                          {
		(yyval.symbol_info) = new SymbolInfo("","func_definition : type_specifier ID LPAREN RPAREN compound_statement") ;
		logFile << "func_definition : type_specifier ID LPAREN RPAREN compound_statement" << endl ;
		(yyval.symbol_info)->add_child((yyvsp[-5].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-5].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

	}
#line 1842 "y.tab.c"
    break;

  case 14: /* parameter_list: parameter_list COMMA type_specifier ID  */
#line 463 "104.y"
                                                         {
		(yyval.symbol_info) = new SymbolInfo("" ,"parameter_list : parameter_list COMMA type_specifier ID") ;
		logFile << "parameter_list : parameter_list COMMA type_specifier ID" <<endl ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-3].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		SymbolInfo* info = new SymbolInfo((yyvsp[0].symbol_info)->getName(),"ID") ;
		info->data_type = (yyvsp[-1].symbol_info)->data_type ;
		info->info_type = (yyvsp[0].symbol_info)->info_type ;
		dec_list.push_back(info) ;
		//$$->_parameter_list = dec_list ; 
		pline = yylineno;
	}
#line 1864 "y.tab.c"
    break;

  case 15: /* parameter_list: parameter_list COMMA type_specifier  */
#line 480 "104.y"
                                              {
		(yyval.symbol_info) = new SymbolInfo("","parameter_list : parameter_list COMMA type_specifier") ;
		logFile << "parameter_list : parameter_list COMMA type_specifier" <<endl ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		SymbolInfo* symbol = new SymbolInfo((yyvsp[0].symbol_info)->getType(),"ID") ;
        symbol->data_type = (yyvsp[0].symbol_info)->data_type ;
		//$$->_parameter_list = dec_list ; 
		pline = yylineno;
		dec_list.push_back(symbol) ;
	}
#line 1883 "y.tab.c"
    break;

  case 16: /* parameter_list: type_specifier ID  */
#line 494 "104.y"
                            {
		(yyval.symbol_info) = new SymbolInfo("","parameter_list : type_specifier ID") ;
		logFile <<"parameter_list : type_specifier ID" <<endl ;
        (yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-1].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		SymbolInfo* symbol = new SymbolInfo((yyvsp[0].symbol_info)->getName(),"ID") ;
        symbol->data_type = (yyvsp[-1].symbol_info)->data_type ;
		symbol->info_type = (yyvsp[0].symbol_info)->info_type ;
		dec_list.push_back(symbol) ;
		//$$->_parameter_list = dec_list ;
		pline = yylineno;
	}
#line 1903 "y.tab.c"
    break;

  case 17: /* parameter_list: type_specifier  */
#line 509 "104.y"
                         {
		(yyval.symbol_info) = new SymbolInfo("", "parameter_list : type_specifier") ;
		logFile << "parameter_list : type_specifier" << endl ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		string s = "" ;
        SymbolInfo* symbol = new SymbolInfo(s,(yyvsp[0].symbol_info)->getType()) ;
        symbol->data_type = (yyvsp[0].symbol_info)->data_type ;
		dec_list.push_back(symbol) ;
		//$$->_parameter_list = dec_list ;
	}
#line 1921 "y.tab.c"
    break;

  case 18: /* $@3: %empty  */
#line 523 "104.y"
                           {st.enterScope(++id , 11);dec_func_param_list(dec_list, pline);}
#line 1927 "y.tab.c"
    break;

  case 19: /* compound_statement: LCURL $@3 statements RCURL  */
#line 523 "104.y"
                                                                                                             {
		logFile << "compound_statement : LCURL statements RCURL" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("","compound_statement : LCURL statements RCURL");
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;

		(yyval.symbol_info)->start_line = (yyvsp[-3].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		st.printAllScope(logFile);
        st.exitScope();
	}
#line 1944 "y.tab.c"
    break;

  case 20: /* $@4: %empty  */
#line 535 "104.y"
                {st.enterScope(++id,11);}
#line 1950 "y.tab.c"
    break;

  case 21: /* compound_statement: LCURL $@4 RCURL  */
#line 535 "104.y"
                                                {
		logFile << "compound_statement : LCURL RCURL" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("","compound_statement : LCURL RCURL") ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		st.printAllScope(logFile) ;
		st.exitScope();
	}
#line 1965 "y.tab.c"
    break;

  case 22: /* var_declaration: type_specifier declaration_list SEMICOLON  */
#line 546 "104.y"
                                                            {
		logFile << "var_declaration : type_specifier declaration_list SEMICOLON" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("", "var_declaration : type_specifier declaration_list SEMICOLON") ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->_parameter_list = dec_list ;
        dec_var((yyvsp[-2].symbol_info)->data_type,dec_list) ;
        dec_list.clear() ;
	}
#line 1982 "y.tab.c"
    break;

  case 23: /* type_specifier: INT  */
#line 559 "104.y"
                     {
		logFile << "type_specifier : INT" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","type_specifier : INT") ;
		(yyval.symbol_info)->data_type = "INT" ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 1995 "y.tab.c"
    break;

  case 24: /* type_specifier: FLOAT  */
#line 567 "104.y"
                {
		logFile << "type_specifier : FLOAT" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","type_specifier : FLOAT") ;
		(yyval.symbol_info)->data_type = "FLOAT" ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		
	}
#line 2009 "y.tab.c"
    break;

  case 25: /* type_specifier: VOID  */
#line 576 "104.y"
               {
		logFile << "type_specifier : VOID" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("","type_specifier : VOID") ;
		(yyval.symbol_info)->data_type = "VOID" ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2022 "y.tab.c"
    break;

  case 26: /* declaration_list: declaration_list COMMA ID  */
#line 585 "104.y"
                                             {
		logFile << "declaration_list : declaration_list COMMA ID" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","declaration_list : declaration_list COMMA ID");
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		dec_list.push_back((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->_parameter_list = dec_list ;
	}
#line 2038 "y.tab.c"
    break;

  case 27: /* declaration_list: declaration_list COMMA ID LSQUARE CONST_INT RSQUARE  */
#line 596 "104.y"
                                                              {
		(yyval.symbol_info) = new SymbolInfo("", "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE") ;
		string code = (yyvsp[-3].symbol_info)->getName()+"["+(yyvsp[-1].symbol_info)->getName()+"]";
		(yyvsp[-3].symbol_info)->array_size = (yyvsp[-1].symbol_info)->getName() ;
		(yyvsp[-3].symbol_info)->info_type = ARRAY ;
		dec_list.push_back((yyvsp[-3].symbol_info)) ;
		logFile << "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE" << endl ;
		(yyval.symbol_info)->add_child((yyvsp[-5].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-5].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		(yyval.symbol_info)->_parameter_list = dec_list ;
	}
#line 2061 "y.tab.c"
    break;

  case 28: /* declaration_list: ID  */
#line 614 "104.y"
             {
		logFile << "declaration_list : ID" << endl ;
		(yyval.symbol_info) = new SymbolInfo("", "declaration_list : ID") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		dec_list.push_back((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->_parameter_list = dec_list ;
	}
#line 2075 "y.tab.c"
    break;

  case 29: /* declaration_list: ID LSQUARE CONST_INT RSQUARE  */
#line 623 "104.y"
                                       {
		(yyval.symbol_info) = new SymbolInfo("","declaration_list : ID LSQUARE CONST_INT RSQUARE") ;
		logFile << "declaration_list : ID LSQUARE CONST_INT RSQUARE" << endl ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-3].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyvsp[-3].symbol_info)->array_size = (yyvsp[-1].symbol_info)->getName();
		(yyvsp[-3].symbol_info)->info_type = ARRAY ;
		dec_list.push_back((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->_parameter_list = dec_list ;
	}
#line 2094 "y.tab.c"
    break;

  case 30: /* statements: statement  */
#line 638 "104.y"
                       {
		logFile << "statements : statement" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","statements : statement") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2106 "y.tab.c"
    break;

  case 31: /* statements: statements statement  */
#line 645 "104.y"
                               {
		logFile << "statements : statements statement" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","statements : statements statement") ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-1].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2119 "y.tab.c"
    break;

  case 32: /* statement: var_declaration  */
#line 654 "104.y"
                            {
		logFile << "statement : var_declaration" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","statement : var_declaration") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2131 "y.tab.c"
    break;

  case 33: /* statement: expression_statement  */
#line 661 "104.y"
                               {
		logFile << "statement : expression_statement" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","statement : expression_statement") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2143 "y.tab.c"
    break;

  case 34: /* statement: compound_statement  */
#line 668 "104.y"
                             {
		logFile << "statement : compound_statement" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("", "statement : compound_statement") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2155 "y.tab.c"
    break;

  case 35: /* statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement  */
#line 675 "104.y"
                                                                                           {
		logFile << "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement" << endl ;
		(yyval.symbol_info) = new SymbolInfo("", "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement") ;
		(yyval.symbol_info)->add_child((yyvsp[-6].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-5].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-6].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2173 "y.tab.c"
    break;

  case 36: /* statement: IF LPAREN expression RPAREN statement  */
#line 688 "104.y"
                                                                      {
		logFile << "statement : IF LPAREN expression RPAREN statement" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("", "statement : IF LPAREN expression RPAREN statement") ;
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-4].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2189 "y.tab.c"
    break;

  case 37: /* statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 699 "104.y"
                                                               {
		logFile << "statement : IF LPAREN expression RPAREN statement ELSE statement" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","statement : IF LPAREN expression RPAREN statement ELSE statement") ;
		(yyval.symbol_info)->add_child((yyvsp[-6].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-5].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-6].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2207 "y.tab.c"
    break;

  case 38: /* statement: WHILE LPAREN expression RPAREN statement  */
#line 712 "104.y"
                                                   {
		logFile << "statement : WHILE LPAREN expression RPAREN statement" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","statement : WHILE LPAREN expression RPAREN statement");
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-4].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2223 "y.tab.c"
    break;

  case 39: /* statement: PRINTLN LPAREN ID RPAREN SEMICOLON  */
#line 723 "104.y"
                                             {
		logFile << "statement : PRINTLN LPAREN ID RPAREN SEMICOLON" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","statement : PRINTLN LPAREN ID RPAREN SEMICOLON") ;
		if(!st.lookup((yyvsp[-2].symbol_info)->getName())){
			error <<"Line# "<<yylineno << ": Undeclared variable  '"+(yyvsp[-2].symbol_info)->getName()+"'" << endl ;
			err_cnt++ ;
		}
		(yyval.symbol_info)->add_child((yyvsp[-4].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-4].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2243 "y.tab.c"
    break;

  case 40: /* statement: RETURN expression SEMICOLON  */
#line 738 "104.y"
                                      {
		logFile << "statement : RETURN expression SEMICOLON" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("","statement : RETURN expression SEMICOLON") ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2257 "y.tab.c"
    break;

  case 41: /* expression_statement: SEMICOLON  */
#line 749 "104.y"
                                 {
		logFile << "expression_statement : SEMICOLON" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","expression_statement : SEMICOLON");
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2269 "y.tab.c"
    break;

  case 42: /* expression_statement: expression SEMICOLON  */
#line 756 "104.y"
                               {
		logFile << "expression_statement : expression SEMICOLON" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","expression_statement : expression SEMICOLON");
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-1].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2282 "y.tab.c"
    break;

  case 43: /* variable: ID  */
#line 765 "104.y"
              { 
		logFile << "variable : ID" << endl ;
		SymbolInfo *info = st.lookup((yyvsp[0].symbol_info)->getName());
		(yyval.symbol_info) = new SymbolInfo("","variable : ID",true) ;
		if(info){
			if(info->is_array()){
				//error << "Line# " << yylineno <<": Type mismatch, "+info->getName()+" is an array" << endl;
				//err_cnt++ ;
			}
			(yyval.symbol_info)->setName(info->getName()) ;
			(yyval.symbol_info)->info_type = info->info_type ;
			(yyval.symbol_info)->data_type = info->data_type ;
			(yyval.symbol_info)->array_size = info->array_size ;
			(yyval.symbol_info)->position = info->position ;
			(yyval.symbol_info)->_parameter_list = info->_parameter_list ;
			(yyval.symbol_info)->var = info->info_type ;

			////cout << $1->getName() << endl;
			//delete $1; // free ID SymbolInfo*
		}else{
			error << "Line# " <<yylineno << ": Undeclared variable '"+(yyvsp[0].symbol_info)->getName()+"'" << endl;
			err_cnt++ ;
			(yyval.symbol_info)->setName((yyvsp[0].symbol_info)->getName()) ;
			(yyval.symbol_info)->setType("variable : ID") ;
			(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
			(yyval.symbol_info)->var = VARIABLE ;
			
		}
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
	}
#line 2319 "y.tab.c"
    break;

  case 44: /* variable: ID LSQUARE expression RSQUARE  */
#line 797 "104.y"
                                        {
		//string code = $1->getName()+"["+$3->getName()+"]";
		logFile << "variable : ID LSQUARE expression RSQUARE" << endl ;
		SymbolInfo *info = st.lookup((yyvsp[-3].symbol_info)->getName());
		if(info != nullptr){
			(yyvsp[-3].symbol_info)->data_type = info->data_type ;
			if(!info->is_array()){
				error << "Line# "<<yylineno <<": '" <<(yyvsp[-3].symbol_info)->getName() <<"' is not an array" << endl ;
				err_cnt++ ;
			}
			// Generate an error message if the index of an array is not an integer
			if((yyvsp[-1].symbol_info)->data_type !="INT"){
				error << "Line# " <<yylineno <<": Array subscript is not an integer"<<endl;
				err_cnt++ ;
			}
			else if((yyvsp[-1].symbol_info)->data_type =="") {
				error << "Line# " <<yylineno <<": Array subscript cannot be empty"<<endl;
				err_cnt++ ;
			}
		}else{
			error <<"Line# " << yylineno << ": Undeclared variable '"+(yyvsp[-3].symbol_info)->getName()+"'" << endl ;
			err_cnt++ ;
		}
		(yyval.symbol_info) = new SymbolInfo("","variable : ID LSQUARE expression RSQUARE") ;

		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-3].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->info_type = ARRAY ;
		(yyval.symbol_info)->setName((yyvsp[-3].symbol_info)->getName()) ;
		(yyval.symbol_info)->setType("variable : ID LSQUARE expression RSQUARE") ;
		(yyval.symbol_info)->rule = true ;
		(yyval.symbol_info)->data_type = (yyvsp[-1].symbol_info)->data_type ;
		(yyval.symbol_info)->var = VARIABLE ; 
		//cout << $$->data_type <<"HERE AT LINE ---------->792" << endl ;
	}
#line 2363 "y.tab.c"
    break;

  case 45: /* expression: logic_expression  */
#line 837 "104.y"
                              {
		logFile << "expression : logic_expression" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","expression : logic_expression") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		(yyval.symbol_info)->setName((yyvsp[0].symbol_info)->getName()) ;
		(yyval.symbol_info)->rule = true ;
		(yyval.symbol_info)->setType("expression : logic_expression") ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->var = (yyvsp[0].symbol_info)->var ;
	}
#line 2381 "y.tab.c"
    break;

  case 46: /* expression: variable ASSIGNOP logic_expression  */
#line 850 "104.y"
                                             {
		logFile << "expression : variable ASSIGNOP logic_expression" << endl ;
		SymbolInfo *info = st.lookup((yyvsp[-2].symbol_info)->getName());
		if(info!=nullptr){
			if(allCapital(info->data_type) == "INT" && allCapital((yyvsp[0].symbol_info)->data_type)=="FLOAT"){
				error << "Line# " <<yylineno<<": Warning: possible loss of data in assignment of FLOAT to INT" << endl ;
				err_cnt++ ;
			}
		}
		if(allCapital((yyvsp[0].symbol_info)->data_type)=="VOID"){
				error << "Line# "<<yylineno <<": Void cannot be used in expression" << endl ;
				err_cnt++ ;
		}
		(yyval.symbol_info) = new SymbolInfo("", "expression : variable ASSIGNOP logic_expression");
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		(yyval.symbol_info)->data_type = (yyvsp[-2].symbol_info)->data_type ;
		//$$->var = $1->var ;
	}
#line 2409 "y.tab.c"
    break;

  case 47: /* logic_expression: rel_expression  */
#line 874 "104.y"
                                  { 
		logFile << "logic_expression : rel_expression" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","logic_expression : rel_expression") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->info_type = (yyvsp[0].symbol_info)->info_type ;
		(yyval.symbol_info)->var = (yyvsp[0].symbol_info)->var ;
		//cout <<"rel expression data_type : " << $1->data_type << endl;
	}
#line 2425 "y.tab.c"
    break;

  case 48: /* logic_expression: rel_expression LOGICOP rel_expression  */
#line 885 "104.y"
                                                {
		logFile << "logic_expression : rel_expression LOGICOP rel_expression" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("","logic_expression : rel_expression LOGICOP rel_expression") ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = "INT" ;
	}
#line 2440 "y.tab.c"
    break;

  case 49: /* rel_expression: simple_expression  */
#line 896 "104.y"
                                   {
		logFile << "rel_expression : simple_expression"<< endl ;
		(yyval.symbol_info) = new SymbolInfo("","rel_expression : simple_expression") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->info_type = (yyvsp[0].symbol_info)->info_type ;
		(yyval.symbol_info)->var = (yyvsp[0].symbol_info)->var ;
	}
#line 2455 "y.tab.c"
    break;

  case 50: /* rel_expression: simple_expression RELOP simple_expression  */
#line 906 "104.y"
                                                        {
		logFile << "rel_expression : simple_expression RELOP simple_expression" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("","rel_expression : simple_expression RELOP simple_expression") ;
		
		if((yyvsp[-2].symbol_info)->data_type == "INT" && (yyvsp[0].symbol_info)->data_type == "FLOAT"){
			(yyvsp[-2].symbol_info)->data_type = "FLOAT" ;
		}
		else if((yyvsp[-2].symbol_info)->data_type == "FLOAT" && (yyvsp[0].symbol_info)->data_type == "INT"){
			(yyvsp[0].symbol_info)->data_type = "FLOAT" ;
		}
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = "INT" ;
	}
#line 2477 "y.tab.c"
    break;

  case 51: /* simple_expression: term  */
#line 924 "104.y"
                         {
		logFile << "simple_expression : term" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","simple_expression : term") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->info_type = (yyvsp[0].symbol_info)->info_type ;
		(yyval.symbol_info)->var = (yyvsp[0].symbol_info)->var ;
	}
#line 2492 "y.tab.c"
    break;

  case 52: /* simple_expression: simple_expression ADDOP term  */
#line 934 "104.y"
                                       {
		logFile << "simple_expression : simple_expression ADDOP term" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","simple_expression : simple_expression ADDOP term") ;
		if(allCapital((yyvsp[-2].symbol_info)->data_type) == "VOID" || allCapital((yyvsp[0].symbol_info)->data_type) == "VOID") {
			error << "Line# " << yylineno << ": Void cannot be used in expression" <<endl ;
			err_cnt++ ;
		}
		string TYPE ="" ;
		if((yyvsp[-2].symbol_info)->data_type == (yyvsp[0].symbol_info)->data_type) {
			TYPE = (yyvsp[-2].symbol_info)->data_type ;
		}
		else if((yyvsp[-2].symbol_info)->data_type == "INT" && (yyvsp[0].symbol_info)->data_type == "FLOAT"){
			(yyvsp[-2].symbol_info)->data_type = "FLOAT" ;
			TYPE = "FLOAT" ;
		}
		else if((yyvsp[-2].symbol_info)->data_type == "FLOAT" && (yyvsp[0].symbol_info)->data_type == "INT"){
			(yyvsp[0].symbol_info)->data_type = "FLOAT" ;
			TYPE = "FLOAT" ;
		}
		else if((yyvsp[-2].symbol_info)->data_type != "VOID") {
			TYPE = (yyvsp[-2].symbol_info)->data_type ;
		}
		else {
			TYPE = (yyvsp[0].symbol_info)->data_type ;
		}
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		(yyval.symbol_info)->data_type = TYPE ;
		//$$->var = $3->var ;
	}
#line 2531 "y.tab.c"
    break;

  case 53: /* term: unary_expression  */
#line 969 "104.y"
                         {
		logFile << "term : unary_expression" << endl ; 
		(yyval.symbol_info) = new SymbolInfo("","term : unary_expression") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->info_type = (yyvsp[0].symbol_info)->info_type ;
		(yyval.symbol_info)->var = (yyvsp[0].symbol_info)->var ;
	}
#line 2546 "y.tab.c"
    break;

  case 54: /* term: term MULOP unary_expression  */
#line 979 "104.y"
                                   {
		
		logFile <<"term : term MULOP unary_expression" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("","term : term MULOP unary_expression") ;
		if(allCapital((yyvsp[-2].symbol_info)->data_type) == "VOID" || allCapital((yyvsp[0].symbol_info)->data_type) == "VOID") {
			error << "Line# " << yylineno << ": Void cannot be used in expression" <<endl ;
			err_cnt++ ;
		}
		if((yyvsp[-1].symbol_info)->getName() == "%"){
			if((yyvsp[0].symbol_info)->getName() == "0") {
				err_cnt++ ;
				error << "Line# " << yylineno << ": Warning: division by zero i=0f=1Const=0" << endl;
			}
			//Type Checking: Both the operands of the modulus operator should be integers.
			if(allCapital((yyvsp[-2].symbol_info)->data_type) != "INT" || allCapital((yyvsp[0].symbol_info)->data_type) != "INT"){
				error << "Line# " << yylineno <<": Operands of modulus must be integers " << endl ;
				err_cnt++ ;
			}
			(yyvsp[-2].symbol_info)->data_type = "INT" ;
			(yyvsp[0].symbol_info)->data_type ="INT" ;
		}
		string TYPE ="" ;
		if((yyvsp[-2].symbol_info)->data_type == (yyvsp[0].symbol_info)->data_type) {
			TYPE = (yyvsp[-2].symbol_info)->data_type ;
		}
		else if((yyvsp[-2].symbol_info)->data_type == "INT" && (yyvsp[0].symbol_info)->data_type == "FLOAT"){
			(yyvsp[-2].symbol_info)->data_type = "FLOAT" ;
			TYPE = "FLOAT" ;
		}
		else if((yyvsp[-2].symbol_info)->data_type == "FLOAT" && (yyvsp[0].symbol_info)->data_type == "INT"){
			(yyvsp[0].symbol_info)->data_type = "FLOAT" ;
			TYPE = "FLOAT" ;
		}
		else if((yyvsp[-2].symbol_info)->data_type != "VOID") {
			TYPE = (yyvsp[-2].symbol_info)->data_type ;
		}
		else {
			TYPE = (yyvsp[0].symbol_info)->data_type ;
		}

		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		(yyval.symbol_info)->data_type = TYPE ;
		//$$->var = $3->var ;
	}
#line 2600 "y.tab.c"
    break;

  case 55: /* unary_expression: ADDOP unary_expression  */
#line 1029 "104.y"
                                          {
		logFile << "unary_expression : ADDOP unary_expression" ;
		(yyval.symbol_info) = new SymbolInfo("","unary_expression : ADDOP unary_expression") ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-1].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		//$$->var = $2->var ;
	}
#line 2615 "y.tab.c"
    break;

  case 56: /* unary_expression: NOT unary_expression  */
#line 1039 "104.y"
                               {
		logFile << "unary_expression : NOT unary_expression" << endl ;
		(yyval.symbol_info) = new SymbolInfo("", "unary_expression : NOT unary_expression") ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-1].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		//$$->var = $2->var ;
	}
#line 2630 "y.tab.c"
    break;

  case 57: /* unary_expression: factor  */
#line 1049 "104.y"
                 {
		logFile << "unary_expression : factor" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","unary_expression : factor") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->setType("unary_expression : factor") ;
		//if($1->getName()!="")	//cout << $1->getName() <<"783568734587" << endl;
		(yyval.symbol_info)->setName((yyvsp[0].symbol_info)->getName()) ;
		(yyval.symbol_info)->rule = true ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->info_type = (yyvsp[0].symbol_info)->info_type ;
		(yyval.symbol_info)->var = (yyvsp[0].symbol_info)->var ;
	}
#line 2649 "y.tab.c"
    break;

  case 58: /* factor: variable  */
#line 1064 "104.y"
                   {
		logFile << "factor : variable" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","factor : variable") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->setName((yyvsp[0].symbol_info)->getName()) ;
		(yyval.symbol_info)->rule = true ;
		(yyval.symbol_info)->setType("factor : variable") ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->info_type = (yyvsp[0].symbol_info)->info_type ;
		(yyval.symbol_info)->var = (yyvsp[0].symbol_info)->var ;
	}
#line 2667 "y.tab.c"
    break;

  case 59: /* factor: ID LPAREN argument_list RPAREN  */
#line 1077 "104.y"
                                         {
		//string code = $1->getName() + "(" + toSymbolNameListStr($3) + ")" ;
		logFile << "factor : ID LPAREN argument_list RPAREN" << endl ;
		
		(yyval.symbol_info) = new SymbolInfo("","factor : ID LPAREN argument_list RPAREN") ;
		call_func((yyvsp[-3].symbol_info),dec_list) ;

		(yyval.symbol_info)->add_child((yyvsp[-3].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-3].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;

		(yyval.symbol_info)->data_type = (yyvsp[-3].symbol_info)->data_type ;
	}
#line 2688 "y.tab.c"
    break;

  case 60: /* factor: LPAREN expression RPAREN  */
#line 1093 "104.y"
                                   {
		logFile << "factor : LPAREN expression RPAREN" <<endl ;
		(yyval.symbol_info) = new SymbolInfo("","factor : LPAREN expression RPAREN") ;
		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[-1].symbol_info)->data_type ;
		//$$->var = $1->var ;
	}
#line 2704 "y.tab.c"
    break;

  case 61: /* factor: CONST_INT  */
#line 1104 "104.y"
                    { 
		logFile << "factor : CONST_INT" << endl ;
		(yyval.symbol_info) = new SymbolInfo((yyvsp[0].symbol_info)->getName(),"factor : CONST_INT",true) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = "INT" ;
	}
#line 2717 "y.tab.c"
    break;

  case 62: /* factor: CONST_FLOAT  */
#line 1112 "104.y"
                      { 
		logFile << "factor : CONST_FLOAT" << endl ;
		(yyval.symbol_info) = new SymbolInfo((yyvsp[0].symbol_info)->getName(), "factor : CONST_FLOAT",true) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = "FLOAT" ;
	}
#line 2730 "y.tab.c"
    break;

  case 63: /* factor: variable INCOP  */
#line 1120 "104.y"
                         {
		logFile << "factor : variable INCOP" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","factor : variable INCOP") ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-1].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[-1].symbol_info)->data_type ;
	}
#line 2744 "y.tab.c"
    break;

  case 64: /* factor: variable DECOP  */
#line 1129 "104.y"
                         {
		logFile << "factor : variable DECOP" << endl ;
		(yyval.symbol_info) = new SymbolInfo("", "factor : variable DECOP",true) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-1].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[-1].symbol_info)->data_type ;
		//$$->var = $1->var ;
	}
#line 2759 "y.tab.c"
    break;

  case 65: /* factor: %empty  */
#line 1139 "104.y"
          {
		(yyval.symbol_info) = new SymbolInfo("","factor : empty") ;
	}
#line 2767 "y.tab.c"
    break;

  case 66: /* argument_list: arguments  */
#line 1145 "104.y"
                          {
		logFile <<"argument_list : arguments" << endl ;
		(yyval.symbol_info) = new SymbolInfo("", "argument_list : arguments") ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->_parameter_list = dec_list ;
	}
#line 2780 "y.tab.c"
    break;

  case 67: /* argument_list: %empty  */
#line 1154 "104.y"
        {
		logFile  << "argument_list" << endl ;
		//$$ = new vector<SymbolInfo*>();
		(yyval.symbol_info) = new SymbolInfo("","argument_list") ;

	}
#line 2791 "y.tab.c"
    break;

  case 68: /* arguments: arguments COMMA logic_expression  */
#line 1163 "104.y"
                                             {
		//string code = toSymbolNameListStr($1) + "," + $3->getName();
		logFile << "arguments : arguments COMMA logic_expression" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","arguments : arguments COMMA logic_expression") ;

		(yyval.symbol_info)->add_child((yyvsp[-2].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[-1].symbol_info)) ;
		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[-2].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->info_type = (yyvsp[-2].symbol_info)->info_type ;
		(yyval.symbol_info)->var = (yyvsp[-2].symbol_info)->var ;
		//cout <<$3->data_type <<"(&*^7825678243" << endl;

		//$$->push_back($3);
		dec_list.push_back((yyvsp[0].symbol_info)) ;
		//$$->_parameter_list = dec_list ;
	}
#line 2815 "y.tab.c"
    break;

  case 69: /* arguments: logic_expression  */
#line 1182 "104.y"
                           {
		logFile << "arguments : logic_expression" << endl ;
		(yyval.symbol_info) = new SymbolInfo("","arguments : logic_expression") ;

		(yyval.symbol_info)->add_child((yyvsp[0].symbol_info)) ;
		(yyval.symbol_info)->start_line = (yyvsp[0].symbol_info)->start_line ;
		(yyval.symbol_info)->end_line = (yyvsp[0].symbol_info)->end_line ;
		(yyval.symbol_info)->data_type = (yyvsp[0].symbol_info)->data_type ;
		(yyval.symbol_info)->info_type = (yyvsp[0].symbol_info)->info_type ;
		(yyval.symbol_info)->var = (yyvsp[0].symbol_info)->var ;
		//cout <<$1->data_type <<" efihsdfv" << endl;

		//$$ = new vector<SymbolInfo*>(); // init list
		//$$->push_back($1);
		dec_list.push_back((yyvsp[0].symbol_info)) ;
		//$$->_parameter_list = dec_list ;
	}
#line 2837 "y.tab.c"
    break;


#line 2841 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1200 "104.y"


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

