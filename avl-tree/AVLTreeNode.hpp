#ifndef AVL_TREE_NODE
#define AVL_TREE_NODE

#include <iostream>
#include <cmath>
#include <algorithm>

template <typename T>
class AVLTreeNode {
    private:
        AVLTreeNode<T>* left;
        AVLTreeNode<T>* right;
        T& data;

        unsigned int lastHeight; // the last calculated height
        unsigned int lHeight();
        unsigned int rHeight();

        int balanceFactor();

        void balance();
        void rRotate();
        void lRotate();

        void insert(const T& data, AVLTreeNode<T>*& pptr);
        AVLTreeNode<T>& removeHighestLeaf();

    public:
        AVLTreeNode(const T& data);
        virtual ~AVLTreeNode();
        AVLTreeNode(const AVLTreeNode& other);
        AVLTreeNode<T>& operator= (AVLTreeNode<T> other);
        AVLTreeNode(AVLTreeNode&& other);

        // Note: does NOT pop from itself if the data is in itself
        T& popMin();
        T& popMax();

        bool isLeaf();

        unsigned int height();
        bool balanced();

        void insert(const T& data);
        bool remove (const T& data);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const AVLTreeNode<U>& t);

    template <typename U>
    friend void swap(AVLTreeNode<U>& a, AVLTreeNode<U>& b);
};

template <typename T>
void swap(AVLTreeNode<T>& a, AVLTreeNode<T>& b) {
    std::swap(a.data, b.data);
    std::swap(a.right, b.right);
    std::swap(a.left, b.left);
}

template <typename T>
AVLTreeNode<T>::AVLTreeNode(const T& data) : left(nullptr), right(nullptr), data(*new T(data)), lastHeight(1) 
{}

template <typename T>
AVLTreeNode<T>::~AVLTreeNode() {
    delete right;
    right = nullptr;
    delete left;
    left = nullptr;
}

template <typename T>
AVLTreeNode<T>::AVLTreeNode(const AVLTreeNode& other) : data(new T(other.data)) {
    if (other.right != nullptr)
        right = new AVLTreeNode(other->right);
    if (other.left != nullptr)
        left = new AVLTreeNode(other->left);
}

template <typename T>
AVLTreeNode<T>& AVLTreeNode<T>::operator=(AVLTreeNode<T> other) {
    swap(*this, other);
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
        insert(data, left);
    else
        insert(data, right);

    balance();
}

template <typename T>
void AVLTreeNode<T>::insert(const T& data, AVLTreeNode<T>*& ptr) {
    if (ptr == nullptr)
        ptr = new AVLTreeNode(data);
    else
        ptr->insert(data);

    // recalculates the height at this node
    unsigned int newHeight = ptr->height() + 1;
    if (newHeight > lastHeight)
        lastHeight = newHeight;
}

template <typename T>
bool AVLTreeNode<T>::remove(const T& data) {
    if (data > this.data) {
        if (right == nullptr)
            return false;

        if (right->data != data)
            return right->remove(data);

        right->data = right->popMin();
        return true;
    }
    else if (data < this.data) {
        if (left == nullptr)
            return false;
        if (*left->data != data)
            return left->remove(data);

        left->data = left->popMax();
        return true;
    }
    
    return false;
}

template <typename T>
T& AVLTreeNode<T>::popMin() {
    
}

template <typename T>
T& AVLTreeNode<T>::popMax() {
    
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const AVLTreeNode<T>& t) {
    os << "(";
    if (t.left != nullptr)
        os << *t.left;
    os << t.data;
    if (t.right != nullptr)
        os << *t.right;
    os << ")";

    return os;
}

template <typename T>
unsigned int AVLTreeNode<T>::height() {
    return lastHeight;
}

template <typename T>
unsigned int AVLTreeNode<T>::lHeight() {
    if (left == nullptr)
        return 0;
    return left->lastHeight;
}

template <typename T>
unsigned int AVLTreeNode<T>::rHeight() {
    if (right == nullptr)
        return 0;
    return right->lastHeight;
}

template <typename T>
int AVLTreeNode<T>::balanceFactor() {
    unsigned int l = left == nullptr ? 0 : left->height(),
                 r = right == nullptr ? 0 : right->height();

    return (int)r - (int)l;
}

template <typename T>
bool AVLTreeNode<T>::balanced() {
    return std::abs(balanceFactor()) <= 1;
}

template <typename T>
bool AVLTreeNode<T>::isLeaf() {
    return left == nullptr && right == nullptr;
}

template <typename T>
void AVLTreeNode<T>::balance() {
    int balanceFactor = this->balanceFactor();
    if (balanceFactor > 1) { // right-heavy
        if (right->balanceFactor() <= -1) // RL case
            right->rRotate();
        lRotate();
    }
    else if (balanceFactor < -1) { // left-heavy
        if (left->balanceFactor() >= 1 ) // LR case
            left->lRotate();
        rRotate();
    }
}

template <typename T>
void AVLTreeNode<T>::rRotate() {
    AVLTreeNode<T>& newRoot = *left;
    left = newRoot.right;
    swap(*this, newRoot);
    right = &newRoot;

    newRoot.lastHeight = std::max(newRoot.lHeight(), newRoot.rHeight()) + 1;
    lastHeight = std::max(lHeight(), rHeight()) + 1;
}

template <typename T>
void AVLTreeNode<T>::lRotate() {
    AVLTreeNode<T>& newRoot = *right;
    right = newRoot.left;
    swap(*this, newRoot);
    left = &newRoot;

    newRoot.lastHeight = std::max(newRoot.lHeight(), newRoot.rHeight()) + 1;
    lastHeight = std::max(lHeight(), rHeight()) + 1;
}

#endif