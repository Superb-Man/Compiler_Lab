
%option noyywrap yylineno
%{
#pragma once
#include<bits/stdc++.h>
#include"SymbolTable.h"
#include"y.tab.hpp"
using namespace std ;

extern YYSTYPE yylval;
extern SymbolTable st;
extern int err_cnt ;
extern ofstream logFile ;

    int line_cnt = 1 ;
    int starting = 0 ;
    string comments = "";
    string strr ="" ;
    string ss ;

    map<char, string> charMap = {
        {'n', "\n"},
        {'t', "\t"},
        {'\\', "\\"},
        {'\'', "\'"},
        {'a', "\a"},
        {'f', "\f"},
        {'r', "\r"},
        {'b', "\b"},
        {'v', "\v"},
        {'0', "\0"},
        {'\"', "\""},
    } ;

    string generateString(string str) {
        string str2 = "" ;
        for(int i = 0; i < str.length(); i++){
            if(str[i] == '\\'){
                str2 += charMap[str[i+1]];
                i++;
            }
            else{
                str2 += str[i];
            }
        }

        return str2 ;
    }

    string allCapital(string str) {
        for(char& ch : str) ch = toupper(ch);
        return str ;
    }

%}



WHITESPACE [ \t\f\r\v]+ 
DIGIT [0-9]
NEWLINE [\r]?\n
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
CONST_CHAR \'[^\\\n\r]\'
CONST_ESCAPE_CHAR \'\\[nt\\'afrbv0]\'
%%
{WHITESPACE} {
    //do nothing
}

{NEWLINE} {
            line_cnt++ ;
        }

"if"        { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return IF;}
"else"      {   yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return ELSE;}
"for"       { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return FOR; 
            }
"while"     { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return WHILE;
            }
"do"        { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return DO;
            }
"break"     { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return BREAK;
            }
"int"       { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return INT;
            }
"char"      {
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl; 
                return CHAR;
            }
"float"     {
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl; 
                return FLOAT;
            }
"double"    { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return DOUBLE;
            }
"void"      {
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return VOID; 
            }
"return"    { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return RETURN;
            }
"switch"    {  
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;    
                return SWITCH;
            }
"case"      { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return CASE;
            }
"default"   { 
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return DEFAULT;
            }
"continue"  {
                yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;
                return CONTINUE;
            }

"println"    {
                yylval.symbol_info = new SymbolInfo("println", "ID") ;
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"ID"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
                return PRINTLN ;

            }
"main"      {
                //yylval.symbol_info = new SymbolInfo(yytext, allCapital(yytext)) ;
                yylval.symbol_info = new SymbolInfo(yytext, "ID");
                
                yylval.symbol_info->start_line = line_cnt ;
                yylval.symbol_info->end_line = line_cnt ;
                yylval.symbol_info->leaf = true ;
                logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"ID"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
                return ID ;   

            }

{DIGIT}+    {
            yylval.symbol_info = new SymbolInfo(yytext, "CONST_INT") ;
            yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"CONST_INT"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return CONST_INT ;
        }
({DIGIT}*\.{DIGIT}+([Ee][+-]?{DIGIT}+)?|{DIGIT}+[Ee][+-]?{DIGIT}+) {
            yylval.symbol_info = new SymbolInfo(yytext, "CONST_FLOAT") ;
            yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"CONST FLOAT"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return CONST_FLOAT ; 
        }

{CONST_CHAR} {
            yylval.symbol_info = new SymbolInfo(yytext, "CONST_CHAR");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"CONST_CHARA"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return CONST_CHAR;    
        }

{CONST_ESCAPE_CHAR} {
            yylval.symbol_info = new SymbolInfo(yytext, "CONST_CHAR") ;
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"CONST_CHAR"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return CONST_CHAR;
        }

{IDENTIFIER} {
            cout << yytext << endl;
            yylval.symbol_info = new SymbolInfo(yytext, "ID");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"ID"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return ID;     
        }

"+"|"-"	{
            yylval.symbol_info = new SymbolInfo(yytext, "ADDOP") ;
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"ADDOP"<< ">"<<" Lexeme "<<yytext<<" found"<<endl; 
            return ADDOP ;     
        }

"*"|"/"|"%"	{
            yylval.symbol_info = new SymbolInfo(yytext, "MULOP");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"MULOP"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return MULOP ;
        }

"++"    {
            yylval.symbol_info = new SymbolInfo(yytext, "INCOP");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"INCOP"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return INCOP ;

        }

"--"	{
            yylval.symbol_info = new SymbolInfo(yytext, "DECOP");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"DECOPP"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
		    return DECOP ;
        }

"<"|"<="|">"|">="|"=="|"!="	{
            yylval.symbol_info = new SymbolInfo(yytext, "RELOP");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"RELOP"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return RELOP ;
        }

"="	    {
            yylval.symbol_info = new SymbolInfo(yytext, "ASSIGNOP");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"ASSIGNOP"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return ASSIGNOP ;
        }

"&&"|"||" {
            yylval.symbol_info = new SymbolInfo(yytext, "LOGICOP");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"LOGICOP"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return LOGICOP;
        }

"!"	    {
            yylval.symbol_info = new SymbolInfo(yytext, "NOT");
            yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"NOT"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return NOT ;
        }

"("	    {
            yylval.symbol_info = new SymbolInfo(yytext, "LPAREN");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"LPAREN"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return LPAREN ;
        }

")"	    {
            yylval.symbol_info = new SymbolInfo(yytext, "RPAREN");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"RPAREN"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return RPAREN ;
        }

"{"	    {
            yylval.symbol_info = new SymbolInfo(yytext, "LCURL");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"LCURL"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return LCURL ;
        }

"}"	    {
            yylval.symbol_info = new SymbolInfo(yytext, "RCURL");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"RCURL"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return RCURL ;
        }

"["	    {
            yylval.symbol_info = new SymbolInfo(yytext, "LSQUARE");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"LSQUARE"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
		    return LSQUARE ;
        }

"]"	    {
            yylval.symbol_info = new SymbolInfo(yytext, "RSQUARE");
            yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"RSQUARE"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return RSQUARE ;
        }

","	    {
            yylval.symbol_info = new SymbolInfo(yytext, "COMMA");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"COMMA"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
            return COMMA ;
        }

";"	    {
            yylval.symbol_info = new SymbolInfo(yytext, "SEMICOLON");
                        yylval.symbol_info->start_line = line_cnt ;
            yylval.symbol_info->end_line = line_cnt ;
            yylval.symbol_info->leaf = true ;
            logFile << "Line# "<< line_cnt <<": Token " <<"<"<<"SEMICOLON"<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
		    return SEMICOLON ;
        }

<INITIAL><<EOF>> {
            return 0 ;
        }



%%

