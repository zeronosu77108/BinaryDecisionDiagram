#include <iostream>
#include <map>

#include "BDD.h"

std::vector< std::vector<std::string> > split(std::string str) {
    std::vector< std::vector<std::string> >  exp;
    std::vector<std::string> e;
    std::string s;
    for(int i=0; i<=str.size(); i++) {
        if( str[i] == '(' ) {
            continue;
        } else if( str[i] == ')' ) {
            e.push_back(s);
            exp.push_back(e);
            e = {}; 
            s = ""; 
        } else if( str[i] == '+' ) { 
            e.push_back(s);
            s = ""; 
        } else {
            s.push_back(str[i]);
        }   
    }   
    return exp;
}

void test(std::vector< std::vector<std::string> > exp) {
    BDD bdd;

    std::map<std::string, Node*> mp;
    std::vector<Node *> ors;
    for( auto ex : exp ) {
        bool f = true;
        for( auto e : ex ) {
           if( mp.count(e) == 0 ) {
              mp[e] = bdd.Var(e);
           }

           if( f ) {
               f = false;
               ors.push_back(mp[e]);
           } else {
               ors.back() = Or( ors.back() , mp[e]);
           }
        }
    }
    Node * final = ors[0];
    for(int i=1; i<ors.size(); i++){
        final = And(final, ors[i]);
    }
    bdd.Tail_Pass(std::cout);
    // bdd.Tail_DumpDot(std::cout);
}
               

           


int main() {
    std::vector< std::vector<std::string> >  exp;
    // std::string str = "(P1+P2)(P1+P7+P9)(P3+P5+P6+P7)(P4+P5+P8)(P6+P7)(P7+P9+P10)(P8+P10)(P9+P11)(P12+P13)(P13+P15)(P14+P16)(P16+P19)(P17+P18+P19)";
    std::string str = "(P1+P2)(P2+P3)(P4+P5)";
    std::vector<std::string> s;
    exp = split(str);
    test(exp);
}

