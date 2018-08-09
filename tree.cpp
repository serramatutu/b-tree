#include "tree.hpp"

#include <algorithm>
#include <utility>

template <typename T>
Tree<T>::Tree(T info, int n) : info(info) {
    children.resize(n);
}

template <typename T>
Tree<T>::~Tree() {
    for (auto it = children.begin(); it != children.end(); it++) {
        if (*it != nullptr) {
            delete **it;
            *t = nullptr;
        }
    }
}

// magia
template <typename T>
Tree<T>::Tree(const Tree& other) : info(other.info) {
    children.resize(other.children.size());
    for (int i=0; i<other.children.size(); i++) {
        Tree* current = other.children[i];
        if (current != nullptr)
            children[i] = new Tree(*current);
    }
}

template <typename T>
Tree& Tree<T>::operator=(Tree other) {
    std::swap(info, other.info);
    std::swap(children, other.children);
    return *this;
}

template <typename T>
Tree<T>::~Tree(Tree&& other) 
    : info(std::move(other.info)), children(std::move(other.children))
    {}

template <typename T>
std::ostream& Tree<T>::operator<<(std::ostream& s) {
    s << "( " << info << " ";
    for (Tree* t : children)
        if (t != nullptr)
            s << *t;
    s << ")";
    return s;
}