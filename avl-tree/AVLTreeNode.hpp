#ifndef AVL_TREE_NODE
#define AVL_TREE_NODE

#include <iostream>
#include <cmath>

template <typename T>
class AVLTreeNode {
    private:
        AVLTreeNode<T>* left;
        AVLTreeNode<T>* right;
        T data;

        int lastHeight; // the last calculated height
        int lheight();
        int rHeight();

        int balanceFactor();

        void insert(const T& data, AVLTreeNode<T>*& pptr);

    public:
        AVLTreeNode(const T& data);
        virtual ~AVLTreeNode();
        AVLTreeNode(const AVLTreeNode& other);
        AVLTreeNode<T>& operator= (AVLTreeNode<T> other);
        AVLTreeNode(AVLTreeNode&& other);

        int height();
        bool balanced();

        void insert(const T& data);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const AVLTreeNode<U>& t);

    template <typename U>
    friend AVLTreeNode<U>& balance(AVLTreeNode<U>& root);

    template <typename U>
    friend AVLTreeNode<U>& rRotate(AVLTreeNode<U>& root);

    template <typename U>
    friend AVLTreeNode<U>& lRotate(AVLTreeNode<U>& root);
};

template <typename T>
AVLTreeNode<T>::AVLTreeNode(const T& data) : left(nullptr), right(nullptr), data(data), lastHeight(1) 
{}

template <typename T>
AVLTreeNode<T>::~AVLTreeNode() {
    delete right;
    right = nullptr;
    delete left;
    left = nullptr;
}

template <typename T>
AVLTreeNode<T>::AVLTreeNode(const AVLTreeNode& other) : data(other.data) {
    if (other.right != nullptr)
        right = new AVLTreeNode(other->right);
    if (other.left != nullptr)
        left = new AVLTreeNode(other->left);
}

template <typename T>
AVLTreeNode<T>& AVLTreeNode<T>::operator=(AVLTreeNode<T> other) {
    std::swap(data, other.data);
    std::swap(right, other.right);
    std::swap(left, other.left);
    return *this;
}

template <typename T>
AVLTreeNode<T>::AVLTreeNode(AVLTreeNode&& other)
    : right(std::move(other.right)),
      left(std::move(other.left)),
      data(std::move(other.data))
      {}

template <typename T>
void AVLTreeNode<T>::insert(const T& data) {
    if (data < this->data)
        insert(data, right);
    else
        insert(data, left);
}

template <typename T>
void AVLTreeNode<T>::insert(const T& data, AVLTreeNode<T>*& pptr) {
    if (pptr == nullptr)
        pptr = new AVLTreeNode(data);
    else {
        pptr->insert(data);
        pptr = &balance(*pptr);
    }

    // recalculates the height at this node
    int newHeight = pptr->height() + 1;
    if (newHeight > lastHeight)
        lastHeight = newHeight;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const AVLTreeNode<T>& t) {
    os << "(";
    if (t.right != nullptr)
        os << *t.right;
    os << t.data;
    if (t.left != nullptr)
        os << *t.left;
    os << ")";

    return os;
}

template <typename T>
int AVLTreeNode<T>::height() {
    return lastHeight;
}

template <typename T>
int AVLTreeNode<T>::lheight() {
    if (left == nullptr)
        return 0;
    return left->lastHeight;
}

template <typename T>
int AVLTreeNode<T>::rHeight() {
    if (right == nullptr)
        return 0;
    return right->lastHeight;
}

template <typename T>
int AVLTreeNode<T>::balanceFactor() {
    int l = left == nullptr ? 0 : left->height(),
        r = right == nullptr ? 0 : right->height();

    return r - l;
}

template <typename T>
bool AVLTreeNode<T>::balanced() {
    return std::abs(balanceFactor()) <= 1;
}

template <typename T>
AVLTreeNode<T>& balance(AVLTreeNode<T>& root) {
    int balanceFactor = root.balanceFactor();
    if (balanceFactor > 1) { // right-heavy
        if (root.right->balanceFactor() <= -1) // RL case
            root.right = &rRotate(*root.right);
        return lRotate(root);
    }
    else if (balanceFactor < -1) { // left-heavy
        if (root.left->balanceFactor() >= 1 ) // LR case
            root.left = &lRotate(*root.left);
        return rRotate(root);
    }

    return root; // in case the tree is already balanced
}

template <typename T>
AVLTreeNode<T>& rRotate(AVLTreeNode<T>& root) {
    AVLTreeNode<T>& newRoot = *root.left;
    root.left = newRoot.right;
    newRoot.right = &root;

    if (root.lheight() + 1 > newRoot.height())
        newRoot.lastHeight = root.lheight() + 1;

    return newRoot;
}

template <typename T>
AVLTreeNode<T>& lRotate(AVLTreeNode<T>& root) {
    AVLTreeNode<T>& newRoot = *root.right;
    root.right = newRoot.left;
    newRoot.left = &root;

    if (root.rHeight() + 1 > newRoot.height())
        newRoot.lastHeight = root.rHeight() + 1;

    return newRoot;
}

#endif