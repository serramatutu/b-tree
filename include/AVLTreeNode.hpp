#ifndef AVL_TREE_NODE
#define AVL_TREE_NODE

#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>

#include "useful.hpp"

template <typename T,
          class Comparison = compare<T>>
class AVLTreeNode {
    private:
        AVLTreeNode<T, Comparison>** parentPtr;
        AVLTreeNode<T, Comparison>* left;
        AVLTreeNode<T, Comparison>* right;
        T data;

        void recalcHeight();
        unsigned int lastHeight; // the last calculated height
        unsigned int lHeight();
        unsigned int rHeight();

        int balanceFactor();

        void balance();
        void rRotate();
        void lRotate();

        void insert(const T& data, AVLTreeNode<T, Comparison>*& pptr);

        AVLTreeNode<T, Comparison>& findMin();
        AVLTreeNode<T, Comparison>& findMax();

    public:
        AVLTreeNode(const T& data, AVLTreeNode<T, Comparison>*& parentPtr);
        virtual ~AVLTreeNode();
        AVLTreeNode(const AVLTreeNode& other);
        AVLTreeNode<T, Comparison>& operator= (AVLTreeNode<T, Comparison> other);
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

template <typename T, class Comparison>
AVLTreeNode<T, Comparison>::AVLTreeNode(const T& data, AVLTreeNode<T, Comparison>*& parentPtr)
    : left(nullptr), right(nullptr), data(data), lastHeight(1), parentPtr(&parentPtr)
{}

template <typename T, class Comparison>
AVLTreeNode<T, Comparison>::~AVLTreeNode() {
    delete right;
    right = nullptr;
    delete left;
    left = nullptr;
}

template <typename T, class Comparison>
AVLTreeNode<T, Comparison>::AVLTreeNode(const AVLTreeNode& other) : data(other.data), parentPtr(other.parentPtr) {
    if (other.right != nullptr)
        right = new AVLTreeNode(*other.right);
    if (other.left != nullptr)
        left = new AVLTreeNode(*other.left);
}

template <typename T, class Comparison>
AVLTreeNode<T, Comparison>& AVLTreeNode<T, Comparison>::operator=(AVLTreeNode<T, Comparison> other) {
    swap(*this, other);
    return *this;
}

template <typename T, class Comparison>
AVLTreeNode<T, Comparison>::AVLTreeNode(AVLTreeNode&& other)
    : right(std::move(other.right)),
      left(std::move(other.left)),
      data(std::move(other.data)),
      lastHeight(std::move(other.lastHeight)),
      parentPtr(std::move(other.parentPtr))
      {}

template <typename T, class Comparison>
void AVLTreeNode<T, Comparison>::insert(const T& data) {
    Comparison comparison;
    if (comparison(data, this->data) < 0)
        insert(data, left);
    else
        insert(data, right);

    balance();
}

template <typename T, class Comparison>
void AVLTreeNode<T, Comparison>::insert(const T& data, AVLTreeNode<T, Comparison>*& ptr) {
    if (ptr == nullptr)
        ptr = new AVLTreeNode<T, Comparison>(data, ptr);
    else
        ptr->insert(data);

    // recalculates the height at this node
    unsigned int newHeight = ptr->height() + 1;
    if (newHeight > lastHeight)
        lastHeight = newHeight;
}

template <typename T, class Comparison>
bool AVLTreeNode<T, Comparison>::remove(const T& data) {
    Comparison comparison;
    if (comparison(data, this->data) != 0) {
        AVLTreeNode<T, Comparison>* ptr = comparison(data, this->data) > 0 ? right : left;
        if (ptr == nullptr)
            return false;

        bool result = ptr->remove(data);
        recalcHeight();
        balance();
        return result;
    }

    if (left != nullptr && right != nullptr) { // has both children
        AVLTreeNode<T, Comparison>& next = right->findMin();
        this->data = next.data;
        next.remove(next.data);
        recalcHeight();
        balance();
        return true;
    }

    AVLTreeNode<T, Comparison>* ptr = left != nullptr ? left : right;
    if (ptr != nullptr)
        ptr->parentPtr = parentPtr;
    *parentPtr = ptr;
    right = nullptr;
    left = nullptr;

    delete this;
    return true;
}

template <typename T, class Comparison>
AVLTreeNode<T, Comparison>& AVLTreeNode<T, Comparison>::findMin() {
    AVLTreeNode<T, Comparison>* current = this;
    while (current->left != nullptr)
        current = current->left;
    return *current;
}

template <typename T, class Comparison>
AVLTreeNode<T, Comparison>& AVLTreeNode<T, Comparison>::findMax() {
    AVLTreeNode<T, Comparison>* current = this;
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

template <typename T, class Comparison>
void AVLTreeNode<T, Comparison>::recalcHeight() {
    lastHeight = std::max(lHeight(), rHeight()) + 1;
}

template <typename T, class Comparison>
unsigned int AVLTreeNode<T, Comparison>::height() {
    return lastHeight;
}

template <typename T, class Comparison>
unsigned int AVLTreeNode<T, Comparison>::lHeight() {
    if (left == nullptr)
        return 0;
    return left->lastHeight;
}

template <typename T, class Comparison>
unsigned int AVLTreeNode<T, Comparison>::rHeight() {
    if (right == nullptr)
        return 0;
    return right->lastHeight;
}

template <typename T, class Comparison>
int AVLTreeNode<T, Comparison>::balanceFactor() {
    unsigned int l = left == nullptr ? 0 : left->height(),
                 r = right == nullptr ? 0 : right->height();

    return (int)r - (int)l;
}

template <typename T, class Comparison>
bool AVLTreeNode<T, Comparison>::balanced() {
    return std::abs(balanceFactor()) <= 1;
}

template <typename T, class Comparison>
bool AVLTreeNode<T, Comparison>::isLeaf() {
    return left == nullptr && right == nullptr;
}

template <typename T, class Comparison>
void AVLTreeNode<T, Comparison>::balance() {
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

template <typename T, class Comparison>
void AVLTreeNode<T, Comparison>::rRotate() {
    AVLTreeNode<T, Comparison>& tmp = *left;
    left = tmp.right;
    swap(*this, tmp);
    right = &tmp;

    right->parentPtr = &right;
    if (left != nullptr)
        left->parentPtr = &left;

    tmp.recalcHeight();
    recalcHeight();
}

template <typename T, class Comparison>
void AVLTreeNode<T, Comparison>::lRotate() {
    AVLTreeNode<T, Comparison>& tmp = *right;
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