#include <memory>

template <typename T, class Allocator = std::allocator<T>>
class AVLTree {
    private:
        AVLTree<T, Allocator>* left;
        AVLTree<T, Allocator>* right;
        T* data;

    public:
        AVLTree();
        ~AVLTree();
        AVLTree(const AVLTree& other);
        AVLTree<T, Allocator>& operator= (AVLTree other);
        AVLTree(AVLTree&& other);
};

template <typename T, class Allocator>
AVLTree::AVLTree(const T& data) : left(nullptr), right(nullptr)
{
    
}