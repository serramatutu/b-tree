#ifndef AVL_TREE
#define AVL_TREE

#include <iostream>
#include "AVLTreeNode.hpp"

#include "useful.hpp"

template <typename T, 
          class Less = std::less<T>>
class AVLTree {
    private:
        AVLTreeNode<T, Less>* root;

    public:
        AVLTree();
        virtual ~AVLTree();
        AVLTree(const AVLTree& other);
        AVLTree<T, Less>& operator= (AVLTree<T, Less> other);
        AVLTree(AVLTree&& other);

        void insert(const T& data);
        bool remove(const T& data);

        int height();

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const AVLTree<U>& t);
};

template <typename T, class Less>
AVLTree<T, Less>::AVLTree() : root(nullptr) {}

template <typename T, class Less>
AVLTree<T, Less>::~AVLTree() {
    delete root;
    root = nullptr;
}

template <typename T, class Less>
AVLTree<T, Less>::AVLTree(const AVLTree& other) {
    if (other.root != nullptr)
        root = new AVLTreeNode<T>(other->root);
};

template <typename T, class Less>
AVLTree<T, Less>& AVLTree<T, Less>::operator=(AVLTree<T, Less> other) {
    std::swap(root, other.root);

    return *this;
}

template <typename T, class Less>
AVLTree<T, Less>::AVLTree(AVLTree&& other) : root(std::move(other.root)) {}

template <typename T, class Less>
void AVLTree<T, Less>::insert(const T& data) {
    if (root == nullptr)
        root = new AVLTreeNode<T>(data, root);
    else
        root->insert(data);
}

template <typename T, class Less>
bool AVLTree<T, Less>::remove(const T& data) {
    if (root == nullptr)
        return false;
    return root->remove(data);
}

template <typename T, class Less>
int AVLTree<T, Less>::height() {
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