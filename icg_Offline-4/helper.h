#pragma once 
#include<bits/stdc++.h>
#include"SymbolTable.h"


extern SymbolTable st;
int gv_cnt = 0 ;
int lv_cnt = 0 ;

int label_cnt = 0 ;
int tmp_cnt = 0 ;
string main_terminal_label ;
bool _main = false ;

ofstream codeSegOut ; // output files

ofstream codeAsmOut("code.asm", ios::out);

//creating new label 
string newlabel() {
    string s  = to_string(label_cnt++) ;
    return "L"+s ;
}

void gen_var_dec_code_asmbly(SymbolInfo*& varInfo, bool gs = false) {
    if (gs == true) {
        codeAsmOut << "\t" << varInfo->getName() << " DW " << varInfo->array_size  ;
        if (varInfo->is_array() == true) {
            codeAsmOut << " DUP(" << 0 << ")" << endl ;
            //Array Declared!!
        }
        else {
            codeAsmOut << "0 " << endl;
        }
        varInfo->asm_name = varInfo->getName() ;
        varInfo->global = true ;
        varInfo->arr_strt = 0 ;
    }
    else {
        if (varInfo->is_array() == true) {
            cout <<"it's an array" << endl;
            int n = stoi(varInfo->array_size) ;
            int arr_strt = ((st.getCurrScope()->var_cnt + 1) * 2) ;
            string base_address = "W. [BP-" + to_string(arr_strt) + "]" ;
            varInfo->asm_name = base_address ;
            varInfo->global = false ;
            varInfo->arr_strt = arr_strt ; 
            if(n > 0 && st.getCurrScope()) {
                st.getCurrScope()->var_cnt = st.getCurrScope()->var_cnt + n ;
            }
            codeSegOut << "\t\tSUB SP, " << (n * 2) << "\t" << endl;
        }
        else {
            if(st.getCurrScope())
                st.getCurrScope()->var_cnt = st.getCurrScope()->var_cnt + 1 ;
            varInfo->asm_name = "W. [BP-" + to_string(2 * st.getCurrScope()->var_cnt) + "]";
            cout << varInfo->asm_name << endl ;
            codeSegOut << "\t\tSUB SP, 2\t" << endl ;
        }
    }
}

string get_relop_jmp_asmbly(string relop){
    if(relop == "<") return "JL";
    if(relop == "<=") return "JLE";
    if(relop == ">") return "JG";
    if(relop == ">=") return "JGE";
    if(relop == "==") return "JE";
    if(relop == "!=") return "JNE";
}

string get_var_address(SymbolInfo* info , bool pop = false) {
    //codeSegOut <<"ekhane dhukche" << endl;
    if(pop == true) {
        if(info->is_array() == true && info->global == false) {
            codeSegOut << "\t\tPOP BX" << endl ;
        }
    }
    return info->asm_name ;
}
void print_new_line() {
    codeSegOut << "\tPRINT_NEWLINE PROC" << endl;
    codeSegOut <<"\t\tPUSH AX"<<endl;
    codeSegOut << "\t\tPUSH DX" << endl ;
    codeSegOut << "\t\tMOV AH, 2" << endl ;
    codeSegOut << "\t\tMOV DL, 0Dh" << endl ;
    codeSegOut <<"\t\tINT 21h" << endl ;
    codeSegOut << "\t\tMOV DL, 0Ah" << endl ;
    codeSegOut << "\t\tINT 21h" << endl ;
    codeSegOut << "\t\tPOP DX" << endl ;
    codeSegOut << "\t\tPOP AX" << endl ;
    codeSegOut << "\t\tRET" << endl ;
    codeSegOut <<"\tPRINT_NEWLINE ENDP" << endl;
}

void print_number() {
    codeSegOut << "PRINT_NUMBER PROC"<< endl;
        codeSegOut << "\tPUSH CX" << endl;  
    
        codeSegOut << "\tMOV AX, 'X'" << endl;
        codeSegOut << "\tPUSH AX" <<endl;
        
        codeSegOut << "\tCMP BX, 0" << endl;  
        codeSegOut << "\tJE ZERO_NUM" << endl;
        codeSegOut << "\tJNL NON_NEGATIVE" << endl; 
        
        codeSegOut << "\tNEG BX" << endl ;

        codeSegOut << "\tMOV DL, '-'" << endl ;
        codeSegOut << "\tMOV AH, 2" << endl ;
        codeSegOut << "\tINT 21H" << endl ;
        codeSegOut << "\tJMP NON_NEGATIVE" <<endl ;  
        
        codeSegOut << "\tZERO_NUM:" << endl ;
            codeSegOut << "\t\tMOV DX, 0 " << endl ;
            codeSegOut << "\t\tPUSH DX" << endl ;
            codeSegOut << "\t\tJMP POP_PRINT_LOOP" << endl ;
        
        codeSegOut << "\tNON_NEGATIVE:" << endl ;
        
        codeSegOut << "\tMOV CX, 10" << endl ; 
        
        codeSegOut << "\tMOV AX, BX" << endl ;
        codeSegOut << "\tPRINT_LOOP:" << endl ;
            codeSegOut << "\t\tCMP AX, 0" << endl ;
            codeSegOut << "\t\tJE END_PRINT_LOOP" << endl ;
            codeSegOut << "\t\tMOV DX, 0" << endl ; 
            codeSegOut << "\t\tDIV CX" << endl ;
        
            codeSegOut << "\t\tPUSH DX" << endl ;
            
            codeSegOut << "\t\tJMP PRINT_LOOP" << endl ;

        codeSegOut << "\tEND_PRINT_LOOP:" << endl ;
        
        
        
        codeSegOut << "\tPOP_PRINT_LOOP:" << endl ;
            codeSegOut << "\t\tPOP DX" << endl ;
            codeSegOut << "\t\tCMP DX, 'X'" << endl ;
            codeSegOut << "\t\tJE END_POP_PRINT_LOOP" << endl ;
            
            codeSegOut << "\t\tCMP DX, '-'" << endl ;
            codeSegOut << "\t\tJE PRINT_TO_CONSOLE" << endl ;
            
            codeSegOut << "\t\tADD DX, 30H" << endl ;       
            codeSegOut << "\t\tPRINT_TO_CONSOLE:" << endl ;
            codeSegOut << "\t\tMOV AH, 2" << endl ;
            codeSegOut << "\t\tINT 21H" << endl ;
            
            codeSegOut << "\t\tJMP POP_PRINT_LOOP" << endl ;
        
        codeSegOut << "\tEND_POP_PRINT_LOOP:" << endl ; 
    codeSegOut << "CALL PRINT_NEWLINE" << endl ;
        codeSegOut << "\tPOP CX" << endl ;
        codeSegOut << "\tRET" << endl ;
    codeSegOut << "PRINT_NUMBER ENDP" << endl ;
}

void genFunctionForPrintln() {
    print_new_line() ;
    codeSegOut << endl;
    print_number() ;
}
