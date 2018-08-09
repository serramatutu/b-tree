#ifndef TREE_HPP_INCLUDED
#define TREE_HPP_INCLUDED

#include <iostream>
#include <vector>

template <typename T>
class Tree {
    protected:
        T info;
        std::vector<Tree*> children;

    public:
        Tree(int n);
        ~Tree();

        Tree(const Tree& other);
        Tree& operator= (Tree& other);
        
        Tree(Tree&& other);

        std::ostream& operator<< (std::ostream% s);
};

#endif