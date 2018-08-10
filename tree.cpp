#include "tree.hpp"

#include <algorithm>
#include <utility>

template <typename T>
Tree<T>::Tree(int n) {
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
    s << "(";
    for (auto it = info.cbegin(); it != info.cend(); it++) {
        s << "[" << *it << "]";
        if (children[it - info.cbegin()] != nullptr)
            s << *children[it - info.cbegin()];
    }
    // printa o ultimo
    if (children.back() != null)
        s << children.back();

    s << ")";
    return s;
}

template <typename T>
std::forward_list<T>::iterator Tree<T>::getIterator(const T& data) {
    auto it = info.begin();
    while (it != info.end() && *it < data)
        it++;
    return it;
}

template <typename T>
void Tree<T>::insert(T data) {
    auto it = getIterator(data);
    if (info.size() == children.count()) { // se o vetor de dados já está cheio
        int index = it - info.begin();
        if (children[index] == nullptr)
            children[index] = new Tree(children.size());
        children[index].insert(data);
    }
    else {
        info.insert_after(it, data);
    }
}