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

                const T& operator*() const {
                    if (!matrix.rows.containsKey(y))
                        return matrix.defaultValue;
                    if (!matrix.rows[y].containsKey(x))
                        return matrix.defaultValue;
                    return matrix.rows[y][x];
                }

                T& operator*() {
                    return const_cast<T&>(operator*());
                }

                friend std::ostream& operator<<(std::ostream& os, const Cell& c) {
                    const T& val = *c;
                    os << val;
                    return os;
                }
        };

        class Row {
            protected:
                SparseMatrix& matrix;
                size_t y;

            public:
                Row(SparseMatrix& matrix, size_t y) : matrix(matrix), y(y) {};

                Cell operator[](size_t x) {
                    if (x >= matrix.width)
                        throw std::invalid_argument("invalid matrix coordinates");
                    return Cell(matrix, x, y);
                };
        };

        SparseMatrix(const T& defaultValue, size_t w, size_t h) : defaultValue(defaultValue), width(w), height(h) {};

        Row operator[](size_t y) {
            if (y >= height)
                throw std::invalid_argument("invalid matrix coordinates");
            return Row(*this, y);
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