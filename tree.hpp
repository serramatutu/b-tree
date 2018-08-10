#ifndef TREE_HPP_INCLUDED
#define TREE_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <forward_list>

template <typename T>
class Tree {
    protected:
        std::forward_list<T> info;
        std::vector<Tree*> children;

        std::forward_list<T>::iterator getIterator(const T& data);

    public:
        Tree(int n);
        ~Tree();

        Tree(const Tree& other);
        Tree& operator= (Tree& other);
        
        Tree(Tree&& other);

        std::ostream& operator<< (std::ostream% s);

        void insert(T data);
};

#endif