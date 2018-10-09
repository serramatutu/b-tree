#ifndef SPARSE_MATRIX_INCLUDED
#define SPARSE_MATRIX_INCLUDED

#include <cstddef>

#include "Dictionary.hpp"

template <typename T>
class SparseMatrix {
    protected:
        T defaultValue;

        typedef Dictionary<size_t, T> Cols;
        typedef Dictionary<size_t, Col> Rows;
        
        Rows rows;

    public:
        // class iterator {
        //     Col& col;

        //     iterator(Col& col) : col(col) {};

        //     T& operator=(const T& data) {
        //         col.findValue(data) = data;
        //     }
        // }

        SparseMatrix(const T& defaultValue) : defaultValue(defaultValue) {};

        T& at(size_t x, size_t, y) {
            if (!rows.containsKey(x))
                return defaultValue;
            Cols cols(rows[x]);
            if (!cols.containsKey(y))
                return defaultValue;
            return cols[y];
        };

};

#endif