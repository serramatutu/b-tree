#include <iostream>

template <typename T>
class AVLTree {
    private:
        AVLTree<T>* left;
        AVLTree<T>* right;
        T data;

        int lastHeight; // the last calculated height

        void insert(const T& data, AVLTree<T>** pptr);

    public:
        AVLTree(const T& data);
        virtual ~AVLTree();
        AVLTree(const AVLTree& other);
        AVLTree<T>& operator= (AVLTree other);
        AVLTree(AVLTree&& other);

        int height();
        int balanceFactor();

        void insert(const T& data);

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const AVLTree<U>& t);
};

template <typename T>
AVLTree<T>::AVLTree(const T& data) : left(nullptr), right(nullptr), data(data), lastHeight(1) 
{}

template <typename T>
AVLTree<T>::~AVLTree() {
    delete right;
    right = nullptr;
    delete left;
    left = nullptr;
}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree& other) : data(other.data) {
    right = new AVLTree(other->right);
    left = new AVLTree(other->left);
}

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(AVLTree other) {
    std::swap(data, other.data);
    std::swap(right, other.right);
    std::swap(left, other.left);
    return *this;
}

template <typename T>
AVLTree<T>::AVLTree(AVLTree&& other)
    : right(std::move(other.right)),
      left(std::move(other.left)),
      data(std::move(other.data))
      {}

template <typename T>
void AVLTree<T>::insert(const T& data) {
    if (data < this->data)
        insert(data, &right);
    else
        insert(data, &left);
}

template <typename T>
void AVLTree<T>::insert(const T& data, AVLTree<T>** pptr) {
    if (*pptr == nullptr)
        *pptr = new AVLTree(data);
    else
        (*pptr)->insert(data);

    // recalculates the height at this node
    int newHeight = (*pptr)->height() + 1;
    if (newHeight > lastHeight)
        lastHeight = newHeight;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const AVLTree<T>& t) {
    if (t.right != nullptr)
        os << *t.right;
    os << " (" << t.data << ") ";
    if (t.left != nullptr)
        os << *t.left;

    return os;
}

template <typename T>
int AVLTree<T>::height() {
    return lastHeight;
}

template <typename T>
int AVLTree<T>::balanceFactor() {
    int l = left == nullptr ? 0 : left->height(),
        r = right == nullptr ? 0 : right->height();

    return l - r;
}