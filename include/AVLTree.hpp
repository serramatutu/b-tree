#ifndef AVL_TREE
#define AVL_TREE

#include <iostream>
#include <iterator>

#include "AVLTreeNode.hpp"
#include "useful.hpp"

template <typename T, 
          class Less = std::less<T>>
class AVLTree {
    private:
        AVLTreeNode<T, Less>* root;

    public:
        typedef typename AVLTreeNode<T, Less>::const_iterator const_iterator;
        typedef typename AVLTreeNode<T, Less>::iterator iterator;

        AVLTree();
        virtual ~AVLTree();
        AVLTree(const AVLTree& other);
        AVLTree<T, Less>& operator=(AVLTree<T, Less> other);
        AVLTree(AVLTree&& other);

        void insert(const T& data);
        bool remove(const T& data);

        const_iterator find(const T& data) const;
        iterator find(const T& data);

        int height() const;
        const_iterator cbegin() const;
        const_iterator cend() const;
        iterator begin();
        iterator end();

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
        root = new AVLTreeNode<T, Less>(other->root);
};

template <typename T, class Less>
AVLTree<T, Less>& AVLTree<T, Less>::operator=(AVLTree<T, Less> other) {
    std::swap(root, other.root);

    return *this;
}

template <typename T, class Less>
AVLTree<T, Less>::AVLTree(AVLTree&& other) : root(std::move(other.root)) {}

template <typename T, class Less>
typename AVLTree<T, Less>::const_iterator AVLTree<T, Less>::cbegin() const {
    if (root != nullptr)
        return root->cbegin();
    return AVLTree<T, Less>::const_iterator();
}

template <typename T, class Less>
typename AVLTree<T, Less>::const_iterator AVLTree<T, Less>::cend() const {
    if (root != nullptr)
        return root->cend();
    return AVLTree<T, Less>::const_iterator();
}

template <typename T, class Less>
typename AVLTree<T, Less>::iterator AVLTree<T, Less>::begin() {
    if (root != nullptr)
        return root->begin();
    return AVLTree<T, Less>::iterator();
}

template <typename T, class Less>
typename AVLTree<T, Less>::iterator AVLTree<T, Less>::end() {
    if (root != nullptr)
        return root->end();
    return AVLTree<T, Less>::iterator();
}

template <typename T, class Less>
void AVLTree<T, Less>::insert(const T& data) {
    if (root == nullptr)
        root = new AVLTreeNode<T, Less>(data, root);
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
typename AVLTree<T, Less>::iterator AVLTree<T, Less>::find(const T& data) {
    if (root == nullptr)
        return end();
    return root->find(data);
}

template <typename T, class Less>
typename AVLTree<T, Less>::const_iterator AVLTree<T, Less>::find(const T& data) const {
    return find(data);
}

template <typename T, class Less>
int AVLTree<T, Less>::height() const {
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