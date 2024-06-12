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
    unsigned row_size_;
    unsigned column_size_;
    std::vector<std::vector<double> > matrix_;
public:
    Matrix(unsigned row_size, unsigned column_size, double initial_value);

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

    Matrix transpose();


    Matrix multiplyByElement(Matrix &matrix);

    Matrix divideByElement(Matrix &matrix);

    // Scalar Operations
    Matrix operator+(double number);

    Matrix operator-(double number);

    Matrix operator*(double number);

    Matrix operator/(double number);

    // Aesthetic Methods
    double &operator()(const unsigned &row_index, const unsigned &column_index);

    const double &operator()(const unsigned &row_index, const unsigned &column_index) const;


    void print() const;

    [[nodiscard]] unsigned getRowSize() const;

    [[nodiscard]] unsigned getColumnSize() const;
};

