#ifndef DICTIONARY_INCLUDED
#define DICTIONARY_INCLUDED

#include <utility>
#include <memory>
#include <iterator>

#include "AVLTree.hpp"
#include "useful.hpp"

template <typename K,
          typename V,
          class Less = std::less<K>>
class Dictionary {
    private:
        typedef std::pair<K, std::shared_ptr<V>> KVPair;

        class KeyLess {
            public:
                bool operator()(const KVPair& a, const KVPair& b) const {
                    return a.first < b.first;
                }
        };

        AVLTree<KVPair, KeyLess> tree;

    public:
        typedef typename AVLTree<KVPair, KeyLess>::const_iterator const_iterator;
        typedef typename AVLTree<KVPair, KeyLess>::iterator iterator;
        // class const_iterator : public std::iterator<std::bidirectional_iterator_tag, std::pair<K, V&>> {
        //     protected:
        //         typename AVLTree<KVPair, KeyLess>::const_iterator it;
        //         typename AVLTree<KVPair, KeyLess>::const_iterator end;
        //         std::pair<K, V&> pair;

        //         void updatePair() {
        //             if (this->it != this->end) {
        //                 pair.first = this->it->first;
        //                 pair.second =  *this->it->second;
        //             }
        //         };
                
        //     public:
        //         const_iterator(const Dictionary& d, bool end=false) 
        //             : it(const_cast<AVLTree<KVPair, KeyLess>&>(d.tree), end), end(d.tree.cend()) {
        //             updatePair();
        //         };

        //         bool operator==(const const_iterator& other) {
        //             return this->it == other.it;
        //         }

        //         bool operator!=(const const_iterator& other) {
        //             return !(this->it == other.it);
        //         }

        //         const_iterator& operator++() { // prefix
        //             this->it++;
        //             updatePair();
        //             return *this;
        //         }

        //         const_iterator& operator--() { // prefix
        //             this->it--;
        //             updatePair();
        //             return *this;
        //         }

        //         const_iterator operator++(int) { // postfix
        //             const_iterator temp(*this);
        //             ++(*this);
        //             return temp;
        //         };

        //         const_iterator operator--(int) { // postfix
        //             const_iterator temp(*this);
        //             --(*this);
        //             return temp;
        //         };

        //         const std::pair<K, V>& operator*() const {
        //             return this->pair;
        //         }

        //         const std::pair<K, V>* operator->() const {
        //             return &this->pair;
        //         }
        // };

        // class iterator : public const_iterator {
        //     public:
        //         iterator(Dictionary& d, bool end=false) : const_iterator(d, end) {};

        //         std::pair<K, V&>& operator*() {
        //             return this->pair;
        //         }

        //         std::pair<K, V&>* operator->() {
        //             return &this->pair;
        //         }
        // };

        const_iterator cbegin() const;
        const_iterator cend() const;
        iterator begin();
        iterator end();

        // Dictionary();
        // Dictionary(const Dictionary& other);
        // Dictionary& operator=(const Dictionary& other);
        // Dictionary(const Dictionary&& other);
        void insert(K k, const V& v);
        bool remove(const K& k);
        V& operator[](const K& key);
        bool containsKey(const K& key) const;

        bool empty() const;

        template <typename L, typename B>
        friend std::ostream& operator<<(std::ostream& os, const Dictionary<L, B>& d);
};

template <typename K, typename V, class Less>
void Dictionary<K, V, Less>::insert(K key, const V& value) {
    tree.insert(KVPair(key, std::shared_ptr<V>(new V(value))));
}

template <typename K, typename V, class Less>
bool Dictionary<K, V, Less>::remove(const K& key) {
    return tree.remove(KVPair(key, nullptr));
}

template <typename K, typename V, class Less>
bool Dictionary<K, V, Less>::containsKey(const K& key) const {
    return tree.find(KVPair(key, nullptr)) != tree.cend();
}

template <typename K, typename V, class Less>
bool Dictionary<K, V, Less>::empty() const {
    return tree.empty();
}

template <typename K, typename V, class Less>
V& Dictionary<K, V, Less>::operator[](const K& key) {
    auto it = tree.find(KVPair(key, nullptr));
    if (it == tree.end()) {
        tree.insert(KVPair(key, std::shared_ptr<V>(new V()))); // TODO: optimize
        it = tree.find(KVPair(key, nullptr));
    }
    return *it->second;
}

template <typename K, typename V, class Less>
typename Dictionary<K, V, Less>::iterator Dictionary<K, V, Less>::begin() {
    return Dictionary<K, V, Less>::iterator(this->tree);
}

template <typename K, typename V, class Less>
typename Dictionary<K, V, Less>::iterator Dictionary<K, V, Less>::end() {
    return Dictionary<K, V, Less>::iterator(this->tree, true);
}

template <typename K, typename V, class Less>
typename Dictionary<K, V, Less>::const_iterator Dictionary<K, V, Less>::cbegin() const {
    return Dictionary<K, V, Less>::const_iterator(this->tree);
}

template <typename K, typename V, class Less>
typename Dictionary<K, V, Less>::const_iterator Dictionary<K, V, Less>::cend() const {
    return Dictionary<K, V, Less>::const_iterator(this->tree, true);
}



template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Dictionary<K, V>& d) {
    os << "[";
    for (auto it = d.tree.cbegin(); it != d.tree.cend(); ++it)
        os << "(" << it->first << ":" << *it->second << ")";
    os << "]";

    return os;
}

#endif