#pragma once

#include <vector>
#include <stdexcept>
#include <cstddef>
#include <initializer_list>

class Matrix2D {
public:
    // Constructs an empty 0x0 matrix
    Matrix2D() : rows_(0), cols_(0), data_() {}

    // Constructs a matrix with given rows and cols, value-initialized elements
    Matrix2D(std::size_t rows, std::size_t cols)
    : rows_(rows), cols_(cols), data_(rows, std::vector<int>(cols)) {}

    // Constructs a matrix with given rows and cols, filled with value
    Matrix2D(std::size_t rows, std::size_t cols, int value)
    : rows_(rows), cols_(cols), data_(rows, std::vector<int>(cols, value)) {}

    // Constructs from initializer list of rows
    Matrix2D(std::initializer_list<std::initializer_list<int>> init) {
        rows_ = init.size();
        cols_ = rows_ ? init.begin()->size() : 0;
        data_.assign(rows_, std::vector<int>(cols_));
        std::size_t r = 0;
        for (const auto& row : init) {
            if (row.size() != cols_) {
                throw std::invalid_argument("All rows must have the same number of columns");
            }
            std::size_t c = 0;
            for (const auto& v : row) {
                data_[r][c++] = v;
            }
            ++r;
        }
    }

    std::size_t rows() const noexcept { return rows_; }
    std::size_t cols() const noexcept { return cols_; }
    bool empty() const noexcept { return rows_ == 0 || cols_ == 0; }

    // Bounds-checked element access
    int& at(std::size_t r, std::size_t c) {
        check_bounds(r, c);
        return data_[r][c];
    }
    const int& at(std::size_t r, std::size_t c) const {
        check_bounds(r, c);
        return data_[r][c];
    }

    // Fill all elements with a value
    void fill(int value) {
        for (auto& row : data_) {
            std::fill(row.begin(), row.end(), value);
        }
    }

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<std::vector<int>> data_;

    void check_bounds(std::size_t r, std::size_t c) const {
        if (r >= rows_ || c >= cols_) {
            throw std::out_of_range("Matrix2D index out of range");
        }
    }
};
