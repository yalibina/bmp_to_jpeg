#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <cmath>

class Matrix {
private:
    size_t row_size_;
    size_t column_size_;
    std::vector<std::vector<double> > matrix_;
public:
    Matrix(size_t row_size, size_t column_size, double initial_value);

    Matrix(const Matrix &matrix);

    explicit Matrix(const std::vector<std::vector<double> > &vector_matrix);

    Matrix(Matrix &&matrix) noexcept;

    ~Matrix() = default;

    // Matrix Operations
    Matrix operator+(const Matrix &matrix) const;

    Matrix operator-(const Matrix &matrix) const;

    Matrix operator*(const Matrix &matrix) const;

    Matrix &operator=(const Matrix &matrix) = default;

    Matrix &operator=(Matrix &&matrix) noexcept;

    Matrix &transpose();

    [[nodiscard]] Matrix transposed() const;

    [[nodiscard]] Matrix multiplyByElement(const Matrix &matrix) const;

    [[nodiscard]] Matrix divideByElement(const Matrix &matrix) const;

    Matrix &resize(size_t new_row_size, size_t new_column_size, double padding_value);

    [[nodiscard]] Matrix resized(size_t new_row_size, size_t new_column_size, double padding_value) const;

    // Scalar Operations
    Matrix operator+(double number);

    Matrix operator-(double number);

    Matrix operator*(double number);

    Matrix operator/(double number);

    // Aesthetic Methods
    double &operator()(const size_t &row_index, const size_t &column_index);

    const double &operator()(const size_t &row_index, const size_t &column_index) const;

    void print() const;

    [[nodiscard]] size_t getRowSize() const;

    [[nodiscard]] size_t getColumnSize() const;
};

