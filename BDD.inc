// inline functions

Node::Node():low(NULL),high(NULL){
}

Node &Node::operator=(Node &x)
{
  bdd = x.bdd;
  var = x.var;
  node_number = x.node_number;
  low = x.low;
  high = x.high;
  reference_counter = x.reference_counter;
  return *this;
}

Node::Node(
    BDD *_bdd,
    unsigned int _var, unsigned int _node_number,
    Node *_low, Node *_high):
    bdd(_bdd), var(_var), node_number(_node_number),
    low(_low), high(_high), reference_counter(1){
    /* reference_counter(0){ */
}

Node::~Node(){}

void Node::add_parent(Node * _parent) {
    std::vector<Node *>::iterator itr;
    itr = std::find(parents.begin(), parents.end(), _parent);

    if( itr == parents.end() ) {
        parents.push_back(_parent);
    }
}

void Node::remove_parent(Node *_parent) {
    std::vector<Node *>::iterator itr;
    itr = std::find(parents.begin(), parents.end(), _parent);
    
    if( itr != parents.end() ) {
        parents.erase(itr);
    }
}

bool Node::is_constant() const{
    return node_number<=1;
}

bool Node::is_true() const{
    return node_number==1;
}

bool Node::is_false() const{
    return node_number==0;
}

/*
* BDD class inline functions
*/

BDD::var_table_entryt::var_table_entryt(const std::string &_label):label(_label){}

BDD::reverse_keyt::reverse_keyt(
    unsigned _var, Node &_low, Node &_high):
    var(_var), low(_low.node_number), high(_high.node_number) {
}

void BDD::add_root(Node *_root) {
    std::vector<Node *>::iterator itr;
    itr = std::find(roots.begin(), roots.end(), _root);

    if( itr == roots.end() ) {
        roots.push_back(_root);
    }
}

void BDD::remove_root(Node *_root) {
    /* std::clog << roots.size() << std::endl; */
    std::vector<Node *>::iterator itr;
    itr = std::find(roots.begin(), roots.end(), _root);
    
    if( itr != roots.end() ) {
        roots.erase(itr);
        /* std::clog << "remove root" << std::endl; */
        /* std::clog << roots.size() << std::endl; */
    }
}
