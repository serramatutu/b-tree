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
    public:
        class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            protected:
                AVLTreeNode<T, Less>* root;
                std::stack<AVLTreeNode<T, Less>*> s;
                
                bool end() {
                    return s.empty();
                };

                bool null() {
                    return root == nullptr;
                };

            public:
                const_iterator() : root(nullptr) {};

                const_iterator(const AVLTreeNode<T, Less>* tree, bool end=false) 
                    : root(const_cast<AVLTreeNode<T, Less>*>(tree)) {
                    if (!end && root != nullptr) {
                        AVLTreeNode<T, Less>* ptr = root;
                        do {
                            s.push(ptr);
                            ptr = ptr->left;
                        } while(ptr != nullptr);
                    }
                }

                const_iterator(const AVLTreeNode<T, Less>& tree, bool end=false) 
                    : const_iterator(&tree, end) {};

                bool operator==(const const_iterator& other) const {
                    if (root != other.root)
                        return false;
                        
                    if (!s.empty() && !other.s.empty())
                        return s.top() == other.s.top();

                    return s.empty() == other.s.empty();
                };

                bool operator!=(const const_iterator& other) const {
                    return !(*this == other);
                };

                const T& operator*() const {
                    return s.top()->data;
                };

                const T* operator->() const {
                    return &s.top()->data;
                };

                const_iterator& operator++() { // prefix
                    if (end())
                        throw std::out_of_range("iterator out of range"); 

                    AVLTreeNode<T, Less>* current = s.top();
                    s.pop();

                    if (current->right != nullptr) {
                        s.push(current->right);
                        while(s.top()->left != nullptr)
                            s.push(s.top()->left);
                    }

                    return *this;
                };

                const_iterator& operator--() { // prefix
                    // TODO: Redo this
                    if (end())
                        s.push(*root);

                    std::stack<AVLTreeNode<T, Less>*> lastStack(s);

                    AVLTreeNode<T, Less>* current = s.top();
                    s.pop();

                    if (current->left != nullptr) {
                        s.push(current->left);
                        while(s.top()->right != nullptr)
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

                const_iterator operator--(int) { // postfix
                    const_iterator temp(*this);
                    --(*this);
                    return temp;
                };

            friend void swap(const_iterator& a, const_iterator& b) {
                std::swap(a.root, b.root);
                std::swap(a.s, b.s);
            };

            friend AVLTreeNode<T, Less>;
        };

        class iterator : public const_iterator {
            public:
                iterator() : const_iterator() {};

                iterator(AVLTreeNode<T, Less>* tree, bool end=false) : const_iterator(tree, end) {};

                iterator(AVLTreeNode<T, Less>& tree, bool end=false) : const_iterator(tree, end) {};

                // iterator(const const_iterator& other) : const_iterator(other) {};

                // iterator(const const_iterator&& other) : const_iterator(other) {};

                // iterator& operator=(iterator other) {
                //     return const_iterator::operator=(other);
                // };

                T& operator= (const T& data) {
                    if (this->root.comparison(this->s.top()->data, data) != 0) { // only removes if changed key
                        this->root.remove(data);
                        this->root.insert(data);
                    }
                    return data;
                }

                T& operator*() { // TODO: proxy
                    return this->s.top()->data;
                }

                T* operator->() {
                    return &this->s.top()->data;
                }

                friend AVLTreeNode<T, Less>;
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

        const_iterator find(const T& data) const;
        iterator find(const T& data);

        const_iterator cbegin() const;
        const_iterator cend() const;

        iterator begin();
        iterator end();

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
        const_iterator find(const T& data, const_iterator& it) const;

    template <typename U, class L>
    friend std::ostream& operator<<(std::ostream& os, const AVLTreeNode<U, L>& n);

    template <typename U, class L>
    friend void swap(AVLTreeNode<U, L>& a, AVLTreeNode<U, L>& b);
};

template <typename T, class Less>
void swap(AVLTreeNode<T, Less>& a, AVLTreeNode<T, Less>& b) {
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
typename AVLTreeNode<T, Less>::const_iterator AVLTreeNode<T, Less>::cbegin() const {
    return AVLTreeNode<T, Less>::const_iterator(*this);
}

template <typename T, class Less>
typename AVLTreeNode<T, Less>::const_iterator AVLTreeNode<T, Less>::cend() const {
    return AVLTreeNode<T, Less>::const_iterator(*this, true);
}

template <typename T, class Less>
typename AVLTreeNode<T, Less>::iterator AVLTreeNode<T, Less>::begin() {
    return AVLTreeNode<T, Less>::iterator(*this);
}

template <typename T, class Less>
typename AVLTreeNode<T, Less>::iterator AVLTreeNode<T, Less>::end() {
    return AVLTreeNode<T, Less>::iterator(*this, true);
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
typename AVLTreeNode<T, Less>::const_iterator AVLTreeNode<T, Less>::find(const T& data) const {
    const_iterator i = cend();
    i = find(data, i);
    if (i != cend())
        return i;
    return const_iterator(*this, true);
}

template <typename T, class Less>
typename AVLTreeNode<T, Less>::iterator AVLTreeNode<T, Less>::find(const T& data) {
    const_iterator i = cend();
    i = find(data, i);
    if (i != cend())
        return static_cast<iterator&>(i);
    return iterator(*this, true);
}

template <typename T, class Less>
typename AVLTreeNode<T, Less>::const_iterator AVLTreeNode<T, Less>::find
    (const T& data, typename AVLTreeNode<T, Less>::const_iterator& it) const {
    it.s.push(const_cast<AVLTreeNode<T, Less>*>(this));
    int comp = this->comparison(data, this->data);
    if (comp == 0)
        return it;

    AVLTreeNode<T, Less>* ptr;
    if (comp < 0)
        ptr = left;
    else
        ptr = right;

    if (ptr == nullptr)
        return it.root->cend();
    else
        return ptr->find(data, it);
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

template <typename T, class Less>
std::ostream& operator<<(std::ostream& os, const AVLTreeNode<T, Less>& n) {
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