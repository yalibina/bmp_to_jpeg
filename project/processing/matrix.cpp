#include "matrix.h"

Matrix::Matrix(unsigned row_size, unsigned column_size, double initial_value) : row_size_(row_size),
                                                                                column_size_(column_size) {
    matrix_.resize(column_size);
    for (std::vector<double> &row: matrix_) {
        row.resize(row_size, initial_value);
    }
}

Matrix::Matrix(const Matrix &matrix) {
    this->column_size_ = matrix.getColumnSize();
    this->row_size_ = matrix.getRowSize();
    this->matrix_ = matrix.matrix_;
}

Matrix::Matrix(const std::vector<std::vector<double>> &vector_matrix) : matrix_(vector_matrix),
                                                                        column_size_(vector_matrix.size()) {
    if (column_size_ == 0) {
        return;
    }
    row_size_ = matrix_[0].size();
    for (std::vector<double> &row: matrix_) {
        if (row_size_ != row.size()) {
            throw std::runtime_error("all rows in vector must have equal sizes to make a matrix");
        }
    }
}

Matrix::Matrix(Matrix &&matrix) noexcept: row_size_(matrix.row_size_),
                                          column_size_(matrix.column_size_), matrix_(std::move(matrix.matrix_)) {
}

Matrix Matrix::operator+(Matrix &matrix) {
    if (row_size_ != matrix.row_size_ || column_size_ != matrix.column_size_) {
        throw std::runtime_error("incorrect sizes of matrices for sum");
    }
    Matrix sum(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            sum(i, j) = matrix_[i][j] + matrix(i, j);
        }
    }
    return sum;
}

Matrix Matrix::operator-(Matrix &matrix) {
    if (row_size_ != matrix.row_size_ || column_size_ != matrix.column_size_) {
        throw std::runtime_error("incorrect sizes of matrices for subtraction");
    }
    Matrix diff(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            diff(i, j) = matrix_[i][j] - matrix(i, j);
        }
    }

    return diff;
}

Matrix Matrix::operator*(Matrix &matrix) {
    if (row_size_ != matrix.column_size_) {
        throw std::runtime_error(
                "Количество столбцов первой матрицы должно совпадать с количеством строк второй матрицы.");
    }
    Matrix result(matrix.row_size_, column_size_, 0.0);

    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < matrix.row_size_; ++j) {
            for (size_t k = 0; k < row_size_; ++k) {
                result(i, j) += matrix_[i][k] * matrix(k, j);
            }
        }
    }

    return result;
}

Matrix &Matrix::operator=(Matrix &&matrix) noexcept {
    row_size_ = matrix.row_size_;
    column_size_ = matrix.column_size_;
    matrix_ = std::move(matrix.matrix_);
    return *this;
}

Matrix Matrix::operator+(double number) {
    Matrix result(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; i++) {
        for (size_t j = 0; j < row_size_; j++) {
            result(i, j) = matrix_[i][j] + number;
        }
    }
    return result;
}

Matrix Matrix::operator-(double number) {
    Matrix result(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; i++) {
        for (size_t j = 0; j < row_size_; j++) {
            result(i, j) = matrix_[i][j] - number;
        }
    }
    return result;
}

Matrix Matrix::operator*(double number) {
    Matrix result(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; i++) {
        for (size_t j = 0; j < row_size_; j++) {
            result(i, j) = matrix_[i][j] * number;
        }
    }
    return result;
}

Matrix Matrix::operator/(double number) {
    if (number == 0) {
        throw std::runtime_error("can't divide by zero");
    }
    Matrix result(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; i++) {
        for (size_t j = 0; j < row_size_; j++) {
            result(i, j) = matrix_[i][j] / number;
        }
    }
    return result;
}


double &Matrix::operator()(const unsigned &row_index, const unsigned &column_index) {
    return matrix_[row_index][column_index];
}

unsigned Matrix::getRowSize() const {
    return row_size_;
}

unsigned Matrix::getColumnSize() const {
    return column_size_;
}

Matrix Matrix::transpose() {
    Matrix transposed(column_size_, row_size_, 0.0);
    for (unsigned i = 0; i < column_size_; i++) {
        for (unsigned j = 0; j < row_size_; j++) {
            transposed(j, i) = this->matrix_[i][j];
        }
    }
    return transposed;
}

void Matrix::print() const {
    for (unsigned i = 0; i < column_size_; i++) {
        for (unsigned j = 0; j < row_size_; j++) {
            std::cout << "[" << matrix_[i][j] << "] ";
        }
        std::cout << std::endl;
    }
}