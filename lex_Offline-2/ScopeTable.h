#pragma once
#include<bits/stdc++.h>
#include"SymbolInfo.h"


size_t SDBMHash(std::string str, size_t siz) {
    size_t hash = 0;
	size_t i = 0;
	size_t  len = str.length();

	for (i = 0; i < len; i++) {
		hash = (((str[i]) + (hash << 6) + (hash << 16)) - hash);
	}
	return hash%siz ;
}

class ScopeTable {
    size_t id ;
    size_t table_size ;
    SymbolInfo** bucketarray ; //Can't use stl
    void clear() ;

public:
    ScopeTable() ;
    ~ScopeTable() ;
    ScopeTable(size_t , size_t ) ;
    ScopeTable* parent ;
    size_t hash(std::string , size_t) ;
    size_t get_id() ;
    void set_id(size_t) ;
    size_t get_size() ;
    SymbolInfo* lookupHelper(std::string) ;
    void lookup(std::string) ;
    bool insertSymbol(SymbolInfo*, std::ofstream&) ;
    bool insertSymbol(std::string , std::string, std::ofstream&) ;
    bool deleteSymbol(std::string) ;
    void printScopeTable(std::ofstream&) ;
} ;

ScopeTable::ScopeTable() {
    //Do nothing
}

ScopeTable::ScopeTable(size_t id, size_t table_size) {
    this->parent = nullptr ;
    this->id = id ;
    this->table_size = table_size ;

    bucketarray = new SymbolInfo*[table_size] ;
    for(size_t i = 0 ; i < table_size ; i++) {
        bucketarray[i] =  new SymbolInfo ;
    }
}

size_t ScopeTable::hash(std::string key , size_t table_size) {
    return SDBMHash(key, table_size) ;
}

size_t ScopeTable::get_id() {
    return id ;
}

void ScopeTable::set_id(size_t id) {
    this->id = id ;
}

size_t ScopeTable::get_size() {
    return table_size ;
}

SymbolInfo* ScopeTable::lookupHelper(std::string key) {
    //searching in seperate chaining
    size_t hash_index = hash(key , table_size) ;

    SymbolInfo* cur = bucketarray[hash_index] ;
    cur = cur->nxt ;
    while(cur) {
        if(cur->getName() == key) {
            //if found
            //std::cout << "\t" << "Found in ScopeTable #" << id << " at position " << hash_index << ", " << cur->position << std::endl;
            return cur ;
        }
        cur = cur->nxt ;
    }

    //std::cout << "\t" << "Not found in the current ScopeTable" << std::endl;

    return nullptr ;

}

void ScopeTable::lookup(std:: string key) {
    size_t hash_index = hash(key,table_size) ;
    SymbolInfo* cur = lookupHelper(key) ;
    if(lookupHelper(key))
        std::cout << "\t" << "'"<<key << "' "<<"found in ScopeTable# " << id << " at position " << hash_index + 1 << ", " << cur->position << std::endl ;
    //else
        //std::cout << "\t" << "Not found in the current ScopeTable" << std::endl;
}

bool ScopeTable::insertSymbol(SymbolInfo* symbol,std::ofstream& out) {
    SymbolInfo* ptr = lookupHelper(symbol->getName()) ;
    if(ptr) {
        //Already exists
        out << "\t" << symbol << " already exists in the current ScopeTable" << std::endl;
        return false ;
    }
    size_t pos = 1 ;//1 for dummy node


    size_t hash_index = hash(symbol->getName() , table_size) ;
    ptr = bucketarray[hash_index] ;


    while(ptr->nxt) {
        ptr=ptr->nxt;
        ++pos ;
    }
    if(ptr->nxt == nullptr) ptr->nxt = symbol ;
    ptr->nxt->position = pos ;

    //Symbol inserted
    std::cout << "\t" << "Inserted in ScopeTable# " << id << " at position " << hash_index + 1 << ", " << pos << std::endl;
    return true ;
}

bool ScopeTable::insertSymbol(std::string name , std::string type, std::ofstream& out) {
    if(lookupHelper(name)) {
        out << "\t" << name << " already exists in the current ScopeTable" << std::endl;
        return false ;
    }
    return insertSymbol(new SymbolInfo(name , type),out) ;
}

bool ScopeTable::deleteSymbol(std::string key) {
   SymbolInfo* ptr = lookupHelper(key) ;
   if(!ptr) {
        //Not present in the hashTable
        std::cout <<"\t" << "Not found in the current ScopeTable" <<'\n' ;
        return false ;
    }

    size_t pos = ptr->position ;
    size_t hash_index = hash(key , table_size) ;
    ptr = bucketarray[hash_index];
    SymbolInfo* temp = ptr ;

    while (ptr->nxt != nullptr) {
        ++pos ;
        ptr = ptr->nxt ;
        if(ptr->getName() == key)   break;
        temp = ptr ;
    }

    //link between temp and ptr->next
    if (temp != ptr) {
        temp->nxt = ptr->nxt ;
        //delete ptr ;
    }

    std::cout << "\t" << "Deleted '" <<key << "' from ScopeTable# "<< id <<" at position " <<hash_index + 1 <<", "<< pos-1 << std::endl;
    delete ptr ;

    return true ;
}

void ScopeTable::printScopeTable(std::ofstream& out) {
    out << "\t" << "ScopeTable# " << id << std::endl ;
    for(size_t i = 0 ; i< table_size ; i++) {
        SymbolInfo* cur = bucketarray[i] ;

        cur = cur->nxt ;
        if(!cur) continue ;
        out << "\t" << i+1 << "-->" ;

        while(cur) {
            out << " <" << cur->getName() << "," << cur->getType() << ">" ;
            cur = cur->nxt ;
        }
    out << std::endl ;

    }
}

void ScopeTable::clear() {
    SymbolInfo* temp , *ptr;
    for(size_t i = 0 ; i < table_size ; i++){
        temp = bucketarray[i] ;
        while(temp){
            ptr=temp;
            temp = temp->nxt;
            delete ptr;
        }
    }
}

ScopeTable::~ScopeTable() {

    clear() ;
    delete[] bucketarray;
}
