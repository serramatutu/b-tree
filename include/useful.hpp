#ifndef USEFUL_INCLUDED
#define USEFUL_INCLUDED

template <typename T,
          class Less = std::less<T>,
          class Equal = std::equal_to<T>>
class compare {
    public:
    int operator()(const T& a, const T& b) {
        Less less;
        Equal equal;
        if (less(a, b)) return -1;
        if (equal(a, b)) return 0;
        return 1;
    }
};

#endif
