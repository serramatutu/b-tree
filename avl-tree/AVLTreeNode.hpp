#ifndef AVL_TREE_NODE
#define AVL_TREE_NODE

#include <iostream>
#include <cmath>

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

        void insert(const T& data, AVLTreeNode<T>*& pptr);
        AVLTreeNode<T>& removeHighestLeaf();

    public:
        AVLTreeNode(const T& data);
        virtual ~AVLTreeNode();
        AVLTreeNode(const AVLTreeNode& other);
        AVLTreeNode<T>& operator= (AVLTreeNode<T> other);
        AVLTreeNode(AVLTreeNode&& other);

        T& popMin();
        T& popMax();

        bool isLeaf();

        unsigned int height();
        bool balanced();

        void insert(const T& data);
        bool remove (const T& data);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const AVLTreeNode<U>& t);

    
    static AVLTreeNode<T>& balance(AVLTreeNode<T>& root);
    static AVLTreeNode<T>& rRotate(AVLTreeNode<T>& root);
    static AVLTreeNode<T>& lRotate(AVLTreeNode<T>& root);
};

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
        insert(data, left);
    else
        insert(data, right);
}

template <typename T>
void AVLTreeNode<T>::insert(const T& data, AVLTreeNode<T>*& ptr) {
    if (ptr == nullptr)
        ptr = new AVLTreeNode(data);
    else {
        ptr->insert(data);
        ptr = &balance(*ptr);
    }

    // recalculates the height at this node
    unsigned int newHeight = ptr->height() + 1;
    if (newHeight > lastHeight)
        lastHeight = newHeight;
}

template <typename T>
bool AVLTreeNode<T>::remove(const T& data) {
    if (data > this.data) {
        if (right != nullptr) {
            if (right->data != data)
                return right->remove(data);

            // if (right->isLeaf()) {
            //     delete right;
            //     right = nullptr;
            // }
            // else
            //     right->data = right->popMin();
        }
    }
    else if (data < this.data) {
        if (left != nullptr) {
            if (*left->data != data)
                return left->remove(data);

            
        }
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
AVLTreeNode<T>& AVLTreeNode<T>::balance(AVLTreeNode<T>& root) {
    int balanceFactor = root.balanceFactor();
    if (balanceFactor > 1) { // right-heavy
        if (root.right->balanceFactor() <= -1) // RL case
            root.right = &AVLTreeNode<T>::rRotate(*root.right);
        return lRotate(root);
    }
    else if (balanceFactor < -1) { // left-heavy
        if (root.left->balanceFactor() >= 1 ) // LR case
            root.left = &AVLTreeNode<T>::lRotate(*root.left);
        return rRotate(root);
    }

    return root; // in case the tree is already balanced
}

template <typename T>
AVLTreeNode<T>& AVLTreeNode<T>::rRotate(AVLTreeNode<T>& root) {
    AVLTreeNode<T>& newRoot = *root.left;
    root.left = newRoot.right;
    newRoot.right = &root;

    if (root.lHeight() > root.rHeight())
        root.lastHeight = root.lHeight() + 1;
    else
        root.lastHeight = root.rHeight() + 1;

    if (newRoot.lHeight() > newRoot.rHeight())
        newRoot.lastHeight = newRoot.lHeight() + 1;
    else
        newRoot.lastHeight = newRoot.rHeight() + 1;

    return newRoot;
}

template <typename T>
AVLTreeNode<T>& AVLTreeNode<T>::lRotate(AVLTreeNode<T>& root) {
    AVLTreeNode<T>& newRoot = *root.right;
    root.right = newRoot.left;
    newRoot.left = &root;

    if (root.lHeight() > root.rHeight())
        root.lastHeight = root.lHeight() + 1;
    else
        root.lastHeight = root.rHeight() + 1;

    if (newRoot.lHeight() > newRoot.rHeight())
        newRoot.lastHeight = newRoot.lHeight() + 1;
    else
        newRoot.lastHeight = newRoot.rHeight() + 1;

    return newRoot;
}

#endif