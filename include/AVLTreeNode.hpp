#ifndef AVL_TREE_NODE
#define AVL_TREE_NODE

#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <stack>

#include "useful.hpp"

template <typename T,
          class Less = std::less<T>>
class AVLTreeNode {
    private:
        AVLTreeNode<T, Less>** parentPtr;
        AVLTreeNode<T, Less>* left;
        AVLTreeNode<T, Less>* right;
        T data;

        compare<T, Less> comparison;

        void recalcHeight();
        unsigned int lastHeight; // the last calculated height
        unsigned int lHeight();
        unsigned int rHeight();

        int balanceFactor();

        void balance();
        void rRotate();
        void lRotate();

        void insert(const T& data, AVLTreeNode<T, Less>*& pptr);

        AVLTreeNode<T, Less>& findMin();
        AVLTreeNode<T, Less>& findMax();

    public:
        class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            private:
                const AVLTreeNode<T>* root;
                std::stack<const AVLTreeNode<T>*> s;
                
                bool end() {
                    return s.empty();
                }

                bool null() {
                    return root == nullptr;
                }

            public:
                const_iterator() : root(nullptr) {};

                const_iterator(const AVLTreeNode<T>& tree, bool end=false) : root(&tree) {
                    if (!end) {
                        const AVLTreeNode<T>* ptr = root;
                        do {
                            s.push(ptr);
                            ptr = ptr->left;
                        } while(ptr != nullptr);
                    }
                };

                const_iterator(const const_iterator& other) : s(other.s), 
                                                        root(other.root) {};

                const_iterator& operator=(const_iterator other) {
                    swap(*this, other);
                    return *this;
                };

                bool operator==(const const_iterator& other) {
                    if (root != other.root)
                        return false;
                    if (s.empty() != other.s.empty())
                        return false;

                    return s.top() == other.s.top();
                };

                bool operator!=(const const_iterator& other) {
                    return !(*this == other);
                };

                const T& operator*() const {
                    return s.top()->data;
                };

                const T& operator->() const {
                    return s.top()->data;
                };

                const_iterator& operator++() { // prefix
                    if (end())
                        throw std::out_of_range("iterator out of range"); 

                    while(!s.empty() && s.top()->right == nullptr)
                        s.pop();

                    if (!s.empty() && s.top()->right != nullptr) {
                        s.push(s.top()->right);
                        while(s.top()->left != nullptr)
                            s.push(s.top()->left);
                    }

                    return *this;
                };

                const_iterator& operator--() { // prefix
                    if (end())
                        s.push(*root);

                    std::stack<AVLTreeNode<T>*> lastStack(s);

                    while (!s.empty() && s.top()->left == nullptr)
                        s.pop();
                    if (!s.empty() && s.top()->left != nullptr) {
                        s.push(s.top()->left);
                        while (s.top()->right != nullptr)
                            s.push(s.top()->right);
                    }

                    if (s.empty()) {
                        s = lastStack;
                        throw std::out_of_range("iterator out of range");
                    }

                    return *this;
                };

                const_iterator operator++(int) { // postfix
                    const_iterator temp(*this);
                    ++(*this);
                    return temp;
                };

                const_iterator operator--(int) { // prefix
                    const_iterator temp(*this);
                    --(*this);
                    return temp;
                };

            friend void swap(const_iterator& a, const_iterator& b) {
                std::swap(a.root, b.root);
                std::swap(a.index, b.index);
                std::swap(a.s, b.s);
            };
        };

        AVLTreeNode(const T& data, AVLTreeNode<T, Less>*& parentPtr);
        virtual ~AVLTreeNode();
        AVLTreeNode(const AVLTreeNode& other);
        AVLTreeNode<T, Less>& operator= (AVLTreeNode<T, Less> other);
        AVLTreeNode(AVLTreeNode&& other);

        bool isLeaf();

        unsigned int height();
        bool balanced();

        void insert(const T& data);
        bool remove(const T& data);

        const_iterator begin() const;
        const_iterator end() const;

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

template <typename T, class Less>
AVLTreeNode<T, Less>::AVLTreeNode(const T& data, AVLTreeNode<T, Less>*& parentPtr)
    : left(nullptr), right(nullptr), data(data), lastHeight(1), parentPtr(&parentPtr)
{}

template <typename T, class Less>
AVLTreeNode<T, Less>::~AVLTreeNode() {
    delete right;
    right = nullptr;
    delete left;
    left = nullptr;
}

template <typename T, class Less>
AVLTreeNode<T, Less>::AVLTreeNode(const AVLTreeNode& other) : data(other.data), parentPtr(other.parentPtr) {
    if (other.right != nullptr)
        right = new AVLTreeNode(*other.right);
    if (other.left != nullptr)
        left = new AVLTreeNode(*other.left);
}

