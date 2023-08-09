%option noyywrap
%x STRING_STATE 
%x SL_COMMENT
%x STATE_STAR
%x COMMENT_CONTENT
%{
#include<bits/stdc++.h>
#include"SymbolTable.h"
using namespace std ;

    int line_cnt = 1 ;
    int starting = 0 ;
    int id = 0 ;
    int err_cnt = 0 ;
    SymbolTable st ; 
    string comments = "";
    string strr ="" ;
    string ss ;

    ofstream logout;
    ofstream tokenout;

    map<char, string> charMap ;
    charMap['n'] = "\n" ;
    charMap['t'] = "\t" ;
    charMap['\\'] = "\\" ;
    charMap['\''] =  "\'" ;
    charMap['a'] = "\a" ;
    charMap['f'] = "\f" ;
    charMap['r'] = "\r" ;
    charMap['b'] = "\b" ;
    charMap['v'] = "\v" ;
    charMap['0'] = "\0" ;
    charMap['\"'] = "\"" ;
    charMap['?'] =  "\?" ;

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
LETTER [a-zA-Z_]
DIGIT [0-9]
NEWLINE [\r]?\n
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
CONST_CHAR \'[^\\\n\r]\'
CONST_ESCAPE_CHAR \'\\[nt\\'afrbv0]\'
MULTILINE_COMMENT \/\*([^\*]|\*[^\/])*\*\/
%%
{WHITESPACE} {
    //do nothing
}

{NEWLINE} {
            line_cnt++ ;
        }

"if"|"else"|"for"|"while"|"do"|"break" {
			tokenout<<"<"<< allCapital(yytext)<<", " << yytext << ">" << endl ;
			logout<<"Line# "<< line_cnt <<": Token " <<"<"<<allCapital(yytext)<< ">"<<" Lexeme "<<yytext<<" found"<<endl;
		}
"main"  {
            tokenout << "<ID, "<<yytext << ">" << endl ;
			logout<<"Line# "<< line_cnt <<": Token " <<"<ID>"<<" Lexeme "<<yytext<<" found"<<endl;
            st.enterScope(++id,10) ;
            if(st.insertSymbol(yytext,"ID",logout)) {
                st.printAllScope(logout) ;
            } 
        }

"int"|"char"|"float"|"double"|"void"|"return"|"switch"|"case"|"default"|"continue" {
            tokenout << "<" << allCapital(yytext) << ", " <<yytext << ">" << endl ;
			logout << "Line# " << line_cnt << ": Token " <<"<" << allCapital(yytext) << ">" << " Lexeme " << yytext<<" found" << endl;

        }

{DIGIT}+    {
            tokenout << "<" << "CONST_INT, "  << yytext << ">" << endl ;
			logout << "Line# " << line_cnt << ": Token " <<"<" << "CONST_INT" << ">" << " Lexeme " << yytext<<" found" << endl;

            //insert in symbtable
            /*if(st.insertSymbol(yytext,"CONST_INT",logout)) {
                st.printAllScope(logout) ;
            }*/ 
   
        }
({DIGIT}*\.{DIGIT}+([Ee][+-]?{DIGIT}+)?|{DIGIT}+[Ee][+-]?{DIGIT}+) {
            tokenout << "<" << "CONST_FLOAT, " << yytext << "> " << endl ;
			logout << "Line# " << line_cnt << ": Token " <<"<" << "CONST_FLOAT" << ">" << " Lexeme " << yytext<<" found" << endl;

            //insert in symbtable
            /*if(st.insertSymbol(yytext,"CONST_FLOAT",logout)) {
                st.printAllScope(logout) ;
            }*/ 
        }

{CONST_CHAR} {
            tokenout << "<" << "CONST_CHAR, " << yytext[1] << ">" <<endl ;
			logout << "Line# " << line_cnt << ": Token " <<"<" << "CONST_CHAR" << ">" << " Lexeme " << yytext[1]<<" found" << endl;

            //insert in symbtable 
            /*if(st.insertSymbol(yytext,"CONST_CHAR",logout)) {
                st.printAllScope(logout) ;
            }*/ 
        }

{CONST_ESCAPE_CHAR} {
            tokenout << "<" << "CONST_CHAR, " << charMap[yytext[2]] << ">" <<endl ;
			logout << "Line# " << line_cnt << ": Token " << "<CONST_CHAR>" <<  " Lexeme " << charMap[yytext[2]]<<" found" << endl;
        }

{IDENTIFIER} {
            tokenout << "<ID, " << yytext << "> " << endl ;
			logout << "Line# " << line_cnt << ": Token " << "<ID>"  << " Lexeme " << yytext<<" found" << endl;

            //insert in symbtable    
            if(st.insertSymbol(yytext,"ID",logout)) {
                st.printAllScope(logout) ;
            } 
        }

