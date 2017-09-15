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
        Node *low; 
        Node *high;
        inline Node(
                class BDD *_bdd,
                unsigned int _var,
                unsigned int _node_number,
                Node *_low,
                Node *_high);

        inline Node &operator=(Node &x);

        friend class BDD;
        
        BDD *bdd;
        unsigned int var, node_number, reference_counter;
        std::vector<Node> parent;

};

class BDD {
    friend class Node;
    public:
        BDD();
        ~BDD();

        Node Var(const std::string &label);

        void DumpDot(std::ostream &out) const;

        inline const BDD &True();
        inline const BDD &False();

    protected:
        typedef std::list<Node> nodest;
        nodest nodes;
        Node true_bdd, false_bdd;

        struct var_table_entryt {
            std::string label;
            inline var_table_entryt(const std::string &_label);
        };

        typedef std::vector<var_table_entryt> var_tablet;
        var_tablet var_table;

        struct reverse_keyt {
            unsigned var, low, high;
            inline reverse_keyt(
                    unsigned int _var, Node *_low, Node *high);
        };

        typedef std::map<reverse_keyt, Node*> reverse_mapt;
        reverse_mapt reverse_map;

        friend bool operator < (const reverse_keyt &x, const reverse_keyt &y);

        Node* make(unsigned int var, Node *low, Node *high);
        friend BDD apply(bool (*fkt)(bool x, bool y), BDD &x, BDD &y);

};

BDD apply(bool (*fkt)(bool x, bool y), const BDD &x, const BDD &y);

// inline functions
#include "BDD.inc"

#endif
