#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

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

void test(std::vector< std::vector<std::string> > exp, std::ostream &out) {
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
    bdd.Tail_Pass(out);
    // bdd.Tail_DumpDot(out);
}
               
void test2() {
    BDD bdd;
    Node *A = bdd.Var("A");
    Node *B = bdd.Var("B");
    Node *C = bdd.Var("C");

    Node * final = And( And(A,B), Not(C));

    bdd.Tail_DumpDot(std::cout);
}
           


int main(int argc, char *argv[], char *envp[]) {
    std::vector< std::vector<std::string> >  exp;
    std::string str = argv[1];
    std::ofstream outputfile(argv[2]);
    std::ofstream timefile(argv[3]);
    std::chrono::system_clock::time_point start, end;

    exp = split(str);


    start = std::chrono::system_clock::now();

    test(exp,outputfile);
    end = std::chrono::system_clock::now();

    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    timefile << elapsed << "ms";
}

