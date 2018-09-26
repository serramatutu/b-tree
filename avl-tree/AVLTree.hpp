#ifndef AVL_TREE
#define AVL_TREE

#include <iostream>
#include "AVLTreeNode.hpp"

template <typename T>
class AVLTree {
    private:
        AVLTreeNode<T>* root;

    public:
        AVLTree();
        virtual ~AVLTree();
        AVLTree(const AVLTree& other);
        AVLTree<T>& operator= (AVLTree<T> other);
        AVLTree(AVLTree&& other);

        void insert(const T& data);

        int height();

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const AVLTree<U>& t);
};

template <typename T>
AVLTree<T>::AVLTree() : root(nullptr) {}

template <typename T>
AVLTree<T>::~AVLTree() {
    delete root;
    root = nullptr;
}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree& other) {
    if (other.root != nullptr)
        root = new AVLTreeNode<T>(other->root);
};

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(AVLTree<T> other) {
    std::swap(root, other.root);

    return *this;
}

template <typename T>
AVLTree<T>::AVLTree(AVLTree&& other) : root(std::move(other.root)) {}

template <typename T>
void AVLTree<T>::insert(const T& data) {
    if (root == nullptr)
        root = new AVLTreeNode<T>(data);
    else {
        root->insert(data);
        root = &balance(*root);
    }
}

template <typename T>
int AVLTree<T>::height() {
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