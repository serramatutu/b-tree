#ifndef AVL_TREE_NODE
#define AVL_TREE_NODE

#include <iostream>
#include <cmath>
#include <algorithm>

template <typename T>
class AVLTreeNode {
    private:
        AVLTreeNode<T>** parentPtr;
        AVLTreeNode<T>* left;
        AVLTreeNode<T>* right;
        T data;

        void recalcHeight();
        unsigned int lastHeight; // the last calculated height
        unsigned int lHeight();
        unsigned int rHeight();

        int balanceFactor();

        void balance();
        void rRotate();
        void lRotate();

        void insert(const T& data, AVLTreeNode<T>*& pptr);

        AVLTreeNode<T>& findMin();
        AVLTreeNode<T>& findMax();

    public:
        AVLTreeNode(const T& data, AVLTreeNode<T>*& parentPtr);
        virtual ~AVLTreeNode();
        AVLTreeNode(const AVLTreeNode& other);
        AVLTreeNode<T>& operator= (AVLTreeNode<T> other);
        AVLTreeNode(AVLTreeNode&& other);

        bool isLeaf();

        unsigned int height();
        bool balanced();

        void insert(const T& data);
        bool remove(const T& data);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const AVLTreeNode<U>& n);

    template <typename U>
    friend void swap(AVLTreeNode<U>& a, AVLTreeNode<U>& b);
};

template <typename T>
void swap(AVLTreeNode<T>& a, AVLTreeNode<T>& b) {
    std::swap(a.lastHeight, b.lastHeight);
    std::swap(a.data, b.data);
    std::swap(a.right, b.right);
    std::swap(a.left, b.left);
}

template <typename T>
AVLTreeNode<T>::AVLTreeNode(const T& data, AVLTreeNode<T>*& parentPtr)
    : left(nullptr), right(nullptr), data(data), lastHeight(1), parentPtr(&parentPtr)
{}

template <typename T>
AVLTreeNode<T>::~AVLTreeNode() {
    delete right;
    right = nullptr;
    delete left;
    left = nullptr;
}

template <typename T>
AVLTreeNode<T>::AVLTreeNode(const AVLTreeNode& other) : data(other.data), parentPtr(other.parentPtr) {
    if (other.right != nullptr)
        right = new AVLTreeNode(*other.right);
    if (other.left != nullptr)
        left = new AVLTreeNode(*other.left);
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
      data(std::move(other.data)),
      lastHeight(std::move(other.lastHeight)),
      parentPtr(std::move(other.parentPtr))
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
        ptr = new AVLTreeNode<T>(data, ptr);
    else
        ptr->insert(data);

    // recalculates the height at this node
    unsigned int newHeight = ptr->height() + 1;
    if (newHeight > lastHeight)
        lastHeight = newHeight;
}

template <typename T>
bool AVLTreeNode<T>::remove(const T& data) {
    if (data != this->data) {
        AVLTreeNode<T>* ptr = data > this->data ? right : left;
        if (ptr == nullptr)
            return false;

        bool result = ptr->remove(data);
        recalcHeight();
        balance();
        return result;
    }

    if (left != nullptr && right != nullptr) { // has both children
        AVLTreeNode<T>& next = right->findMin();
        this->data = next.data;
        next.remove(next.data);
        recalcHeight();
        balance();
        return true;
    }

    AVLTreeNode<T>* ptr = left != nullptr ? left : right;
    if (ptr != nullptr)
        ptr->parentPtr = parentPtr;
    *parentPtr = ptr;
    right = nullptr;
    left = nullptr;

    delete this;
    return true;
}

template <typename T>
AVLTreeNode<T>& AVLTreeNode<T>::findMin() {
    AVLTreeNode<T>* current = this;
    while (current->left != nullptr)
        current = current->left;
    return *current;
}

template <typename T>
AVLTreeNode<T>& AVLTreeNode<T>::findMax() {
    AVLTreeNode<T>* current = this;
    while (current->right != nullptr)
        current = current->right;
    return *current;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const AVLTreeNode<T>& n) {
    os << "(";
    if (n.left != nullptr)
        os << *n.left;
    os << n.data;
    if (n.right != nullptr)
        os << *n.right;
    os << ")";

    return os;
}

template <typename T>
void AVLTreeNode<T>::recalcHeight() {
    lastHeight = std::max(lHeight(), rHeight()) + 1;
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
    AVLTreeNode<T>& tmp = *left;
    left = tmp.right;
    swap(*this, tmp);
    right = &tmp;

    right->parentPtr = &right;
    if (left != nullptr)
        left->parentPtr = &left;

    tmp.recalcHeight();
    recalcHeight();
}

template <typename T>
void AVLTreeNode<T>::lRotate() {
    AVLTreeNode<T>& tmp = *right;
    right = tmp.left;
    swap(*this, tmp);
    left = &tmp;

    left->parentPtr = &left;
    if (right != nullptr)
        right->parentPtr = &right;

    tmp.recalcHeight();
    recalcHeight();
}

#endif