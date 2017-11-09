
#include "BDD.h"

Node* BDD::Var(const std::string &label) {
    var_table.push_back(var_table_entryt(label));
    Node* tmp = make(var_table.size(), &true_bdd);
    true_bdd.add_parent(tmp);
    roots.push_back( tmp );
    return tmp;
}

Node* BDD::make(unsigned int var, Node *high) {
    // for(auto itr=roots.begin(); itr!=roots.end(); itr++){
    //     if( (*itr)->var == var ) {
    //         (*itr)->high.push_back(high);
    //         return *itr;
    //     }
    // }

    reverse_keyt reverse_key(var, *high);
    reverse_mapt::const_iterator it= reverse_map.find(reverse_key);

    // auto itr = std::find(roots.begin(), roots.end(), it->second);
    // if( it != reverse_map.end() && itr==roots.end()) {
    if( it != reverse_map.end() ) {
        return it->second;
    } else {
        unsigned int new_number = nodes.back().node_number+1;
        nodes.push_back( Node(this, var, new_number, high) );
        reverse_map[reverse_key] = &nodes.back();
        return &nodes.back();
    }
}

BDD::BDD() {
    nodes.push_back( Node(this,-1,1,NULL) );
    true_bdd = nodes.back();
}

BDD::~BDD() {
}
void BDD::DumpDot(std::ostream &out) const {
    std::vector<Node *> done;

    out << "digraph BDD {" << std::endl;

    out << "center = true;" << std::endl;
    out << "margin = 0;" << std::endl;

    out << "  { node [shape=box,fontsize=24]; \"1\"; }" << std::endl
        << std::endl;

    std::string str;
    bool f = false;
    for( auto v : var_table ) {
        if(f){
            str += "->";
        }
        f=true;
        out << v.label << "[style=invis];";
        str += v.label;
    }
    out << std::endl << str << "[style=invis]" << std::endl;;

    for(auto root : roots){
        out << root->node_number
            << "[shape=doublecircle]";
        DumpDot_2(out,root,&done);
    }

    out << "}" << std::endl;
}

void BDD::DumpDot_2(std::ostream &out,Node *node, std::vector<Node *> *done ) const {
    auto itr = std::find(done->begin(), done->end(), node);

    if( itr == done->end() ) {
        done->push_back(node);
        for( auto h : node->high ) {
            if( h!=NULL){
                out << node->node_number << "[label=\""
                    << var_table[node->var-1].label << "\"]"
                    << std::endl;

                out << "{rank=same; " << var_table[node->var-1].label << "; "
                    << node->node_number << ";}"
                    << std::endl;


                out << node->node_number << "->"
                    << h->node_number
                    << std::endl; 

                DumpDot_2(out,h, done);
            }
        }
    }
}


void BDD::DumpPath(std::ostream &out) const {
    for(auto root : roots) {
        DumpPath_2(out, root, "");
    }
}

void BDD::DumpPath_2(std::ostream &out, Node *node, std::string str) const {
    // std::clog << node->high.size() << std::endl;
    for(auto h : node->high) {
        if(h != NULL) {
            DumpPath_2(out, h, str + var_table[node->var-1].label + " ");
        }
    }

    if(node->node_number==1)
        out << str << std::endl;
}
void BDD::DumpCountPath(std::ostream &out) const {
    int n = 0;
    for(auto root : roots) {
        _DumpCountPath(root, &n);
    }
    out << n << " ";
}

void BDD::_DumpCountPath(Node *node, int *n) const {
    // if(node->low!=NULL) {
    //     _DumpCountPath(node->low, n);
    // }
    // if(node->high!=NULL) {
    //     _DumpCountPath(node->high, n);
    // }
    //
    // if(node->node_number==1) {
    //     *n = *n + 1;
    // }
}

void BDD::DumpTailPath(std::ostream &out) const {
    for( auto parent : true_bdd.parents ) {
        DumpTailPath2(out, parent, "", &true_bdd);
    }
}

void BDD::DumpTailPath2(std::ostream &out, Node *node, std::string str,const Node *child) const {
    // if( node->low == child) {
    //     // str = var_table[node->var-1].label + "'" + str;
    // } else {
    //     str = var_table[node->var-1].label + " " + str;
    // }
    //
    // if( node->parents.size() <= 0) {
    //     out << str << std::endl;
    //     return;
    // }
    //
    // for( auto parent : node->parents) {
    //     DumpTailPath2(out, parent, str, node);
    // }
}