"+"|"-"	{
		    tokenout << "<ADDOP, " << yytext << "> "<< endl ;
		    logout << "Line# " << line_cnt <<": Token " << "<ADDOP>" << " Lexeme " << yytext << " found" << endl;
        }

"*"|"/"|"%"	{
		    tokenout << "<MULOP, " << yytext << "> "<< endl ;
		    logout << "Line# " << line_cnt << ": Token " << "<MULOP>" << " Lexeme " <<yytext <<" found"<<endl;
        }

"++"|"--"	{
		    tokenout<<"<INCOP, "<<yytext<<"> " << endl ;
		    logout<<"Line# "<< line_cnt <<": Token " <<"<INCOP>"<<" Lexeme "<<yytext<<" found"<<endl;
        }

"<"|"<="|">"|">="|"=="|"!="	{
            tokenout<<"<RELOP, "<<yytext<<"> " << endl ;
            logout<<"Line# "<< line_cnt <<": Token " <<"<RELOP>"<<" Lexeme "<<yytext<<" found"<<endl;
        }

"="	    {
            tokenout<<"<ASSIGNOP, "<<yytext<<"> " << endl ;
            logout<<"Line# "<< line_cnt <<": Token " <<"<ASSIGNOP>"<<" Lexeme "<<yytext<<" found"<<endl;
        }

"&&"|"||" {
            tokenout<<"<logICOP, "<<yytext<<"> " << endl ;
            logout<<"Line# "<< line_cnt <<": Token " <<"<logICOP>"<<" Lexeme "<<yytext<<" found"<<endl;
        }

"!"	    {
            tokenout<<"<NOT, "<<yytext<<"> " << endl ;
            logout<<"Line# "<< line_cnt <<": Token " <<"NOT"<<" Lexeme "<<yytext<<" found"<<endl;
        }

"("	    {
            tokenout<<"<LPAREN, "<<yytext<<"> " << endl ;
            logout<<"Line# "<< line_cnt <<": Token " <<"<LPAREN>"<<" Lexeme "<<yytext<<" found"<<endl;
        }

")"	    {
            tokenout<<"<RPAREN, "<<yytext<<"> "<< endl ;
            logout<<"Line# "<< line_cnt <<": Token " <<"<RPAREN>"<<" Lexeme "<<yytext<<" found"<<endl;
        }

"{"	    {
            tokenout<<"<LCURL, "<<yytext<<"> " << endl ;
            logout<<"Line# "<< line_cnt <<": Token " <<"<LCURL>"<<" Lexeme "<<yytext<<" found"<<endl;

            //enter scope
            st.enterScope(++id,10) ;
        }

"}"	    {
            tokenout<<"<RCURL, "<<yytext<<"> " << endl;
            logout<<"Line# "<< line_cnt <<": Token " <<"<RCURL>"<<" Lexeme "<<yytext<<" found"<<endl;
            st.exitScope() ;
            //exit scope
        }

"["	    {
		    tokenout<<"<LSQUARE, "<<yytext<<"> " << endl ;
		    logout<<"Line# "<< line_cnt <<": Token " <<"<LSQUARE>"<<" Lexeme "<<yytext<<" found"<<endl;
        }

"]"	    {
            tokenout << "<RSQUARE, " << yytext << "> " << endl ;
		    logout << "Line# " << line_cnt << ": Token " << "<RSQUARE>" << " Lexeme " << yytext << " found" << endl ;
        }

","	    {
            tokenout << "<COMMA, " << yytext << "> " << endl ;
		    logout << "Line# " << line_cnt << ": Token " << "<COMMA>" << " Lexeme " << yytext << " found" << endl;
        }

";"	    {
		    tokenout << "<SEMICOLON, " << yytext << "> " << endl ;
		    logout << "Line# " << line_cnt << ": Token " << "<SEMICOLON>" << " Lexeme " << yytext << " found" << endl;
        }

\"      {
            ss+=yytext ;
            strr = "";
            starting = line_cnt ;

            BEGIN STRING_STATE;
        }

<STRING_STATE>\\{NEWLINE} {
            line_cnt++ ;
            strr+=yytext ;
            ss+=yytext ;
        } 
<STRING_STATE>\\\"|[^\"\n\r] {
            strr += yytext;
            ss+=yytext ;
        }
<STRING_STATE>\" {
            ss += yytext;
            string x  = "SINGLE LINE STRING" ;
            for(auto c : strr) {
                if(c == '\n') {
                    x = "MULTI LINE STRING" ;
                    break ;
                }
            }
            tokenout << "<" << x << ", " << generateString(strr) << "> " << endl ;
            logout << "Line# " << starting << ": Token <" << x <<">" <<" Lexeme " << ss << " found" << endl;
            ss = "" ;
            BEGIN INITIAL;
        }

