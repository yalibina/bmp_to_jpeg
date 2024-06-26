#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <cmath>
#include <iomanip>

template<typename T>
class Matrix {
private:
    size_t row_size_;
    size_t column_size_;
    std::vector<std::vector<T>> matrix_;
public:
    Matrix() = default;
    Matrix(size_t row_size, size_t column_size, T initial_value = T());

    Matrix(const Matrix &matrix);

    explicit Matrix(const std::vector<std::vector<T>> &vector_matrix);

    Matrix(Matrix &&matrix) noexcept;

    ~Matrix() = default;

    // Matrix Operations
    Matrix operator+(const Matrix &matrix) const;

    Matrix operator-(const Matrix &matrix) const;

    Matrix operator*(const Matrix &matrix) const;

    Matrix &operator=(const Matrix &matrix) = default;

    Matrix &operator=(Matrix &&matrix) noexcept;

    Matrix &transpose();

    Matrix &round();

    [[nodiscard]] Matrix transposed() const;

    [[nodiscard]] Matrix multiplyByElement(const Matrix &matrix) const;

    [[nodiscard]] Matrix divideByElement(const Matrix &matrix) const;

    Matrix &resize(size_t new_row_size, size_t new_column_size, T padding_value = T());

    [[nodiscard]] Matrix resized(size_t new_row_size, size_t new_column_size, T padding_value = T()) const;

    [[nodiscard]] Matrix
    getSlice(std::pair<size_t, size_t> upper_left_point, std::pair<size_t, size_t> lower_right_point) const;

    Matrix &insert(std::pair<size_t, size_t> insert_point, const Matrix &matrix_to_insert);

    [[nodiscard]] Matrix flatten() const;

    [[nodiscard]] std::vector<T> flattenVector() const;

    // Scalar Operations
    Matrix operator+(T number);

    Matrix operator-(T number);

    Matrix operator*(T number);

    Matrix operator/(T number);

    // Aesthetic Methods
    T &operator()(const size_t &row_index, const size_t &column_index);

    const T &operator()(const size_t &row_index, const size_t &column_index) const;

    void print(int precision = 6) const;

    [[nodiscard]] size_t getRowSize() const;

    [[nodiscard]] size_t getColumnSize() const;
};

#include "matrix.tpp"