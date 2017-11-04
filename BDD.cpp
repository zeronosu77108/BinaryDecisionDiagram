
#include "BDD.h"

Node* BDD::Var(const std::string &label) {
    var_table.push_back(var_table_entryt(label));
    Node* tmp = make(var_table.size(), &false_bdd, &true_bdd);
    true_bdd.add_parent(tmp);
    roots.push_back( tmp );
    return tmp;
}

Node* BDD::make(unsigned int var, Node *low, Node *high) {
    if(low->node_number == high->node_number) {
        return low;
    } else {
        reverse_keyt reverse_key(var, *low, *high);
        reverse_mapt::const_iterator it= reverse_map.find(reverse_key);
        
        if( it != reverse_map.end() ) {
            return it->second;
        } else {
            unsigned int new_number = nodes.back().node_number+1;
            nodes.push_back( Node(this, var, new_number, low, high) );
            reverse_map[reverse_key] = &nodes.back();
            return &nodes.back();
        }
    }
}

BDD::BDD() {
    nodes.push_back( Node(this,-1,0,NULL,NULL) );
    false_bdd = nodes.back();
    nodes.push_back( Node(this,-1,1,NULL,NULL) );
    true_bdd = nodes.back();
}

BDD::~BDD() {
}

void BDD::DumpDot(std::ostream &out) const {
    std::vector<Node *> done;

    out << "digraph BDD {" << std::endl;

    out << "center = true;" << std::endl;
    out << "margin = 0;" << std::endl;

    out << "  { node [shape=box,fontsize=24]; \"0\"; }" << std::endl;
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
        DumpDot_2(out,root,&done);
    }

    out << "}" << std::endl;
}

void BDD::DumpDot_2(std::ostream &out,Node *node, std::vector<Node *> *done ) const {
    auto itr = std::find(done->begin(), done->end(), node);

    if( itr == done->end() ) {
        done->push_back(node);
        if( node->low!=NULL||node->high!=NULL){
            out << node->node_number << "[label=\""
                << var_table[node->var-1].label << "\"]"
                << std::endl;

            out << "{rank=same; " << var_table[node->var-1].label << "; "
                << node->node_number << ";}"
                << std::endl;
        }

        if( node->low != NULL){
            out << node->node_number << "->"
                << node->low->node_number
                << "[style=dashed];"
                << std::endl; 
            DumpDot_2(out, node->low, done);
        }
        if( node->high != NULL){
            out << node->node_number << "->"
                << node->high->node_number
                << std::endl; 
            DumpDot_2(out,node->high, done);
        }
    }
}


void BDD::DumpPath(std::ostream &out) const {

    for(auto root : roots) {
        DumpPath_2(out, root, "");
    }
}

void BDD::DumpPath_2(std::ostream &out, Node *node, std::string str) const {
    if(node->low!=NULL) {
        DumpPath_2(out, node->low, str);
        // Path_2(out, node->low, done, var_table[node->var-1].label + " " + str);
    }
    if(node->high!=NULL) {
        DumpPath_2(out, node->high,
                var_table[node->var-1].label + " " + str);
        return;
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
    if(node->low!=NULL) {
        _DumpCountPath(node->low, n);
    }
    if(node->high!=NULL) {
        _DumpCountPath(node->high, n);
    }

    if(node->node_number==1) {
        *n = *n + 1;
    }
}

bool operator < (const BDD::reverse_keyt &x,
        const BDD::reverse_keyt &y) {
    if(x.var<y.var) return true;
    if(x.var>y.var) return false;
    if(x.low<y.low) return true;
    if(x.low>y.low) return false;
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
Node* And(Node *x, Node *y) {
    x->bdd->remove_root(x);
    y->bdd->remove_root(y);
    x->bdd->add_root(apply(and_fkt, x, y));
    return x->bdd->roots.back();
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

    // 両方定数ノード
    if( fkt == or_fkt && ( x->is_true() || y->is_true()) ) {
        u = &(bdd->true_bdd);
    } else if( fkt == and_fkt && ( x->is_false() || y->is_false()) ) {
        u = &(bdd->false_bdd);
    } else if(x->is_constant() && y->is_constant()) {
        u=&(fkt(x->is_true(), y->is_true())? bdd->true_bdd : bdd->false_bdd);
    } else if(x->var==y->var) {
        u=bdd->make(x->var,
                apply(fkt, x->low, y->low),
                apply(fkt, x->high, y->high)
                );  
    } else if(x->var<y->var) {
        u=bdd->make(x->var,
                apply(fkt, y, x->low), 
                apply(fkt, y, x->high)
                );  
    } else /* x->var() > y->var() */ {
        u=bdd->make(y->var,
                apply(fkt, x, y->low),
                apply(fkt, x, y->high)
                );  
    }   

    return u;
}