void BDD::DumpTailDot2(std::ostream &out, Node *node, std::vector<Node *> *done, const Node *child) const {
    // auto itr = std::find(done->begin(), done->end(), node);
    //
    // if( itr == done->end() ) {
    //     done->push_back(node);
    //     out << node->node_number << "[label=\""
    //         << var_table[node->var-1].label << "\"]"
    //         << std::endl;
    //
    //     out << "{rank=same; " << var_table[node->var-1].label << "; "
    //         << node->node_number << ";}"
    //         << std::endl;
    //
    //     out << node->node_number << "->"
    //         << node->low->node_number
    //         << "[style=dashed];"
    //         << std::endl; 
    //
    //     out << node->node_number << "->"
    //         << node->high->node_number
    //         << std::endl; 
    // }
    //
    // for( auto parent : node->parents) {;
    //     DumpTailDot2(out, parent, done, node);
    // }
}

bool operator < (const BDD::reverse_keyt &x,
        const BDD::reverse_keyt &y) {
    if(x.var<y.var) return true;
    if(x.var>y.var) return false;
    // if(x.low<y.low) return true;
    // if(x.low>y.low) return false;
    return x.high<y.high;
}


bool equal_fkt(bool x, bool y) {
    return x==y;
}
bool xor_fkt(bool x, bool y) {
    return x^y;
}
bool and_fkt(bool x, bool y) {
    return x&&y;
}
bool or_fkt(bool x, bool y) {
    return x||y;
}

Node Node::operator ==(Node &other) {
    return *apply(equal_fkt, this, &other);
}
Node Node::operator ^(Node &other) {
    return *apply(xor_fkt, this, &other);
}
Node Node::operator !() {
    return bdd->true_bdd ^ *this;
}
Node Node::operator &(Node &other) {
    return *apply(and_fkt, this, &other);
}
Node Node::operator |(Node &other) {
    return *apply(or_fkt, this, &other);
}
Node* Not(Node *x) {
    x->bdd->remove_root(x);
    x->bdd->add_root(apply(xor_fkt, &x->bdd->true_bdd, x));
    return x->bdd->roots.back();
}
std::vector<Node *> AND(std::vector<Node *> xl, std::vector<Node *> yl) {
    std::vector<Node *> head;
    bool f = true;
    for( auto x : xl) {
        for( auto y : yl) {
            x->bdd->remove_root(x);
            y->bdd->remove_root(y);
            head.push_back(apply(and_fkt, x,y));
        }
        f = false;
    }
    for( auto tmp : head ){ 
        xl[0]->bdd->add_root( tmp );
    }
    return head;
}
Node* Or(Node *x, Node *y) {
    x->bdd->remove_root(x);
    y->bdd->remove_root(y);
    x->bdd->add_root(apply(or_fkt, x, y));
    return x->bdd->roots.back();
}


Node* apply(bool (*fkt)(bool x, bool y), Node *x, Node *y) {
    BDD *bdd = x->bdd;
    Node *u;
    // if( x!=NULL && !x->is_constant())
    // std::clog << bdd->var_table[x->var-1].label;
    // std::clog << " & ";
    // if( y!=NULL && !y->is_constant())
    // std::clog << bdd->var_table[y->var-1].label;
    // std::clog << std::endl;

    // 両方定数ノード
    if(x->is_constant() && y->is_constant()) {
        u=(fkt(x->is_true(), y->is_true())? &bdd->true_bdd : NULL);
    } else if(x->var==y->var) {
        for(auto xh : x->high ) {
            for(auto yh : x->high ) {
                if( xh == NULL || yh == NULL ){
                    u = NULL;
                } else {
                    u=bdd->make(x->var,apply(fkt, xh, yh));  
                }
            }
        }
    } else if(x->var<y->var) {
        for(auto xh : x->high ) {
            if( xh == NULL) {
                u = NULL;
            } else {
                u=bdd->make(x->var,apply(fkt, xh, y));  
            }
        }
    } else /* x->var() > y->var() */ {
        for(auto yh : y->high ) {
            if( yh == NULL) {
                u = NULL;
            } else {
                u=bdd->make(y->var,apply(fkt, x, yh));
            }
        }
    }   

    return u;
}