\/\*	{
            starting = line_cnt ;
			BEGIN COMMENT_CONTENT ;
		}

<COMMENT_CONTENT>{NEWLINE} {
            comments+=yytext ;
            line_cnt++ ;
        }        
<COMMENT_CONTENT>([\t !-~]{-}[\*])* {
			comments+=yytext ;
		}

<COMMENT_CONTENT>\* {
            comments+=yytext ;
			BEGIN STATE_STAR ;
            //entering into a new state 
		}
<STATE_STAR>\/ {
            // we found something like /*-----*/
            logout << "Line# " << starting << ": Token <MULTI LINE COMMENT> Lexeme /*" << comments << "/ found" << endl;
            comments = "" ; 
			BEGIN INITIAL ;
		}
<STATE_STAR>\* {
			comments+=yytext ;	
		}
<STATE_STAR>([\t\n !-~]{-}[\/\*]) {
			comments+=yytext ;
			BEGIN COMMENT_CONTENT ;
		}

\/\/    {
            comments+= yytext;
            starting = line_cnt ;
            BEGIN SL_COMMENT;
        }

<SL_COMMENT>. {
            comments+=yytext;
        }

<SL_COMMENT>\\{NEWLINE} {
            comments+=yytext ; 
            line_cnt++;
        }

<SL_COMMENT>{NEWLINE} {
            logout << "Line# " << line_cnt << ": Token <SINGLE LINE COMMENT> Lexeme " << comments << " found" << endl ;
            line_cnt++ ;
            comments ="" ;
            BEGIN INITIAL ;
        }


{DIGIT}*(\.{DIGIT}*)+(\.{DIGIT}*)+([Ee][+-]?{DIGIT}+)? {
            logout << "Error at line# " << line_cnt << ": TOO_MANY_DECIMAL_POINTS " << yytext << endl;
			err_cnt++ ;
        }

{DIGIT}*[Ee][+-]?{DIGIT}*(\.{DIGIT}*)+ {
            logout << "Error at line# " << line_cnt << ": ILLFORMED_NUMBER " << yytext << endl;
			err_cnt++ ;
        }

({DIGIT}*(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?)+({IDENTIFIER})+ {
            logout << "Error at line# " << line_cnt << ": INVALID_ID_SUFFIX_NUM_PREFIX " << yytext << endl;
			err_cnt++;
        }

''      {
            logout << "Error at line# " << line_cnt << ": EMPTY_CHARACTER_CONSTANT " << yytext << endl;
		    err_cnt++;  
            BEGIN INITIAL ;

        }
'\\([\t !-~]{-}[ntvfrab0\\"'e?])' {
            logout << "Error at line# " << line_cnt << ": UNKNOWN_ESCAPE_SEQUENCE " << yytext << endl;
		    err_cnt++;
        }

\'.[^\'\r\n]+\' {
            logout << "Error at line# " << line_cnt << ": MULTICHAR_CONST_CHAR " << yytext << endl;
		    err_cnt++;
            BEGIN INITIAL ;
        }

\'(.|\\\')?[^\'\r\n]* {
            logout << "Error at line# " << line_cnt << ": UNFINISHED_CONST_CHAR " << yytext[0] <<yytext[1] << endl;
            if (yytext[2] == ';') {
                tokenout << "<SEMICOLON, " << yytext[2] << "> " << endl ;
            }
		    err_cnt++;
            BEGIN INITIAL ;

        }

<STRING_STATE>{NEWLINE} {
            //line_cnt++ ;
            logout << "Error at line# " << line_cnt << ": UNFINISHED_STRING " << ss << endl;
		    err_cnt++;
            ss = "" ;
            line_cnt++;
            BEGIN INITIAL ;
        }
.       {
		    logout << "Error at line " << line_cnt << ": UNRECOGNIZED_CHAR " << yytext << endl;
		    err_cnt++;
	    }

<STATE_STAR><<EOF>> {
		    logout << "Error at line# " << line_cnt << ": UNFINISHED_COMMENT " << "/*" << comments << endl;
		    comments = "" ;
            //line_cnt++ ;
			err_cnt++ ;		
            BEGIN INITIAL ;
		}

<COMMENT_CONTENT><<EOF>> {
			logout << "Error at line# " << line_cnt << ": UNFINISHED_COMMENT " << "/*" << comments << endl;
            //line_cnt++  ;
			comments = "" ;
			err_cnt++ ;		
            BEGIN INITIAL ;
		}   

<INITIAL><<EOF>> {
            st.printAllScope(logout);
            return 0 ;
        }



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

	tokenout.open("token.txt");
	logout.open("log.txt");

	yyin= fin;
	yylex();
	fclose(yyin);


	logout <<"Total lines: " << line_cnt << endl;
	logout <<"Total errors: "  << err_cnt << endl;

	tokenout.close();
	logout.close();


	return 0;
}