#pragma once 
#include<bits/stdc++.h>
#include<vector>
#define size_t unsigned long long int 
#define VARIABLE 1 
#define FUNCTION_DECLARATION 2
#define FUNCTION_DEFINITION 3
#define ARRAY 4

class SymbolInfo {
    std::string name  ;
    std::string type ;
public:
    size_t start_line ;
    size_t end_line ;
    size_t var ;
    bool rule ;
    size_t position ;
    SymbolInfo* nxt ;
    bool leaf ;
    std::string getName() {
        return name ;
    }
    std::string getType() {
        return type ;
    }
    void setName(std::string name ) {
        this->name = name ;
    }
    void setType(std::string type) {
        this->type = type ;
    }
    std::string data_type ;
    size_t info_type ;
    std::string array_size ;
    std::vector<SymbolInfo*>_parameter_list ;
    std::vector<SymbolInfo*>child_list ;
    bool is_array(){
        return array_size != "" ;
    }

    void add(std::string param_name , std::string data_type,size_t info_type) {
        SymbolInfo* sym = new SymbolInfo(name , "") ;
        sym->data_type = data_type ;
        sym->info_type = info_type ;
        _parameter_list.push_back(sym) ;
    }
    SymbolInfo() {
        nxt = nullptr ;
        position = 0 ;
    }
    SymbolInfo(std::string name , std::string type , bool rule = false) {
        this->name = name ;
        this->type = type ;
        this->nxt = nullptr ;
        position = 0 ; 
        this->data_type = "" ;
        this->info_type = VARIABLE ;
        this->array_size = "" ;
        //if(_parameter_list.size() > 0) {
            _parameter_list.clear() ;
        //}
        child_list.clear() ;
        this->rule = rule ;
        this->leaf = false ;
        this->var = VARIABLE ;
    }

    SymbolInfo(SymbolInfo* symbol) {
        this->name = symbol->getName() ;
        this->type = symbol->getType() ;
        this->nxt = symbol->nxt ;
        this->position = symbol->position ; 
        this->data_type = symbol->data_type ;
        this->info_type = symbol->info_type ;
        this->array_size = symbol->array_size ;
        //if(_parameter_list.size() > 0) {
            this->_parameter_list = symbol->_parameter_list ;
        //}
        this->rule = symbol->rule ;
        this->leaf = symbol->leaf ; 
        this->child_list = symbol->child_list ;  
        this->start_line = symbol->start_line ;
        this->end_line = symbol->end_line ;
        this->var = symbol->var ;
        //std::cout <<"COPY CALLED ----------------->" << '\n' ;
    }

    void add_child(SymbolInfo*& symbol) {
        child_list.push_back(symbol) ;
    }
    ~SymbolInfo()  {
        _parameter_list.clear() ;
        child_list.clear() ;
    }
} ;

