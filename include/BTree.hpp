#ifndef BTree_HPP
#define BTree_HPP

#include <iostream>
#include <vector>

#include <algorithm>
#include <utility>
#include <stdexcept>
#include <functional>

template <typename T, 
          class Less = std::less<T>>
class BTree {
    private:
        unsigned int childrenCount;
        std::vector<T> info;
        std::vector<BTree*> children;

        typename std::vector<T>::iterator getIterator(const T& data);

        void addChild(unsigned int i);
        void removeChild(unsigned int i);

        T removeAt(unsigned int i);

    public:
        BTree(unsigned int n);
        virtual ~BTree();

        BTree(const BTree& other);
        BTree<T, Less>& operator= (BTree other);
        
        BTree(BTree&& other);

        void insert(T data);
        bool remove(const T& data);

        T popMin();
        T popMax();

        bool empty() const;
        bool full() const;
        bool leaf() const;

        template <typename U> friend std::ostream& operator<<(std::ostream& os, const BTree<U>& t);
};

template <typename T, class Less>
BTree<T, Less>::BTree(unsigned int n) : childrenCount(0) {
    if (n < 1)
        throw std::invalid_argument("Invalid BTree node size");

    info.reserve(n);
    children.resize(n+1, nullptr);
}

template <typename T, class Less>
BTree<T, Less>::~BTree() {
    for (auto it = children.begin(); it != children.end(); it++) {
        if (*it != nullptr) {
            delete *it;
            *it = nullptr;
        }
    }
}

template <typename T, class Less>
BTree<T, Less>::BTree(const BTree& other) : info(other.info), childrenCount(other.childrenCount) {
    children.resize(other.children.size());
    for (int i=0; i<other.children.size(); i++) {
        BTree* current = other.children[i];
        if (current != nullptr)
            children[i] = new BTree(*current);
    }
}

template <typename T, class Less>
BTree<T, Less>& BTree<T, Less>::operator=(BTree other) {
    std::swap(childrenCount, other.childrenCount);
    std::swap(info, other.info);
    std::swap(children, other.children);
    return *this;
}

template <typename T, class Less>
BTree<T, Less>::BTree(BTree&& other) 
    : info(std::move(other.info)), 
      children(std::move(other.children)),
      childrenCount(std::move(childrenCount))
    {}

template <typename T>
std::ostream& operator<<(std::ostream& os, const BTree<T>& t) {
    os << "(";
    for (unsigned int i=0; i<t.info.size(); i++) {
        if (t.children[i] != nullptr) // prints left
            os << *t.children[i];

        os << " " << t.info[i] << " ";
    }

    if (t.children.back() != nullptr) // prints last's right
        os << *t.children.back();

    os << ")";
    return os;
}

template <typename T, class Less>
bool BTree<T, Less>::full() const {
    return info.size() >= children.size() - 1;
}

template <typename T, class Less>
bool BTree<T, Less>::empty() const {
    return info.size() <= 0;
}

template <typename T, class Less>
bool BTree<T, Less>::leaf() const {
    return childrenCount <= 0;
}

template <typename T, class Less>
void BTree<T, Less>::addChild(unsigned int i) {
    children[i] = new BTree(children.size() - 1);
    childrenCount++;
}

template <typename T, class Less>
void BTree<T, Less>::removeChild(unsigned int i) {
    delete children[i];
    children[i] = nullptr;
    childrenCount--;
}

template <typename T, class Less>
typename std::vector<T>::iterator BTree<T, Less>::getIterator(const T& data) {
    Less isLess;

    auto it = info.begin();
    while (it != info.end() && isLess(*it, data))
        it++;
    return it;
}

template <typename T, class Less>
void BTree<T, Less>::insert(T data) {
    auto it = getIterator(data);
    if (full()) {
        int index = it - info.begin();
        if (children[index] == nullptr)
            addChild(index);
        children[index]->insert(data);
    }
    else {
        info.insert(it, data);
    }
}

template <typename T, class Less>
T BTree<T, Less>::removeAt(unsigned int index) {
    auto currentIterator = info.begin() + index;
    unsigned int reverseIndex = info.size() - index - 1;
    T ret(info[index]);

    if (leaf()) {
        std::rotate(currentIterator, currentIterator + 1, info.end()); // shifts right
        info.pop_back();
        return ret;
    }

    // searches before
    for (int i=index; i >= 0; i--) {
        if (children[i] != nullptr) {
            // shift right
            std::rotate(info.begin() + i,
                        info.begin() + index,
                        info.begin() + index + 1);
            info[i] = children[i]->popMax();
            if (children[i]->empty())
                removeChild(i);

            return ret;
        }
    }

    // searches after
    for (int i=index; i + 1 < children.size(); i++) {
        if (children[i+1] != nullptr) {
            // shift left
            std::rotate(info.rbegin() + info.size() - i - 1, // reverse i 
                        info.rbegin() + reverseIndex, 
                        info.rbegin() + reverseIndex + 1);
            info[i] = children[i+1]->popMin();
            if (children[i+1]->empty()) 
                removeChild(i + 1);

            return ret;
        }
    }
}

template <typename T, class Less>
bool BTree<T, Less>::remove(const T& data) {
    auto it = getIterator(data);
    int index = it - info.begin();
    // not in current cBTree
    if (*it != data) {
        if (children[index] == nullptr) // not in children either
            return false;
        bool ret = children[index]->remove(data);
        if (children[index]->empty())
            removeChild(index);
        return ret;
    }

    removeAt(index);
    return true;
}

template <typename T, class Less>
T BTree<T, Less>::popMax() {
    if (children.back() != nullptr) 
        return children.back()->popMax();

    if (leaf()) {
        T ret(info.back());
        info.pop_back();
        return ret;
    }

    return removeAt(info.size() - 1);
}

template <typename T, class Less>
T BTree<T, Less>::popMin() {
    if (children.front() != nullptr)
        return children.front()->popMin();

    if (leaf()) {
        T ret(info.front());
        info.erase(info.begin());
        return ret;
    }

    return removeAt(0);
}

#endif