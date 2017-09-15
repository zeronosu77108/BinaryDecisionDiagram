// inline functions

Node::Node():low(NULL),high(NULL){

}

Node &Node::operator=(Node &x)
{
  /* bdd = x.bdd; */
  /* var = x.var; */
  /* node_number = x.node_number; */
  /* low = x.low; */
  /* high = x.high; */

  return *this;
  /* return Node(x.bdd, */
  /*               x.var, x.node_number, */
  /*               x.low, x.high); */
}

Node::Node(
    BDD *_bdd,
    unsigned int _var, unsigned int _node_number,
    Node *_low, Node *_high):
    bdd(_bdd), var(_var), node_number(_node_number),
    low(_low), high(_high), reference_counter(0){
}

Node::~Node(){}


BDD::reverse_keyt::reverse_keyt(
    unsigned _var, Node *_low, Node *_high):
    var(_var), low(_low->node_number), high(_high->node_number) {
}
