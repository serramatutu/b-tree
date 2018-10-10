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

        
        static std::ostream& printDefaultUntil(std::ostream& os, 
                                               const SparseMatrix<T>& m, 
                                               size_t xs, size_t ys, 
                                               size_t x, size_t y) {
            while (ys < y) {
                while (xs < m.width) {
                    os << m.defaultValue << " ";
                    xs++;
                }
                ys++;
                xs = 0;
                os << std::endl;
            }
            while (xs < x) {
                os << m.defaultValue << " ";
                xs++;
            }

            return os;
        }

    public:
        class Cell {
            protected:
                SparseMatrix& matrix;
                size_t x, y;

            public:
                Cell(SparseMatrix& matrix, size_t x, size_t y) : matrix(matrix), x(x), y(y) {};

                T& operator=(const T& data) {
                    if (data == matrix.defaultValue) {
                        matrix.rows[y].remove(x);
                        if (matrix.rows[y].empty())
                            matrix.rows.remove(y);
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

            size_t lastX = 0,
                   lastY = 0;
            for (auto colPair = m.rows.cbegin(); colPair != m.rows.cend(); colPair++) {
                for (auto valPair = colPair->second->cbegin(); valPair != colPair->second->cend(); valPair++) {
                    printDefaultUntil(os, m, lastX, lastY, colPair->first, valPair->first);
                    os << *valPair->second << " ";

                    lastX = colPair->first + 1;
                    lastY = valPair->first;
                }
            }
            printDefaultUntil(os, m, lastX, lastY, m.width, m.height - 1);
            os << std::endl;
            os << "---------------------";
            return os;
        };
};

#endif