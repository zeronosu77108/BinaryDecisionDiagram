/*
 * A mini BDD library.
 *
 */

#ifndef BDD_H
#define BDD_H

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

class Node {
    public:
        inline Node();
        inline ~Node();
        std::vector<Node *>high;
        inline Node(
                class BDD *_bdd,
                unsigned int _var,
                unsigned int _node_number,
                Node *_high);

        Node operator !();
        Node operator ^(Node &);
        Node operator ==(Node &);
        Node operator &(Node &);
        Node operator |(Node &);
        

        inline Node &operator=(Node &x);


        friend class BDD;
        
        BDD *bdd;
        unsigned int var, node_number, reference_counter;
        std::vector<Node*> parents;

        inline bool is_constant() const;
        inline bool is_true() const;
        inline bool is_false() const;

        inline void add_parent(Node *_parent);
        inline void remove_parent(Node *_parent);
        friend Node* apply(bool (*fkt)(bool x, bool y), Node *x, Node *y);

};

class BDD {
    public:
        BDD();
        ~BDD();

        Node* Var(const std::string &label);

        void DumpDot(std::ostream &out) const;
        void DumpDot_2(std::ostream &out, Node *node, std::vector<Node *> *done) const;
        void DumpPath(std::ostream &out) const;
        void DumpPath_2(std::ostream &out, Node *node, std::string str) const;
        void DumpTailPath(std::ostream &out) const;
        void DumpTailPath2(std::ostream &out, Node *node, std::string str,const Node *child) const;
        void DumpCountPath(std::ostream &out) const;
        void _DumpCountPath(Node *node, int *n) const;
        void DumpTailDot(std::ostream &out) const;
        void DumpTailDot2(std::ostream &out, Node *node, std::vector<Node *> *done, const Node *child) const;

        inline const BDD &True();
        inline const BDD &False();
        inline void add_root(Node *_root) ;
        inline void remove_root(Node *_root) ;

        friend class Node;

    // protected:
        typedef std::list<Node> nodest;
        Node true_bdd, false_bdd;
        nodest nodes;
        std::vector<Node *> roots;

        struct var_table_entryt {
            std::string label;
            inline var_table_entryt(const std::string &_label);
        };

        typedef std::vector<var_table_entryt> var_tablet;
        var_tablet var_table;

        struct reverse_keyt {
            unsigned var, high;
            inline reverse_keyt(
                    unsigned int _var, Node &high);
        };

        typedef std::map<reverse_keyt, Node*> reverse_mapt;
        reverse_mapt reverse_map;

        friend bool operator < (const reverse_keyt &x, const reverse_keyt &y);

        Node* make(unsigned int var, Node *high);
        friend Node* apply(bool (*fkt)(bool x, bool y), Node *x, Node *y, bool flag);

};

// BDD apply(bool (*fkt)(bool x, bool y), const BDD &x, const BDD &y);
Node* Not(Node *x);
std::vector<Node *> AND(std::vector<Node *> x, std::vector<Node *> y);
Node* Or(Node *x, Node *y);

#define forall_nodes(it) for(nodest::const_iterator it=nodes.begin(); \
        it!=nodes.end(); it++)

// inline functions
#include "BDD.inc"

#endif
