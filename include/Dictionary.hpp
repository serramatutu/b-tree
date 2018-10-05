#ifndef DICTIONARY_INCLUDED
#define DICTIONARY_INCLUDED

#include <utility>

#include "AVLTree.hpp"
#include "useful.hpp"

template <typename Key,
          typename Value,
          class Comparison = std::less<Key>>
class Dictionary {
    private:
        typedef std::pair<Key, Value> KVPair;

        class KeyComparison {
            int operator(const KVPair& a, const KVPair& b) {
                Comparison comp;
                return comp(a.first, b.first);
            }
        };

        AVLTree<KVPair, KeyComparison> tree;

    public:
        // Dictionary();
        // Dictionary(const Dictionary& other);
        // Dictionary& operator=(const Dictionary& other);
        // Dictionary(const Dictionary&& other);
        void insert(Key k, Value v);
        bool remove(Key k)
        Value& operator[](const Key& key);

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const Dictionary<U>& d);
};

template <typename K, typename V, class C>
void Dictionary::insert(const K& key, V value) {
    tree.insert(KVPair(key, value));
}

template <typename K, typename V, class C>
bool Dictionary::remove(const K& key) {
    return tree.remove(key);
}

template <typename K, typename V, class C>
V& operator[](const K& key) {
    auto it = tree.find(key);
    return *it->second;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Dictionary<T>& d) {
    os << "[";
    for (KVPair p : d.tree) // TODO: implement AVLTree iterator
        os << "(" <<  p.first << ":" << p.second << ")";
    os << "]";
    return os;
}

#endif