#include <iostream>
#include <map>
#include <vector> 
#include <algorithm>
#include <fstream>
#include <chrono>

#include "BDD.h"

void print_vec(std::vector<std::string> str, bool f){
    for( auto s : str ) {
        std::cout << s << " ";
    }
    if( f ) { std::cout << std::endl; }
}

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


void test(std::vector<std::string> orders, std::vector< std::vector<std::string> > exp, std::ostream &out) {
    BDD bdd;
    std::map<std::string, Node*> mp;
    std::vector<Node *> ors;

    for( auto pr : orders ) {
        if( mp.count(pr) == 0 ) {
            mp[pr] = bdd.Var(pr);
        }
    }

    std::vector<Node *> final;
    bool f = true;
    for( auto ex : exp ) {
        std::vector<Node *> list;
        for( auto e : ex ){
            if(mp.count(e)==0){
                mp[e]=bdd.Var(e);
            }
            list.push_back(mp[e]);
        }

        if( f ) {
            f = false;
            final = list;
            continue;
        }
        final = AND(final, list);
    }

    // bdd.DumpPass(out);
    // bdd.DumpCountPath(out);
    bdd.DumpDot(out);
}


void count_path(int n, std::vector< std::vector<std::string> > exp, std::ostream &out) {
    std::vector<std::string> orders;

    for(int i=1; i<=n; i++) {
        orders.push_back( "P" + std::to_string(i) ); 
    }

    while (next_permutation(orders.begin(), orders.end())) {
        print_vec(orders,false);
        std::chrono::system_clock::time_point start, end;

        start = std::chrono::system_clock::now();
        test(orders,exp,out);
        end = std::chrono::system_clock::now();

        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << " " << elapsed << "ms" << std::endl;;

    }
}

void mes_time(std::vector<std::string> orders,std::vector< std::vector<std::string> > exp, std::ostream &outputfile, std::ostream &timefile) {
    std::chrono::system_clock::time_point start, end;

    start = std::chrono::system_clock::now();
    test(orders,exp,outputfile);
    end = std::chrono::system_clock::now();

    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    timefile << elapsed << "ms";
}

int main(int argc, char *argv[], char *envp[]) {
    std::vector< std::vector<std::string> >  exp;
    std::vector<std::string> orders;
    // orders = {"P7","P9","P5","P10","P8","P6","P1","P2","P3","P4"};
    orders = {"P1","P2","P3","P4","P5","P6"};

    int n = 10;
    std::string str = argv[1];
    std::ofstream outputfile(argv[2]);
    std::ofstream timefile(argv[3]);


    exp = split(str);



    // count_path(n,exp,std::cout);
    test(orders,exp,outputfile);
    // test2();
    // mes_time(orders, exp, outputfile, timefile);
}