template <typename T, class Less>
AVLTreeNode<T, Less>& AVLTreeNode<T, Less>::operator=(AVLTreeNode<T, Less> other) {
    swap(*this, other);
    return *this;
}

template <typename T, class Less>
AVLTreeNode<T, Less>::AVLTreeNode(AVLTreeNode&& other)
    : right(std::move(other.right)),
      left(std::move(other.left)),
      data(std::move(other.data)),
      lastHeight(std::move(other.lastHeight)),
      parentPtr(std::move(other.parentPtr))
      {}

template <typename T, class Less>
typename AVLTreeNode<T, Less>::const_iterator AVLTreeNode<T, Less>::begin() const {
    return AVLTreeNode<T, Less>::const_iterator(*this);
}

template <typename T, class Less>
typename AVLTreeNode<T, Less>::const_iterator AVLTreeNode<T, Less>::end() const {
    return AVLTreeNode::const_iterator(*this, true);
}

template <typename T, class Less>
void AVLTreeNode<T, Less>::insert(const T& data) {
    if (comparison(data, this->data) < 0)
        insert(data, left);
    else
        insert(data, right);

    balance();
}

template <typename T, class Less>
void AVLTreeNode<T, Less>::insert(const T& data, AVLTreeNode<T, Less>*& ptr) {
    if (ptr == nullptr)
        ptr = new AVLTreeNode<T, Less>(data, ptr);
    else
        ptr->insert(data);

    // recalculates the height at this node
    unsigned int newHeight = ptr->height() + 1;
    if (newHeight > lastHeight)
        lastHeight = newHeight;
}

template <typename T, class Less>
bool AVLTreeNode<T, Less>::remove(const T& data) {
    if (comparison(data, this->data) != 0) {
        AVLTreeNode<T, Less>* ptr = comparison(data, this->data) > 0 ? right : left;
        if (ptr == nullptr)
            return false;

        bool result = ptr->remove(data);
        recalcHeight();
        balance();
        return result;
    }

    if (left != nullptr && right != nullptr) { // has both children
        AVLTreeNode<T, Less>& next = right->findMin();
        this->data = next.data;
        next.remove(next.data);
        recalcHeight();
        balance();
        return true;
    }

    AVLTreeNode<T, Less>* ptr = left != nullptr ? left : right;
    if (ptr != nullptr)
        ptr->parentPtr = parentPtr;
    *parentPtr = ptr;
    right = nullptr;
    left = nullptr;

    delete this;
    return true;
}

template <typename T, class Less>
AVLTreeNode<T, Less>& AVLTreeNode<T, Less>::findMin() {
    AVLTreeNode<T, Less>* current = this;
    while (current->left != nullptr)
        current = current->left;
    return *current;
}

template <typename T, class Less>
AVLTreeNode<T, Less>& AVLTreeNode<T, Less>::findMax() {
    AVLTreeNode<T, Less>* current = this;
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

template <typename T, class Less>
void AVLTreeNode<T, Less>::recalcHeight() {
    lastHeight = std::max(lHeight(), rHeight()) + 1;
}

template <typename T, class Less>
unsigned int AVLTreeNode<T, Less>::height() {
    return lastHeight;
}

template <typename T, class Less>
unsigned int AVLTreeNode<T, Less>::lHeight() {
    if (left == nullptr)
        return 0;
    return left->lastHeight;
}

template <typename T, class Less>
unsigned int AVLTreeNode<T, Less>::rHeight() {
    if (right == nullptr)
        return 0;
    return right->lastHeight;
}

template <typename T, class Less>
int AVLTreeNode<T, Less>::balanceFactor() {
    unsigned int l = left == nullptr ? 0 : left->height(),
                 r = right == nullptr ? 0 : right->height();

    return (int)r - (int)l;
}

template <typename T, class Less>
bool AVLTreeNode<T, Less>::balanced() {
    return std::abs(balanceFactor()) <= 1;
}

template <typename T, class Less>
bool AVLTreeNode<T, Less>::isLeaf() {
    return left == nullptr && right == nullptr;
}

template <typename T, class Less>
void AVLTreeNode<T, Less>::balance() {
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

template <typename T, class Less>
void AVLTreeNode<T, Less>::rRotate() {
    AVLTreeNode<T, Less>& tmp = *left;
    left = tmp.right;
    swap(*this, tmp);
    right = &tmp;

    right->parentPtr = &right;
    if (left != nullptr)
        left->parentPtr = &left;

    tmp.recalcHeight();
    recalcHeight();
}

template <typename T, class Less>
void AVLTreeNode<T, Less>::lRotate() {
    AVLTreeNode<T, Less>& tmp = *right;
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