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
    if (row_size_ != new_row_size) {
        for (std::vector<double> &row: matrix_) {
            row.resize(new_row_size, padding_value);
        }
    }

    matrix_.resize(new_column_size, std::vector<double>(new_row_size, padding_value));

    row_size_ = new_row_size;
    column_size_ = new_column_size;

    return *this;
}

Matrix Matrix::getSlice(std::pair<size_t, size_t> upper_left_point, std::pair<size_t, size_t> lower_right_point) const {
    if (upper_left_point.first > lower_right_point.first || upper_left_point.second > lower_right_point.second) {
        throw std::runtime_error("upper_left_point's i and j must be not bigger than lower_right_point's");
    }
    if (upper_left_point.first >= column_size_ || upper_left_point.second >= row_size_) {
        throw std::runtime_error("upper_left_point must be placed in matrix");
    }
    lower_right_point.first = std::min(lower_right_point.first, column_size_ - 1);
    lower_right_point.second = std::min(lower_right_point.second, row_size_ - 1);

    Matrix result(lower_right_point.second - upper_left_point.second + 1,
                  lower_right_point.first - upper_left_point.first + 1, 0);
    for (size_t i = upper_left_point.first; i < lower_right_point.first + 1; ++i) {
        for (size_t j = upper_left_point.second; j < lower_right_point.second + 1; ++j) {
            result(i - upper_left_point.first, j - upper_left_point.second) = matrix_[i][j];
        }
    }
    return result;
}

Matrix &Matrix::insert(std::pair<size_t, size_t> insert_point, const Matrix &matrix_to_insert) {
    if (insert_point.first >= column_size_ || insert_point.second >= row_size_) {
        throw std::runtime_error("insert_point must be placed in matrix");
    }
    for (size_t i = insert_point.first;
         i < std::min(column_size_, insert_point.first + matrix_to_insert.getColumnSize()); ++i) {
        for (size_t j = insert_point.second;
             j < std::min(row_size_, insert_point.second + matrix_to_insert.getRowSize()); ++j) {
            matrix_[i][j] = matrix_to_insert(i - insert_point.first, j - insert_point.second);
        }
    }
    return *this;
}

Matrix Matrix::flatten() const {
    Matrix result(row_size_ * column_size_, 1, 0);
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            result(0, i * row_size_ + j) = matrix_[i][j];
        }
    }
    return result;
}

std::vector<double> Matrix::flatten_vector() const {
    std::vector<double> result;
    for (size_t i = 0; i < column_size_; ++i) {
        for (size_t j = 0; j < row_size_; ++j) {
            result.push_back(matrix_[i][j]);
        }
    }
    return result;
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

void Matrix::print(int precision) const {
    std::streamsize previous_precision = std::cout.precision();
    std::cout << std::fixed << std::setprecision(precision);
    std::cout << '[';
    for (size_t i = 0; i < column_size_; ++i) {
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << "[";
        for (size_t j = 0; j < row_size_; ++j) {
            if (matrix_[i][j] >= 0) {
                std::cout << ' ';
            }
            std::cout << matrix_[i][j];
            if (j != row_size_ - 1) {
                std::cout << ",\t";
            }
        }
        std::cout << " ]";
        if (i != column_size_ - 1) {
            std::cout << std::endl;
        }
    }
    std::cout << ']' << std::endl;
    std::cout << std::defaultfloat << std::setprecision((int) previous_precision);
}