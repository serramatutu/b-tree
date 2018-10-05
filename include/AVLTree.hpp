#ifndef AVL_TREE
#define AVL_TREE

#include <iostream>
#include "AVLTreeNode.hpp"

#include "useful.hpp"

template <typename T, 
          class Comparison = compare<T>>
class AVLTree {
    private:
        AVLTreeNode<T>* root;

    public:
        AVLTree();
        virtual ~AVLTree();
        AVLTree(const AVLTree& other);
        AVLTree<T, Comparison>& operator= (AVLTree<T, Comparison> other);
        AVLTree(AVLTree&& other);

        void insert(const T& data);
        bool remove (const T& data);

        int height();

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const AVLTree<U>& t);
};

template <typename T, class Comparison>
AVLTree<T, Comparison>::AVLTree() : root(nullptr) {}

template <typename T, class Comparison>
AVLTree<T, Comparison>::~AVLTree() {
    delete root;
    root = nullptr;
}

template <typename T, class Comparison>
AVLTree<T, Comparison>::AVLTree(const AVLTree& other) {
    if (other.root != nullptr)
        root = new AVLTreeNode<T>(other->root);
};

template <typename T, class Comparison>
AVLTree<T, Comparison>& AVLTree<T, Comparison>::operator=(AVLTree<T, Comparison> other) {
    std::swap(root, other.root);

    return *this;
}

template <typename T, class Comparison>
AVLTree<T, Comparison>::AVLTree(AVLTree&& other) : root(std::move(other.root)) {}

template <typename T, class Comparison>
void AVLTree<T, Comparison>::insert(const T& data) {
    if (root == nullptr)
        root = new AVLTreeNode<T>(data, root);
    else
        root->insert(data);
}

template <typename T, class Comparison>
bool AVLTree<T, Comparison>::remove(const T& data) {
    if (root == nullptr)
        return false;
    return root->remove(data);
}

template <typename T, class Comparison>
int AVLTree<T, Comparison>::height() {
    if (root == nullptr)
        return 0;
    return root->height(); 
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const AVLTree<T>& t) {
    os << "[";
    if (t.root != nullptr)
        os << *t.root;
    os << "]";

    return os;
}

#endif