#include "matrix.h"

Matrix::Matrix(size_t row_size, size_t column_size, double initial_value) : row_size_(row_size),
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

Matrix Matrix::operator+(const Matrix &matrix) const {
    if (row_size_ != matrix.row_size_ || column_size_ != matrix.column_size_) {
        throw std::runtime_error("incorrect sizes of matrices for result");
    }
    Matrix result(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            result(i, j) = matrix_[i][j] + matrix(i, j);
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix &matrix) const {
    if (row_size_ != matrix.row_size_ || column_size_ != matrix.column_size_) {
        throw std::runtime_error("incorrect sizes of matrices for subtraction");
    }
    Matrix result(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            result(i, j) = matrix_[i][j] - matrix(i, j);
        }
    }

    return result;
}

Matrix Matrix::operator*(const Matrix &matrix) const {
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
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            result(i, j) = matrix_[i][j] + number;
        }
    }
    return result;
}

Matrix Matrix::operator-(double number) {
    Matrix result(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            result(i, j) = matrix_[i][j] - number;
        }
    }
    return result;
}

Matrix Matrix::operator*(double number) {
    Matrix result(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
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
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            result(i, j) = matrix_[i][j] / number;
        }
    }
    return result;
}

Matrix Matrix::resized(size_t new_row_size, size_t new_column_size, double padding_value) const {
    Matrix result(new_row_size, new_column_size, padding_value);
    size_t min_row_size = std::min(row_size_, new_row_size);
    size_t min_column_size = std::min(column_size_, new_column_size);
    for (size_t i = 0; i < min_column_size; ++i) {
        for (size_t j = 0; j < min_row_size; ++j) {
            result(i, j) = matrix_[i][j];
        }
    }
    return result;
}

Matrix &Matrix::resize(size_t new_row_size, size_t new_column_size, double padding_value) {
    for (std::vector<double> &row: matrix_) {
        row.resize(new_row_size, padding_value);
    }
    matrix_.resize(new_column_size, std::vector<double>(new_row_size, padding_value));
    return *this;
}

double &Matrix::operator()(const size_t &row_index, const size_t &column_index) {
    return matrix_[row_index][column_index];
}

const double &Matrix::operator()(const size_t &row_index, const size_t &column_index) const {
    return matrix_[row_index][column_index];
}

size_t Matrix::getRowSize() const {
    return row_size_;
}

size_t Matrix::getColumnSize() const {
    return column_size_;
}

Matrix Matrix::transposed() const {
    Matrix result(column_size_, row_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            result(j, i) = this->matrix_[i][j];
        }
    }
    return result;
}

Matrix &Matrix::transpose() {
    *this = std::move(this->transposed());
    return *this;
}

Matrix Matrix::multiplyByElement(const Matrix &matrix) const {
    if (row_size_ != matrix.row_size_ || column_size_ != matrix.column_size_) {
        throw std::runtime_error("matrices must have equal sizes for multiplication by element");
    }
    Matrix res(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            res(i, j) = matrix_[i][j] * matrix(i, j);
        }
    }
    return res;
}

Matrix Matrix::divideByElement(const Matrix &matrix) const {
    if (row_size_ != matrix.row_size_ || column_size_ != matrix.column_size_) {
        throw std::runtime_error("matrices must have equal sizes for division by element");
    }
    Matrix res(row_size_, column_size_, 0.0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            if (matrix(i, j) == 0) {
                throw std::runtime_error("matrix divider mustn't contain zeros");
            }
            res(i, j) = matrix_[i][j] / matrix(i, j);
        }
    }
    return res;
}

void Matrix::print() const {
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            std::cout << "[" << matrix_[i][j] << "] ";
        }
        std::cout << std::endl;
    }
}