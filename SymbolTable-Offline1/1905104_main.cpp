#include<bits/stdc++.h>
#include<stdlib.h>
#include<vector>
#include<sstream>
#include"SymbolTable.h"


int main() {

    freopen("inp2.txt","r",stdin) ;
    freopen("output.txt","w",stdout) ;

    size_t len  , cnt_sct = 0 ;
    std::cin >> len ;

    size_t x = 0 ;
    SymbolTable st ;
    st.enterScope(++cnt_sct , len) ;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while(true) {
        std::string inp , temp ;
        getline(std::cin, inp) ;
        std::string cmd ;
        std::cout <<"Cmd "<<++x<<": " << inp<<'\n' ;
        std::vector<std::string>tokens;

        std::stringstream str(inp);
        while(getline(str,temp,' ')) {
            tokens.push_back(temp);
        }

        cmd = tokens[0] ;

        if(cmd == "I") {
            //std::cin >> name >> type ;
            if(tokens.size() !=3){
                std::cout <<"\tNumber of parameters mismatch for the command I\n" ;
            }
            else {
                st.insertSymbol(tokens[1] , tokens[2]) ;
            }
        }
        else if(cmd == "L") {
            if(tokens.size() != 2)  std::cout <<"\tNumber of parameters mismatch for the command L\n" ;
            else {
                //std::cout << cmd << " " << name << "\n" ;
                st.lookup(tokens[1]) ;
            }
        }

        else if(cmd == "D") {
            if(tokens.size() != 2){
               std::cout <<"\tNumber of parameters mismatch for the  command D\n" ;
            }
            else st.deleteSymbol(tokens[1]) ;
        }

        else if(cmd == "P") {
            //std::cin >> type ;
            //std :: cout << cmd << " " << type << "\n" ;
            if(tokens.size() != 2){
               std::cout <<"\tNumber of parameters mismatch for the command P\n" ;
            }
            else if(tokens[1] == "A") st.printAllScope() ;
            else if(tokens[1]== "C") st.printCurrentScope() ;
            else std::cout << "\t" << "\tInvalid Operation" <<"\n" ;
        }

        else if(cmd == "S") {
            //std::cout << cmd <<"\n" ;

            if(tokens.size() != 1){
               std::cout <<"\tNumber of parameters mismatch for the command S\n" ;
            }
            else st.enterScope(++cnt_sct , len) ;
        }

        else if(cmd == "E") {
            if(tokens.size() != 1){
               std::cout <<"\tNumber of parameters mismatch for the command E\n" ;
            }
            else {
                if(st.getCurrScope()->parent == nullptr){
                    std::cout << "ScopeTable# "<<st.getCurrScope()->get_id()<<" cannot be removed\n" ;
                }
                //std::cout << cmd <<"\n" ;
                else st.exitScope() ;
            }
            //cnt_sct-- ;
        }
        else if(cmd =="Q") return 0 ;
        else break ;

    }


}
