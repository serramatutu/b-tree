#ifndef TREE_HPP
#define TREE_HPP

// usados pela definição
#include <iostream>
#include <vector>

// usados pela implementação
#include <algorithm>
#include <utility>

template <typename T>
class Tree {
    protected:
        std::vector<T> info;
        std::vector<Tree*> children;

        typename std::vector<T>::iterator getIterator(const T& data);

    public:
        Tree(int n);
        ~Tree();

        Tree(const Tree& other);
        Tree<T>& operator= (Tree other);
        
        Tree(Tree&& other);

        void insert(T data);

        template <typename U> friend std::ostream& operator<<(std::ostream& os, const Tree<U>& t);
};

template <typename T>
Tree<T>::Tree(int n) {
    info.reserve(n);
    children.resize(n+1, nullptr);
}

template <typename T>
Tree<T>::~Tree() {
    for (auto it = children.begin(); it != children.end(); it++) {
        if (*it != nullptr) {
            delete *it;
            *it = nullptr;
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
Tree<T>& Tree<T>::operator=(Tree other) {
    std::swap(info, other.info);
    std::swap(children, other.children);
    return *this;
}

template <typename T>
Tree<T>::Tree(Tree&& other) 
    : info(std::move(other.info)), children(std::move(other.children))
    {}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Tree<T>& t) {
    os << "(";
    for (int i=0; i<t.info.size(); i++) {
        if (t.children[i] != nullptr) // printa a esquerda
            os << *t.children[i];

        os << " " << t.info[i] << " "; // printa o valor
    }

    if (t.children.back() != nullptr) // printa a direita do ultimo
        os << *t.children.back();

    os << ")";
    return os;
}

template <typename T>
typename std::vector<T>::iterator Tree<T>::getIterator(const T& data) {
    auto it = info.begin();
    while (it != info.end() && *it < data)
        it++;
    return it;
}

template <typename T>
void Tree<T>::insert(T data) {
    auto it = getIterator(data);
    if (info.size() >= children.size() - 1) { // se o vetor de dados já está cheio
        int index = it - info.begin();
        if (children[index] == nullptr)
            children[index] = new Tree(children.size() - 1);
        children[index]->insert(data);
    }
    else {
        info.insert(it, data);
    }
}

#endif