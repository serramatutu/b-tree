#ifndef SPARSE_MATRIX_INCLUDED
#define SPARSE_MATRIX_INCLUDED

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>

#include "Dictionary.hpp"

template <typename T>
class SparseMatrix {
    protected:
        T defaultValue;

        size_t width, height;

        typedef Dictionary<size_t, T> Cols;
        typedef Dictionary<size_t, Cols> Rows;
        
        Rows rows;

    public:
        class Cell {
            protected:
                SparseMatrix& matrix;
                size_t x, y;

            public:
                Cell(SparseMatrix& matrix, size_t x, size_t y) : matrix(matrix), x(x), y(y) {};

                T& operator=(const T& data) {
                    if (data == matrix.defaultValue) {
                        matrix.rows[x].remove(y);
                        return matrix.defaultValue;
                    }

                    T& val = matrix.rows[y][x];
                    val = data;
                    return val;
                }

                T& operator*() {
                    return *operator->();
                }

                T* operator->() {
                    if (!matrix.rows.containsKey(y))
                        return matrix.defaultValue;
                    Cols cols(matrix.rows[y]);
                    if (!cols.containsKey(x))
                        return matrix.defaultValue;
                    return &cols[x];
                }
        };

        SparseMatrix(const T& defaultValue, size_t w, size_t h) : defaultValue(defaultValue), width(w), height(h) {};

        Cell at(size_t x, size_t y) {
            if (x >= width || y >= height)
                throw std::invalid_argument("invalid matrix coordinates");
            return Cell(*this, x, y);
        };

        friend std::ostream& operator<<(std::ostream& os, const SparseMatrix<T>& m) {
            os << "---------------------" << std::endl;
            for (auto colPair = m.rows.cbegin(); colPair != m.rows.cend(); colPair++) {
                size_t index = 0;
                std::cout << m.rows << std::endl;
                // std::cout << colPair->second << std::endl;
                // for (auto valPair = colPair->second.cbegin(); valPair != colPair->second.cend(); valPair++) {
                //     while (index < valPair->first && index < m.width) {
                //         os << m.defaultValue << " ";
                //         index++;
                //     }
                //     os << valPair->second;
                // }
                os << std::endl;
            }
            
            os << "---------------------";
            return os;
        };
};

#endif