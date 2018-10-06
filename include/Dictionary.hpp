#ifndef DICTIONARY_INCLUDED
#define DICTIONARY_INCLUDED

#include <utility>

#include "AVLTree.hpp"
#include "useful.hpp"

template <typename K,
          typename V,
          class Less = std::less<K>>
class Dictionary {
    private:
        typedef std::pair<K, V*> KVPair;

        class KeyComparison {
            private:
                compare<K, Less> comparison;
            public:
                int operator()(const KVPair& a, const KVPair& b) {
                    return comparison(a.first, b.first);
                }
        };

        AVLTree<KVPair, KeyComparison> tree;

    public:
        // Dictionary();
        // Dictionary(const Dictionary& other);
        // Dictionary& operator=(const Dictionary& other);
        // Dictionary(const Dictionary&& other);
        void insert(K k, V v);
        bool remove(const K& k);
        V& operator[](const K& key);

        template <typename L, typename B>
        friend std::ostream& operator<<(std::ostream& os, const Dictionary<L, B>& d);
};

template <typename K, typename V, class Less>
void Dictionary<K, V, Less>::insert(K key, V value) {
    tree.insert(KVPair(key, &value)); // TODO: Check for possible memory leak
}

template <typename K, typename V, class Less>
bool Dictionary<K, V, Less>::remove(const K& key) {
    return tree.remove(KVPair(key, nullptr));
}

template <typename K, typename V, class Less>
V& Dictionary<K, V, Less>::operator[](const K& key) {
    auto it = tree.find(KVPair(key, nullptr));
    return *it->second;
}

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Dictionary<K, V>& d) {
    os << "[";
    for (auto it = d.tree.cbegin(); it != d.tree.cend(); ++it) // TODO: implement AVLTree iterator
        os << "(" << it->first << ":" << *it->second << ")";
    os << "]";
    return os;
}

#endif