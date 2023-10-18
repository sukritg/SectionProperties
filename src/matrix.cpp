#include <iomanip>
#include <iostream>

#include "xprop/matrix.h"

Matrix::Matrix() {
    m_rows = 0;
    m_columns = 0;
}

Matrix::Matrix(int size) {
    m_rows = size;
    m_columns = size;
    if (size <= 0)
        throw std::out_of_range("Matrix index cannot be negative or zero value.");
    m_data = std::make_unique<double[]>(m_rows * m_columns);
}

Matrix::Matrix(int rows, int columns){
    m_rows = rows;
    m_columns = columns;
    if (rows <= 0 || columns <= 0)
        throw std::out_of_range("Matrix index cannot be negative or zero value.");
    m_data = std::make_unique<double[]>(m_rows * m_columns);
}

Matrix::Matrix(const Matrix &other) {
    m_rows = other.m_rows;
    m_columns = other.m_columns;
    if (!m_data)
    {
        m_data = std::make_unique<double[]>(m_rows * m_columns);
        for (int i = 0; i < m_rows * m_columns; i++)
            m_data[i] = other.m_data[i];
    }
}

Matrix::Matrix(Matrix &&other) noexcept {
    m_rows = other.m_rows;
    m_columns = other.m_columns;
    m_data = std::move(other.m_data);
    other.m_rows = 0;
    other.m_columns = 0;
}

void Matrix::clear() {
    m_rows = m_columns = 0;
    m_data.reset(nullptr);
}

void Matrix::reSize(int rows, int columns) {
    if (rows <=0 || columns <=0)
        throw std::out_of_range("Matrix index cannot be negative or zero value.");
    m_rows = rows;
    m_columns = columns;
    m_data.reset(nullptr);
    m_data = std::make_unique<double[]>(rows*columns);
}

Matrix Matrix::scale(double scalingFactor) const {
    Matrix scaledMatrix(m_rows, m_columns);
    for (int i = 0; i < m_rows * m_columns; i++)
        scaledMatrix.m_data[i] = m_data[i] * scalingFactor;
    return scaledMatrix;
}

Matrix Matrix::operator+(const Matrix &other) const {
    if (m_rows != other.m_rows || m_columns != other.m_columns)
        throw std::logic_error("Incompatible matrices for addition operation.");
    Matrix resultantMatrix(m_rows, m_columns);
    for (int i = 0; i < m_rows * m_columns; i++)
        resultantMatrix.m_data[i] = m_data[i] + other.m_data[i];
    return resultantMatrix;
}

Matrix Matrix::operator-(const Matrix &other) const {
    if (m_rows != other.m_rows || m_columns != other.m_columns)
        throw std::logic_error("Incompatible matrices for subtraction operation.");
    Matrix resultantMatrix(m_rows, m_columns);
    for (int i = 0; i < m_rows * m_columns; i++)
        resultantMatrix.m_data[i] = m_data[i] - other.m_data[i];
    return resultantMatrix;
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (m_columns != other.m_rows)
        throw std::logic_error("Incompatible matrices for multiplication operation.");
    Matrix resultantMatrix(m_rows, other.m_columns);
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < other.m_columns; j++)
        {
            resultantMatrix.m_data[i * other.m_columns + j] = 0.0;
            for (int k = 0; k < m_columns; k++)
                resultantMatrix.m_data[i * other.m_columns + j] += m_data[i * m_columns + k] * other.m_data[k * other.m_columns + j];
        }
    return resultantMatrix;
}

double& Matrix::operator()(int row, int column) const {
    if (row > m_rows || column > m_columns || row <= 0 || column <= 0)
        throw std::out_of_range("Matrix index out of bounds.");
    return m_data[(row - 1) * m_columns + (column - 1)];
}

double& Matrix::operator()(int row, int column) {
    if (row > m_rows || column > m_columns || row <= 0 || column <= 0)
        throw std::out_of_range("Matrix index out of bounds.");
    return m_data[(row - 1) * m_columns + (column - 1)];
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
    os << std::fixed << std::setprecision(4);
    for (int i = 1; i <= matrix.m_rows; i++)
        for (int j = 1; j <= matrix.m_columns; j++)
            os << std::setw(8) << matrix(i,j) << (j == matrix.m_columns ? "\n" : "\t");
    return os;
}

Matrix& Matrix::operator=(const Matrix &other) {
    if (this != &other)
    {
        m_rows = other.m_rows;
        m_columns = other.m_columns;
        m_data = std::make_unique<double[]>(m_rows * m_columns);
        for (int i = 0; i < m_rows * m_columns; i++)
            m_data[i] = other.m_data[i];
    }
    return *this;
}

Matrix &Matrix::operator=(Matrix &&other) noexcept {
    if (this != &other)
    {
        m_rows = other.m_rows;
        m_columns = other.m_columns;
        m_data = std::move(other.m_data);
    }
    return *this;
}

Matrix Matrix::transpose() const {
    Matrix resultantMatrix(m_columns, m_rows);
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++)
            resultantMatrix.m_data[j * m_columns + i] = m_data[i * m_columns + j];
    return resultantMatrix;
}

Matrix& Matrix::operator+=(const Matrix &other) {
    if (m_rows != other.m_rows || m_columns != other.m_columns)
        throw std::logic_error("Incompatible matrices for addition operation.");
    for (int i = 0; i < m_rows * m_columns; i++)
        m_data[i] += other.m_data[i];
    return *this;
}

Matrix& Matrix::operator-=(const Matrix &other) {
    if (m_rows != other.m_rows || m_columns != other.m_columns)
        throw std::logic_error("Incompatible matrices for subtraction operation.");
    Matrix resultantMatrix(m_rows, m_columns);
    for (int i = 0; i < m_rows * m_columns; i++)
       m_data[i] -= other.m_data[i];
    return *this;
}


