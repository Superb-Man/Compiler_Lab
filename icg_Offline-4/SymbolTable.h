#pragma once
#include<bits/stdc++.h>
#include"ScopeTable.h"


class SymbolTable {
    ScopeTable* cur ;
    size_t _totScope ;

public:
    SymbolTable() {
        cur = nullptr ;
        _totScope = 0 ;
    }

    ~SymbolTable() {
        /*for(size_t i = 0 ; i < _totScope + 1 ; i++) {
            exitScope() ;
        }*/
        while(cur){
            exitScope() ;
        }
    }

    void enterScope(size_t id , size_t bucket) {
        ++_totScope ;
        ScopeTable* sct = new ScopeTable(id ,bucket) ;
        if(cur) {
            ScopeTable* temp = cur ;
            cur = sct ;
            cur->parent = temp ;
        }
        else {
            cur = sct ;
            cur->parent = nullptr ;
        }
        if(cur->parent) {
            size_t lol = cur->parent->get_id() ;
        }
        //std::cout << "\t" << "ScopeTable# " << id << " created\n" ;
    }
    void exitScope() {
        if(!cur) {
            //std::cout <<"\t" << "no scopetable in symboltable\n" ;
            return ;
        }
        ScopeTable* temp = cur ;
        //std::cout << "\t" << "ScopeTable# " << cur->get_id() << " removed\n";
        cur = cur->parent ;

        if(temp) {
            std::cout <<"deleted form <-------------------->" << '\n' ;
            delete temp ;
        }
        _totScope-- ;
    }

    SymbolInfo* lookup(std::string key) {
        if(!cur) {
            //std::cout <<"\t" << "no scopetable in symboltable\n" ;
            return nullptr ;
        }

        ScopeTable* temp = cur ;
        while(temp) {
            SymbolInfo* symbol = temp->lookupHelper(key);
            if(symbol) {
                size_t hash_index = temp->hash(key,temp->get_size()) ;
                //std::cout << "\t" << "'"<<key << "' "<<"found in ScopeTable# " << temp->get_id() << " at position " << hash_index + 1 << ", " << symbol->position << std::endl ;
                return symbol ;
            }

            temp = temp->parent ;
        }

        std::cout << "\t" << "'"<<key << "' "<<"not found in any of the ScopeTables" <<'\n' ;

        return nullptr ;
    }
    bool insertSymbol(std::string name , std::string type, std::ofstream& out) {
        if(!cur) {
            //std::cout <<"\t" << "no scopetable in symboltable\n" ;
            return false ;
        }

        return cur->insertSymbol(name , type, out) ;
    }

    bool deleteSymbol(std::string key) {
        if(!cur) {
            //std::cout <<"\t" << "no scopetable in symboltable\n" ;
            return false ;
        }
        return cur->deleteSymbol(key) ;
    }

    void printCurrentScope(std::ofstream& out) {
        if(!cur) {
            //std::cout << "\t" << "no scopetable in the symboltable\n" ;
            return ;
        }

        cur->printScopeTable(out);
        return ;
    }
    void  printAllScope(std::ofstream& out) {
        if(!cur) {
            //out << "\t" << "no ScopeTable in the SymbolTable" << std::endl ;
            return ;
        }
        ScopeTable* sct = cur ;

        while(sct) {
            sct->printScopeTable(out);
            sct = sct->parent ;
            //if(sct) std::cout <<'\n';
        }

        return ;
    }
    ScopeTable* getCurrScope(){
        return cur ;
    }
} ;


