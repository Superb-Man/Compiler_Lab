#include<iostream>
#define size_t unsigned long long int 

class SymbolInfo {
    std::string name  ;
    std::string type ;
public:
    size_t position ;
    SymbolInfo* nxt ;
    std::string getName() ;
    std::string getType() ;
    SymbolInfo() ;
    SymbolInfo(std::string , std::string) ;
    ~SymbolInfo() ;
} ;

SymbolInfo::SymbolInfo() {
    nxt = nullptr ;
    position = 0 ;
}
SymbolInfo::SymbolInfo(std::string name , std::string type) {
    this->name = name ;
    this->type = type ;
    this->nxt = nullptr ;
    position = 0 ; 
}

std::string SymbolInfo::getName() {
    return name ;
}

std::string SymbolInfo::getType() {
    return type ;
}

SymbolInfo::~SymbolInfo() {
    //Nothing allocated !!
}
