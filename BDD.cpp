
#include "BDD.h"

Node BDD::Var(const std::string &label) {
    return Node();
}

Node* BDD::make(unsigned int var, Node *low, Node *high) {
    
    if(low->node_number == high->node_number) {
        return low;
    } else {
        reverse_keyt reverse_key(var, low, high);
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

bool operator < (const BDD::reverse_keyt &x,
        const BDD::reverse_keyt &y) {
    if(x.var<y.var) return true;
    if(x.var>y.var) return false;
    if(x.low<y.low) return true;
    if(x.low>y.low) return false;
    return x.high<y.high;
}
